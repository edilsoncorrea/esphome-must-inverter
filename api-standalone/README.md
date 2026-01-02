# MUST Inverter API Device - PlatformIO C++

## 🚀 Projeto PlatformIO Standalone

Este é um **firmware completo em C++/Arduino** para **ESP32, ESP32-C3 e ESP32-S3** que monitora inversores MUST via RS485/Modbus e expõe uma **API REST** pública com **interface web moderna** usando arquivos separados (LittleFS).

**Características:**
- ✅ **Suporte multi-plataforma**: ESP32, ESP32-C3, ESP32-S3
- ✅ **Configuração automática de pinos** por plataforma
- ✅ **WiFi Manager** com portal captivo para configuração fácil
- ✅ **API REST** com autenticação HTTP Basic
- ✅ **Leitura Modbus RTU** de todos os sensores do inversor
- ✅ **Interface web** moderna e responsiva com arquivos HTML/CSS/JS separados
- ✅ **LittleFS** - arquivos estáticos servidos do filesystem
- ✅ **Factory Reset** via botão BOOT (5 segundos)
- ✅ **OTA Updates** (via PlatformIO)
- ✅ **Modo Demo** - Validação da API sem conexão com inversor
- ✅ **Atalhos VS Code** para compilação e upload rápidos

**🔧 Modo de Demonstração**: Teste e valide a API mesmo sem o inversor conectado! O sistema detecta automaticamente quando não há comunicação Modbus e exibe dados simulados realistas. Perfeito para desenvolvimento e demonstrações. [Ver detalhes](DEMO_MODE.md)

**Tecnologias:**
- PlatformIO + Arduino Framework
- AsyncWebServer (API REST assíncrona)
- ModbusRTU (comunicação RS485)
- WiFiManager (configuração WiFi via AP)
- ArduinoJson (respostas JSON estruturadas)
- LittleFS (filesystem para HTML/CSS/JS)

---

## 📂 Estrutura do Projeto

```
api-standalone/
├── platformio.ini          # Configuração PlatformIO
├── src/
│   ├── main.cpp           # Código principal (C++)
│   └── config.h           # Configurações centralizadas
├── data/                  # Arquivos do filesystem (LittleFS)
│   ├── index.html        # Dashboard de monitoramento
│   ├── config.html       # Página de configuração (modo AP)
│   ├── css/
│   │   └── style.css     # Estilos CSS
│   └── js/
│       └── app.js        # Lógica JavaScript
└── README.md

```

---

## 📋 Requisitos Hardware

### Placas Suportadas

#### ESP32 DevKit (Original)
- **Placa**: ESP32 DevKit V1 (30 pinos)
- **Flash**: 4MB mínimo
- **RAM**: 320KB
- **Serial Modbus**: Serial2
- **Pinos**: TX=GPIO19, RX=GPIO18

#### ESP32-C3 DevKit
- **Placa**: ESP32-C3-DevKitM-1
- **Flash**: 4MB
- **RAM**: 400KB
- **Serial Modbus**: Serial1
- **Pinos**: TX=GPIO21, RX=GPIO20
- **LED interno**: GPIO8

#### ESP32-S3 DevKit
- **Placa**: ESP32-S3-DevKitC-1
- **Flash**: 8MB
- **RAM**: 320KB
- **Serial Modbus**: Serial1
- **Pinos**: TX=GPIO17, RX=GPIO18
- **USB CDC**: Suportado

### Conversor RS485
- **Modelo**: MAX485, MAX3485 ou similar
- **Conexões RS485** (varia por plataforma - ver tabela acima):
  - `RO (Receiver Output)` → GPIO RX da plataforma
  - `DI (Driver Input)` → GPIO TX da plataforma
  - `DE/RE` → Pode ser conectado ao VCC (sempre enabled) ou GPIO controlado (ver seção Controle de Fluxo)
  - `VCC` → 3.3V ou 5V (conforme módulo)
  - `GND` → GND
  - `A/B` → Terminais RS485 do inversor MUST

**Nota**: O firmware detecta automaticamente a plataforma e configura os pinos corretos.

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

#### Compilar para ESP32 (padrão)
```bash
cd api-standalone
pio run -e esp32dev
```

#### Compilar para ESP32-C3
```bash
pio run -e esp32c3
```

#### Compilar para ESP32-S3
```bash
pio run -e esp32s3
```

#### Compilar todas as plataformas
```bash
pio run
```

### 2. Upload do Filesystem (IMPORTANTE!)
Antes de fazer o upload do firmware, você precisa fazer upload dos arquivos HTML/CSS/JS:

```bash
# Upload dos arquivos da pasta data/ para o LittleFS
pio run --target uploadfs
```

**Nota**: Este comando irá fazer upload de todos os arquivos em `data/` para o filesystem do ESP32.

### 3. Flash via USB

#### ESP32
```bash
pio run -e esp32dev -t upload
# Ou com porta específica
pio run -e esp32dev -t upload --upload-port COM3
```

#### ESP32-C3
```bash
pio run -e esp32c3 -t upload
```

#### ESP32-S3
```bash
pio run -e esp32s3 -t upload
```

**Nota**: A porta é auto-detectada, mas pode ser especificada com `--upload-port`

### 4. Monitorar Serial
```bash
pio device monitor
# Ou com taxa específica
pio device monitor -b 115200
```

### 5. OTA Updates (após primeira instalação)
```bash
# Editar platformio.ini e adicionar:
# upload_protocol = espota
# upload_port = 192.168.x.x

pio run --target upload
```

### 6. Atalhos VS Code 🎯

Se você estiver usando VS Code, o projeto já vem configurado com atalhos de teclado:

| **Atalho** | **Ação** |
|-----------|----------|
| `Ctrl+Alt+B` | Compilar (ESP32) |
| `Ctrl+Alt+U` | Upload (ESP32) |
| `Ctrl+Alt+C` | Limpar build |
| `Ctrl+Alt+M` | Abrir monitor serial |
| `Ctrl+Shift+B` | Build task padrão |

#### Menu de Tasks (Ctrl+Shift+P → "Run Task")

**Build por plataforma:**
- PlatformIO: Build (ESP32)
- PlatformIO: Build (ESP32-C3)
- PlatformIO: Build (ESP32-S3)

**Upload por plataforma:**
- PlatformIO: Upload (ESP32)
- PlatformIO: Upload (ESP32-C3)
- PlatformIO: Upload (ESP32-S3)
- PlatformIO: Upload All (Filesystem + Firmware)
- PlatformIO: Upload Filesystem

**Outros:**
- PlatformIO: Clean
- PlatformIO: Monitor

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

### 3. **Painel de Monitoramento em Tempo Real** ⭐ NOVO!
Quando o dispositivo estiver conectado ao WiFi, acessando `http://<IP_DO_DISPOSITIVO>/` você terá:

#### Interface Moderna e Responsiva:
- 📱 **Design responsivo** - Funciona perfeitamente em celular, tablet e desktop
- 🎨 **Interface moderna** - Cards organizados com gradientes e animações suaves
- ⚡ **Atualização automática** - Dados atualizados a cada 5 segundos sem recarregar a página

#### Cards Organizados por Categoria:

**🔋 Bateria:**
- Estado de carga (SOC) em destaque
- Tensão, corrente e potência
- Temperatura da bateria

**☀️ Painel Solar:**
- Potência total em destaque
- Tensão e corrente PV

**⚡ Carregador:**
- Tensão, corrente e potência
- Energia total acumulada

**🔌 Inversor:**
- Modo de operação com badge colorido
- Tensão, corrente e frequência AC
- Potência e percentual de carga

**📊 Energia Acumulada:**
- Total carregado
- Total descarregado

**💻 Sistema:**
- Temperatura do dispositivo
- Última atualização
- Links rápidos para API e configuração

### 4. **Configuração de Credenciais API**
Acesse via web: `http://192.168.4.1` (ou IP da rede)

As credenciais padrão são:
- **API Username:** `admin`
- **API Password:** `admin123`

⚠️ **Importante:** Estas credenciais são usadas apenas para acessar a API REST via autenticação HTTP Basic. 
O painel web de monitoramento (interface visual) **não requer autenticação** e pode ser acessado diretamente.

### 5. **Modos de Acesso**

#### Modo Não Configurado (AP Mode):
Quando o dispositivo não está conectado a nenhuma rede WiFi:
- Acesse `http://192.168.4.1`
- Será exibida uma página simples com botão para configurar WiFi
- Clique em "Configurar WiFi" para acessar o portal captivo

#### Modo Configurado (Connected):
Quando o dispositivo está conectado à sua rede WiFi:
- Acesse `http://<IP_DO_DISPOSITIVO>/`
- **Painel completo de monitoramento** é exibido automaticamente
- Todos os dados são atualizados em tempo real a cada 5 segundos
- Não requer autenticação para visualização

### 6. **Acessando a API REST**

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
  "charger": {
    "voltage": 54.2,
    "current": 8.2,
    "power": 445,
    "accumulated_power": 125.5
  },
  "pv": {
    "voltage": 85.0,
    "current": 4.5,
    "power": 383
  },
  "battery": {
    "voltage": 52.8,
    "current": 8.0,
    "power": 422,
    "soc": 65,
    "temperature": 25.0
  },
  "inverter": {
    "mode": "Off-Grid",
    "mode_id": 3,
    "ac_voltage": 220.0,
    "ac_current": 2.5,
    "ac_frequency": 50.0,
    "ac_power": 550,
    "load_percent": 28,
    "dc_voltage": 52.8,
    "max_charge_current": 60,
    "max_discharge_current": 60,
    "accumulated_power": 87.6
  },
  "totals": {
    "total_charged": 170.7,
    "total_discharged": 185.9,
    "device_temperature": 42.0
  },
  "last_update": 12345678,
  "uptime": 1234,
  "modbus_error": false,
  "demo_mode": false
}
```

**🔧 Modo Demo**: Quando o campo `"demo_mode": true` estiver presente, os dados exibidos são simulados (inversor não conectado). [Ver detalhes sobre o modo demo](DEMO_MODE.md)

#### Endpoints Individuais
Cada sensor tem seu próprio endpoint:
```bash
GET http://192.168.4.1/sensor/pv1_voltage
GET http://192.168.4.1/sensor/battery_voltage_inverter
GET http://192.168.4.1/sensor/state_of_charge
```

### 7. **Reset para Modo AP**

#### Opção 1: Via Botão Físico (Boot Button)
1. Mantenha pressionado o botão BOOT (GPIO0) por **5 segundos**
2. O dispositivo fará factory reset
3. Reiniciará em modo AP

#### Opção 2: Via Web Interface
1. Acesse `http://<IP>/` e clique em "Config" no card Sistema
2. OU acesse diretamente `http://<IP>/config`
3. Dispositivo reinicia em modo AP

#### Opção 3: Via API REST
```bash
# Não implementado - use as opções 1 ou 2
```

### 8. **Monitoramento via Web**

**Painel Principal:** `http://<IP_DO_DISPOSITIVO>/`

#### Quando NÃO Configurado (Modo AP):
- Página simples com botão "Configurar WiFi"
- Redirecionamento para portal captivo

#### Quando Configurado (Conectado):
**Painel Completo de Monitoramento** com:
- ✅ Atualização automática a cada 5 segundos
- ✅ Cards organizados por categoria
- ✅ Valores em destaque com gradientes coloridos
- ✅ Interface responsiva (mobile-friendly)
- ✅ Animações suaves nos cards
- ✅ Indicador de última atualização
- ✅ Links rápidos para API JSON e configuração
- ✅ Modo de operação do inversor com badge
- ✅ Todas as métricas importantes visíveis de uma vez

**Dados Exibidos:**
1. **Bateria**: SOC, tensão, corrente, potência, temperatura
2. **Painel Solar**: Potência total, tensão, corrente
3. **Carregador**: Tensão, corrente, potência, energia acumulada
4. **Inversor**: Modo, AC V/A/Hz, potência, carga %
5. **Energia**: Total carregado/descarregado
6. **Sistema**: Temperatura do dispositivo, timestamp

**Recursos Visuais:**
- 🔋 Battery com SOC em destaque (gradiente roxo)
- ☀️ Solar com potência em destaque (gradiente rosa)
- ⚡ Charger com métricas de carga
- 🔌 Inverter com badge de modo de operação
- 📊 Totalizadores de energia
- 💻 Info do sistema com ações rápidas

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

### Configuração de Pinos por Plataforma

| **Plataforma** | **Serial** | **TX (Modbus)** | **RX (Modbus)** | **LED** |
|---------------|-----------|----------------|----------------|----------|
| ESP32         | Serial2   | GPIO19         | GPIO18         | GPIO2    |
| ESP32-C3      | Serial1   | GPIO21         | GPIO20         | GPIO8    |
| ESP32-S3      | Serial1   | GPIO17         | GPIO18         | GPIO2    |

### Exemplo: ESP32 DevKit

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
│      Inversor MUST PV18/PV19            │
│         Porta RS485                     │
│          A  B  GND                      │
└─────────────────────────────────────────┘
```

**Nota**: Para ESP32-C3 e ESP32-S3, use os pinos indicados na tabela acima.

### Controle de Fluxo RS485 (Opcional)

O firmware suporta controle DE/RE (Driver Enable / Receiver Enable) para módulos RS485.
Para habilitar, edite `src/config.h` e descomente as linhas correspondentes:

```cpp
// Exemplo para ESP32
#define MODBUS_DE_PIN 4
#define MODBUS_RE_PIN 4
```

Quando habilitado, o firmware controla automaticamente a direção da comunicação RS485.

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
curl -u admin:must2024 http://192.168.4.1/api/sensors

# Status do dispositivo
curl -u admin:must2024 http://192.168.4.1/api/status

# Pretty print JSON
curl -u admin:must2024 http://192.168.4.1/api/sensors | jq .
```

---

## 🎨 Personalização da Interface Web

### Estrutura dos Arquivos

Os arquivos da interface estão organizados na pasta `data/`:

```
data/
├── index.html     # Dashboard principal (quando WiFi conectado)
├── config.html    # Página de configuração (modo AP)
├── css/
│   └── style.css  # Todos os estilos
└── js/
    └── app.js     # Lógica JavaScript e atualização de dados
```

### Como Modificar a Interface

1. **Editar os arquivos**:
   - `index.html` - Estrutura do dashboard
   - `css/style.css` - Cores, fontes, layout
   - `js/app.js` - Lógica de atualização, formato dos dados

2. **Upload para o ESP32**:
```bash
# Fazer upload do filesystem atualizado
pio run --target uploadfs
```

3. **Não é necessário recompilar o firmware!**
   - Os arquivos HTML/CSS/JS são servidos diretamente do LittleFS
   - Apenas execute `uploadfs` novamente após modificações

### Exemplos de Personalização

#### Mudar Cores do Dashboard
Edite `data/css/style.css`:
```css
body {
    background: linear-gradient(135deg, #your-color1 0%, #your-color2 100%);
}

.card {
    background: #your-card-color;
}
```

#### Adicionar Novo Card
1. Edite `data/index.html` - adicione novo card HTML
2. Edite `data/js/app.js` - adicione lógica para popular dados
3. Execute: `pio run --target uploadfs`

#### Alterar Intervalo de Atualização
Edite `data/js/app.js`:
```javascript
// Mudar de 5000ms (5s) para outro valor
setInterval(updateData, 10000);  // 10 segundos
```

---

## ⚙️ Configurações Importantes

### Alterar Credenciais API e WiFi via Interface Web
Acesse `http://<IP>/settings.html` para:
- **Alterar usuário e senha da API** (mínimo 6 caracteres)
- **Configurar WiFi** (SSID e senha, mínimo 8 caracteres)
- Ambas as alterações exigem a senha atual para confirmar
- O dispositivo reinicia automaticamente após salvar

**Documentação completa**: [WIFI_CONFIG.md](WIFI_CONFIG.md)

### Alterar Credenciais API Padrão (código)
Edite em `src/config.h`:
```cpp
#define DEFAULT_API_USER "seu_usuario"
#define DEFAULT_API_PASS "sua_senha"
```

### Alterar Senha do AP
Edite em `src/config.h`:
```cpp
#define AP_PASSWORD "sua_senha_ap"
```

### Alterar Intervalo de Atualização Modbus
Edite em `src/main.cpp`:
```cpp
const unsigned long MODBUS_UPDATE_INTERVAL = 10000;  // 10 segundos
```

---

## 🔒 Segurança

⚠️ **IMPORTANTE:**
1. **Altere as senhas padrão** antes de usar em produção
2. A autenticação HTTP Basic **não é criptografada** - use em redes confiáveis
3. Para segurança adicional, considere:
   - Implementar HTTPS
   - Usar tokens de API
   - Firewall para restringir acesso

---

## 🛠️ Troubleshooting

### Erro de compilação
- Certifique-se de estar na pasta `api-standalone`
- Execute `pio run` para instalar dependências
- Se houver erros de plataforma, especifique: `pio run -e esp32dev` ou `-e esp32c3` ou `-e esp32s3`

### Dispositivo não entra em modo AP
- Verifique se o LED está piscando
- Aguarde 30 segundos após ligar
- Pressione BOOT por 5 segundos para reset de fábrica
- Verifique se há redes WiFi salvas conflitantes

### Interface web não carrega
- Certifique-se de ter feito upload do filesystem: `pio run --target uploadfs`
- Acesse o IP correto (verifique no Serial Monitor)
- Limpe o cache do navegador
- Tente acessar `http://<IP>/` diretamente

### Erro "LittleFS mount failed"
- Faça upload do filesystem: `pio run --target uploadfs`
- O ESP32 precisa de partição configurada (já está no platformio.ini)
- Reformate o LittleFS se necessário

### API não responde
- Verifique se está usando as credenciais corretas
- Confirme o IP correto (veja na interface web)
- Teste sem autenticação: `http://<IP>/` (página web)

### Não consegue conectar ao RS485
- **Verifique a plataforma e pinos corretos** (ver tabela de pinos)
- Confirme conexões A/B/GND
- Tente inverter A e B
- Confirme baudrate 19200 no inversor
- ESP32: GPIO19/GPIO18
- ESP32-C3: GPIO21/GPIO20
- ESP32-S3: GPIO17/GPIO18

### Valores zerados ou NaN
- Aguarde 20-30 segundos após boot
- Verifique comunicação Modbus nos logs
- Confirme que inversor está ligado
- Ative o [Modo Demo](DEMO_MODE.md) para testar sem inversor

### Problema com upload
- Segure o botão BOOT durante upload se necessário
- Verifique se a porta COM está correta
- No ESP32-C3/S3, pode ser necessário instalar drivers USB adicionais
- Tente `pio run -e <plataforma> -t upload --upload-port COM<X>`

---

## 📝 Changelog

### Versão 1.1.0 (Janeiro 2026)
- ✨ **Suporte multi-plataforma**: ESP32, ESP32-C3, ESP32-S3
- ✨ **Configuração automática de pinos** por plataforma
- ✨ **Atalhos VS Code** para compilação e upload rápidos
- ✨ **Suporte opcional para controle DE/RE** (RS485 flow control)
- 🔧 Detecção automática de plataforma no boot
- 🔧 Logs melhorados com informação de pinos configurados
- 📚 Documentação atualizada com exemplos multi-plataforma

### Versão 1.0.0
- 🎉 Release inicial
- ✅ Suporte ESP32 original
- ✅ Interface web moderna
- ✅ API REST com autenticação
- ✅ Modo Demo
- ✅ WiFi Manager
- Aguarde 1 minuto após ligar
- Verifique se não há redes WiFi configuradas

### Interface web não carrega
- Verifique se fez upload do filesystem: `pio run --target uploadfs`
- Monitore serial para ver se LittleFS montou corretamente
- Procure por mensagem: `✓ LittleFS mounted`

### Erro "LittleFS mount failed"
- Execute: `pio run --target uploadfs`
- Se persistir, tente apagar flash: `pio run --target erase`
- Depois: `pio run --target uploadfs` e `pio run --target upload`
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
