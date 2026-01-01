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
// Pin configuration varies by ESP32 variant
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  // ESP32-C3: Only has Serial and Serial1
  #define MODBUS_SERIAL Serial1
  #define MODBUS_TX_PIN 21
  #define MODBUS_RX_PIN 20
  // Optional: Control DE/RE pins for RS485 transceiver
  // #define MODBUS_DE_PIN 10
  // #define MODBUS_RE_PIN 10
  
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  // ESP32-S3: Has Serial, Serial1, Serial2
  #define MODBUS_SERIAL Serial1
  #define MODBUS_TX_PIN 17
  #define MODBUS_RX_PIN 18
  // Optional: Control DE/RE pins for RS485 transceiver
  // #define MODBUS_DE_PIN 16
  // #define MODBUS_RE_PIN 16
  
#else
  // ESP32 original: Has Serial, Serial1, Serial2
  #define MODBUS_SERIAL Serial2
  #define MODBUS_TX_PIN 19
  #define MODBUS_RX_PIN 18
  // Optional: Control DE/RE pins for RS485 transceiver
  // #define MODBUS_DE_PIN 4
  // #define MODBUS_RE_PIN 4
#endif

#define MODBUS_BAUD 19200
#define MODBUS_SLAVE_ID 0x04
#define MODBUS_TIMEOUT_MS 1000
#define MODBUS_READ_INTERVAL_MS 20000  // 20 seconds

// If DE/RE pins are defined, enable flow control
#if defined(MODBUS_DE_PIN) && defined(MODBUS_RE_PIN)
  #define MODBUS_FLOW_CONTROL_ENABLED
#endif

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
#ifdef CONFIG_IDF_TARGET_ESP32C3
  #ifdef LED_BUILTIN
    #undef LED_BUILTIN
  #endif
  #define LED_BUILTIN 8  // ESP32-C3 built-in LED
#else
  #ifndef LED_BUILTIN
    #define LED_BUILTIN 2
  #endif
#endif
#define LED_BLINK_INTERVAL_MS 1000

#endif // CONFIG_H
