#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "vivo";
const char* password = "rakeshsingh";

// MQTT broker details
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttTopicWave = "SIT210wavesAkshit";
const char* mqttTopicPat = "SIT210wavespatAkshit";

WiFiClient espClient;
PubSubClient client(espClient);

// Pin definition for LED
const int ledPin = 4;

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  // Connect to WiFi
  connectToWiFi();

  // Setup MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Connect to MQTT
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();
}

void connectToWiFi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ReceiverClient")) {
      Serial.println("Connected to MQTT");

      // Subscribe to the topics
      client.subscribe(mqttTopicWave);
      client.subscribe(mqttTopicPat);
    } else {
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, mqttTopicWave) == 0) {
    Serial.println("Wave message received. Flashing LED 3 times...");
    flashLED(3);
  } else if (strcmp(topic, mqttTopicPat) == 0) {
    Serial.println("Pat message received. Flashing LED 5 times...");
    flashLED(5);  
  }
}

void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}
