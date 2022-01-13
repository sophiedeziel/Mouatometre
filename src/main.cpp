#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "settings.h"

#define DHTPIN D3 
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

// Mqtt Vars
WiFiClient espClient;
PubSubClient client(espClient);


void sendDiscoveryTopic(){
  Serial.println("Sending config");
  char configMessage[300];
  char configTopic[50];
  sprintf(configMessage,"{ \"name\": \"%s\", \"unique_id\": \"%s\", \"state_topic\": \"%s\", \"device_class\": \"humidity\", \"unit_of_measurement\": \"%s\", \"value_template\": \"{{ value_json.humidity }}\"}", mqtt_name, mqtt_clientid, mqtt_state_topic, "%");
  sprintf(configTopic, "homeassistant/sensor/%s/config", mqtt_clientid);
  client.publish(configTopic, configMessage);
}

void reconnectMqtt() {
  Serial.println(client.state());
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(mqtt_clientid, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      sendDiscoveryTopic();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);

  dht.begin();

  Serial.printf("Connecting to %s\n", ssid);
  if (String(WiFi.SSID()) != String(ssid)) {
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! With IP ");
  Serial.print(WiFi.localIP());
  Serial.println(" have FUN :) ");

  //  Mqtt Init
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  yield(); // Avoid crashes on ESP8266

  if (!client.connected()) {
    reconnectMqtt();
  }
  client.loop();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);

  char sensorMessage[100];
  sprintf(sensorMessage, "{ \"humidity\": %f }", round(h));
  client.publish(mqtt_state_topic, sensorMessage);

  delay(3000);
}


