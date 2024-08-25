#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Arduino_MKRIoTCarrier.h>
#include "config.h"

//constants
MKRIoTCarrier carrier;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

float temperature = 0;
float humidity = 0;
float moisture = 0;
const int moistPin = A6;
String BROKER, PORT_NUMBER, device_id, key;

void resetScreen()
{
  carrier.display.setTextSize(2);
  carrier.display.setCursor(20, 100);
}

void connectWiFi(){
  while(WiFi.status() != WL_CONNECTED){
    resetScreen();
    Serial.println("Connecting to WiFi...");
    carrier.display.fillScreen(0xFC00);
    carrier.display.print("Connecting to WiFi...");   
    WiFi.begin(WIFI_NAME,WIFI_PASSWORD);
    delay(500);
  }

  Serial.println("Connected!");
  carrier.display.fillScreen(0x07E0);
  carrier.display.print("Connected");
  delay(2000);

}

void reconnectToMQTTClient()
{
  while(!client.connected())
  {
    resetScreen();
    Serial.println("Attempting to reconnect to MQTTClient");
    carrier.display.fillScreen(0xFC00);
    carrier.display.print("Attempting to reconnect to MQTTClient");
    
    if(client.connect(device_id.c_str()))
    {
      Serial.println("Connected");
      carrier.display.fillScreen(0x07E0);
      carrier.display.print("Connected");
      delay(2000);
    }
    else
    {
      Serial.println("Retrying in 5 seconds - failed, rc =");
      Serial.println(client.state());
      carrier.display.fillScreen(0xF800);
      carrier.display.println("Retrying in 5 seconds - failed, rc =");
      carrier.display.println(client.state());

    randomSeed(analogRead(A0));
    delay(random(2000, 5000));
    }
  }
}

void parseConnectionString(const String& connectionString, 
                           String& BROKER, 
                           String& PORT_NUMBER, 
                           String& device_id, 
                           String& key) {
    int start = 0;
    int pos = 0;
    
    // Parse HostName
    pos = connectionString.indexOf("HostName=", start);
    if (pos != -1) {
        start = pos + 9;  // Length of "HostName="
        pos = connectionString.indexOf(':', start);
        if (pos != -1) {
            BROKER = connectionString.substring(start, pos);
            start = pos + 1;
            pos = connectionString.indexOf(';', start);
            PORT_NUMBER = connectionString.substring(start, pos);
            start = pos + 1;
        }
    }

    // Parse DeviceID
    pos = connectionString.indexOf("DeviceID=", start);
    if (pos != -1) {
        start = pos + 9;  // Length of "DeviceID="
        pos = connectionString.indexOf(';', start);
        device_id = connectionString.substring(start, pos);
        start = pos + 1;
    }

    // Parse Key
    pos = connectionString.indexOf("Key=", start);
    if (pos != -1) {
        start = pos + 4;  // Length of "Key="
        key = connectionString.substring(start);
    }
}

void createMQTTClient(const char * BROKER,
                      uint16_t PORT_NUMBER){

  client.setServer(BROKER,PORT_NUMBER);
  reconnectToMQTTClient();
  client.setKeepAlive(30);

}

void setup() {
  Serial.begin(9600);
  CARRIER_CASE = true;
  carrier.begin();

  parseConnectionString(CONNECTION_STRING, BROKER, PORT_NUMBER, device_id, key);
  connectWiFi();

  Serial.println(BROKER);
  Serial.println(PORT_NUMBER);
  
  createMQTTClient(BROKER.c_str(),PORT_NUMBER.toInt());
  resetScreen();
}

void loop() {
  resetScreen();
  reconnectToMQTTClient();
  client.loop();

  //Get device observations
  temperature = carrier.Env.readTemperature(CELSIUS) -5;
  humidity = carrier.Env.readHumidity();
  int raw_moisture = analogRead(moistPin);
  moisture = map(raw_moisture, 0, 1023, 0, 100);

  
  // Correct JSON format
  String message = "{\"device_id\":\"";
  message+=device_id;
  message+="\",\"key\":\"";
  message+=key;
  message+="\",\"data\":{\"temperature\":";
  message += temperature;
  message += ",\"humidity\":";
  message += humidity;
  message += ",\"moisture\":";
  message += moisture;
  message += "}}";


  //convert message to char pointer
  const char * message_ = message.c_str();

  //check if connection is still up
  if (!client.connected()) {
    reconnectToMQTTClient();
  }

  // Check if publish is successful
  if (client.publish("d2c.device_001",message_)) {
    Serial.println("Message published successfully");
    Serial.println(message_);
    carrier.display.fillScreen(0x07E0);
    carrier.display.print("Message published successfully");
  } else {
    Serial.println("Message publish failed");
    carrier.display.fillScreen(0xF800);
    carrier.display.print("Message publish failed");
  }

  delay(10000);
}
