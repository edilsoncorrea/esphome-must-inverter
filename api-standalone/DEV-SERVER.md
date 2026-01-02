# 🚀 Servidor de Desenvolvimento Local

Este é um servidor de desenvolvimento que simula a API do ESP32 rodando localmente no seu computador. Perfeito para desenvolvimento frontend sem precisar fazer upload no ESP32 toda vez!

## ✨ Características

- ✅ **Mesma API do ESP32** - Endpoints idênticos
- ✅ **Dados simulados realistas** - Mesma lógica do modo demo do ESP32
- ✅ **Hot reload** - Alterações no código são detectadas automaticamente
- ✅ **Mesmos arquivos** - Usa HTML/CSS/JS da pasta `data/`
- ✅ **Autenticação HTTP Basic** - Igual ao ESP32
- ✅ **CORS habilitado** - Para testes com diferentes origens
- ✅ **Credenciais persistentes** - Salvam em `credentials.json`

## 📦 Instalação

### 1. Instalar Python (se ainda não tiver)

```powershell
# Verificar se Python está instalado
python --version

# Se não estiver instalado, baixe em: https://www.python.org/downloads/
```

### 2. Instalar dependências

```powershell
cd api-standalone
pip install -r requirements-dev.txt
```

## 🚀 Executar o Servidor

```powershell
cd api-standalone
python dev-server.py
```

O servidor estará disponível em:
- **URL Principal**: http://localhost:5000
- **Dashboard**: http://localhost:5000/
- **API Sensors**: http://localhost:5000/api/sensors
- **API Status**: http://localhost:5000/api/status

### Credenciais de Autenticação

- **Usuário**: `admin`
- **Senha**: `must2024`

## 🛠️ Desenvolvimento Frontend

### Workflow Recomendado

1. **Editar arquivos** em `data/` (HTML, CSS, JS)
2. **Recarregar página** no navegador (F5)
3. **Ver mudanças instantaneamente** - sem precisar fazer upload no ESP32!

### Arquivos Servidos

```
data/
├── index.html        # Dashboard principal
├── config.html       # Página de configuração
├── css/
│   └── style.css    # Estilos
└── js/
    └── app.js       # Lógica JavaScript
```

## 📡 Endpoints Disponíveis

### GET /api/sensors
Retorna todos os dados dos sensores (requer autenticação).

**Exemplo de resposta:**
```json
{
  "charger": {
    "voltage": 54.2,
    "current": 8.2,
    "power": 445,
    "accumulated_power": 12.5
  },
  "pv": {
    "voltage": 85.0,
    "current": 4.5,
    "power": 382
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
    "accumulated_power": 198.3
  },
  "totals": {
    "total_charged": 245.8,
    "total_discharged": 198.3,
    "device_temperature": 42.0
  },
  "last_update": 1735689600,
  "uptime": 3600,
  "modbus_error": false,
  "demo_mode": true
}
```

### GET /api/status
Retorna status do dispositivo (requer autenticação).

### POST /api/reset
Endpoint de reset (não faz nada no dev server).

## 🔄 Dados Simulados

Os dados são gerados dinamicamente com variações realistas:
- **Valores oscilam** lentamente (simulando condições reais)
- **Acumuladores incrementam** ao longo do tempo
- **Modo demo sempre ativo** (`demo_mode: true`)
- **Banner de demo** será exibido automaticamente no frontend

## 💾 Persistência de Credenciais

### Como Funciona

Quando você altera as credenciais via `/settings.html`, elas são salvas em:
```
api-standalone/credentials.json
```

**Estrutura do arquivo:**
```json
{
  "username": "novousuario",
  "password": "novasenha",
  "updated_at": "2026-01-02T10:30:00.000Z"
}
```

### Restaurar Credenciais Padrão

Para voltar às credenciais padrão (`admin`/`admin123`):

1. **Opção 1**: Delete o arquivo `credentials.json`
2. **Opção 2**: Edite manualmente o arquivo
3. **Reinicie o servidor**

### Backup de Credenciais

É recomendável fazer backup do `credentials.json` antes de reinstalar ou atualizar:

```powershell
# Fazer backup
Copy-Item credentials.json credentials.backup.json

# Restaurar backup
Copy-Item credentials.backup.json credentials.json
```

## 🐛 Debugging

### Ver logs em tempo real
O servidor Flask mostra todos os requests no console:
```
[2026-01-02 10:30:45] "GET /api/sensors HTTP/1.1" 200 -
```

### Testar API via curl

```powershell
# Windows PowerShell
$credentials = [Convert]::ToBase64String([Text.Encoding]::ASCII.GetBytes("admin:must2024"))
Invoke-WebRequest -Uri "http://localhost:5000/api/sensors" -Headers @{"Authorization"="Basic $credentials"} | Select-Object -ExpandProperty Content | ConvertFrom-Json | ConvertTo-Json
```

### Testar no navegador

Abra DevTools (F12) e vá para a aba Network para ver todas as requisições.

## 🔧 Configurações

### Alterar porta

Edite `dev-server.py`:
```python
app.run(
    host='0.0.0.0',
    port=8080,  # Altere aqui
    debug=True
)
```

### Alterar credenciais

Edite `dev-server.py`:
```python
API_USER = 'meuusuario'
API_PASS = 'minhasenha'
```

**Importante**: Lembre-se de atualizar também em `data/js/app.js`:
```javascript
const API_USER = 'meuusuario';
const API_PASS = 'minhasenha';
```

## 📱 Testar em Dispositivos Móveis

O servidor aceita conexões de qualquer dispositivo na rede local:

1. Descubra seu IP local:
```powershell
ipconfig
# Procure por "IPv4 Address" na sua interface de rede
```

2. Acesse do celular/tablet:
```
http://SEU_IP_LOCAL:5000
```

Exemplo: `http://192.168.1.100:5000`

## 🎯 Comparação: Dev Server vs ESP32

| Característica | Dev Server | ESP32 Real |
|----------------|------------|------------|
| **URL** | localhost:5000 | 192.168.x.x |
| **Dados** | Simulados | Reais (Modbus) |
| **Upload necessário** | ❌ Não | ✅ Sim |
| **Hot reload** | ✅ Sim | ❌ Não |
| **Autenticação** | ✅ HTTP Basic | ✅ HTTP Basic |
| **Modo Demo** | ✅ Sempre | ⚠️ Quando desconectado |
| **WiFi Manager** | ❌ N/A | ✅ Sim |

## 💡 Dicas

### VS Code Tasks

Você pode adicionar uma task para iniciar o servidor facilmente. Crie/edite `.vscode/tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Start Dev Server",
      "type": "shell",
      "command": "python",
      "args": ["dev-server.py"],
      "options": {
        "cwd": "${workspaceFolder}/api-standalone"
      },
      "presentation": {
        "reveal": "always",
        "panel": "new"
      },
      "problemMatcher": []
    }
  ]
}
```

Depois use: `Ctrl+Shift+P` → `Tasks: Run Task` → `Start Dev Server`

### Extensão Live Server (alternativa simples)

Se você só quer testar HTML/CSS sem a API:
1. Instale a extensão "Live Server" no VS Code
2. Clique direito em `data/index.html`
3. Escolha "Open with Live Server"

**Atenção**: Sem o servidor Python, as chamadas de API não funcionarão!

## 🛑 Parar o Servidor

Pressione `Ctrl+C` no terminal onde o servidor está rodando.

## ❓ Troubleshooting

### Erro: "Address already in use"
Outra aplicação está usando a porta 5000.
- **Solução 1**: Pare a outra aplicação
- **Solução 2**: Altere a porta no `dev-server.py`

### Erro: "ModuleNotFoundError: No module named 'flask'"
Dependências não instaladas.
```powershell
pip install -r requirements-dev.txt
```

### Página carrega mas API não responde
Verifique se:
1. O servidor está rodando
2. As credenciais no `app.js` estão corretas
3. Não há erros no console do navegador (F12)

---

**Desenvolvido para facilitar o desenvolvimento frontend sem hardware!** 🚀
