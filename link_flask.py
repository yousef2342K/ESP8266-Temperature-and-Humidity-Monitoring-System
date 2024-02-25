from flask import Flask, request
import requests
app = Flask(__name__)

# Threshold temperature for turning on the fan
TEMP_THRESHOLD = 25  # You can adjust this threshold as needed

# Flag to track the fan state (True if fan is on, False if fan is off)
fan_on = False

# IP address and port of the ESP8266 running the Arduino sketch
ESP_IP = "192.168.1.100"  # Change this to the IP address of your ESP8266
ESP_PORT = 80  # Change this to the port number on which your ESP8266 is listening

# Function to simulate turning on the fan
def turn_on_fan():
    global fan_on
    if not fan_on:
        print("Turning on the fan")
        requests.get(f"http://{ESP_IP}:{ESP_PORT}/fan_on")
        fan_on = True

# Function to simulate turning off the fan
def turn_off_fan():
    global fan_on
    if fan_on:
        print("Turning off the fan")
        requests.get(f"http://{ESP_IP}:{ESP_PORT}/fan_off")
        fan_on = False

@app.route('/update', methods=['GET'])
def update():
    temperature = request.args.get('temp', type=float)
    humidity = request.args.get('hum', type=float)

    # Perform actions based on received data
    print(f"Received Temperature: {temperature}°C, Humidity: {humidity}%")

    # Check if temperature exceeds the threshold
    if temperature is not None:
        if temperature > TEMP_THRESHOLD:
            # If temperature exceeds the threshold, turn on the fan
            turn_on_fan()
        else:
            # If temperature is below the threshold, turn off the fan
            turn_off_fan()


    return 'Data received successfully'

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
