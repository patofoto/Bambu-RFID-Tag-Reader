#pragma once

#include <SPI.h>
#include <MFRC522.h>

// RFID initialization
void initRFID(MFRC522& mfrc522);

// RFID reading functions
bool readRFIDTag(MFRC522& mfrc522, MFRC522::Uid& uid);

// UID formatting functions
String formatUIDHex(MFRC522::Uid uid);
String formatUIDDecimal(MFRC522::Uid uid);
unsigned long formatUIDNumber(MFRC522::Uid uid);
void printUIDToSerial(MFRC522::Uid uid);

