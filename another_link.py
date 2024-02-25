from flask import Flask, request

app = Flask(__name__)

# Endpoint to receive temperature and humidity data from ESP8266
@app.route('/update', methods=['GET'])
def update():
    # Retrieve temperature and humidity data from the request parameters
    temperature = request.args.get('temp', type=float)
    humidity = request.args.get('hum', type=float)

    # Perform actions based on the received data
    if temperature is not None and humidity is not None:
        print(f"Received Temperature: {temperature}°C, Humidity: {humidity}%")
        # Add your custom logic here, such as storing data in a database,
        # triggering other devices, or sending notifications.

        # Example: Send a notification if temperature exceeds a threshold
        if temperature > 30:
            send_notification("Temperature is too high!")

        # Example: Turn on a fan if humidity is above a certain level
        if humidity > 70:
            turn_on_fan()

    return 'Data received successfully'

# Function to send a notification (example implementation)
def send_notification(message):
    # Implement your notification mechanism here
    print("Sending notification:", message)

# Function to turn on a fan (example implementation)
def turn_on_fan():
    # Implement your mechanism to turn on the fan here
    print("Turning on the fan")

if __name__ == '__main__':
    # Run the Flask app
    app.run(debug=True, host='0.0.0.0')
