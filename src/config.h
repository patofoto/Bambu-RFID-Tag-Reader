#pragma once

// RFID-RC522 Pin Configuration for LilyGO T3-S3
// Using only exposed pins from the board headers
#define RST_PIN         21   // Reset pin (GPIO21 on P2 header)
#define SS_PIN          13   // Slave Select (SDA) pin (GPIO13 on P1 header, labeled SPI_Q)

// SPI pins matching exposed pins on P1 header
#define SPI_MOSI        11   // GPIO11 (SPI_D on P1 header)
#define SPI_MISO        10   // GPIO10 (SPI_CS on P1 header)
#define SPI_SCK         12   // GPIO12 (SPI_CLK on P1 header)

