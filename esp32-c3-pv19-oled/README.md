# ESP32-C3 + OLED SSD1306 — Monitor MUST PV19 Inverter

Monitor de inversor MUST PV19 via RS485/Modbus com display OLED 0.96" SSD1306 I2C, rodando em ESP32-C3.

## Hardware necessário

| Componente | Modelo |
|---|---|
| Microcontrolador | ESP32-C3 (esp32-c3-devkitm-1) |
| Display | SSD1306 OLED 0.96" 128×64 I2C |
| Conversor RS485 | MAX485 / TTL-485 |
| Inversor | MUST PV19 (110 V saída, banco 48 V) |

---

## Ligações (pinagem)

### RS485 → ESP32-C3

| MAX485 | ESP32-C3 | Descrição |
|--------|----------|-----------|
| TX     | GPIO19   | Transmissão (DE/RE controlados automaticamente) |
| RX     | GPIO18   | Recepção |
| VCC    | 3.3 V    | Alimentação |
| GND    | GND      | Terra |

> O RS485 ocupa GPIO18/19, portanto o I2C **deve** usar outros pinos.

### SSD1306 I2C → ESP32-C3

| SSD1306 | ESP32-C3 | Descrição |
|---------|----------|-----------|
| SDA     | GPIO4    | Dados I2C |
| SCL     | GPIO5    | Clock I2C |
| VCC     | 3.3 V    | Alimentação |
| GND     | GND      | Terra |

> Endereço I2C padrão: `0x3C`. Alguns módulos usam `0x3D` (altere em `address:`).

---

## Layout do display (128×64 px)

```
┌──────────────────────────────────────┐
│ B: 75%  +12A  [REDE]                 │  ← SOC + corrente bateria + fonte
│ Vs:110V   Carga: 42%                 │  ← tensão saída AC + % carga
│ Ti: 38C   Tb: 32C                    │  ← temp inversor + temp bateria
│ [▓▓▓▓▓▓▓▓▓░░░]  75%                 │  ← barra visual SOC (102px)
└──────────────────────────────────────┘
```

| Campo | Descrição |
|-------|-----------|
| `B: XX%` | Estado de carga da bateria (SOC) |
| `+/-XXA` | Corrente da bateria (+ carregando, − descarregando) |
| `[REDE/BAT]` | Fonte de energia ativa (ver tabela abaixo) |
| `Vs: XXXV` | Tensão de saída AC do inversor |
| `Carga: XX%` | % de carga do sistema (potência / capacidade nominal) |
| `Ti: XXC` | Temperatura do radiador AC do inversor |
| `Tb: XXC` | Temperatura da bateria |
| Barra SOC | Representação visual de 0–100% (102 px de largura) |

---

## Registradores Modbus utilizados

| ID ESPHome | Registrador | Tipo | Descrição |
|------------|-------------|------|-----------|
| `soc` | 113 | HOLDING | Estado de carga (%) |
| `inv_work_state` | 25201 | HOLDING | Estado de trabalho do inversor (0–6) |
| `inv_voltage` | 25206 | HOLDING | Tensão saída AC (×0.1 V) |
| `sys_load` | 25216 | HOLDING | Carga do sistema (%) |
| `temp_ac_rad` | 25233 | HOLDING | Temperatura radiador AC (°C) |
| `temp_battery` | 25269 | HOLDING | Temperatura da bateria (°C) ¹ |
| `bat_current` | 25274 | S_WORD | Corrente da bateria (A, pode ser negativo) |

> ¹ O registrador 25269 para temperatura da bateria pode não estar disponível em todos os modelos PV19.
> Se o sensor aparecer sempre como 0 ou com erro, comente/remova o bloco `temp_battery` no YAML
> e ajuste a linha 3 do display lambda para exibir apenas `Ti`.

---

## Tabela de estados do inversor (registrador 25201)

| Valor | Estado | Display |
|-------|--------|---------|
| 0 | Power on | BAT  |
| 1 | Self-test | BAT  |
| 2 | Off grid | BAT  |
| 3 | Grid (on grid) | REDE |
| 4 | Bypass | REDE |
| 5 | Charging | BAT  |
| 6 | Grid charging | REDE |

---

## Segredos necessários (`secrets.yaml`)

```yaml
wifi_ssid: "SuaRedeWifi"
wifi_password: "SuaSenhaWifi"
ap_password: "hotspotsecret"
api_key: "chave_base64_home_assistant"
ota_password: "sua_senha_ota"
```

---

## Build e flash

```bash
# Validar configuração
esphome config esp32-c3-pv19-oled.yaml

# Compilar e flash via USB (primeira vez)
esphome run esp32-c3-pv19-oled.yaml

# Atualizações via OTA (após primeira instalação)
esphome run esp32-c3-pv19-oled.yaml --device <IP_DO_ESP>
```

> **Requisito de build:** A fonte `gfonts://Roboto` é baixada do Google Fonts durante a compilação.
> Certifique-se de ter acesso à internet na máquina que compila o firmware.

---

## Personalização

- **Endereço Modbus**: padrão `0x4`. Altere em `modbus_controller > address` se seu inversor usar outro endereço.
- **Intervalo de atualização Modbus**: padrão `20s`. Altere em `modbus_controller > update_interval`.
- **Intervalo do display**: padrão `5s`. Altere em `display > update_interval`.
- **Pinos I2C**: altere `i2c_sda` / `i2c_scl` nas `substitutions` caso use outros pinos.
