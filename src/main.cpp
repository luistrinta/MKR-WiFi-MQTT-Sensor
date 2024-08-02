#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Arduino_MKRIoTCarrier.h>
#include "config.h"

//constants
MKRIoTCarrier carrier;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void connectWiFi(){
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to WiFi...");
    carrier.display.fillScreen(0xFC00);
    carrier.display.print("Connecting to WiFi...");   
    WiFi.begin(SECRET_SSID,SECRET_PASS);
    delay(500);
  }

  Serial.println("Connected!");
  carrier.display.fillScreen(0xFC00);
  carrier.display.print("Connected");

}

void reconnectToMQTTClient()
{
  while(!client.connected())
  {
    Serial.println("Attempting to reconnect to MQTTClient");
    carrier.display.fillScreen(0xFC00);
    carrier.display.print("Attempting to reconnect to MQTTClient");
    
    if(client.connect(CLIENT_NAME))
    {
      Serial.println("Connected");
      carrier.display.fillScreen(0xFC00);
      carrier.display.print("Connected");
    }
    else
    {
      Serial.println("Retrying in 5 seconds - failed, rc =");
      Serial.println(client.state());
      carrier.display.fillScreen(0xFC00);
      carrier.display.println("Retrying in 5 seconds - failed, rc =");
      carrier.display.println(client.state());

      delay(5000);
    }
  }
}

void createMQTTClient()
{
  client.setServer(BROKER,PORT_NUMBER);
  client.setKeepAlive(60);
  reconnectToMQTTClient();
}



void setup() {
  Serial.begin(9600);
  CARRIER_CASE = true;
  carrier.begin();
  connectWiFi();
  createMQTTClient();
  carrier.display.fillScreen(0xFC00);
  carrier.display.setTextColor(0x0000);
  carrier.display.setTextSize(3);
  carrier.display.setCursor(20, 20);
}

void loop() {
  reconnectToMQTTClient();
  client.loop();
  
  // Correct JSON format
  const char * message = "{\"device_name\":\"device_001\",\"data\":\"this is a dummy message\"}";
  
  // Check if publish is successful
  if (client.publish("d2c.device_001",message)) {
    Serial.println("Message published successfully");
    carrier.display.fillScreen(0xFC00);
    carrier.display.print("Message published successfully");
  } else {
    Serial.println("Message publish failed");
    carrier.display.fillScreen(0xFC00);
    carrier.display.print("Message publish failed");
  }

  delay(5000);
}
