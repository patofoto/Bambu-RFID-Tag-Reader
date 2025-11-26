#include <Arduino.h>
#include <WiFi.h>
#include "wifi_manager.h"
#include "config/wifi_credentials.h"

// Initialize WiFi connection
bool initWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  // Wait for connection with timeout
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
    return false;
  }
}

// Check if WiFi is connected
bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

// Handle WiFi reconnection if needed
void handleWiFi() {
  if (!isWiFiConnected()) {
    // Try to reconnect every 30 seconds
    static unsigned long lastReconnectAttempt = 0;
    unsigned long now = millis();
    
    if (now - lastReconnectAttempt > 30000) {
      lastReconnectAttempt = now;
      Serial.println("Attempting WiFi reconnection...");
      WiFi.disconnect();
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
  }
}

// Get current WiFi SSID
String getWiFiSSID() {
  if (isWiFiConnected()) {
    return WiFi.SSID();
  }
  return String(WIFI_SSID);
}

// Get WiFi IP address
IPAddress getWiFiIP() {
  if (isWiFiConnected()) {
    return WiFi.localIP();
  }
  return IPAddress(0, 0, 0, 0);
}
