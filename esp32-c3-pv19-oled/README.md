# ESP32-C3 + OLED SSD1306 — Monitor MUST PV19 Inverter

Display OLED 0.96" SSD1306 I2C para monitoramento do inversor MUST PV19, rodando em ESP32-C3.  
Os dados são lidos do **Home Assistant** (que já os recebe de outro ESPHome conectado ao inversor via RS485/Modbus), sem necessidade de conexão direta ao inversor.

## Arquitetura

```
Inversor MUST PV19
       │ RS485/Modbus
       ▼
ESP32 (esp32-pv19) ──► Home Assistant
                              │ API nativa
                              ▼
                    ESP32-C3 (este dispositivo)
                              │ I2C
                              ▼
                       Display OLED SSD1306
```

---

## Hardware necessário

| Componente | Modelo |
|---|---|
| Microcontrolador | ESP32-C3 (esp32-c3-devkitm-1) |
| Display | SSD1306 OLED 0.96" 128×64 I2C |

> Não há RS485 neste dispositivo. A leitura do inversor é feita por outro ESP32 e consumida pelo Home Assistant.

---

## Ligações (pinagem)

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

## Sensores lidos do Home Assistant

Os `entity_id` são configurados nas `substitutions` do YAML. O padrão ESPHome é `sensor.{device_name}_{sensor_name}` (espaços substituídos por `_`).

| ID local | Substitution | Sensor padrão (PV19) | Descrição |
|----------|-------------|----------------------|-----------|
| `soc` | `ha_soc` | `sensor.must_inverter_pv19_state_of_charge` | Estado de carga (%) |
| `inv_work_state` | `ha_inv_work_state` | `sensor.must_inverter_pv19_inverter_work_state` | Estado de trabalho do inversor (0–6) |
| `inv_voltage` | `ha_inv_voltage` | `sensor.must_inverter_pv19_inverter_voltage` | Tensão saída AC (V) |
| `sys_load` | `ha_sys_load` | `sensor.must_inverter_pv19_system_load` | Carga do sistema (%) |
| `temp_ac_rad` | `ha_temp_ac_rad` | `sensor.must_inverter_pv19_ac_radiator_temp` | Temperatura radiador AC (°C) |
| `temp_battery` | `ha_temp_battery` | `sensor.must_inverter_pv19_battery_temp` | Temperatura da bateria (°C) |
| `bat_current` | `ha_bat_current` | `sensor.must_inverter_pv19_battery_current` | Corrente da bateria (A, negativo = descarga) |

> Ajuste os valores nas `substitutions` se o nome do seu dispositivo ESPHome for diferente de `must-inverter-pv19`.

---

## Comportamento na inicialização

Enquanto o ESP32-C3 ainda não recebeu dados do Home Assistant (sensores em estado `NaN`), o display exibe:

```
Aguardando
Home Assistant...
```

Assim que os primeiros valores chegarem, o display passa automaticamente para o layout normal.

---

## Tabela de estados do inversor (`inv_work_state`, registrador 25201)

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

- **Entity IDs**: ajuste os valores de `ha_*` nas `substitutions` conforme os nomes dos seus sensores no Home Assistant.
- **Intervalo do display**: padrão `5s`. Altere em `display > update_interval`.
- **Pinos I2C**: altere `i2c_sda` / `i2c_scl` nas `substitutions` caso use outros pinos.
- **Endereço I2C do display**: padrão `0x3C`. Altere em `display > address` se necessário.
