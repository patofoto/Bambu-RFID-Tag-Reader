# Bambu RFID Tag Reader

A PlatformIO project for reading RFID tags from Bambu Labs filament spools using a LilyGO T-Display-S3 (ESP32-S3) and an RFID-RC522 module. The project displays the unique identifier (UID) of each RFID tag on both the integrated TFT display and serial monitor.

## Features

- **RFID Tag Reading**: Reads MIFARE Classic and other compatible RFID tags
- **Multiple UID Formats**: Displays UID in decimal, single decimal number, and hex (in serial)
- **TFT Display**: Shows tag information on the integrated 1.9" ST7789 display (170x320 pixels)
- **WiFi Connectivity**: Connects to WiFi with connection progress display
- **MQTT Publishing**: Publishes UID data to MQTT broker in JSON format
- **Modular Code**: Organized into separate modules for easy maintenance
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
Decimal: 170:187:204:221
UID: 2864434397
IP: 192.168.1.100
```

During WiFi connection:
```
Bambu RFID Reader
─────────────────
Connecting to WiFi...
YourNetworkName
Attempts: 5/30...
```

## Project Structure

```
Bambu-RFID-Tag-Reader/
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp            # Main application code
│   ├── pin_config.h        # Board pin definitions
│   ├── config.h            # RFID pin configuration
│   ├── config/             # Credential files (gitignored)
│   │   ├── wifi_credentials.h
│   │   ├── wifi_credentials.h.example
│   │   ├── mqtt_credentials.h
│   │   └── mqtt_credentials.h.example
│   ├── display.h/cpp       # Display module
│   ├── rfid.h/cpp          # RFID module
│   ├── wifi_manager.h/cpp  # WiFi module
│   └── mqtt_manager.h/cpp  # MQTT module
├── include/                # Header files (if needed)
├── lib/                    # Local libraries (if needed)
└── README.md               # This file
```

## Configuration

### WiFi Configuration

1. Copy the WiFi credentials template:
   ```bash
   cp src/config/wifi_credentials.h.example src/config/wifi_credentials.h
   ```

2. Edit `src/config/wifi_credentials.h` with your WiFi details:
   ```cpp
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASSWORD "your_wifi_password"
   ```

### MQTT Configuration

1. Copy the MQTT credentials template:
   ```bash
   cp src/config/mqtt_credentials.h.example src/config/mqtt_credentials.h
   ```

2. Edit `src/config/mqtt_credentials.h` with your MQTT broker details:
   ```cpp
   #define MQTT_BROKER "192.168.1.100"
   #define MQTT_PORT 1883
   #define MQTT_TOPIC "bambu/rfid/uid"
   ```

### Pin Configuration

The pin configuration can be modified in `src/config.h`:
- `RST_PIN`: RC522 reset pin (default: GPIO21)
- `SS_PIN`: RC522 slave select pin (default: GPIO10)

**Note:** The SPI hardware pins (MOSI, MISO, SCK) must remain on their designated GPIO pins for the RC522 to work properly.

### Display Settings

Display settings are configured in `platformio.ini` via build flags. The display is set to:
- Landscape orientation (rotation 1)
- 320x170 resolution
- ST7789 driver with 8-bit parallel interface

## Libraries Used

- **MFRC522** (v1.4.10+): RFID reader library
- **TFT_eSPI** (v2.5.43+): TFT display library for ESP32
- **PubSubClient** (v2.8+): MQTT client library
- **WiFi**: ESP32 WiFi library
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

- **master**: Initial version with serial monitor output only
- **display-setup**: Version with TFT display support
- **refactor-modules**: Modular code structure with WiFi and MQTT support (current development)

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is open source. Feel free to use and modify as needed.

## Acknowledgments

- [LilyGO](https://www.lilygo.cc/) for the T-Display-S3 board
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library by Bodmer
- [MFRC522](https://github.com/miguelbalboa/MFRC522) library by miguelbalboa

## MQTT Data Format

When a tag is scanned, the device publishes the following JSON to the configured MQTT topic:

```json
{
  "hex": "04:XX:XX:XX:XX:XX:XX",
  "decimal": "4:XX:XX:XX:XX:XX:XX",
  "uid": 1234567890
}
```

## Future Enhancements

- [x] WiFi connectivity for cloud sync
- [x] MQTT publishing for data integration
- [x] Modular code structure
- [ ] OTA (Over-The-Air) updates
- [ ] Store tag history
- [ ] Database integration for filament tracking
- [ ] Touch screen support for navigation
- [ ] Battery voltage monitoring
- [ ] Web interface for configuration

