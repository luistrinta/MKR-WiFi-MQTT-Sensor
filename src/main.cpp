#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "config.h"
void connectWiFi(){
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to WiFi...");
    WiFi.begin(SECRET_SSID,SECRET_PASS);
    delay(500);
  }

  Serial.println("Connected!");

}
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnectToMQTTClient()
{
  while(!client.connected())
  {
    Serial.println("Attempting to reconnect to MQTTClient");

    if(client.connect(CLIENT_NAME))
    {
      Serial.println("Connected");
    }
    else
    {
      Serial.println("Retrying in 5 seconds - failed, rc =");
      Serial.println(client.state());
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
  while(!Serial){
    ;
  }
  connectWiFi();
  createMQTTClient();
}

void loop() {
  reconnectToMQTTClient();
  client.loop();
  
  // Correct JSON format
  const char * message = "{\"device_name\":\"device_001\",\"data\":\"this is a dummy message\"}";
  
  // Check if publish is successful
  if (client.publish(MQTT_TOPIC, message)) {
    Serial.println("Message published successfully");
  } else {
    Serial.println("Message publish failed");
  }

  delay(5000);
}
