This Arduino sketch publishes temperature, humidity, and moisture data from an MKRIoT Carrier to an MQTT broker. It connects to WiFi and reconnects to the broker if necessary. Visual feedback is provided on the carrier display.

Features:

WiFi & MQTT connection management
Sensor data reading (temperature, humidity, moisture)
JSON message formatting
MQTT publishing
Requires:

Arduino IDE
WiFiNINA library
PubSubClient library
Arduino_MKRIoTCarrier library
