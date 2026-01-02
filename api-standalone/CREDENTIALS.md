# 🔐 Funcionalidade de Alteração de Credenciais

## ✅ Implementação Completa

### 📝 O que foi implementado:

#### 1. **Frontend - Página de Configurações**
   - **Arquivo**: `data/settings.html`
   - Interface completa para alterar usuário e senha
   - Validação de senha atual
   - Indicador de força da senha
   - Confirmação de senha
   - Mensagens de erro e sucesso

#### 2. **Backend ESP32 - Endpoint da API**
   - **Arquivo**: `src/main.cpp`
   - Novo endpoint: `POST /api/credentials`
   - Validação de senha atual
   - Salvamento persistente no Preferences (EEPROM)
   - Credenciais carregadas automaticamente no boot
   - Logs de auditoria

#### 3. **Dev Server - Simulação**
   - **Arquivos**: `dev-server.js` e `dev-server.py`
   - Mesma API do ESP32
   - Validações idênticas
   - Simulação (não persiste)

---

## 🚀 Como Usar

### No Dashboard

1. Acesse http://192.168.x.x (IP do ESP32)
2. Clique em **"⚙️ Configurações"** no card Sistema
3. Preencha:
   - **Senha Atual**: sua senha atual
   - **Novo Usuário** (opcional): deixe em branco para manter
   - **Nova Senha** (opcional): mínimo 6 caracteres
   - **Confirmar Nova Senha**
4. Clique em **"💾 Salvar Alterações"**
5. Faça login novamente com as novas credenciais

### Via API (cURL)

```bash
curl -X POST http://192.168.x.x/api/credentials \
  -u admin:admin123 \
  -H "Content-Type: application/json" \
  -d '{
    "current_password": "admin123",
    "username": "novousuario",
    "password": "novasenha123"
  }'
```

**Resposta de sucesso:**
```json
{
  "success": true,
  "message": "Credentials updated successfully",
  "username": "novousuario"
}
```

**Resposta de erro (senha incorreta):**
```json
{
  "error": "Current password is incorrect"
}
```

---

## 🔧 Detalhes Técnicos

### Estrutura da Requisição

**Endpoint:** `POST /api/credentials`  
**Autenticação:** HTTP Basic Auth (credenciais atuais)  
**Content-Type:** `application/json`

**Body:**
```json
{
  "current_password": "senha_atual",
  "username": "novo_usuario",    // opcional
  "password": "nova_senha"        // opcional
}
```

### Validações

✅ Senha atual deve estar correta  
✅ Nova senha deve ter no mínimo 6 caracteres  
✅ Pelo menos um campo (username ou password) deve ser fornecido  
✅ Autenticação HTTP Basic obrigatória

### Armazenamento

**ESP32:**
- **Namespace**: `credentials`
- **Chaves**: `api_user`, `api_pass`
- **Persistência**: Preferences (EEPROM virtual)
- **Carregamento**: Automático no boot

**Variáveis Globais:**
```cpp
String currentApiUser = "admin";  // Carregado do Preferences
String currentApiPass = "admin123"; // Carregado do Preferences
```

### Fluxo de Autenticação

1. **Boot do ESP32**:
   ```cpp
   prefs.begin("credentials", true);
   currentApiUser = prefs.getString("api_user", DEFAULT_API_USER);
   currentApiPass = prefs.getString("api_pass", DEFAULT_API_PASS);
   prefs.end();
   ```

2. **Requisição HTTP**:
   ```cpp
   bool checkAuthentication(AsyncWebServerRequest *request) {
     return request->authenticate(currentApiUser.c_str(), currentApiPass.c_str());
   }
   ```

3. **Alteração de Credenciais**:
   ```cpp
   prefs.begin("credentials", false);
   prefs.putString("api_user", newUsername);
   prefs.putString("api_pass", newPassword);
   prefs.end();
   
   currentApiUser = newUsername;
   currentApiPass = newPassword;
   ```

---

## 🔒 Segurança

### Medidas Implementadas

✅ **Autenticação obrigatória** - Requer credenciais atuais  
✅ **Validação de senha atual** - Impede alterações não autorizadas  
✅ **Senha mínima de 6 caracteres** - Segurança básica  
✅ **HTTPS recomendado** - Use certificados SSL em produção  
✅ **Logs de auditoria** - Registra tentativas de alteração

### Recomendações

⚠️ **Use HTTPS em produção** - Credenciais trafegam em Base64  
⚠️ **Senhas fortes** - Combine letras, números e símbolos  
⚠️ **Backup das credenciais** - Guarde em local seguro  
⚠️ **Factory Reset disponível** - Botão BOOT por 5 segundos

---

## 🐛 Troubleshooting

### Erro: "Current password is incorrect"
- Verifique se está usando a senha atual correta
- Verifique se há espaços extras
- Caso esqueça a senha, use Factory Reset (BOOT por 5s)

### Erro: "Password must be at least 6 characters"
- Nova senha deve ter no mínimo 6 caracteres
- Deixe em branco para manter a senha atual

### Credenciais não persistem no Dev Server
- **Normal!** O dev-server não salva alterações
- Use apenas para testar a interface
- No ESP32 real, as credenciais são salvas

### Factory Reset
1. Pressione e segure o botão **BOOT** por **5 segundos**
2. Credenciais voltam para padrão:
   - Usuário: `admin`
   - Senha: `admin123`
3. WiFi também é resetado

---

## 📊 Códigos HTTP

| Código | Significado |
|--------|-------------|
| `200` | Credenciais atualizadas com sucesso |
| `400` | Requisição inválida (JSON malformado, validação) |
| `401` | Senha atual incorreta ou não autenticado |

---

## 🧪 Testes

### Teste 1: Alterar apenas a senha
```bash
curl -X POST http://localhost:5000/api/credentials \
  -u admin:admin123 \
  -H "Content-Type: application/json" \
  -d '{"current_password":"admin123","password":"novasenha123"}'
```

### Teste 2: Alterar apenas o usuário
```bash
curl -X POST http://localhost:5000/api/credentials \
  -u admin:admin123 \
  -H "Content-Type: application/json" \
  -d '{"current_password":"admin123","username":"novousuario"}'
```

### Teste 3: Alterar ambos
```bash
curl -X POST http://localhost:5000/api/credentials \
  -u admin:admin123 \
  -H "Content-Type: application/json" \
  -d '{"current_password":"admin123","username":"novousuario","password":"novasenha123"}'
```

### Teste 4: Senha atual incorreta (deve falhar)
```bash
curl -X POST http://localhost:5000/api/credentials \
  -u admin:admin123 \
  -H "Content-Type: application/json" \
  -d '{"current_password":"senhaerrada","password":"novasenha123"}'
```

---

## 📝 Logs no ESP32

Ao alterar credenciais, você verá no Serial Monitor:

```
✅ Credentials updated successfully:
   Username: novousuario
   Password: ***
```

Ao inicializar o ESP32:

```
✓ Credentials loaded:
   Username: novousuario
   Password: ***
```

---

## 🔄 Compatibilidade

| Componente | Status |
|------------|--------|
| ESP32 | ✅ Implementado |
| ESP32-C3 | ✅ Implementado |
| ESP32-S3 | ✅ Implementado |
| Dev Server (Node.js) | ✅ Implementado |
| Dev Server (Python) | ✅ Implementado |
| Frontend | ✅ Implementado |

---

## 📚 Arquivos Modificados

1. **src/main.cpp**
   - Adicionadas variáveis globais `currentApiUser` e `currentApiPass`
   - Função `handleApiCredentials()` implementada
   - Carregamento de credenciais no `setup()`
   - Atualização da função `checkAuthentication()`
   - Nova rota `/api/credentials`

2. **data/settings.html**
   - Página completa de configurações
   - Requisição AJAX para `/api/credentials`
   - Validações no frontend

3. **data/index.html**
   - Botão "⚙️ Configurações" adicionado

4. **data/css/style.css**
   - Estilo `.btn-secondary` adicionado

5. **dev-server.js**
   - Endpoint `/api/credentials` implementado
   - Validações idênticas ao ESP32

6. **dev-server.py**
   - Endpoint `/api/credentials` implementado
   - Validações idênticas ao ESP32

---

**Implementação completa e funcional! 🎉**
