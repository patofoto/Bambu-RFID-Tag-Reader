#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "wifi_manager.h"
#include "config/wifi_credentials.h"
#include "display.h"

// Initialize WiFi connection
bool initWiFi(TFT_eSPI* tft) {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  // Show connecting message on display
  if (tft != nullptr) {
    showWiFiConnecting(*tft, WIFI_SSID, 0);
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  // Wait for connection with timeout
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    // Update display with progress
    if (tft != nullptr) {
      showWiFiConnecting(*tft, WIFI_SSID, attempts);
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Show success on display
    if (tft != nullptr) {
      showWiFiConnected(*tft, WiFi.localIP());
      delay(2000); // Show success message for 2 seconds
    }
    
    return true;
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
    
    // Show failure on display
    if (tft != nullptr) {
      showWiFiFailed(*tft);
      delay(2000); // Show failure message for 2 seconds
    }
    
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
