#pragma once

#include <Arduino.h>

// MQTT functions
bool initMQTT();
bool isMQTTConnected();
void handleMQTT();
void publishUID(unsigned long uid);
void publishUID(String uidHex, String uidDecimal, unsigned long uidNumber);

