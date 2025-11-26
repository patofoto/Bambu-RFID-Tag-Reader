#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_manager.h"
#include "config/mqtt_credentials.h"
#include "wifi_manager.h"

// Create MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// MQTT connection state
bool mqttConnected = false;

// Reconnect to MQTT broker
bool reconnectMQTT() {
  if (!isWiFiConnected()) {
    return false; // Can't connect to MQTT without WiFi
  }
  
  Serial.print("Attempting MQTT connection...");
  
  // Attempt to connect
  bool connected = false;
  if (strlen(MQTT_USERNAME) > 0 && strlen(MQTT_PASSWORD) > 0) {
    connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
  } else {
    connected = mqttClient.connect(MQTT_CLIENT_ID);
  }
  
  if (connected) {
    Serial.println(" connected!");
    mqttConnected = true;
    return true;
  } else {
    Serial.print(" failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again later");
    mqttConnected = false;
    return false;
  }
}

// Initialize MQTT
bool initMQTT() {
  if (!isWiFiConnected()) {
    Serial.println("MQTT: WiFi not connected, skipping MQTT initialization");
    return false;
  }
  
  Serial.print("Initializing MQTT broker: ");
  Serial.println(MQTT_BROKER);
  
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  
  // Try to connect
  if (reconnectMQTT()) {
    Serial.println("MQTT initialized successfully");
    return true;
  } else {
    Serial.println("MQTT initialization failed, will retry later");
    return false;
  }
}

// Check if MQTT is connected
bool isMQTTConnected() {
  if (!isWiFiConnected()) {
    mqttConnected = false;
    return false;
  }
  
  if (mqttClient.connected()) {
    mqttConnected = true;
    return true;
  } else {
    mqttConnected = false;
    return false;
  }
}

// Handle MQTT (call in loop)
void handleMQTT() {
  if (!isWiFiConnected()) {
    return; // Can't use MQTT without WiFi
  }
  
  // Maintain connection
  if (!mqttClient.connected()) {
    mqttConnected = false;
    static unsigned long lastReconnectAttempt = 0;
    unsigned long now = millis();
    
    // Try to reconnect every 5 seconds
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      reconnectMQTT();
    }
  } else {
    mqttConnected = true;
    mqttClient.loop(); // Process MQTT messages
  }
}

// Publish UID as single decimal number
void publishUID(unsigned long uid) {
  if (!isMQTTConnected()) {
    Serial.println("MQTT: Not connected, cannot publish");
    return;
  }
  
  String payload = String(uid);
  bool published = mqttClient.publish(MQTT_TOPIC, payload.c_str());
  
  if (published) {
    Serial.print("MQTT: Published UID ");
    Serial.print(uid);
    Serial.print(" to topic ");
    Serial.println(MQTT_TOPIC);
  } else {
    Serial.println("MQTT: Failed to publish UID");
  }
}

// Publish UID with all formats (JSON)
void publishUID(String uidHex, String uidDecimal, unsigned long uidNumber) {
  if (!isMQTTConnected()) {
    Serial.println("MQTT: Not connected, cannot publish");
    return;
  }
  
  // Create JSON payload
  String payload = "{";
  payload += "\"hex\":\"" + uidHex + "\",";
  payload += "\"decimal\":\"" + uidDecimal + "\",";
  payload += "\"uid\":" + String(uidNumber);
  payload += "}";
  
  bool published = mqttClient.publish(MQTT_TOPIC, payload.c_str());
  
  if (published) {
    Serial.print("MQTT: Published UID data to topic ");
    Serial.println(MQTT_TOPIC);
  } else {
    Serial.println("MQTT: Failed to publish UID data");
  }
}

