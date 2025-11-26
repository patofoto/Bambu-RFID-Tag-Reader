#pragma once

// MQTT Configuration
// TODO: Replace with your actual MQTT broker details
// Note: This file should be added to .gitignore to keep credentials private

#define MQTT_BROKER "10.0.20.55"
#define MQTT_PORT 1883
#define MQTT_USERNAME "patofotomqtt"  // Leave empty if no authentication
#define MQTT_PASSWORD "306537"  // Leave empty if no authentication
#define MQTT_CLIENT_ID "bambu-rfid-reader"
#define MQTT_TOPIC "bambu/rfid/uid"

