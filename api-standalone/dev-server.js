#!/usr/bin/env node
/**
 * Development Server - Local Mock API for Frontend Testing
 * Simula o comportamento da API do ESP32 rodando localmente
 */

const express = require('express');
const cors = require('cors');
const basicAuth = require('basic-auth');
const path = require('path');
const fs = require('fs');

const app = express();
const PORT = 5000;

// Arquivo para salvar credenciais
const CREDENTIALS_FILE = path.join(__dirname, 'credentials.json');

// Configuração de autenticação (valores padrão)
let API_USER = 'admin';
let API_PASS = 'admin123';
let WIFI_SSID = '';
let WIFI_PASSWORD = '';

// Carregar credenciais salvas (se existirem)
function loadCredentials() {
    try {
        if (fs.existsSync(CREDENTIALS_FILE)) {
            const data = fs.readFileSync(CREDENTIALS_FILE, 'utf8');
            const saved = JSON.parse(data);
            API_USER = saved.username || 'admin';
            API_PASS = saved.password || 'admin123';
            WIFI_SSID = saved.wifi_ssid || '';
            WIFI_PASSWORD = saved.wifi_password || '';
            console.log('✓ Credenciais carregadas do arquivo credentials.json');
            console.log(`   Usuário: ${API_USER}`);
            if (WIFI_SSID) {
                console.log(`   WiFi SSID: ${WIFI_SSID}`);
                console.log('   WiFi Password: ***');
            }
        } else {
            console.log('ℹ️ Usando credenciais padrão (arquivo não existe)');
        }
    } catch (error) {
        console.warn('⚠️ Erro ao carregar credenciais, usando valores padrão:', error.message);
    }
}

// Salvar credenciais em arquivo
function saveCredentials(username, password, wifiSSID, wifiPassword) {
    try {
        const data = {
            username: username,
            password: password,
            updated_at: new Date().toISOString()
        };
        
        // Adicionar WiFi apenas se fornecido
        if (wifiSSID) {
            data.wifi_ssid = wifiSSID;
            data.wifi_password = wifiPassword;
        } else if (WIFI_SSID) {
            // Manter WiFi existente se não foi fornecido novo
            data.wifi_ssid = WIFI_SSID;
            data.wifi_password = WIFI_PASSWORD;
        }
        
        fs.writeFileSync(CREDENTIALS_FILE, JSON.stringify(data, null, 2), 'utf8');
        console.log('✓ Credenciais salvas em credentials.json');
        return true;
    } catch (error) {
        console.error('❌ Erro ao salvar credenciais:', error.message);
        return false;
    }
}

// Carregar credenciais na inicialização
loadCredentials();

// Timestamp de início do servidor
const START_TIME = Date.now();

// Middleware
app.use(cors()); // Enable CORS for local development
app.use(express.json());

// Middleware de autenticação
function requireAuth(req, res, next) {
    const credentials = basicAuth(req);
    
    if (!credentials) {
        res.set('WWW-Authenticate', 'Basic realm="Login Required"');
        return res.status(401).json({ error: 'Authentication required' });
    }
    
    // Verificar username
    if (credentials.name !== API_USER) {
        res.set('WWW-Authenticate', 'Basic realm="Login Required"');
        return res.status(401).json({ error: 'Authentication required' });
    }
    
    // Tentar múltiplas formas de decodificar a senha (para suportar diferentes escapamentos do &)
    const receivedPass = credentials.pass;
    const validPasswords = [
        API_PASS,                                    // Original
        decodeURIComponent(API_PASS),               // URL decoded
        API_PASS.replace(/&/g, '%26'),              // URL encoded
        API_PASS.replace(/&/g, '&amp;'),            // HTML encoded
    ];
    
    // Também tentar decodificar a senha recebida
    const receivedVariants = [
        receivedPass,
        decodeURIComponent(receivedPass),
        receivedPass.replace(/%26/g, '&'),
        receivedPass.replace(/&amp;/g, '&'),
    ];
    
    let authenticated = false;
    for (const variant of receivedVariants) {
        if (validPasswords.includes(variant)) {
            authenticated = true;
            break;
        }
    }
    
    if (!authenticated) {
        res.set('WWW-Authenticate', 'Basic realm="Login Required"');
        return res.status(401).json({ error: 'Authentication required' });
    }
    
    next();
}

// Função para gerar dados simulados (mesma lógica do ESP32)
function generateDemoData() {
    const currentTime = Date.now() / 1000;
    const variance = Math.sin(currentTime / 30.0) * 0.1; // Variação lenta
    
    // Solar PV - simulando sol parcial
    const pvVoltage = 85.0 + variance * 10.0;
    const pvCurrent = 4.5 + variance * 2.0;
    const pvPower = pvVoltage * pvCurrent;
    
    // Carregador
    const chargerVoltage = 54.2 + variance * 0.5;
    const chargerCurrent = 8.2 + variance * 1.0;
    const chargerPower = 445.0 + variance * 50.0;
    
    // Bateria - simulando estado de carga
    const batteryVoltage = 52.8 + variance * 0.3;
    const batteryCurrent = 8.0 + variance * 1.5;
    const batteryPower = 422.0 + variance * 40.0;
    const batterySOC = 65.0 + variance * 5.0;
    const batteryTemp = 25.0 + variance * 2.0;
    
    // Inversor - simulando modo off-grid com carga
    const acVoltage = 220.0 + variance * 2.0;
    const acCurrent = 2.5 + variance * 0.5;
    const acFrequency = 50.0 + variance * 0.1;
    const acPower = 550.0 + variance * 50.0;
    const loadPercent = 27.5 + variance * 5.0;
    const dcVoltage = 52.8 + variance * 0.3;
    
    // Acumuladores - incrementando lentamente
    const timeHours = currentTime / 3600.0;
    const chargerAccumulated = 12.5 + (timeHours * 0.1) % 5.0;
    const totalCharged = 245.8 + (timeHours * 0.2) % 10.0;
    const totalDischarged = 198.3 + (timeHours * 0.15) % 8.0;
    
    const deviceTemp = 42.0 + variance * 3.0;
    
    return {
        charger: {
            voltage: Math.round(chargerVoltage * 10) / 10,
            current: Math.round(chargerCurrent * 100) / 100,
            power: Math.round(chargerPower),
            accumulated_power: Math.round(chargerAccumulated * 10) / 10
        },
        pv: {
            voltage: Math.round(pvVoltage * 10) / 10,
            current: Math.round(pvCurrent * 100) / 100,
            power: Math.round(pvPower)
        },
        battery: {
            voltage: Math.round(batteryVoltage * 10) / 10,
            current: Math.round(batteryCurrent * 100) / 100,
            power: Math.round(batteryPower),
            soc: Math.round(batterySOC),
            temperature: Math.round(batteryTemp * 10) / 10
        },
        inverter: {
            mode: "Off-Grid",
            mode_id: 3,
            ac_voltage: Math.round(acVoltage * 10) / 10,
            ac_current: Math.round(acCurrent * 100) / 100,
            ac_frequency: Math.round(acFrequency * 100) / 100,
            ac_power: Math.round(acPower),
            load_percent: Math.round(loadPercent),
            dc_voltage: Math.round(dcVoltage * 10) / 10,
            max_charge_current: 60,
            max_discharge_current: 60,
            accumulated_power: Math.round(totalDischarged * 10) / 10
        },
        totals: {
            total_charged: Math.round(totalCharged * 10) / 10,
            total_discharged: Math.round(totalDischarged * 10) / 10,
            device_temperature: Math.round(deviceTemp * 10) / 10
        },
        last_update: Math.floor(currentTime),
        uptime: Math.floor((Date.now() - START_TIME) / 1000),
        modbus_error: false,
        demo_mode: true
    };
}

// ==================== ROTAS DA API ====================

app.get('/api/sensors', requireAuth, (req, res) => {
    const data = generateDemoData();
    res.json(data);
});

app.get('/api/status', requireAuth, (req, res) => {
    res.json({
        device_name: "MUST Inverter API (Dev Server)",
        ip_address: "127.0.0.1",
        mac_address: "00:00:00:00:00:00",
        wifi_ssid: WIFI_SSID || "Development",
        wifi_rssi: -42,
        uptime_seconds: Math.floor((Date.now() - START_TIME) / 1000),
        free_heap: 200000,
        modbus_connected: false
    });
});

app.post('/api/reset', requireAuth, (req, res) => {
    res.json({ message: "Reset não disponível no dev server" });
});

app.get('/api/credentials', requireAuth, (req, res) => {
    // Retorna as credenciais atuais (sem a senha por segurança)
    res.json({
        username: API_USER,
        wifi_ssid: WIFI_SSID || null,
        has_wifi_password: WIFI_PASSWORD && WIFI_PASSWORD.length > 0
    });
});

app.get('/api/wifi/scan', requireAuth, (req, res) => {
    // Simular scan de redes WiFi (apenas 2.4 GHz - ESP32 não suporta 5 GHz)
    console.log('🔍 Simulando scan de redes WiFi 2.4 GHz...');
    
    const simulatedNetworks = [
        { ssid: 'CasteloAlto', rssi: -45, encryption: 'WPA2', channel: 6 },
        { ssid: 'NET_2G_123456', rssi: -55, encryption: 'WPA2', channel: 11 },
        { ssid: 'Vivo-Fibra-2.4G', rssi: -65, encryption: 'WPA2', channel: 1 },
        { ssid: 'Claro_WiFi', rssi: -72, encryption: 'WPA2', channel: 3 },
        { ssid: 'TIM-FIBRA-2.4G', rssi: -78, encryption: 'WPA2', channel: 9 },
        { ssid: 'WiFi-Guest', rssi: -85, encryption: 'Open', channel: 7 }
        // Nota: Redes 5 GHz são filtradas (ESP32 não suporta)
    ];
    
    // Ordenar por sinal (mais forte primeiro)
    simulatedNetworks.sort((a, b) => b.rssi - a.rssi);
    
    res.json({
        success: true,
        networks: simulatedNetworks,
        count: simulatedNetworks.length,
        note: 'Only 2.4 GHz networks (ESP32 compatible)'
    });
});

app.post('/api/credentials', requireAuth, (req, res) => {
    const { username, password, current_password, wifi_ssid, wifi_password } = req.body;
    
    // Validar senha atual
    if (current_password !== API_PASS) {
        return res.status(401).json({ 
            error: 'Current password is incorrect' 
        });
    }
    
    // Validar se há algo para alterar
    if (!username && !password && !wifi_ssid) {
        return res.status(400).json({ 
            error: 'No changes provided' 
        });
    }
    
    // Validar tamanho da senha
    if (password && password.length < 6) {
        return res.status(400).json({ 
            error: 'Password must be at least 6 characters' 
        });
    }
    
    // Validar WiFi
    if (wifi_ssid && !wifi_password) {
        return res.status(400).json({ 
            error: 'WiFi password is required when SSID is provided' 
        });
    }
    
    if (wifi_password && wifi_password.length < 8) {
        return res.status(400).json({ 
            error: 'WiFi password must be at least 8 characters' 
        });
    }
    
    // Avisar sobre limitações do caractere & na senha
    let warning = null;
    if (password && password.includes('&')) {
        warning = 'Atenção: O caractere "&" funciona via API mas pode não funcionar na caixa de login HTTP Basic Auth de alguns navegadores. Para máxima compatibilidade, evite usar "&" ou acesse via JavaScript/API.';
    }
    
    console.log('⚠️ Tentativa de alteração de credenciais:');
    console.log(`   Novo usuário: ${username || '(não alterado)'}`);
    console.log(`   Nova senha: ${password ? '***' : '(não alterada)'}`);
    if (wifi_ssid) {
        console.log(`   WiFi SSID: ${wifi_ssid}`);
        console.log(`   WiFi Password: ${wifi_password ? '***' : '(não alterada)'}`);
        console.log('   ⚠️ Nota: WiFi config é simulado no dev-server (apenas log)');
    }
    if (warning) {
        console.log(`   ⚠️ Aviso: ${warning}`);
    }
    
    // Definir valores finais
    const finalUsername = username || API_USER;
    const finalPassword = password || API_PASS;
    const finalWifiSSID = wifi_ssid || WIFI_SSID;
    const finalWifiPassword = wifi_password || WIFI_PASSWORD;
    
    // Salvar em arquivo
    if (saveCredentials(finalUsername, finalPassword, finalWifiSSID, finalWifiPassword)) {
        // Atualizar variáveis globais
        API_USER = finalUsername;
        API_PASS = finalPassword;
        
        if (wifi_ssid) {
            WIFI_SSID = finalWifiSSID;
            WIFI_PASSWORD = finalWifiPassword;
        }
        
        const response = { 
            success: true, 
            message: "Credentials updated successfully. Server will restart in 2 seconds.",
            username: finalUsername,
            note: "Dev-server will automatically restart to load new credentials"
        };
        
        if (warning) {
            response.warning = warning;
        }
        
        console.log('✅ Credenciais alteradas com sucesso!');
        console.log('🔄 Reiniciando servidor em 2 segundos...');
        
        // Enviar resposta e depois reiniciar o servidor
        res.json(response);
        
        // Aguardar 2 segundos e reiniciar o processo Node.js
        setTimeout(() => {
            console.log('\n🔄 Reiniciando dev-server...\n');
            process.exit(0); // Exit code 0 indica sucesso
        }, 2000);
    } else {
        res.status(500).json({
            error: "Failed to save credentials"
        });
    }
});

// ==================== SERVIR ARQUIVOS ESTÁTICOS ====================

// IMPORTANTE: Rotas específicas ANTES do express.static para ter controle de autenticação

// Rota raiz (requer autenticação)
app.get('/', requireAuth, (req, res) => {
    res.sendFile(path.join(__dirname, 'data', 'index.html'));
});

// Página de configuração (sem autenticação - modo AP)
app.get('/config.html', (req, res) => {
    res.sendFile(path.join(__dirname, 'data', 'config.html'));
});

// Página de teste de autenticação (sem autenticação - para debug)
app.get('/test-auth.html', (req, res) => {
    res.sendFile(path.join(__dirname, 'data', 'test-auth.html'));
});

// Endpoint para forçar logout (retorna 401 para limpar cache)
app.get('/logout', (req, res) => {
    res.set('WWW-Authenticate', 'Basic realm="Login Required"');
    res.status(401).send('Logout realizado. Feche o navegador ou use modo anônimo.');
});

// Página de configurações (requer autenticação)
app.get('/settings.html', requireAuth, (req, res) => {
    res.sendFile(path.join(__dirname, 'data', 'settings.html'));
});

// index.html também deve exigir autenticação
app.get('/index.html', requireAuth, (req, res) => {
    res.sendFile(path.join(__dirname, 'data', 'index.html'));
});

// Servir apenas CSS e JS sem autenticação (arquivos estáticos necessários)
app.use('/css', express.static(path.join(__dirname, 'data', 'css')));
app.use('/js', express.static(path.join(__dirname, 'data', 'js')));

// ==================== INICIALIZAÇÃO ====================

app.listen(PORT, '0.0.0.0', () => {
    console.log('\n' + '='.repeat(60));
    console.log('  🚀 MUST Inverter API - Development Server');
    console.log('='.repeat(60));
    console.log('\n📌 Servidor rodando em modo de desenvolvimento');
    console.log(`   URL Local:  http://localhost:${PORT}`);
    console.log(`   URL Rede:   http://0.0.0.0:${PORT}`);
    console.log('\n🔐 Credenciais de autenticação:');
    console.log(`   Usuário: ${API_USER}`);
    console.log(`   Senha: ${API_PASS}`);
    console.log('\n📁 Servindo arquivos de: ./data/');
    console.log('   • HTML: index.html, config.html');
    console.log('   • CSS:  css/style.css');
    console.log('   • JS:   js/app.js');
    console.log('\n🔄 Modo DEMO ativo - Dados simulados');
    console.log('\n💡 Endpoints disponíveis:');
    console.log('   GET  /api/sensors  - Dados dos sensores (requer auth)');
    console.log('   GET  /api/status   - Status do dispositivo (requer auth)');
    console.log('   POST /api/reset    - Reset de configuração (requer auth)');
    console.log('\n✨ Para parar o servidor: Ctrl+C');
    console.log('='.repeat(60) + '\n');
});

// Tratamento de erros
process.on('SIGINT', () => {
    console.log('\n\n👋 Servidor finalizado!');
    process.exit(0);
});
