#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>

#include "pin_config.h"
#include "config.h"
#include "display.h"
#include "rfid.h"

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
  
  // Initialize RFID reader
  initRFID(mfrc522);
}

void loop() {
  MFRC522::Uid uid;
  
  // Try to read RFID tag
  if (readRFIDTag(mfrc522, uid)) {
    // Print to serial
    printUIDToSerial(uid);
    
    // Update display
    updateDisplay(tft, uid);
    
    // Small delay before next read
    delay(1000);
  }
}
