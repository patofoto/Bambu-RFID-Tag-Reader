#pragma once

#include <Arduino.h>

// Forward declarations
class IPAddress;
class TFT_eSPI;

// WiFi connection functions
bool initWiFi(TFT_eSPI* tft = nullptr);
bool isWiFiConnected();
void handleWiFi();
String getWiFiSSID();
IPAddress getWiFiIP();
