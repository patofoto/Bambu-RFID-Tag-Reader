#pragma once

#include <TFT_eSPI.h>
#include <MFRC522.h>

// Display initialization
void initDisplay(TFT_eSPI& tft);

// Display update functions
void updateDisplay(TFT_eSPI& tft, MFRC522::Uid uid);
void showWaitingMessage(TFT_eSPI& tft);

