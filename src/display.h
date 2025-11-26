#pragma once

#include <TFT_eSPI.h>
#include <MFRC522.h>
#include <WiFi.h>

// Display initialization
void initDisplay(TFT_eSPI& tft);

// Display update functions
void updateDisplay(TFT_eSPI& tft, MFRC522::Uid uid, IPAddress ip = IPAddress(0, 0, 0, 0));
void showWaitingMessage(TFT_eSPI& tft);
void showWiFiConnecting(TFT_eSPI& tft, const char* ssid, int attempts);
void showWiFiConnected(TFT_eSPI& tft, IPAddress ip);
void showWiFiFailed(TFT_eSPI& tft);

