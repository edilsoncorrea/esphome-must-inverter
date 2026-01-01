#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <ModbusRTU.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "config.h"

// ==================== GLOBAL OBJECTS ====================
AsyncWebServer server(80);
Preferences prefs;
ModbusRTU modbus;
WiFiManager wifiManager;

// Factory reset button
const int FACTORY_RESET_PIN = 0;  // GPIO0 (BOOT button)
unsigned long resetButtonPressed = 0;
bool resetInProgress = false;

// ==================== SENSOR DATA STRUCTURE ====================
struct SensorData {
  // Charger Stats (15201-15221)
  float chargerVoltage;
  float chargerCurrent;
  float chargerPower;
  float pvVoltage;
  float pvCurrent;
  float pvPower;
  float batteryVoltage;
  float batteryCurrent;
  float batteryPower;
  float batterySOC;
  float batteryTemp;
  float deviceTemp;
  
  // Inverter Stats (25201-25274)
  int inverterMode;
  float acVoltage;
  float acCurrent;
  float acFrequency;
  float acPower;
  float loadPercent;
  float dcVoltage;
  float maxChargeCurrent;
  float maxDischargeCurrent;
  
  // Accumulated Data
  float chargerAccumulatedPower;
  float dischargerAccumulatedPower;
  float acChargerAccumulatedPower;
  float acDischargerAccumulatedPower;
  float totalChargerPower;
  float totalDischargerPower;
  
  // Status
  unsigned long lastUpdate;
  bool modbusError;
  bool demoMode;
  int failedReadCount;
};

SensorData sensorData = {0};

// ==================== DEMO MODE FUNCTIONS ====================
void generateDemoData() {
  Serial.println("Generating demo data - Inverter not connected");
  
  // Simulate realistic solar inverter values
  unsigned long time = millis() / 1000;
  float variance = sin(time / 30.0) * 0.1; // Slow variation
  
  // Solar PV - simulating partial sun
  sensorData.pvVoltage = 85.0 + variance * 10.0;
  sensorData.pvCurrent = 4.5 + variance * 2.0;
  sensorData.pvPower = sensorData.pvVoltage * sensorData.pvCurrent;
  
  // Charger
  sensorData.chargerVoltage = 54.2 + variance * 0.5;
  sensorData.chargerCurrent = 8.2 + variance * 1.0;
  sensorData.chargerPower = 445.0 + variance * 50.0;
  
  // Battery - simulating charging state
  sensorData.batteryVoltage = 52.8 + variance * 0.3;
  sensorData.batteryCurrent = 8.0 + variance * 1.5;
  sensorData.batteryPower = 422.0 + variance * 40.0;
  sensorData.batterySOC = 65.0 + variance * 5.0;
  sensorData.batteryTemp = 25.0 + variance * 2.0;
  
  // Inverter - simulating off-grid mode with load
  sensorData.inverterMode = 3; // Off-Grid
  sensorData.acVoltage = 220.0 + variance * 2.0;
  sensorData.acCurrent = 2.5 + variance * 0.5;
  sensorData.acFrequency = 50.0 + variance * 0.1;
  sensorData.acPower = 550.0 + variance * 50.0;
  sensorData.loadPercent = 27.5 + variance * 5.0;
  sensorData.dcVoltage = 52.8 + variance * 0.3;
  sensorData.maxChargeCurrent = 60.0;
  sensorData.maxDischargeCurrent = 60.0;
  
  // Temperatures
  sensorData.deviceTemp = 42.0 + variance * 3.0;
  
  // Accumulated energy - simulating some usage history
  sensorData.chargerAccumulatedPower = 125.5;
  sensorData.dischargerAccumulatedPower = 98.3;
  sensorData.acChargerAccumulatedPower = 45.2;
  sensorData.acDischargerAccumulatedPower = 87.6;
  sensorData.totalChargerPower = 170.7;
  sensorData.totalDischargerPower = 185.9;
  
  sensorData.lastUpdate = millis();
  sensorData.modbusError = true;
  sensorData.demoMode = true;
}

// ==================== MODBUS FUNCTIONS ====================
bool readModbusRegister(uint16_t address, uint16_t* value) {
  if (!modbus.slave()) {
    modbus.readHreg(MODBUS_SLAVE_ID, address, value, 1);
    
    unsigned long timeout = millis() + 1000;
    while (millis() < timeout) {
      modbus.task();
      delay(10);
      if (modbus.slave() == MODBUS_SLAVE_ID) {
        return true;
      }
    }
  }
  return false;
}

float readModbusFloat(uint16_t address, float scale = 1.0) {
  uint16_t value;
  if (readModbusRegister(address, &value)) {
    return (float)value * scale;
  }
  return 0.0;
}

int readModbusInt(uint16_t address) {
  uint16_t value;
  if (readModbusRegister(address, &value)) {
    return (int)value;
  }
  return 0;
}

void updateSensorData() {
  Serial.println("Reading Modbus sensors...");
  
  // Try to read first register to test connection
  uint16_t testValue;
  bool connectionOk = readModbusRegister(15201, &testValue);
  
  if (!connectionOk) {
    sensorData.failedReadCount++;
    Serial.printf("Modbus read failed (attempt %d/%d)\n", 
                  sensorData.failedReadCount, DEMO_DETECTION_FAILED_READS);
    
    if (DEMO_MODE_ENABLED && sensorData.failedReadCount >= DEMO_DETECTION_FAILED_READS) {
      // Switch to demo mode
      generateDemoData();
      return;
    } else {
      sensorData.modbusError = true;
      return;
    }
  }
  
  // Connection successful - reset counters and disable demo mode
  sensorData.failedReadCount = 0;
  sensorData.demoMode = false;
  
  // Charger Stats (15201-15221)
  sensorData.chargerVoltage = readModbusFloat(15201, 0.1);      // V
  sensorData.chargerCurrent = readModbusFloat(15202, 0.01);     // A
  sensorData.chargerPower = readModbusFloat(15203, 1.0);        // W
  
  sensorData.pvVoltage = readModbusFloat(15205, 0.1);           // V
  sensorData.pvCurrent = readModbusFloat(15206, 0.01);          // A
  sensorData.pvPower = readModbusFloat(15207, 1.0);             // W
  
  sensorData.batteryVoltage = readModbusFloat(15213, 0.1);      // V
  sensorData.batteryCurrent = readModbusFloat(15214, 0.01);     // A
  sensorData.batteryPower = readModbusFloat(15215, 1.0);        // W
  sensorData.batterySOC = readModbusFloat(15219, 1.0);          // %
  
  sensorData.batteryTemp = readModbusFloat(15221, 0.1) - 100.0; // °C
  sensorData.deviceTemp = readModbusFloat(15220, 0.1) - 100.0;  // °C
  
  // Inverter Stats (25201-25274)
  sensorData.inverterMode = readModbusInt(25201);
  sensorData.acVoltage = readModbusFloat(25205, 0.1);           // V
  sensorData.acCurrent = readModbusFloat(25206, 0.01);          // A
  sensorData.acFrequency = readModbusFloat(25207, 0.01);        // Hz
  sensorData.acPower = readModbusFloat(25213, 1.0);             // W
  sensorData.loadPercent = readModbusFloat(25209, 1.0);         // %
  
  sensorData.dcVoltage = readModbusFloat(25211, 0.1);           // V
  sensorData.maxChargeCurrent = readModbusFloat(25235, 1.0);    // A
  sensorData.maxDischargeCurrent = readModbusFloat(25236, 1.0); // A
  
  // Accumulated Power (requires double register reads for 32-bit values)
  uint16_t high, low;
  
  // Charger Accumulated Power (kWh) - registers 15231-15232
  if (readModbusRegister(15231, &high) && readModbusRegister(15232, &low)) {
    sensorData.chargerAccumulatedPower = ((uint32_t)high << 16 | low) * 0.1;
  }
  
  // Discharger Accumulated Power (kWh) - registers 15233-15234
  if (readModbusRegister(15233, &high) && readModbusRegister(15234, &low)) {
    sensorData.dischargerAccumulatedPower = ((uint32_t)high << 16 | low) * 0.1;
  }
  
  // AC Charger Accumulated Power (kWh) - registers 15235-15236
  if (readModbusRegister(15235, &high) && readModbusRegister(15236, &low)) {
    sensorData.acChargerAccumulatedPower = ((uint32_t)high << 16 | low) * 0.1;
  }
  
  // AC Discharger Accumulated Power (kWh) - registers 15237-15238
  if (readModbusRegister(15237, &high) && readModbusRegister(15238, &low)) {
    sensorData.acDischargerAccumulatedPower = ((uint32_t)high << 16 | low) * 0.1;
  }
  
  // Calculate totals
  sensorData.totalChargerPower = sensorData.chargerAccumulatedPower + sensorData.acChargerAccumulatedPower;
  sensorData.totalDischargerPower = sensorData.dischargerAccumulatedPower + sensorData.acDischargerAccumulatedPower;
  
  sensorData.lastUpdate = millis();
  sensorData.modbusError = false;
  
  Serial.println("Sensor data updated successfully");
}

// ==================== API FUNCTIONS ====================
String getInverterModeText(int mode) {
  switch(mode) {
    case 0: return "Power On";
    case 1: return "Standby";
    case 2: return "Mains";
    case 3: return "Off-Grid";
    case 4: return "Bypass";
    case 5: return "Charging";
    case 6: return "Fault";
    default: return "Unknown";
  }
}

bool checkAuthentication(AsyncWebServerRequest *request) {
  if (!request->authenticate(DEFAULT_API_USER, DEFAULT_API_PASS)) {
    request->requestAuthentication();
    return false;
  }
  return true;
}

void handleRoot(AsyncWebServerRequest *request) {
  // Check if WiFi is connected (not in AP mode)
  bool isConfigured = WiFi.status() == WL_CONNECTED && String(WiFi.SSID()) != String(AP_SSID);
  
  if (!isConfigured) {
    // Serve configuration page when not connected
    request->send(LittleFS, "/config.html", "text/html");
  } else {
    // Serve full monitoring dashboard when connected
    request->send(LittleFS, "/index.html", "text/html");
  }
}

void handleApiSensors(AsyncWebServerRequest *request) {
  if (!checkAuthentication(request)) return;
  
  // Create JSON document
  JsonDocument doc;
  
  // Charger section
  JsonObject charger = doc["charger"].to<JsonObject>();
  charger["voltage"] = round(sensorData.chargerVoltage * 10) / 10.0;
  charger["current"] = round(sensorData.chargerCurrent * 100) / 100.0;
  charger["power"] = round(sensorData.chargerPower);
  charger["accumulated_power"] = round(sensorData.chargerAccumulatedPower * 10) / 10.0;
  
  // PV section
  JsonObject pv = doc["pv"].to<JsonObject>();
  pv["voltage"] = round(sensorData.pvVoltage * 10) / 10.0;
  pv["current"] = round(sensorData.pvCurrent * 100) / 100.0;
  pv["power"] = round(sensorData.pvPower);
  
  // Battery section
  JsonObject battery = doc["battery"].to<JsonObject>();
  battery["voltage"] = round(sensorData.batteryVoltage * 10) / 10.0;
  battery["current"] = round(sensorData.batteryCurrent * 100) / 100.0;
  battery["power"] = round(sensorData.batteryPower);
  battery["soc"] = round(sensorData.batterySOC);
  battery["temperature"] = round(sensorData.batteryTemp * 10) / 10.0;
  
  // Inverter section
  JsonObject inverter = doc["inverter"].to<JsonObject>();
  inverter["mode"] = getInverterModeText(sensorData.inverterMode);
  inverter["mode_id"] = sensorData.inverterMode;
  inverter["ac_voltage"] = round(sensorData.acVoltage * 10) / 10.0;
  inverter["ac_current"] = round(sensorData.acCurrent * 100) / 100.0;
  inverter["ac_frequency"] = round(sensorData.acFrequency * 100) / 100.0;
  inverter["ac_power"] = round(sensorData.acPower);
  inverter["load_percent"] = round(sensorData.loadPercent);
  inverter["dc_voltage"] = round(sensorData.dcVoltage * 10) / 10.0;
  inverter["max_charge_current"] = round(sensorData.maxChargeCurrent);
  inverter["max_discharge_current"] = round(sensorData.maxDischargeCurrent);
  inverter["accumulated_power"] = round(sensorData.acDischargerAccumulatedPower * 10) / 10.0;
  
  // Totals section
  JsonObject totals = doc["totals"].to<JsonObject>();
  totals["total_charged"] = round(sensorData.totalChargerPower * 10) / 10.0;
  totals["total_discharged"] = round(sensorData.totalDischargerPower * 10) / 10.0;
  totals["device_temperature"] = round(sensorData.deviceTemp * 10) / 10.0;
  
  // Metadata
  doc["last_update"] = sensorData.lastUpdate;
  doc["uptime"] = millis() / 1000;
  doc["modbus_error"] = sensorData.modbusError;
  doc["demo_mode"] = sensorData.demoMode;
  
  String response;
  serializeJsonPretty(doc, response);
  
  request->send(200, "application/json", response);
}

void handleApiStatus(AsyncWebServerRequest *request) {
  JsonDocument doc;
  
  doc["device_name"] = DEVICE_NAME;
  doc["ip_address"] = WiFi.localIP().toString();
  doc["mac_address"] = WiFi.macAddress();
  doc["wifi_ssid"] = WiFi.SSID();
  doc["wifi_rssi"] = WiFi.RSSI();
  doc["uptime_seconds"] = millis() / 1000;
  doc["free_heap"] = ESP.getFreeHeap();
  doc["modbus_connected"] = !sensorData.modbusError;
  
  String response;
  serializeJsonPretty(doc, response);
  
  request->send(200, "application/json", response);
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404: Not Found");
}

// ==================== WIFI MANAGER ====================
void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println("AP IP: " + WiFi.softAPIP().toString());
  Serial.println("SSID: " + String(AP_SSID));
}

// ==================== FACTORY RESET ====================
void checkFactoryReset() {
  if (digitalRead(FACTORY_RESET_PIN) == LOW) {
    if (resetButtonPressed == 0) {
      resetButtonPressed = millis();
      Serial.println("Factory reset button pressed...");
    }
    
    if (millis() - resetButtonPressed > 5000 && !resetInProgress) {
      resetInProgress = true;
      Serial.println("FACTORY RESET TRIGGERED!");
      
      // Clear WiFi credentials
      wifiManager.resetSettings();
      
      // Clear preferences
      prefs.begin("credentials", false);
      prefs.clear();
      prefs.end();
      
      Serial.println("All settings cleared. Restarting...");
      delay(1000);
      ESP.restart();
    }
  } else {
    resetButtonPressed = 0;
    resetInProgress = false;
  }
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=================================");
  Serial.println("   MUST Inverter API Device");
  Serial.println("   PlatformIO C++ Version");
  Serial.println("=================================\n");
  
  // Configure factory reset button
  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
  
  // Initialize LittleFS
  if (!LittleFS.begin(true)) {
    Serial.println("✗ LittleFS mount failed!");
    Serial.println("  Please upload filesystem with: pio run --target uploadfs");
  } else {
    Serial.println("✓ LittleFS mounted");
  }
  
  // Initialize Modbus
  MODBUS_SERIAL.begin(19200, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);
  
  #ifdef MODBUS_FLOW_CONTROL_ENABLED
    // Configure DE/RE pins for RS485 flow control
    pinMode(MODBUS_DE_PIN, OUTPUT);
    pinMode(MODBUS_RE_PIN, OUTPUT);
    digitalWrite(MODBUS_DE_PIN, LOW);  // Disable transmit by default
    digitalWrite(MODBUS_RE_PIN, LOW);  // Enable receive by default
    Serial.printf("✓ RS485 flow control enabled (DE/RE: GPIO%d)\n", MODBUS_DE_PIN);
  #endif
  
  modbus.begin(&MODBUS_SERIAL);
  modbus.master();
  Serial.println("✓ Modbus RTU initialized");
  
  #if defined(CONFIG_IDF_TARGET_ESP32C3)
    Serial.println("  Platform: ESP32-C3");
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    Serial.println("  Platform: ESP32-S3");
  #else
    Serial.println("  Platform: ESP32");
  #endif
  Serial.printf("  Pins: TX=%d, RX=%d\n", MODBUS_TX_PIN, MODBUS_RX_PIN);
  
  // Configure WiFiManager
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(180);  // 3 minutes timeout
  
  // Custom parameters for API credentials
  WiFiManagerParameter custom_text("<p>Configuração de credenciais da API</p>");
  WiFiManagerParameter api_user("api_user", "Usuário API", DEFAULT_API_USER, 40);
  WiFiManagerParameter api_pass("api_pass", "Senha API", DEFAULT_API_PASS, 40);
  
  wifiManager.addParameter(&custom_text);
  wifiManager.addParameter(&api_user);
  wifiManager.addParameter(&api_pass);
  
  // Try to connect
  Serial.println("Connecting to WiFi...");
  if (!wifiManager.autoConnect(AP_SSID, AP_PASSWORD)) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  }
  
  Serial.println("✓ WiFi connected!");
  Serial.print("  IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("  SSID: ");
  Serial.println(WiFi.SSID());
  
  // Save API credentials if provided
  if (strlen(api_user.getValue()) > 0) {
    prefs.begin("credentials", false);
    prefs.putString("api_user", api_user.getValue());
    prefs.putString("api_pass", api_pass.getValue());
    prefs.end();
    Serial.println("✓ API credentials saved");
  }
  
  // Setup web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/sensors", HTTP_GET, handleApiSensors);
  server.on("/api/status", HTTP_GET, handleApiStatus);
  
  // Serve static files (CSS, JS)
  server.serveStatic("/css", LittleFS, "/css/");
  server.serveStatic("/js", LittleFS, "/js/");
  
  // Config portal redirect (for reconfiguring WiFi)
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Config portal requested - restarting in AP mode...");
    request->send(200, "text/plain", "Restarting in configuration mode...");
    delay(1000);
    wifiManager.resetSettings();
    ESP.restart();
  });
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("✓ HTTP server started");
  
  Serial.println("\n=================================");
  Serial.println("Device ready!");
  Serial.println("Access: http://" + WiFi.localIP().toString());
  Serial.println("API: /api/sensors");
  Serial.println("=================================\n");
}

// ==================== LOOP ====================
unsigned long lastModbusUpdate = 0;
const unsigned long MODBUS_UPDATE_INTERVAL = 5000;  // 5 seconds

void loop() {
  // Check factory reset button
  checkFactoryReset();
  
  // Update sensor data periodically
  if (millis() - lastModbusUpdate > MODBUS_UPDATE_INTERVAL) {
    updateSensorData();
    lastModbusUpdate = millis();
  }
  
  // Keep Modbus task running
  modbus.task();
  
  delay(10);
}
