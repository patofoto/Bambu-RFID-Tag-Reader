#include <Arduino.h>
#include "display.h"
#include "pin_config.h"

// Initialize the TFT display
void initDisplay(TFT_eSPI& tft) {
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
  
  // Show initial message
  showWaitingMessage(tft);
}

// Show waiting for tag message
void showWaitingMessage(TFT_eSPI& tft) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Bambu RFID Reader", 5, 5, 4);
  tft.drawLine(0, 35, tft.width(), 35, TFT_WHITE);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Waiting for tag...", 5, 50, 4);
}

// Update display with RFID tag information
void updateDisplay(TFT_eSPI& tft, MFRC522::Uid uid) {
  tft.fillScreen(TFT_BLACK);
  
  // Title - medium font (font 4)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Bambu RFID Reader", 5, 5, 4);
  
  // Draw a line separator
  tft.drawLine(0, 35, tft.width(), 35, TFT_WHITE);
  
  int yPos = 50;
  int lineHeight = 35;
  
  // UID Hex format - font 4
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Hex:", 5, yPos, 4);
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
  tft.drawString(hexStr, 70, yPos, 4);
  yPos += lineHeight;
  
  // UID Decimal format - font 4
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Decimal:", 5, yPos, 4);
  String decStr = "";
  for (byte i = 0; i < uid.size; i++) {
    decStr += String(uid.uidByte[i]);
    if (i < uid.size - 1) {
      decStr += ":";
    }
  }
  tft.drawString(decStr, 100, yPos, 4);
  yPos += lineHeight;
  
  // Single decimal number - font 4
  unsigned long uidDecimal = 0;
  for (byte i = 0; i < uid.size; i++) {
    uidDecimal = uidDecimal * 256 + uid.uidByte[i];
  }
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("UID:", 5, yPos, 4);
  tft.drawString(String(uidDecimal), 70, yPos, 4);
}

