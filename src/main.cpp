#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>
#include "pin_config.h"

// RFID-RC522 Pin Configuration for LilyGO T3-S3
// Using only exposed pins from the board headers
#define RST_PIN         21   // Reset pin (GPIO21 on P2 header)
#define SS_PIN          10   // Slave Select (SDA) pin (GPIO10 on P1 header, labeled SPI_CS)

// SPI pins matching exposed pins on P1 header
#define SPI_MOSI        11   // GPIO11 (SPI_D on P1 header)
#define SPI_MISO        13   // GPIO13 (SPI_Q on P1 header)
#define SPI_SCK         12   // GPIO12 (SPI_CLK on P1 header)

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Create TFT display instance
TFT_eSPI tft = TFT_eSPI();

// Function to update display with RFID tag information
void updateDisplay(MFRC522::Uid uid) {
  tft.fillScreen(TFT_BLACK);
  
  // Title
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Bambu RFID Reader", 5, 5, 4);
  
  // Draw a line separator
  tft.drawLine(0, 30, tft.width(), 30, TFT_WHITE);
  
  int yPos = 40;
  int lineHeight = 20;
  
  // UID Hex format
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Hex:", 5, yPos, 2);
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
  tft.drawString(hexStr, 50, yPos, 2);
  yPos += lineHeight;
  
  // UID Decimal format
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Decimal:", 5, yPos, 2);
  String decStr = "";
  for (byte i = 0; i < uid.size; i++) {
    decStr += String(uid.uidByte[i]);
    if (i < uid.size - 1) {
      decStr += ":";
    }
  }
  tft.drawString(decStr, 80, yPos, 2);
  yPos += lineHeight;
  
  // Single decimal number
  unsigned long uidDecimal = 0;
  for (byte i = 0; i < uid.size; i++) {
    uidDecimal = uidDecimal * 256 + uid.uidByte[i];
  }
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("UID Number:", 5, yPos, 2);
  tft.drawString(String(uidDecimal), 100, yPos, 2);
  yPos += lineHeight;
  
  // Raw bytes
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.drawString("Raw Bytes:", 5, yPos, 2);
  String rawStr = "";
  for (byte i = 0; i < uid.size; i++) {
    rawStr += "0x";
    if (uid.uidByte[i] < 0x10) {
      rawStr += "0";
    }
    rawStr += String(uid.uidByte[i], HEX);
    if (i < uid.size - 1) {
      rawStr += ", ";
    }
  }
  rawStr.toUpperCase();
  // Split raw bytes if too long for one line
  if (rawStr.length() > 25) {
    int splitPos = rawStr.lastIndexOf(", ", 25);
    if (splitPos > 0) {
      tft.drawString(rawStr.substring(0, splitPos), 90, yPos, 2);
      yPos += lineHeight;
      tft.drawString(rawStr.substring(splitPos + 2), 90, yPos, 2);
    } else {
      tft.drawString(rawStr, 90, yPos, 2);
    }
  } else {
    tft.drawString(rawStr, 90, yPos, 2);
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("Bambu RFID Tag Reader");
  Serial.println("====================");
  
  // Power on display
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  
  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape orientation (320x170)
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  
  // Set backlight
  #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
    ledcSetup(0, 2000, 8);
    ledcAttachPin(PIN_LCD_BL, 0);
    ledcWrite(0, 255);
  #else
    ledcAttach(PIN_LCD_BL, 200, 8);
    ledcWrite(PIN_LCD_BL, 255);
  #endif
  
  // Show initial message on display
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Bambu RFID Reader", 5, 5, 4);
  tft.drawLine(0, 30, tft.width(), 30, TFT_WHITE);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Waiting for tag...", 5, 50, 2);
  
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

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Card detected - print UID to serial
  Serial.println("----------------------------------------");
  Serial.println("RFID Tag Detected!");
  Serial.print("UID Size: ");
  Serial.print(mfrc522.uid.size);
  Serial.println(" bytes");
  
  Serial.print("UID (hex): ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) {
      Serial.print(":");
    }
  }
  Serial.println();
  
  Serial.print("UID (decimal): ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i]);
    if (i < mfrc522.uid.size - 1) {
      Serial.print(":");
    }
  }
  Serial.println();
  
  // Print as a single decimal number (useful for database storage)
  unsigned long uidDecimal = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecimal = uidDecimal * 256 + mfrc522.uid.uidByte[i];
  }
  Serial.print("UID (single decimal): ");
  Serial.println(uidDecimal);
  
  // Print raw bytes
  Serial.print("Raw bytes: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print("0x");
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
  Serial.println("----------------------------------------");
  Serial.println();
  
  // Update display with RFID tag information
  updateDisplay(mfrc522.uid);
  
  // Halt PICC
  mfrc522.PICC_HaltA();
  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  
  // Small delay before next read
  delay(1000);
}