#!/usr/bin/env python3
"""
Development Server - Local Mock API for Frontend Testing
Simula o comportamento da API do ESP32 rodando localmente
"""

from flask import Flask, send_from_directory, jsonify
from flask_cors import CORS
import math
import time
import json
import os
from functools import wraps

app = Flask(__name__, static_folder='data')
CORS(app)  # Enable CORS for local development

# Arquivo para salvar credenciais
CREDENTIALS_FILE = os.path.join(os.path.dirname(__file__), 'credentials.json')

# Configuração de autenticação (valores padrão)
API_USER = 'admin'
API_PASS = 'admin123'

def load_credentials():
    """Carregar credenciais salvas do arquivo JSON"""
    global API_USER, API_PASS
    try:
        if os.path.exists(CREDENTIALS_FILE):
            with open(CREDENTIALS_FILE, 'r') as f:
                data = json.load(f)
                API_USER = data.get('username', 'admin')
                API_PASS = data.get('password', 'admin123')
                print('✓ Credenciais carregadas do arquivo credentials.json')
                print(f'   Usuário: {API_USER}')
                print('   Senha: ***')
    except Exception as e:
        print(f'⚠️ Erro ao carregar credenciais, usando valores padrão: {e}')

def save_credentials(username, password):
    """Salvar credenciais em arquivo JSON"""
    try:
        data = {
            'username': username,
            'password': password,
            'updated_at': time.strftime('%Y-%m-%d %H:%M:%S')
        }
        with open(CREDENTIALS_FILE, 'w') as f:
            json.dump(data, f, indent=2)
        print('✓ Credenciais salvas em credentials.json')
        return True
    except Exception as e:
        print(f'❌ Erro ao salvar credenciais: {e}')
        return False

# Carregar credenciais na inicialização
load_credentials()

def check_auth(username, password):
    """Verifica credenciais"""
    return username == API_USER and password == API_PASS

def authenticate():
    """Retorna resposta 401 com desafio de autenticação"""
    return jsonify({'error': 'Authentication required'}), 401, {
        'WWW-Authenticate': 'Basic realm="Login Required"'
    }

def requires_auth(f):
    """Decorator para exigir autenticação HTTP Basic"""
    @wraps(f)
    def decorated(*args, **kwargs):
        from flask import request
        auth = request.authorization
        if not auth or not check_auth(auth.username, auth.password):
            return authenticate()
        return f(*args, **kwargs)
    return decorated

def generate_demo_data():
    """Gera dados simulados realistas (mesma lógica do ESP32)"""
    current_time = time.time()
    variance = math.sin(current_time / 30.0) * 0.1  # Variação lenta
    
    # Solar PV - simulando sol parcial
    pv_voltage = 85.0 + variance * 10.0
    pv_current = 4.5 + variance * 2.0
    pv_power = pv_voltage * pv_current
    
    # Carregador
    charger_voltage = 54.2 + variance * 0.5
    charger_current = 8.2 + variance * 1.0
    charger_power = 445.0 + variance * 50.0
    
    # Bateria - simulando estado de carga
    battery_voltage = 52.8 + variance * 0.3
    battery_current = 8.0 + variance * 1.5
    battery_power = 422.0 + variance * 40.0
    battery_soc = 65.0 + variance * 5.0
    battery_temp = 25.0 + variance * 2.0
    
    # Inversor - simulando modo off-grid com carga
    ac_voltage = 220.0 + variance * 2.0
    ac_current = 2.5 + variance * 0.5
    ac_frequency = 50.0 + variance * 0.1
    ac_power = 550.0 + variance * 50.0
    load_percent = 27.5 + variance * 5.0
    dc_voltage = 52.8 + variance * 0.3
    
    # Acumuladores - incrementando lentamente
    time_hours = current_time / 3600.0
    charger_accumulated = 12.5 + (time_hours * 0.1) % 5.0
    total_charged = 245.8 + (time_hours * 0.2) % 10.0
    total_discharged = 198.3 + (time_hours * 0.15) % 8.0
    
    device_temp = 42.0 + variance * 3.0
    
    return {
        "charger": {
            "voltage": round(charger_voltage, 1),
            "current": round(charger_current, 2),
            "power": round(charger_power, 0),
            "accumulated_power": round(charger_accumulated, 1)
        },
        "pv": {
            "voltage": round(pv_voltage, 1),
            "current": round(pv_current, 2),
            "power": round(pv_power, 0)
        },
        "battery": {
            "voltage": round(battery_voltage, 1),
            "current": round(battery_current, 2),
            "power": round(battery_power, 0),
            "soc": round(battery_soc, 0),
            "temperature": round(battery_temp, 1)
        },
        "inverter": {
            "mode": "Off-Grid",
            "mode_id": 3,
            "ac_voltage": round(ac_voltage, 1),
            "ac_current": round(ac_current, 2),
            "ac_frequency": round(ac_frequency, 2),
            "ac_power": round(ac_power, 0),
            "load_percent": round(load_percent, 0),
            "dc_voltage": round(dc_voltage, 1),
            "max_charge_current": 60,
            "max_discharge_current": 60,
            "accumulated_power": round(total_discharged, 1)
        },
        "totals": {
            "total_charged": round(total_charged, 1),
            "total_discharged": round(total_discharged, 1),
            "device_temperature": round(device_temp, 1)
        },
        "last_update": int(current_time),
        "uptime": int(current_time - app.config['START_TIME']),
        "modbus_error": False,
        "demo_mode": True
    }

# ==================== ROTAS DA API ====================

@app.route('/api/sensors')
@requires_auth
def api_sensors():
    """Endpoint principal de sensores (requer autenticação)"""
    data = generate_demo_data()
    return jsonify(data)

@app.route('/api/status')
@requires_auth
def api_status():
    """Endpoint de status do dispositivo"""
    return jsonify({
        "device_name": "MUST Inverter API (Dev Server)",
        "ip_address": "127.0.0.1",
        "mac_address": "00:00:00:00:00:00",
        "wifi_ssid": "Development",
        "wifi_rssi": -42,
        "uptime_seconds": int(time.time() - app.config['START_TIME']),
        "free_heap": 200000,
        "modbus_connected": False
    })

@app.route('/api/reset', methods=['POST'])
@requires_auth
def api_reset():
    """Endpoint de reset (não faz nada no dev server)"""
    return jsonify({"message": "Reset não disponível no dev server"}), 200

@app.route('/api/credentials', methods=['POST'])
@requires_auth
def api_credentials():
    """Endpoint para alteração de credenciais"""
    from flask import request
    data = request.get_json()
    
    username = data.get('username')
    password = data.get('password')
    current_password = data.get('current_password')
    
    # Validar senha atual
    if current_password != API_PASS:
        return jsonify({'error': 'Current password is incorrect'}), 401
    
    # Validar se há algo para alterar
    if not username and not password:
        return jsonify({'error': 'No changes provided'}), 400
    
    # Validar tamanho da senha
    if password and len(password) < 6:
        return jsonify({'error': 'Password must be at least 6 characters'}), 400
    
    print('⚠️ Tentativa de alteração de credenciais:')
    print(f'   Novo usuário: {username or "(não alterado)"}')
    print(f'   Nova senha: {"***" if password else "(não alterada)"}')
    
    # Definir valores finais
    global API_USER, API_PASS
    final_username = username or API_USER
    final_password = password or API_PASS
    
    # Salvar em arquivo
    if save_credentials(final_username, final_password):
        # Atualizar variáveis globais
        API_USER = final_username
        API_PASS = final_password
        
        return jsonify({
            'success': True,
            'message': 'Credentials updated successfully',
            'username': final_username,
            'note': 'Credenciais salvas em credentials.json'
        }), 200
    else:
        return jsonify({
            'error': 'Failed to save credentials'
        }), 500

# ==================== SERVIR ARQUIVOS ESTÁTICOS ====================

@app.route('/')
@requires_auth
def index():
    """Serve o dashboard principal (requer autenticação)"""
    return send_from_directory('data', 'index.html')

@app.route('/index.html')
@requires_auth
def index_html():
    """Serve index.html com autenticação"""
    return send_from_directory('data', 'index.html')

@app.route('/config.html')
def config():
    """Serve a página de configuração (modo AP - sem auth)"""
    return send_from_directory('data', 'config.html')

@app.route('/settings.html')
@requires_auth
def settings():
    """Serve a página de configurações (requer autenticação)"""
    return send_from_directory('data', 'settings.html')

@app.route('/css/<path:filename>')
def serve_css(filename):
    """Serve arquivos CSS"""
    return send_from_directory('data/css', filename)

@app.route('/js/<path:filename>')
def serve_js(filename):
    """Serve arquivos JavaScript"""
    return send_from_directory('data/js', filename)

# ==================== INICIALIZAÇÃO ====================

if __name__ == '__main__':
    app.config['START_TIME'] = time.time()
    
    print("\n" + "="*60)
    print("  🚀 MUST Inverter API - Development Server")
    print("="*60)
    print("\n📌 Servidor rodando em modo de desenvolvimento")
    print("   URL: http://localhost:5000")
    print("\n🔐 Credenciais de autenticação:")
    print(f"   Usuário: {API_USER}")
    print(f"   Senha: {API_PASS}")
    print("\n📁 Servindo arquivos de: ./data/")
    print("   • HTML: index.html, config.html")
    print("   • CSS:  css/style.css")
    print("   • JS:   js/app.js")
    print("\n🔄 Modo DEMO ativo - Dados simulados")
    print("\n💡 Endpoints disponíveis:")
    print("   GET  /api/sensors  - Dados dos sensores (requer auth)")
    print("   GET  /api/status   - Status do dispositivo (requer auth)")
    print("   POST /api/reset    - Reset de configuração (requer auth)")
    print("\n✨ Para parar o servidor: Ctrl+C")
    print("="*60 + "\n")
    
    app.run(
        host='0.0.0.0',  # Permite acesso de outros dispositivos na rede
        port=5000,
        debug=True,
        use_reloader=True
    )
