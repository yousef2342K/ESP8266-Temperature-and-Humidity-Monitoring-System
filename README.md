ESP8266 Climate Control System Documentation
Introduction
This ESP8266 Climate Control System is designed to control relays, read temperature and humidity data from a DHT sensor, and send this data to a Python server for further processing or monitoring.

Components: 
ESP8266 WiFi module
DHT11 temperature and humidity sensor
IR receiver module
Relay module (4-channel)
Python server (for receiving temperature and humidity data)
Features
WiFi Connectivity: The ESP8266 module connects to a WiFi network as an access point, allowing devices to connect to it for control and data transmission.

Relay Control: The system can control up to four relays, allowing for the activation or deactivation of connected devices such as fans, heaters, or lights.

IR Remote Support: The system can receive IR remote signals to control the relays. Additionally, it supports specific IR codes for turning all relays on or off at once.

Temperature and Humidity Sensing: The DHT11 sensor is used to measure temperature and humidity levels in the environment.

HTTP Server: The ESP8266 hosts a web server providing endpoints to control relays, retrieve relay states, set IR codes, and retrieve temperature and humidity data.

Usage :
Connecting to the System: Connect to the WiFi network named "ESP8266-AP" with the password "12345678".

Controlling Relays: Use HTTP GET requests to /on<i> and /off<i> (where <i> is the relay number) to turn specific relays on or off.

Getting Relay States: Use HTTP GET requests to /state<i> to get the state (on or off) of a specific relay.

Setting IR Codes: Use HTTP GET requests to /setIR<i> to set the IR code for a specific relay. Use /setIRon and /setIRoff to set the IR codes for turning all relays on or off.

Retrieving Temperature and Humidity Data: Use HTTP GET requests to /temp and /hum to retrieve the current temperature and humidity data, respectively.

## Installation

1. **ESP8266 Code:**
   - Upload the provided ESP8266 code to your ESP8266 microcontroller using the Arduino IDE or any compatible IDE.
   - Adjust the WiFi network credentials if needed.

2. **Python Server:**
   - Run the provided Python code on a machine with Python installed.
   - Install the required dependencies using `pip install -r requirements.txt`.

Communication with Python Server
The ESP8266 periodically sends temperature and humidity data to a Python server. The server receives this data through HTTP GET requests to the /temp and /hum endpoints.

Dependencies
ESP8266WiFi.h: Library for connecting the ESP8266 to a WiFi network.
ESP8266WebServer.h: Library for handling HTTP requests on the ESP8266.
IRremote.h: Library for receiving IR signals.
DHT.h: Library for interfacing with DHT temperature and humidity sensors.
Author
This code was written by [JoEKhalid].
