#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>

#include "pin_config.h"
#include "config.h"
#include "display.h"
#include "rfid.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Create TFT display instance
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("Bambu RFID Tag Reader");
  Serial.println("====================");
  
  // Initialize display
  initDisplay(tft);
  
  // Initialize WiFi (pass display for progress updates)
  initWiFi(&tft);
  
  // Initialize MQTT (only if WiFi is connected)
  if (isWiFiConnected()) {
    initMQTT();
  }
  
  // Initialize RFID reader
  initRFID(mfrc522);
}

void loop() {
  // Handle WiFi reconnection if needed
  handleWiFi();
  
  // Handle MQTT (maintain connection and process messages)
  if (isWiFiConnected()) {
    handleMQTT();
  }
  
  MFRC522::Uid uid;
  
  // Try to read RFID tag
  if (readRFIDTag(mfrc522, uid)) {
    // Print to serial
    printUIDToSerial(uid);
    
    // Update display with tag data and IP address
    updateDisplay(tft, uid, getWiFiIP());
    
    // Publish UID to MQTT (if connected)
    if (isWiFiConnected() && isMQTTConnected()) {
      unsigned long uidNumber = formatUIDNumber(uid);
      String uidHex = formatUIDHex(uid);
      String uidDecimal = formatUIDDecimal(uid);
      publishUID(uidHex, uidDecimal, uidNumber);
    }
    
    // Small delay before next read
    delay(1000);
  }
}
