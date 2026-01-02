# 📮 Guia Postman - MUST Inverter API

## 🚀 Como Importar a Coleção

### Opção 1: Importar do Arquivo
1. Abra o **Postman**
2. Clique em **Import** (canto superior esquerdo)
3. Selecione o arquivo `MUST_Inverter_API.postman_collection.json`
4. Clique em **Import**

### Opção 2: Importar do JSON
1. Abra o **Postman**
2. Clique em **Import**
3. Selecione aba **Raw text**
4. Cole o conteúdo do arquivo JSON
5. Clique em **Continue** e depois **Import**

## 🔐 Configurar Autenticação

A coleção já vem com autenticação configurada!

**Credenciais padrão:**
- **Username:** `admin`
- **Password:** `G&n0101cori13`

### Alterar Credenciais
1. Clique na coleção **MUST Inverter API**
2. Aba **Authorization**
3. Type: **Basic Auth**
4. Edite Username e Password
5. Clique em **Save**

## 📡 Endpoints Disponíveis

### 1. **GET /api/sensors**
Retorna todos os dados dos sensores em tempo real.

**Response:**
```json
{
  "battery": {
    "voltage": 52.4,
    "current": 12.5,
    "power": 655,
    "soc": 85,
    "temperature": 28.5
  },
  "pv": {
    "voltage": 85.2,
    "current": 8.3,
    "power": 707
  },
  "charger": {
    "voltage": 220.5,
    "current": 3.2,
    "power": 705,
    "accumulated_kwh": 125.8
  },
  "inverter": {
    "mode": 2,
    "ac_voltage": 220.3,
    "ac_current": 2.8,
    "ac_frequency": 60.0,
    "ac_power": 616,
    "load_percent": 45
  },
  "energy": {
    "total_charged_kwh": 1250.5,
    "total_discharged_kwh": 1100.2
  },
  "device": {
    "temperature": 42.3
  }
}
```

### 2. **GET /api/status**
Retorna informações sobre o dispositivo.

**Response:**
```json
{
  "device_name": "MUST Inverter API (Dev Server)",
  "ip_address": "127.0.0.1",
  "mac_address": "00:00:00:00:00:00",
  "wifi_ssid": "Development",
  "wifi_rssi": -42,
  "uptime_seconds": 3652,
  "free_heap": 200000,
  "modbus_connected": false
}
```

### 3. **POST /api/reset**
Reseta as configurações de WiFi (apenas no ESP32, não funciona no dev-server).

**Request Body:**
```json
{}
```

**Response:**
```json
{
  "message": "Reset não disponível no dev server"
}
```

### 4. **POST /api/credentials**
Altera as credenciais de acesso à API.

**Request Body:**
```json
{
  "current_password": "G&n0101cori13",
  "username": "admin",
  "password": "NewSecurePass123"
}
```

**Parâmetros:**
- `current_password` **(obrigatório)** - Senha atual para validação
- `username` *(opcional)* - Novo usuário (deixe em branco para manter)
- `password` *(opcional)* - Nova senha (deixe em branco para manter)

**Response (Sucesso):**
```json
{
  "success": true,
  "message": "Credentials updated successfully",
  "username": "admin",
  "note": "Credenciais salvas em credentials.json"
}
```

**Response (Erro - Senha Incorreta):**
```json
{
  "error": "Current password is incorrect"
}
```

### 5. **GET /logout**
Limpa o cache de autenticação do navegador.

**Autenticação:** Não requerida

**Response:**
```json
{
  "message": "Logged out"
}
```

## 🎯 Exemplos de Uso

### Teste Básico de Autenticação
1. Inicie o dev-server: `node dev-server.js`
2. No Postman, execute **GET /api/status**
3. Deve retornar status 200 com dados do dispositivo

### Alterar Senha
1. Execute **POST /api/credentials** com:
   ```json
   {
     "current_password": "G&n0101cori13",
     "password": "MinhaNovaSenh@123"
   }
   ```
2. Atualize a senha na aba **Authorization** da coleção
3. Teste novamente com **GET /api/status**

### Monitorar Dados em Tempo Real
1. Execute **GET /api/sensors**
2. Clique em **Save Response** → **Save as example**
3. Use o **Runner** para executar automaticamente a cada X segundos

## 🔧 Variáveis de Ambiente

A coleção já vem com variáveis configuradas:

| Variável | Valor | Descrição |
|----------|-------|-----------|
| `baseUrl` | `http://localhost:5000` | URL base da API |
| `username` | `admin` | Nome de usuário |
| `password` | `G&n0101cori13` | Senha de acesso |

### Usar Variáveis nas Requisições
Para usar em URLs: `{{baseUrl}}/api/sensors`
Para usar em Body: `{{username}}` ou `{{password}}`

## ⚠️ Observações Importantes

### Caractere & na Senha
Se você usar o caractere `&` na senha:
- ✅ Funciona perfeitamente via API/Postman
- ⚠️ Pode ter problemas em navegadores antigos via HTTP Basic Auth
- 💡 Recomendação: Use via API ou evite o caractere para máxima compatibilidade

### Dev Server vs ESP32
- **Dev Server** (localhost:5000): Dados simulados, credenciais em `credentials.json`
- **ESP32** (IP do dispositivo): Dados reais do inversor, credenciais em EEPROM

### CORS
O dev-server está configurado com CORS habilitado, permitindo requisições de qualquer origem durante o desenvolvimento.

## 📊 Collection Runner

Para monitoramento contínuo:
1. Clique nos **três pontos** da coleção
2. Selecione **Run collection**
3. Desmarque endpoints que não quer executar
4. Configure **Delay** entre requisições (ex: 5000ms = 5s)
5. Clique em **Run MUST Inverter API**

## 🐛 Troubleshooting

### Erro: "Could not get any response"
- Verifique se o dev-server está rodando: `node dev-server.js`
- Confirme a URL: `http://localhost:5000`

### Erro: 401 Unauthorized
- Verifique usuário e senha na aba **Authorization**
- Credenciais padrão: admin / G&n0101cori13
- Se alterou as credenciais, atualize na coleção

### Erro: Connection refused
- Certifique-se que o servidor está rodando
- Verifique se a porta 5000 não está em uso por outro processo

## 📚 Recursos Adicionais

- [Documentação do Projeto](../README.md)
- [CHANGELOG](CHANGELOG.md)
- [Guia de Internacionalização](data/I18N_README.md)

---

**Desenvolvido para o projeto MUST PV19 Monitor** 🔋⚡
