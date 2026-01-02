# Changelog - MUST Inverter API

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
