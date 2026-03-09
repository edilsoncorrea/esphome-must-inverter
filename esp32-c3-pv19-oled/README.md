# ESP32-C3 + OLED SSD1306 — Monitor MUST PV19 Inverter

Display OLED 0.96" SSD1306 I2C para monitoramento do inversor MUST PV19, rodando em ESP32-C3.  
Os dados são lidos do **Home Assistant** (que já os recebe de outro ESPHome conectado ao inversor via RS485/Modbus), sem necessidade de conexão direta ao inversor.

Inclui sistema de **alarmes visuais** (pisca a 1 Hz) e **alarmes sonoros** via buzzer ativo/passivo para valores críticos configuráveis.

## Arquitetura

```
Inversor MUST PV19
       │ RS485/Modbus
       ▼
ESP32 (esp32-pv19) ──► Home Assistant
                              │ API nativa
                              ▼
                    ESP32-C3 (este dispositivo)
                              │ I2C               ┌─────────────┐
                              ▼                   │ Buzzer ativo│
                       Display OLED SSD1306   GPIO3 (configurável)
```

---

## Hardware necessário

| Componente | Modelo |
|---|---|
| Microcontrolador | ESP32-C3 (esp32-c3-devkitm-1) |
| Display | SSD1306 OLED 0.96" 128×64 I2C |
| Buzzer | Ativo ou passivo (opcional) |

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

### Buzzer → ESP32-C3

| Buzzer | ESP32-C3 | Descrição |
|--------|----------|-----------|
| +      | GPIO3    | Sinal PWM (LEDC) — configurável em `buzzer_gpio` |
| −      | GND      | Terra |

> Funciona com buzzers ativos (tom fixo) e passivos (tom por frequência PWM). O GPIO pode ser alterado na substitution `buzzer_gpio`.

---

## Layout do display (128×64 px)

```
┌──────────────────────────────────────┐
│ B:75%  +12A  [BAT ]                  │  ← SOC¹ + corrente + fonte²
│ Vb:52.4V  Vs:110V                    │  ← tensão bateria³ + tensão AC⁴
│ Ti:38C  Tb:32C  C:42%               │  ← temp inv⁵ + temp bat⁶ + carga⁷
│ Consumo: 850W                        │  ← potência instantânea⁷
│ [▓▓▓▓▓▓▓░░░]  75%                   │  ← barra visual SOC¹
└──────────────────────────────────────┘
```

| Campo | Descrição |
|-------|-----------|
| `B: XX%` | Estado de carga da bateria (SOC) |
| `+/-XXA` | Corrente da bateria (+ carregando, − descarregando) |
| `[REDE]` / `[BAT ]` | Fonte de energia ativa |
| `Vb: XX.XV` | Tensão da bateria (V) |
| `Vs: XXXV` | Tensão de saída AC do inversor |
| `Ti: XXC` | Temperatura do radiador AC do inversor |
| `Tb: XXC` | Temperatura da bateria |
| `C: XX%` | % de carga do sistema (potência / capacidade nominal) |
| `Consumo: XXXW` | Potência de carga instantânea (W) |
| Barra SOC | Representação visual de 0–100% (102 px de largura) |

### Indicador de fonte

| Estado `inv_work_state` | Valor | Exibição |
|------------------------|-------|----------|
| Grid / Bypass / Grid Charging | 3, 4, 6 | `[REDE]` fixo |
| Outros (bateria/inversor) | demais | `[BAT ]` pisca 10× ao mudar, depois fixo |

---

## Alarmes

### Alarmes visuais (pisca a 1 Hz)

| Superscript | Condição | Campo que pisca |
|---|---|---|
| ¹ | `SOC < alarm_soc_min` | `B:XX%` + barra + `XX%` |
| ³ | `Vbat < alarm_bat_v_min` | `Vb:XX.XV` |
| ³ | `Vbat > alarm_bat_v_max` | `Vb:XX.XV` |
| ⁴ | `Vs fora de [grid_v_min..grid_v_max]` | `Vs:XXXV` |
| ⁵ | `Temp inversor > alarm_temp_inv_max` | `Ti:XXC` |
| ⁶ | `Temp bateria > alarm_temp_bat_max` | `Tb:XXC` |
| ⁷ | `Carga > alarm_load_max` | `C:XX%` + linha `Consumo` |

### Alarmes sonoros (buzzer, verificados a cada 15 s)

Os alarmes têm prioridade — apenas o mais grave toca:

| Prioridade | Condições | Padrão |
|---|---|---|
| **Crítico** | Temp bateria alta **ou** temp inversor alta **ou** tensão bateria mínima | 5× bipe 100 ms a **2 kHz** |
| **Alto** | SOC mínimo **ou** tensão bateria máxima | 3× bipe 200 ms a **1,5 kHz** |
| **Médio** | Carga máxima **ou** tensão AC fora de faixa | 2× bipe 300 ms a **1 kHz** |

---

## Limiares de alarme configuráveis

Todos os limiares estão nas `substitutions` do YAML — basta editar os valores:

| Substitution | Padrão | Unidade | Descrição |
|---|---|---|---|
| `alarm_soc_min` | `20` | % | SOC mínimo |
| `alarm_bat_v_min` | `46.0` | V | Tensão mínima da bateria |
| `alarm_bat_v_max` | `58.4` | V | Tensão máxima da bateria |
| `alarm_load_max` | `90` | % | Carga máxima do sistema |
| `alarm_grid_v_min` | `100` | V | Tensão mínima da saída AC |
| `alarm_grid_v_max` | `130` | V | Tensão máxima da saída AC |
| `alarm_temp_inv_max` | `65` | °C | Temperatura máxima do inversor |
| `alarm_temp_bat_max` | `45` | °C | Temperatura máxima da bateria |
| `buzzer_gpio` | `GPIO3` | — | GPIO do buzzer |

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
| `bat_voltage` | `ha_bat_voltage` | `sensor.must_inverter_pv19_battery_voltage` | Tensão da bateria (V) |
| `bat_current` | `ha_bat_current` | `sensor.must_inverter_pv19_battery_current` | Corrente da bateria (A, negativo = descarga) |
| `load_power` | `ha_load_power` | `sensor.must_inverter_pv19_load_power` | Potência de carga (W) |

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
