#pragma once

#include <Arduino.h>

// Forward declarations
class IPAddress;

// WiFi connection functions
bool initWiFi();
bool isWiFiConnected();
void handleWiFi();
String getWiFiSSID();
IPAddress getWiFiIP();
