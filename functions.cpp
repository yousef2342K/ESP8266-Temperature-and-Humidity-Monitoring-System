#include <ESP8266WiFi.h>

// Define GPIO pin connected to the relay module
#define RELAY_PIN D1

void setup() {
  // Set relay pin as output
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  // Turn on the fan (close relay)
  digitalWrite(RELAY_PIN, HIGH);
  delay(5000); // Keep the fan on for 5 seconds
  
  // Turn off the fan (open relay)
  digitalWrite(RELAY_PIN, LOW);
  delay(5000); // Wait for 5 seconds before turning on the fan again
}

