#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// Device Configuration
// ============================================
#define DEVICE_NAME "MUST-Inverter-API"
#define FIRMWARE_VERSION "1.0.0"

// ============================================
// WiFi AP Configuration
// ============================================
#define AP_SSID "must-inverter-api"
#define AP_PASSWORD "inverter123"
#define AP_CHANNEL 1
#define AP_HIDDEN false
#define AP_MAX_CLIENTS 4

// Static IP for AP mode
#define AP_IP_ADDRESS IPAddress(192, 168, 4, 1)
#define AP_GATEWAY IPAddress(192, 168, 4, 1)
#define AP_SUBNET IPAddress(255, 255, 255, 0)

// ============================================
// Default API Credentials
// ============================================
#define DEFAULT_API_USER "admin"
#define DEFAULT_API_PASS "admin123"

// ============================================
// RS485/Modbus Configuration
// ============================================
#define MODBUS_TX_PIN 19
#define MODBUS_RX_PIN 18
#define MODBUS_BAUD 19200
#define MODBUS_SLAVE_ID 0x04
#define MODBUS_TIMEOUT_MS 1000
#define MODBUS_READ_INTERVAL_MS 20000  // 20 seconds

// ============================================
// Reset Button Configuration
// ============================================
#define RESET_BUTTON_PIN 0  // BOOT button on ESP32 DevKit
#define RESET_BUTTON_HOLD_TIME_MS 5000  // 5 seconds

// ============================================
// Web Server Configuration
// ============================================
#define WEB_SERVER_PORT 80
#define ENABLE_CORS true

// ============================================
// EEPROM/Preferences Configuration
// ============================================
#define PREFS_NAMESPACE "inverter"
#define PREFS_KEY_API_USER "api_user"
#define PREFS_KEY_API_PASS "api_pass"
#define PREFS_KEY_WIFI_CONFIGURED "wifi_cfg"

// ============================================
// Demo Mode Configuration
// ============================================
#define DEMO_MODE_ENABLED true  // Enable demo mode when inverter not connected
#define DEMO_DETECTION_FAILED_READS 3  // Number of failed reads before activating demo

// ============================================
// LED Configuration (optional)
// ============================================
#define LED_BUILTIN 2
#define LED_BLINK_INTERVAL_MS 1000

#endif // CONFIG_H
