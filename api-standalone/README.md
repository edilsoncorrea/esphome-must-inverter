# MUST Inverter API Device - PlatformIO C++

## 🚀 Projeto PlatformIO Standalone

Este é um **firmware completo em C++/Arduino** para ESP32 que monitora inversores MUST via RS485/Modbus e expõe uma **API REST** pública.

**Características:**
- ✅ **WiFi Manager** com portal captivo para configuração fácil
- ✅ **API REST** com autenticação HTTP Basic
- ✅ **Leitura Modbus RTU** de todos os sensores do inversor
- ✅ **Interface web** moderna e responsiva
- ✅ **Factory Reset** via botão BOOT (5 segundos)
- ✅ **OTA Updates** (via PlatformIO)

**Tecnologias:**
- PlatformIO + Arduino Framework
- AsyncWebServer (API REST assíncrona)
- ModbusRTU (comunicação RS485)
- WiFiManager (configuração WiFi via AP)
- ArduinoJson (respostas JSON estruturadas)

---

## 📋 Requisitos Hardware

### ESP32 DevKit
- **Placa**: ESP32 DevKit V1 (30 pinos)
- **Flash**: 4MB mínimo
- **RAM**: 520KB

### Conversor RS485
- **Modelo**: MAX485, MAX3485 ou similar
- **Conexões**:
  - `RO (Receiver Output)` → GPIO18 (RX)
  - `DI (Driver Input)` → GPIO19 (TX)
  - `DE/RE` → Pode ser conectado ao VCC (sempre enabled) ou GPIO controlado
  - `VCC` → 3.3V ou 5V (conforme módulo)
  - `GND` → GND
  - `A/B` → Terminais RS485 do inversor MUST

### Inversor MUST
- **Modelos suportados**: PV18, PV19
- **Interface**: RS485 (terminais A/B na parte traseira)
- **Protocolo**: Modbus RTU @ 19200 baud, 8N1
- **Slave ID**: 0x04

---

## 📦 Instalação e Flash

### Pré-requisitos
```bash
# Instalar PlatformIO Core
pip install platformio

# OU via VSCode: instalar extensão PlatformIO IDE
```

### 1. Clonar e Compilar
```bash
cd api-standalone
pio run
```

### 2. Flash via USB
```bash
# Conectar ESP32 via USB
pio run --target upload

# Ou especificar porta manualmente
pio run --target upload --upload-port COM3  # Windows
pio run --target upload --upload-port /dev/ttyUSB0  # Linux
```

### 3. Monitorar Serial
```bash
pio device monitor
# Ou com taxa específica
pio device monitor -b 115200
```

### 4. OTA Updates (após primeira instalação)
```bash
# Editar platformio.ini e adicionar:
# upload_protocol = espota
# upload_port = 192.168.x.x

pio run --target upload
```

---

## Funcionalidades Implementadas

### 1. **Modo AP Automático com IP Fixo**
- IP fixo: `192.168.4.1`
- SSID: `must-inverter-api`
- Senha padrão: `inverter123`
- O dispositivo entra automaticamente em modo AP se não conseguir conectar ao WiFi

### 2. **Configuração WiFi via Portal**
Para configurar WiFi:
1. Conecte ao AP `must-inverter-api` (senha: `inverter123`)
2. Acesse `http://192.168.4.1` no navegador
3. Será redirecionado para página de configuração (Captive Portal)
4. Configure SSID e senha WiFi
5. Dispositivo reinicia e conecta na rede

### 3. **Configuração de Credenciais API**
Acesse via web: `http://192.168.4.1` (ou IP da rede)

No painel web você pode configurar:
- **API Username**: Campo "API Username" (padrão: `admin`)
- **API Password**: Campo "API Password" (padrão: `admin123`)

### 4. **Acessando a API REST**

#### Endpoint Principal - GET All Data
```bash
GET http://<IP_DO_DISPOSITIVO>/sensor/
```

**Autenticação:**
As credenciais são passadas via HTTP Basic Auth:

```bash
# Exemplo usando curl
curl -u admin:admin123 http://192.168.4.1/sensor/

# Ou com header Authorization
curl -H "Authorization: Basic YWRtaW46YWRtaW4xMjM=" http://192.168.4.1/sensor/
```

**Resposta JSON (exemplo):**
```json
{
  "state_of_charge": {"value": 85.0, "unit": "%"},
  "pv1_voltage": {"value": 380.5, "unit": "V"},
  "pv2_voltage": {"value": 375.2, "unit": "V"},
  "pv1_current": {"value": 12.3, "unit": "A"},
  "pv2_current": {"value": 11.8, "unit": "A"},
  "pv1_power": {"value": 4681, "unit": "W"},
  "pv2_power": {"value": 4428, "unit": "W"},
  "battery_voltage_charger": {"value": 52.8, "unit": "V"},
  "battery_voltage_inverter": {"value": 52.7, "unit": "V"},
  "battery_current": {"value": 45.2, "unit": "A"},
  "battery_power": {"value": 2385, "unit": "W"},
  "inverter_voltage": {"value": 230.1, "unit": "V"},
  "grid_voltage": {"value": 228.5, "unit": "V"},
  "inverter_power": {"value": 1850, "unit": "W"},
  "grid_power": {"value": -535, "unit": "W"},
  "load_power": {"value": 1315, "unit": "W"},
  "ac_radiator_temp": {"value": 45.2, "unit": "°C"},
  "transformer_temp": {"value": 48.5, "unit": "°C"},
  "accumulated_charger_power": {"value": 1543.2, "unit": "kWh"},
  "accumulated_discharger_power": {"value": 987.5, "unit": "kWh"},
  "accumulated_buy_power": {"value": 234.8, "unit": "kWh"},
  "accumulated_sell_power": {"value": 156.3, "unit": "kWh"}
}
```

#### Endpoints Individuais
Cada sensor tem seu próprio endpoint:
```bash
GET http://192.168.4.1/sensor/pv1_voltage
GET http://192.168.4.1/sensor/battery_voltage_inverter
GET http://192.168.4.1/sensor/state_of_charge
```

### 5. **Reset para Modo AP**

#### Opção 1: Via Botão Físico (Boot Button)
1. Mantenha pressionado o botão BOOT (GPIO0) por **5 segundos**
2. O dispositivo fará factory reset
3. Reiniciará em modo AP

#### Opção 2: Via Web Interface
1. Acesse `http://<IP>/`
2. Clique no botão "Factory Reset (Clears WiFi)"
3. Dispositivo reinicia em modo AP

#### Opção 3: Via API REST
```bash
POST http://192.168.4.1/button/factory_reset_btn/press
Authorization: Basic YWRtaW46YWRtaW4xMjM=
```

### 6. **Monitoramento via Web**

Acesse: `http://<IP_DO_DISPOSITIVO>/`

Você verá:
- Status da conexão WiFi
- IP atual
- Todos os valores dos sensores em tempo real
- Campos para alterar credenciais API
- Botões de controle (restart, factory reset)

## Instalação e Flash

### Pré-requisitos
```bash
# Instalar PlatformIO
pip install platformio

# Ou via VSCode: instalar extensão PlatformIO IDE
```

### 1. Compilar
```bash
cd api-standalone
pio run
```

### 2. Flash via USB
```bash
pio run --target upload

# Ou especificar porta
pio run --target upload --upload-port COM3
```

### 3. Monitorar Serial
```bash
pio device monitor
```

### 4. Flash via OTA (após primeira instalação)
```bash
# Editar platformio.ini para habilitar OTA
pio run --target upload --upload-port 192.168.4.1
```

## Diagrama de Conexão

```
┌─────────────────────────────────────────┐
│           ESP32 DevKit                  │
│                                         │
│  GPIO19 (TX) ────→ Módulo RS485 (TXD)  │
│  GPIO18 (RX) ←──── Módulo RS485 (RXD)  │
│  3.3V       ────→ Módulo RS485 (VCC)   │
│  GND        ────→ Módulo RS485 (GND)   │
│  GPIO0      → Boot Button (Reset)      │
└─────────────────────────────────────────┘
                     │
                     │ RS485 (A/B)
                     ▼
┌─────────────────────────────────────────┐
│      Inversor MUST PV19                 │
│         Porta RS485                     │
│          A  B  GND                      │
└─────────────────────────────────────────┘
```

## Exemplos de Uso da API

### Python
```python
import requests
from requests.auth import HTTPBasicAuth

# Credenciais
username = "admin"
password = "admin123"
device_ip = "192.168.4.1"

# GET todos os dados
response = requests.get(
    f"http://{device_ip}/sensor/",
    auth=HTTPBasicAuth(username, password)
)

data = response.json()
print(f"Battery SOC: {data['state_of_charge']['value']}%")
print(f"PV1 Power: {data['pv1_power']['value']}W")
```

### JavaScript (Browser)
```javascript
const username = 'admin';
const password = 'admin123';
const deviceIP = '192.168.4.1';

// Encode credentials
const credentials = btoa(`${username}:${password}`);

fetch(`http://${deviceIP}/sensor/`, {
  headers: {
    'Authorization': `Basic ${credentials}`
  }
})
.then(response => response.json())
.then(data => {
  console.log('Battery SOC:', data.state_of_charge.value);
  console.log('PV1 Power:', data.pv1_power.value);
});
```

### Node.js
```javascript
const axios = require('axios');

const deviceIP = '192.168.4.1';
const auth = {
  username: 'admin',
  password: 'admin123'
};

axios.get(`http://${deviceIP}/sensor/`, { auth })
  .then(response => {
    console.log('All Data:', response.data);
  })
  .catch(error => {
    console.error('Error:', error.message);
  });
```

### curl (Linux/Mac/Windows)
```bash
# GET all data
curl -u admin:admin123 http://192.168.4.1/sensor/

# GET específico
curl -u admin:admin123 http://192.168.4.1/sensor/battery_voltage_inverter

# Pretty print JSON
curl -u admin:admin123 http://192.168.4.1/sensor/ | jq .
```

## Configurações Importantes

### Alterar Credenciais API Padrão
Edite no arquivo YAML:
```yaml
globals:
  - id: api_username
    initial_value: '"seu_usuario"'  # Altere aqui
  
  - id: api_password
    initial_value: '"sua_senha"'     # Altere aqui
```

### Alterar Senha do AP
```yaml
wifi:
  ap:
    password: "sua_senha_ap"  # Altere aqui
```

### Alterar IP do AP
```yaml
wifi:
  ap:
    manual_ip:
      static_ip: 192.168.10.1  # Altere aqui
```

### Alterar Intervalo de Atualização Modbus
```yaml
modbus_controller:
  update_interval: 10s  # Altere aqui (padrão: 20s)
```

## Segurança

⚠️ **IMPORTANTE:**
1. **Altere as senhas padrão** antes de usar em produção
2. A autenticação HTTP Basic **não é criptografada** - use em redes confiáveis
3. Para segurança adicional, considere:
   - Implementar HTTPS
   - Usar tokens de API
   - Firewall para restringir acesso

## Troubleshooting

### Dispositivo não entra em modo AP
- Aguarde 1 minuto após ligar
- Verifique se não há redes WiFi configuradas
- Pressione o botão Boot por 5 segundos para forçar reset

### API não responde
- Verifique se está usando as credenciais corretas
- Confirme o IP correto (veja na interface web)
- Teste sem autenticação: `http://<IP>/` (página web)

### Não consegue conectar ao RS485
- Verifique conexões A/B/GND
- Tente inverter A e B
- Confirme baudrate 19200 no inversor
- Verifique pinos GPIO19/GPIO18

### Valores zerados ou NaN
- Aguarde 20-30 segundos após boot
- Verifique comunicação Modbus nos logs
- Confirme que inversor está ligado

## Logs e Debug

Para ver logs detalhados:
```bash
esphome logs esp32-pv19-api.yaml --device <IP>
```

Ou via serial (USB):
```bash
esphome logs esp32-pv19-api.yaml
```

## Licença
MIT - Livre para uso e modificação
