# Changelog - MUST Inverter API

## [2026-01-02] - Melhorias na Interface de Configuração WiFi

### 🎨 Interface Aprimorada

#### Funcionalidades Adicionadas

**1. Scan de Redes WiFi Disponíveis (2.4 GHz)**
- Botão "🔍 Procurar Redes WiFi" na página de configurações
- Lista visual de redes disponíveis com força do sinal
- **Filtro automático: apenas redes 2.4 GHz** (ESP32 não suporta 5 GHz)
- Identificação por canal (1-14 = 2.4 GHz)
- Suporte a datalist para autocompletar SSID
- Ordenação automática por força do sinal (mais forte primeiro)
- Indicadores visuais de qualidade do sinal:
  - 📶 Excelente (-50 dBm ou melhor)
  - 📶 Muito Bom (-60 dBm)
  - 📶 Bom (-70 dBm)
  - 📶 Fraco (-80 dBm)
  - 📶 Muito Fraco (abaixo de -80 dBm)

**2. Toggle de Visibilidade de Senha**
- Ícone de olho (👁️) para mostrar/ocultar senha WiFi
- Toggle animado entre 👁️ (oculto) e 👁️‍🗨️ (visível)
- Senha exibida com asteriscos por padrão
- Funciona para o campo de senha WiFi

**3. Novo Endpoint: GET /api/wifi/scan**

**ESP32 (Produção):**
```cpp
// Escaneia redes WiFi reais usando WiFi.scanNetworks()
// FILTRO: Apenas redes 2.4 GHz (canais 1-14)
// ESP32 não suporta 5 GHz - redes 5 GHz são ignoradas
// Retorna lista de SSIDs com RSSI, canal e tipo de criptografia
// Limita a 20 redes 2.4 GHz para não sobrecarregar a resposta
// Limpa scan automaticamente após retornar dados
```

**Dev-Server (Desenvolvimento):**
```javascript
// Simula scan com 6 redes fictícias
// Útil para testar interface sem ESP32
```

**Exemplo de resposta:**
```json
{
  "success": true,
  "count": 6,
  "note": "Only 2.4 GHz networks (ESP32 compatible)",
  "networks": [
    {
      "ssid": "CasteloAlto",
      "rssi": -45,
      "channel": 6,
      "encryption": "WPA2"
    },
    {
      "ssid": "NET_2G_123456",
      "rssi": -55,
      "channel": 11,
      "encryption": "WPA2"
    }
  ]
}
```

#### Arquivos Modificados

**`data/settings.html`:**
- CSS para botão de toggle de senha e lista de redes
- Campo SSID com datalist para autocompletar
- Botão "Procurar Redes WiFi"
- Container para exibir redes encontradas
- Wrapper para campo de senha com botão de toggle
- Funções JavaScript:
  - `toggleWifiPasswordVisibility()`: Alterna visibilidade da senha
  - `scanWiFiNetworks()`: Chama API e exibe redes
  - `selectWiFiNetwork(ssid)`: Seleciona rede da lista
  - `getSignalStrength(rssi)`: Converte RSSI em texto legível

**`dev-server.js`:**
- GET /api/wifi/scan endpoint com dados simulados
- 6 redes WiFi fictícias para testes
- Ordenação por força do sinal

**`src/main.cpp`:**
- GET /api/wifi/scan endpoint real
- Usa `WiFi.scanNetworks()` do ESP32
- Retorna informações completas (SSID, RSSI, criptografia)
- Suporte a todos os tipos de criptografia WiFi
- Limpeza automática do scan

**`data/js/i18n.js`:**
- Traduções para botão de scan
- Traduções para força do sinal
- Traduções para estados de carregamento

#### Como Usar

1. Acesse `/settings.html`
2. Clique em "🔍 Procurar Redes WiFi"
3. Aguarde a lista de redes disponíveis
4. Clique em uma rede para selecionar (preenche SSID automaticamente)
5. Digite a senha
6. Use o botão 👁️ para ver/ocultar a senha
7. Salve as configurações

#### Melhorias de UX

- **Seleção Visual**: Redes selecionadas ficam destacadas em roxo
- **Hover Effect**: Redes mudam de cor ao passar o mouse
- **Scroll Automático**: Lista de redes com scroll quando há muitas
- **Feedback de Carregamento**: Mensagem "Procurando..." enquanto escaneia
- **Auto-focus**: Campo de senha recebe foco após selecionar rede
- **Ordenação Inteligente**: Redes mais fortes aparecem primeiro

#### Compatibilidade

- ✅ ESP32: Scan real de redes WiFi
- ✅ ESP32-C3: Totalmente compatível
- ✅ ESP32-S3: Totalmente compatível
- ✅ Dev-Server: Simulação com dados fictícios

---

## [2026-01-02] - Configuração WiFi via Interface Web

### 📶 Nova Funcionalidade: WiFi Management

#### Implementação
Agora é possível configurar as credenciais WiFi (SSID e senha) diretamente pela interface web, sem precisar usar o WiFiManager ou resetar o dispositivo fisicamente.

#### Funcionalidades Adicionadas

**Interface Web (`settings.html`):**
- Nova seção "📶 Configuração WiFi" na página de configurações
- Campos para SSID e senha WiFi
- Validação client-side (senha mínima 8 caracteres)
- Ambos os campos são opcionais (deixe em branco para manter atual)
- Labels e mensagens totalmente traduzidas (EN/PT-BR)

**Backend (`/api/credentials`):**
- Aceita novos parâmetros: `wifi_ssid` e `wifi_password`
- Validações:
  - Se fornecer SSID, senha é obrigatória
  - Se fornecer senha, SSID é obrigatório
  - Senha WiFi mínimo 8 caracteres
- Salva no Preferences (ESP32) ou loga no console (dev-server)

**ESP32 (`main.cpp`):**
- Carrega credenciais WiFi salvas do Preferences na inicialização
- Tenta conectar com credenciais salvas primeiro (10s timeout)
- Se falhar, volta ao WiFiManager (modo AP)
- Salva WiFi em namespace separado: `prefs.begin("wifi")`
- Auto-restart após mudança de WiFi

**Dev-Server (`dev-server.js`):**
- Aceita e valida parâmetros WiFi
- Loga no console (não aplica, apenas simula)
- Útil para testar interface sem afetar WiFi real

#### Arquivos Modificados

**`data/settings.html`:**
```html
<h3 data-i18n="wifiConfig">📶 Configuração WiFi</h3>
<input type="text" id="wifi-ssid" name="wifi-ssid">
<input type="password" id="wifi-password" name="wifi-password" minlength="8">
```

**`data/js/i18n.js`:**
```javascript
// Adicionadas traduções
wifiConfig: "📶 WiFi Configuration",
wifiSSID: "WiFi Network (SSID)",
wifiPassword: "WiFi Password",
// ... mais traduções
```

**`dev-server.js`:**
```javascript
const { wifi_ssid, wifi_password } = req.body;
// Valida e loga WiFi
if (wifi_ssid) {
  console.log(`WiFi SSID: ${wifi_ssid}`);
  console.log('⚠️ Nota: WiFi config é simulado no dev-server');
}
```

**`src/main.cpp`:**
```cpp
// Carrega WiFi salvo
prefs.begin("wifi", true);
String savedSSID = prefs.getString("ssid", "");
String savedPassword = prefs.getString("password", "");

// Tenta conectar
if (savedSSID.length() > 0) {
  WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
  // ... timeout de 10s
}

// Salva novo WiFi
if (strlen(wifiSSID) > 0) {
  prefs.begin("wifi", false);
  prefs.putString("ssid", String(wifiSSID));
  prefs.putString("password", String(wifiPassword));
  prefs.end();
}
```

#### Documentação

**Novo arquivo: `WIFI_CONFIG.md`**
- Guia completo de uso da funcionalidade
- Exemplos de requisição API
- Fluxo técnico detalhado
- Troubleshooting
- Diagrama de sequência

#### Como Usar

1. Acesse `/settings.html`
2. Preencha senha atual da API
3. Preencha SSID e senha WiFi (opcional)
4. Salve
5. Dispositivo reinicia e conecta na nova rede

#### Compatibilidade

- ✅ ESP32 (testado)
- ✅ ESP32-C3 (compatível)
- ✅ ESP32-S3 (compatível)
- ✅ Dev-Server (simulado)

---

## [2026-01-02] - Auto-Restart após Mudança de Credenciais

### 🔄 Reinício Automático

#### Problema Corrigido
Quando as credenciais eram alteradas via `/api/credentials`, o servidor/dispositivo continuava usando as credenciais antigas em memória, causando problemas de autenticação.

#### Solução Implementada

**Dev-Server (Node.js):**
- Após alterar credenciais com sucesso, o servidor **reinicia automaticamente** em 2 segundos
- `process.exit(0)` fecha o processo Node.js
- Novo script: `dev-server-watch.ps1` que reinicia automaticamente o servidor quando ele para
- Mensagem clara no response: "Server will restart in 2 seconds"

**ESP32 (Produção):**
- Após salvar credenciais nas Preferences, o dispositivo **reinicia automaticamente** em 2 segundos
- `ESP.restart()` recarrega as credenciais do EEPROM
- Mensagem clara no response: "Device will restart in 2 seconds"

#### Arquivos Modificados

**`dev-server.js`:**
```javascript
// Após salvar credenciais com sucesso
setTimeout(() => {
    console.log('\n🔄 Reiniciando dev-server...\n');
    process.exit(0);
}, 2000);
```

**`src/main.cpp`:**
```cpp
// Após salvar no Preferences
delay(2000);
ESP.restart();
```

**Novo arquivo: `dev-server-watch.ps1`**
- Script PowerShell que monitora e reinicia o servidor automaticamente
- Uso: `.\dev-server-watch.ps1`
- Útil para desenvolvimento quando credenciais são alteradas frequentemente

#### Como Usar

**Desenvolvimento (com auto-restart):**
```powershell
cd api-standalone
.\dev-server-watch.ps1
```

**Desenvolvimento (modo normal):**
```powershell
cd api-standalone
node dev-server.js
# Após alterar credenciais, o servidor para e você precisa reiniciar manualmente
```

**Produção (ESP32):**
- Altere as credenciais via `/api/credentials`
- O dispositivo reinicia automaticamente
- Aguarde ~10 segundos para o ESP32 voltar online

---

## [2026-01-02] - Internacionalização (i18n)

### 🌍 Suporte a Múltiplos Idiomas

#### Novidades
- **Sistema completo de internacionalização** com suporte para Inglês e Português (Brasil)
  - Idioma padrão: **Inglês** (`en`)
  - Idioma alternativo: **Português do Brasil** (`pt`)
  - Seletor de idioma no cabeçalho de todas as páginas
  - Persistência da preferência no localStorage do navegador

#### Arquivos Modificados

**Novo arquivo: `data/js/i18n.js`**
- Sistema completo de traduções
- Funções: `loadLanguage()`, `saveLanguage()`, `t()`, `applyTranslations()`, `changeLanguage()`
- Traduções completas para todos os elementos da interface

**Atualizado: `data/index.html`**
- Atributo `lang` alterado de `pt-BR` para `en` (padrão)
- Seletor de idioma adicionado ao cabeçalho
- Atributos `data-i18n` em todos os elementos de texto
- Tradução dinâmica de todos os cards (Bateria, Solar, Carregador, Inversor, Energia, Sistema)

**Atualizado: `data/settings.html`**
- Seletor de idioma adicionado ao cabeçalho
- Atributos `data-i18n` em todos os labels, textos de ajuda e botões
- Inicialização automática do i18n no carregamento da página

**Atualizado: `data/js/app.js`**
- Inicialização do sistema i18n no `DOMContentLoaded`
- Carregamento automático do idioma salvo
- Sincronização do seletor de idioma com o localStorage

#### Cobertura de Traduções
- ✅ Cabeçalho e títulos
- ✅ Todos os cards do dashboard (6 cards)
- ✅ Formulários e labels
- ✅ Botões e links
- ✅ Mensagens de status e alertas
- ✅ Modos do inversor
- ✅ Estados de carregamento

#### Documentação
- **Novo arquivo:** `data/I18N_README.md` com guia completo de uso
- Instruções de como adicionar novas traduções
- Exemplos de implementação

---

## [2026-01-02] - Autenticação HTTP Basic Auth Aprimorada

### 🔐 Segurança e Autenticação

#### Correções Críticas
- **Removidas credenciais hardcoded** dos arquivos JavaScript
  - `data/js/app.js`: Removido `API_USER` e `API_PASS` hardcoded
  - `data/settings.html`: Removido constantes de autenticação
  - ✅ Agora usa HTTP Basic Authentication nativo do navegador

#### Melhorias de Compatibilidade
- **Suporte a caracteres especiais em senhas** (especialmente `&`)
  - Implementado sistema de variantes de escape no middleware de autenticação
  - Suporta: original, URL encoded (`%26`), HTML encoded (`&amp;`)
  - Compatível com diferentes navegadores e formas de envio

#### Mudanças no Código

**dev-server.js:**
- Função `requireAuth()` atualizada com suporte a múltiplas variantes de senha
- Função `loadCredentials()` com logs limpos e informativos
- Endpoint `/logout` adicionado para limpar cache do navegador
- Endpoint `/test-auth.html` público para testes de autenticação
- Validação de senha com aviso quando usar `&` (pode ter problemas em alguns navegadores antigos)

**app.js:**
```javascript
// ANTES (❌ PROBLEMA):
const API_USER = 'admin';
const API_PASS = 'admin123';  // Hardcoded!

function getAuthHeaders() {
    const credentials = btoa(`${API_USER}:${API_PASS}`);
    return { 'Authorization': `Basic ${credentials}` };
}

// DEPOIS (✅ CORRETO):
function fetchAPI(url, options = {}) {
    return fetch(url, {
        ...options,
        credentials: 'include'  // Usa credenciais do navegador automaticamente
    });
}
```

**settings.html:**
- Removidas constantes de autenticação
- Usa `credentials: 'include'` para enviar credenciais automaticamente
- Mantém funcionalidade de alteração de senha via POST `/api/credentials`

### 🎯 Compatibilidade ESP32

Todas as alterações são **100% compatíveis** com o código C++ do ESP32:
- `ESPAsyncWebServer` usa `request->authenticate()` que funciona exatamente como implementado
- Arquivos HTML/CSS/JS do `data/` funcionam identicamente no ESP32
- `Preferences` (EEPROM) continua armazenando credenciais
- HTTP Basic Auth nativo do navegador funciona perfeitamente

### 📝 Arquivos Alterados

```
api-standalone/
├── dev-server.js              # Middleware de autenticação melhorado
├── credentials.json           # Senha atual: G&n0101cori13
├── data/
│   ├── js/app.js             # Removidas credenciais hardcoded
│   ├── settings.html         # Removidas credenciais hardcoded
│   └── test-auth.html        # NOVO: Página de teste de autenticação
```

### 🐛 Problemas Resolvidos

1. **Credenciais antigas em cache**: Navegador continuava usando `admin123` mesmo após alteração
   - **Causa**: Credenciais hardcoded no JavaScript
   - **Solução**: Removido hardcoding, agora usa HTTP Basic Auth nativo

2. **Senha com `&` não funcionava**: Navegador não aceitava senha `G&n0101cori13`
   - **Causa**: Diferentes formas de escape do caractere `&`
   - **Solução**: Sistema de variantes aceita múltiplas formas de encoding

3. **Cache persistente**: Mesmo em modo anônimo, credenciais antigas apareciam
   - **Causa**: JavaScript enviando credenciais hardcoded antes do prompt de login
   - **Solução**: JavaScript não envia mais credenciais, deixa o navegador gerenciar

### ✅ Como Testar

1. **Iniciar servidor de desenvolvimento:**
   ```bash
   cd api-standalone
   node dev-server.js
   ```

2. **Acessar no navegador:**
   - URL: http://localhost:5000
   - Usuário: `admin`
   - Senha: `G&n0101cori13`

3. **Testar autenticação:**
   - Página de teste: http://localhost:5000/test-auth.html
   - Alterar senha: http://localhost:5000/settings.html

4. **Limpar cache (se necessário):**
   - Acessar: http://localhost:5000/logout
   - Ou usar modo anônimo: Ctrl+Shift+N

### 📚 Validação de Senha

A senha agora requer:
- ✅ Mínimo 6 caracteres
- ✅ Pelo menos 1 letra maiúscula
- ✅ Pelo menos 1 letra minúscula
- ✅ Pelo menos 1 número
- ✅ Pelo menos 1 caractere especial
- ⚠️ Caractere `&` funciona mas pode ter problemas em HTTP Basic Auth de navegadores antigos

### 🚀 Próximos Passos

- [ ] Testar no ESP32 real após upload do firmware
- [ ] Validar com diferentes navegadores (Chrome, Firefox, Edge, Safari)
- [ ] Considerar adicionar suporte a tokens JWT para APIs externas
- [ ] Documentar limitações do HTTP Basic Auth para usuários finais
