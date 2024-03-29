#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremote.h>
#include <DHT.h>
#include <WiFiClient.h>

// WiFi settings
const char *ssid = "ESP8266-AP";
const char *password = "12345678";
const char *serverAddress = "192.168.1.100";  // Replace with the IP address of your Python server

// Pin definitions
#define DHTPIN D7
#define DHTTYPE DHT11
const int relayPins[] = {D1, D2, D5, D6};
const int IR_RECEIVER_PIN = D0;

// Number of relays
const int numRelays = 4;

// IR codes
unsigned long ircodes[6];

// Web server
ESP8266WebServer server(80);

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Relay states
bool relayStates[] = {false, false, false, false};

// Temperature and humidity variables
float temperature;
float humidity;

// Function prototypes
void handleOn(int relayIndex);
void handleOff(int relayIndex);
void getState(int relayIndex);
void handleIRCode(unsigned long code);
void toggleLedState(int ledIndex);
void setIRCode(int x);
void sendData(float temperature, float humidity);

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("Access Point IP Address: ");
  Serial.println(ip);

  // Initialize DHT sensor
  dht.begin();

  // Initialize IR receiver
  IRrecv irReceiver(IR_RECEIVER_PIN);
  irReceiver.enableIRIn();

  // Set up web server routes
  for (int i = 0; i < numRelays; i++) {
    server.on("/on" + String(i + 1), HTTP_GET, [i]() { handleOff(i); });
    server.on("/off" + String(i + 1), HTTP_GET, [i]() { handleOn(i); });
    server.on("/state" + String(i + 1), HTTP_GET, [i]() { getState(i); });
    server.on("/setIR" + String(i + 1), HTTP_GET, [i]() { setIRCode(i); });
  }
  server.on("/setIRon", HTTP_GET, []() { setIRCode(4); });
  server.on("/setIRoff", HTTP_GET, []() { setIRCode(5); });
  server.on("/temp", HTTP_GET, []() { sendData(temperature, humidity); });
  server.on("/hum", HTTP_GET, []() { sendData(temperature, humidity); });

  // Begin web server
  server.begin();

  // Set initial state of relays
  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);
  }
}

void loop() {
  server.handleClient();

  // Handle IR codes
  if (irReceiver.decode(&irResults)) {
    handleIRCode(irResults.value);
    Serial.println(irResults.value, HEX);
    irReceiver.resume();
  }
  
  // Read temperature and humidity sensor
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}

void handleOn(int relayIndex) {
  digitalWrite(relayPins[relayIndex], HIGH);
  relayStates[relayIndex] = true;
  server.send(200, "text/plain", "Relay " + String(relayIndex + 1) + " turned on");
}

void handleOff(int relayIndex) {
  digitalWrite(relayPins[relayIndex], LOW);
  relayStates[relayIndex] = false;
  server.send(200, "text/plain", "Relay " + String(relayIndex + 1) + " turned off");
}

void getState(int relayIndex) {
  for (int i = 0; i < numRelays; i++) {
    relayStates[i] = digitalRead(relayPins[i]);
  }
  String state = !relayStates[relayIndex] ? "on" : "off";
  server.send(200, "text/plain", "r" + String(relayIndex + 1) + state);
}

void handleIRCode(unsigned long code) {
  for (int i = 0; i < numRelays; i++) {
    if (code == ircodes[i]) {
      toggleLedState(i);
    }
  }

  if (code == ircodes[4]) {
    // All on
    for (int i = 0; i < numRelays; i++) {
      handleOff(i);
    }
  } else if (code == ircodes[5]) {
    // All off
    for (int i = 0; i < numRelays; i++) {
      handleOn(i);
    }
  }
}

void toggleLedState(int ledIndex) {
  relayStates[ledIndex] = !relayStates[ledIndex];
  digitalWrite(relayPins[ledIndex], relayStates[ledIndex] ? HIGH : LOW);
}

void setIRCode(int x) {
  while (!irReceiver.decode(&irResults2)) {
    ircodes[x] = irResults2.value;
    Serial.println("Wait");
  }
  String response = String(ircodes[x], HEX);
  Serial.println(response);
  server.send(200, "text/plain", response);
}

void sendData(float temperature, float humidity) {
  WiFiClient client;
  if (!client.connect(serverAddress, 80)) {
    Serial.println("Connection to server failed");
    return;
  }

  String url = "/update?temp=" + String(temperature) + "&hum=" + String(humidity);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + serverAddress + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10); // Give the server some time to respond

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}
