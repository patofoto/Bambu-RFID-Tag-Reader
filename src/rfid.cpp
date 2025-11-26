#include <Arduino.h>
#include "rfid.h"
#include "config.h"

// Initialize RFID reader
void initRFID(MFRC522& mfrc522) {
  // Initialize SPI bus
  // ESP32-S3 default SPI pins (VSPI) match exposed pins: MOSI=GPIO11, MISO=GPIO13, SCK=GPIO12
  SPI.begin();
  
  // Initialize MFRC522 (it will manage the SS pin separately)
  mfrc522.PCD_Init();
  
  // Show details of PCD - MFRC522 Card Reader details
  Serial.print("MFRC522 Version: ");
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println();
  Serial.println("Place a Bambu RFID tag near the reader...");
  Serial.println();
}

// Read RFID tag if present
bool readRFIDTag(MFRC522& mfrc522, MFRC522::Uid& uid) {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  
  // Copy UID
  uid = mfrc522.uid;
  
  // Halt PICC
  mfrc522.PICC_HaltA();
  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  
  return true;
}

// Format UID as hex string
String formatUIDHex(MFRC522::Uid uid) {
  String hexStr = "";
  for (byte i = 0; i < uid.size; i++) {
    if (uid.uidByte[i] < 0x10) {
      hexStr += "0";
    }
    hexStr += String(uid.uidByte[i], HEX);
    if (i < uid.size - 1) {
      hexStr += ":";
    }
  }
  hexStr.toUpperCase();
  return hexStr;
}

// Format UID as decimal string
String formatUIDDecimal(MFRC522::Uid uid) {
  String decStr = "";
  for (byte i = 0; i < uid.size; i++) {
    decStr += String(uid.uidByte[i]);
    if (i < uid.size - 1) {
      decStr += ":";
    }
  }
  return decStr;
}

// Format UID as single decimal number
unsigned long formatUIDNumber(MFRC522::Uid uid) {
  unsigned long uidDecimal = 0;
  for (byte i = 0; i < uid.size; i++) {
    uidDecimal = uidDecimal * 256 + uid.uidByte[i];
  }
  return uidDecimal;
}

// Print UID to serial monitor
void printUIDToSerial(MFRC522::Uid uid) {
  Serial.println("----------------------------------------");
  Serial.println("RFID Tag Detected!");
  Serial.print("UID Size: ");
  Serial.print(uid.size);
  Serial.println(" bytes");
  
  Serial.print("UID (hex): ");
  Serial.println(formatUIDHex(uid));
  
  Serial.print("UID (decimal): ");
  Serial.println(formatUIDDecimal(uid));
  
  Serial.print("UID (single decimal): ");
  Serial.println(formatUIDNumber(uid));
  
  // Print raw bytes
  Serial.print("Raw bytes: ");
  for (byte i = 0; i < uid.size; i++) {
    Serial.print("0x");
    if (uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(uid.uidByte[i], HEX);
    if (i < uid.size - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println();
}

