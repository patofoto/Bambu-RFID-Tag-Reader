#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

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

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("Bambu RFID Tag Reader");
  Serial.println("====================");
  
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
  
  // Card detected - print UID
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
  
  // Halt PICC
  mfrc522.PICC_HaltA();
  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  
  // Small delay before next read
  delay(1000);
}