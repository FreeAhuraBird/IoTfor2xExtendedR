#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// wifi
const char* ssid = "iPhone";
const char* password = "lollollol";

// broker
const char* broker = "172.20.10.8";
const int port = 1883;

// topics
const char* topicSound = "sensor/sound";

int analogPin = A0; 
int led = 13;  
int threshold = 70;

unsigned long lastPublishTime = 0; 
const unsigned long publishInterval = 300;

WiFiClient espClient;
PubSubClient client(espClient);

// function for connecting to wifi
void setupWifi() {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");
}

// function for connecting to the mqtt broker
void reconnect() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (client.connect("ArduinoPublisher")) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed to connect, rc=");
            Serial.println(client.state());
            delay(5000); // Retry after 5 seconds
        }
    }
}

void ensureWifiConnected() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected, reconnecting...");
        //WiFi.disconnect();
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nWi-Fi reconnected!");
        
        // Reinitialize MQTT connection
        if (!client.connected()) {
            reconnect();
        }
    }
}

void setup() {
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial.begin(115200);

    setupWifi();
    client.setServer(broker, port);
}

void loop() {
  //Serial.print("Wi-Fi RSSI: ");
  //Serial.println(WiFi.RSSI());
    ensureWifiConnected();

    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    //Serial.print(WiFi.Status());
    unsigned long currentMillis = millis();
    if (currentMillis - lastPublishTime >= publishInterval) {
      String Sounddata = String(analogRead(analogPin));
      if (client.connected()) {
        if (client.publish(topicSound, Sounddata.c_str())) {
          Serial.println("Published: " + Sounddata);
        }
        else
        {
          Serial.println("Failed to publish data.");
        }
      }
      else if (!client.connected())
      {
        client.connect("ArduinoPublisher");
        if (client.publish(topicSound, Sounddata.c_str())) {
          Serial.println("Published: " + Sounddata);
        }
        else
        {
          Serial.println("Failed to publish data.");
        }
        client.loop();
        Serial.println("Failed to publish data.");
      }

        lastPublishTime = currentMillis; // Update the last publish time
    }
}
