#include <Arduino.h>
#include <ModbusRTU.h>

// Pinos UART (ajuste conforme seu ESP32-C3)
#define RXD2 20  // GPIO20 - conecta ao TX do ESP32 DevKit (GPIO19)
#define TXD2 21  // GPIO21 - conecta ao RX do ESP32 DevKit (GPIO18)

ModbusRTU mb;

// Registradores Modbus simulados (holding registers)
uint16_t holdingRegs[30000];

// Endereço Modbus do inversor
const uint8_t MODBUS_ADDRESS = 0x04;

void setupModbusRegisters() {
  // Limpa todos os registros
  memset(holdingRegs, 0, sizeof(holdingRegs));
  
  // ===== Registros de Bateria =====
  holdingRegs[113] = 85;  // SOC: 85%
  
  // ===== Registros do Carregador Solar (15xxx) =====
  holdingRegs[15201] = 3;   // Charger workstate: 3 = charging
  holdingRegs[15202] = 2;   // MPPT state: 2 = MPPT tracking
  holdingRegs[15203] = 1;   // Charging state: 1 = bulk charging
  
  holdingRegs[15205] = 950;  // PV1 voltage: 95.0V (mult by 0.1)
  holdingRegs[16205] = 0;    // PV2 voltage: 0V (sem PV2)
  holdingRegs[15206] = 528;  // Battery voltage charger: 52.8V
  holdingRegs[15207] = 185;  // PV1 Current: 18.5A
  holdingRegs[16207] = 0;    // PV2 Current: 0A
  holdingRegs[15208] = 1757; // PV1 Power: 1757W
  holdingRegs[16208] = 0;    // PV2 Power: 0W
  
  holdingRegs[15211] = 1;    // Battery Relay: ON
  holdingRegs[15212] = 1;    // PV Relay: ON
  
  holdingRegs[15213] = 0;    // Charger error: nenhum
  holdingRegs[15214] = 0;    // Charger warning: nenhum
  
  holdingRegs[15217] = 0;    // Accumulated charger MWh
  holdingRegs[15218] = 1234; // Accumulated charger kWh: 123.4 kWh
  
  // ===== Registros do Inversor (25xxx) =====
  holdingRegs[25201] = 2;    // Inverter work state: 2 = normal
  holdingRegs[25205] = 525;  // Battery voltage: 52.5V
  holdingRegs[25206] = 1270; // Inverter voltage: 127.0V
  holdingRegs[25207] = 1255; // Grid voltage: 125.5V
  holdingRegs[25208] = 600;  // Grid frequency: 60.0 Hz (valor * 0.1)
  
  // Potências (signed values)
  holdingRegs[25213] = 450;  // Inverter power: 450W
  holdingRegs[25214] = 100;  // Grid power: 100W (comprando)
  holdingRegs[25215] = 550;  // Load power: 550W
  holdingRegs[25216] = 22;   // System load: 22%
  
  // Temperaturas
  holdingRegs[25233] = 45;   // AC radiator temp: 45°C
  holdingRegs[25234] = 42;   // Transformer temp: 42°C
  holdingRegs[25235] = 38;   // DC Radiator temp: 38°C
  
  // Energia acumulada
  holdingRegs[25245] = 0;    // INV Charger MWh
  holdingRegs[25246] = 567;  // INV Charger kWh: 56.7 kWh
  holdingRegs[25247] = 0;    // Discharger MWh
  holdingRegs[25248] = 890;  // Discharger kWh: 89.0 kWh
  holdingRegs[25249] = 0;    // Buy MWh
  holdingRegs[25250] = 456;  // Buy kWh: 45.6 kWh
  holdingRegs[25251] = 0;    // Sell MWh
  holdingRegs[25252] = 123;  // Sell kWh: 12.3 kWh
  holdingRegs[25253] = 1;    // Load MWh
  holdingRegs[25254] = 2345; // Load kWh: 1234.5 kWh
  holdingRegs[25259] = 0;    // Grid charge MWh
  holdingRegs[25260] = 234;  // Grid charge kWh: 23.4 kWh
  
  holdingRegs[25261] = 0;    // Inverter error 1: nenhum
  holdingRegs[25262] = 0;    // Inverter error 2: nenhum
  holdingRegs[25263] = 0;    // Inverter error 3: nenhum
  holdingRegs[25265] = 0;    // Inverter warning 1: nenhum
  holdingRegs[25266] = 0;    // Inverter warning 2: nenhum
  
  holdingRegs[25273] = -50;  // Battery power: -50W (descarregando)
  holdingRegs[25274] = -10;  // Battery current: -1.0A (descarregando)
  
  // ===== Registros de Configuração (10xxx e 20xxx) =====
  holdingRegs[10103] = 540;  // Float voltage: 54.0V
  holdingRegs[10104] = 584;  // Absorb voltage: 58.4V
  
  holdingRegs[20109] = 1;    // Energy use mode: SBU
  holdingRegs[20112] = 0;    // Solar use aim: LBU
  holdingRegs[20118] = 500;  // Battery stop discharge: 50.0V
  holdingRegs[20119] = 584;  // Battery stop charge: 58.4V
  holdingRegs[20125] = 300;  // Max grid charger current: 30.0A
  holdingRegs[20127] = 480;  // Battery low voltage: 48.0V
  holdingRegs[20128] = 584;  // Battery high voltage: 58.4V
  holdingRegs[20132] = 600;  // Max combine charger: 60.0A
  holdingRegs[20142] = 0;    // System settings
  holdingRegs[20143] = 0;    // Charger source priority: CSO
}

// Callback para leitura de registros
uint16_t cbRead(TRegister* reg, uint16_t val) {
  return val;
}

// Callback para escrita de registros
uint16_t cbWrite(TRegister* reg, uint16_t val) {
  Serial.print("Write to register ");
  Serial.print(reg->address.address);
  Serial.print(": ");
  Serial.println(val);
  return val;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=================================");
  Serial.println("MUST Inverter Modbus Simulator");
  Serial.println("ESP32-C3");
  Serial.println("=================================");
  Serial.println("Device Address: 0x04");
  Serial.println("Baud Rate: 19200");
  Serial.println("=================================\n");

  // Configura registradores simulados
  setupModbusRegisters();

  // Configura UART para Modbus (ESP32-C3 usa Serial1)
  Serial1.begin(19200, SERIAL_8N1, RXD2, TXD2);
  
  // Configura Modbus RTU
  mb.begin(&Serial1);
  mb.slave(MODBUS_ADDRESS);
  
  // Adiciona intervalos de registros específicos que o ESPHome usa
  mb.addHreg(0, 0, 200);           // 0-199 (inclui reg 113)
  mb.addHreg(10100, 0, 10);        // 10100-10109 (float/absorb voltage)
  mb.addHreg(15200, 0, 100);       // 15200-15299 (charger registers)
  mb.addHreg(16200, 0, 20);        // 16200-16219 (PV2 registers)
  mb.addHreg(20100, 0, 100);       // 20100-20199 (config registers)
  mb.addHreg(25200, 0, 100);       // 25200-25299 (inverter registers)
  
  // Copia valores simulados para os registros Modbus
  for (uint16_t i = 0; i < 30000; i++) {
    if (holdingRegs[i] != 0) {
      mb.Hreg(i, holdingRegs[i]);
    }
  }
  
  Serial.println("✓ Simulator ready!");
  Serial.println("Waiting for Modbus requests...\n");
}

unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 5000; // Atualiza valores a cada 5s

void updateSimulatedValues() {
  // Simula variação de valores
  static float soc = 30.0;  // Inicia em 30%
  static float pv_voltage = 95.0;
  static float battery_voltage = 52.5;
  static float grid_voltage = 125.5;
  static float grid_frequency = 60.0;
  static int16_t battery_power = -50;
  
  // Tensão da rede: 123V a 128V
  grid_voltage += random(-3, 4) * 0.1;
  if (grid_voltage < 123.0) grid_voltage = 123.0;
  if (grid_voltage > 128.0) grid_voltage = 128.0;
  holdingRegs[25207] = (uint16_t)(grid_voltage * 10);
  mb.Hreg(25207, holdingRegs[25207]);
  
  // Frequência da rede: 59Hz a 61Hz
  grid_frequency += random(-2, 3) * 0.1;
  if (grid_frequency < 59.0) grid_frequency = 59.0;
  if (grid_frequency > 61.0) grid_frequency = 61.0;
  holdingRegs[25208] = (uint16_t)(grid_frequency * 10);
  mb.Hreg(25208, holdingRegs[25208]);
  
  // SOC: aumenta gradativamente quando há rede (simulando carga)
  if (grid_voltage > 120.0) {  // Se há tensão de rede
    if (soc < 100.0) {
      soc += random(1, 4) * 0.1;  // Aumenta entre 0.1% e 0.3% por ciclo
      if (soc > 100.0) soc = 100.0;
    }
  } else {
    // Sem rede, descarrega lentamente
    soc -= random(0, 2) * 0.1;
    if (soc < 30.0) soc = 30.0;
  }
  holdingRegs[113] = (uint16_t)soc;
  mb.Hreg(113, holdingRegs[113]);
  
  // Varia tensão PV
  pv_voltage += random(-10, 11) * 0.1;
  if (pv_voltage < 80) pv_voltage = 80;
  if (pv_voltage > 100) pv_voltage = 100;
  holdingRegs[15205] = (uint16_t)(pv_voltage * 10);
  mb.Hreg(15205, holdingRegs[15205]);
  
  // Tensão bateria aumenta conforme SOC
  battery_voltage = 48.0 + (soc / 100.0) * 8.0;  // 48V a 56V
  holdingRegs[25205] = (uint16_t)(battery_voltage * 10);
  mb.Hreg(25205, holdingRegs[25205]);
  
  // Potência bateria: negativa quando carregando
  if (soc < 100.0 && grid_voltage > 120.0) {
    battery_power = -random(50, 150);  // Carregando: -50W a -150W
  } else {
    battery_power = random(20, 100);   // Descarregando: 20W a 100W
  }
  holdingRegs[25273] = (uint16_t)battery_power;
  mb.Hreg(25273, holdingRegs[25273]);
  
  Serial.print("📊 SOC: ");
  Serial.print(soc, 1);
  Serial.print("% | Grid: ");
  Serial.print(grid_voltage, 1);
  Serial.print("V ");
  Serial.print(grid_frequency, 1);
  Serial.print("Hz | Bat: ");
  Serial.print(battery_voltage, 1);
  Serial.print("V | Power: ");
  Serial.print(battery_power);
  Serial.println("W");
}

void loop() {
  mb.task();
  
  // Atualiza valores simulados periodicamente
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    lastUpdate = millis();
    updateSimulatedValues();
  }
  
  yield();
}
