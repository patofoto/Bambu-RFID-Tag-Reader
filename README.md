# Bambu RFID Tag Reader

A PlatformIO project for reading RFID tags from Bambu Labs filament spools using a LilyGO T-Display-S3 (ESP32-S3) and an RFID-RC522 module. The project displays the unique identifier (UID) of each RFID tag on both the integrated TFT display and serial monitor.

## Features

- **RFID Tag Reading**: Reads MIFARE Classic and other compatible RFID tags
- **Multiple UID Formats**: Displays UID in hex, decimal, single decimal number, and raw bytes
- **TFT Display**: Shows tag information on the integrated 1.9" ST7789 display (170x320 pixels)
- **Serial Output**: Also outputs tag information to serial monitor for debugging
- **Landscape Display**: Optimized for landscape orientation (320x170)

## Hardware Requirements

- **LilyGO T-Display-S3** (ESP32-S3 based board with integrated ST7789 TFT display)
- **RFID-RC522** module
- **Jumper wires** for connections

## Wiring Diagram

Connect the RFID-RC522 module to the LilyGO T-Display-S3 as follows:

| RC522 Pin | LilyGO T-Display-S3 Pin | Notes |
|-----------|------------------------|-------|
| SDA (SS)  | GPIO10 (P1 header, SPI_CS) | Slave Select |
| SCK       | GPIO12 (P1 header, SPI_CLK) | SPI Clock |
| MOSI      | GPIO11 (P1 header, SPI_D) | SPI Data Out |
| MISO      | GPIO13 (P1 header, SPI_Q) | SPI Data In |
| RST       | GPIO21 (P2 header) | Reset pin |
| 3.3V      | 3.3V (P1 or P2 header) | Power |
| GND       | GND (P1 or P2 header) | Ground |

**Important Notes:**
- GPIO5 is used by the LCD reset and should not be used for the RC522
- The display uses an 8-bit parallel interface, so it doesn't conflict with the SPI bus used by the RC522
- All pins used are from the exposed headers (P1 and P2)

## Installation

### Prerequisites

- [PlatformIO](https://platformio.org/) installed (VS Code extension or CLI)
- USB cable to connect the LilyGO T-Display-S3 to your computer

### Setup Steps

1. **Clone the repository:**
   ```bash
   git clone https://github.com/patofoto/Bambu-RFID-Tag-Reader.git
   cd Bambu-RFID-Tag-Reader
   ```

2. **Open in PlatformIO:**
   - If using VS Code, open the project folder
   - PlatformIO will automatically install dependencies

3. **Build the project:**
   ```bash
   pio run
   ```

4. **Upload to board:**
   ```bash
   pio run --target upload
   ```

5. **Monitor serial output:**
   ```bash
   pio device monitor
   ```

## Usage

1. **Power on**: Connect the LilyGO T-Display-S3 via USB
2. **Initial Display**: The display will show "Bambu RFID Reader" and "Waiting for tag..."
3. **Read Tag**: Place a Bambu Labs RFID tag (or any compatible MIFARE tag) near the RC522 module
4. **View Results**: The display will update showing:
   - **Hex format**: UID in hexadecimal (e.g., `AA:BB:CC:DD`)
   - **Decimal format**: UID in decimal bytes (e.g., `170:187:204:221`)
   - **UID Number**: Single decimal number (useful for database storage)
   - **Raw Bytes**: Hexadecimal bytes with 0x prefix
5. **Serial Monitor**: All information is also printed to the serial monitor at 115200 baud

## Display Layout

When a tag is detected, the display shows:
```
Bambu RFID Reader
─────────────────
Hex: AA:BB:CC:DD
Decimal: 170:187:204:221
UID Number: 2864434397
Raw Bytes: 0xAA, 0xBB, 0xCC, 0xDD
```

## Project Structure

```
Bambu-RFID-Tag-Reader/
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp            # Main application code
│   └── pin_config.h        # Pin definitions for the board
├── include/                # Header files (if needed)
├── lib/                    # Local libraries (if needed)
└── README.md               # This file
```

## Configuration

### Pin Configuration

The pin configuration can be modified in `src/main.cpp`:
- `RST_PIN`: RC522 reset pin (default: GPIO21)
- `SS_PIN`: RC522 slave select pin (default: GPIO10)

### Display Settings

Display settings are configured in `platformio.ini` via build flags. The display is set to:
- Landscape orientation (rotation 1)
- 320x170 resolution
- ST7789 driver with 8-bit parallel interface

## Libraries Used

- **MFRC522** (v1.4.10+): RFID reader library
- **TFT_eSPI** (v2.5.43+): TFT display library for ESP32
- **SPI**: Standard Arduino SPI library

## Troubleshooting

### Display Not Working
- Check that the board is powered via USB
- Verify the display backlight is on (GPIO38)
- Ensure `PIN_POWER_ON` (GPIO15) is set HIGH

### RFID Not Reading Tags
- Verify all connections are secure
- Check that the RC522 module is powered (3.3V, not 5V)
- Ensure the tag is close enough to the reader (typically 1-5cm)
- Try different tags to rule out tag issues

### Compilation Errors
- Ensure you're using PlatformIO with the correct ESP32 platform version (6.5.0)
- Check that all libraries are properly installed
- Verify the board name in `platformio.ini` matches your hardware

### Serial Monitor Not Showing Output
- Check the baud rate is set to 115200
- Ensure the USB cable supports data transfer (not just power)
- Try a different USB port or cable

## Branches

- **master**: Stable version with serial monitor output only
- **display-setup**: Version with TFT display support (current development)

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is open source. Feel free to use and modify as needed.

## Acknowledgments

- [LilyGO](https://www.lilygo.cc/) for the T-Display-S3 board
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library by Bodmer
- [MFRC522](https://github.com/miguelbalboa/MFRC522) library by miguelbalboa

## Future Enhancements

- [ ] Store tag history
- [ ] Database integration for filament tracking
- [ ] WiFi connectivity for cloud sync
- [ ] Touch screen support for navigation
- [ ] Battery voltage monitoring
- [ ] Multiple tag format support

