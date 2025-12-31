// Configuração da API
const API_BASE = '';
const API_USER = 'admin';
const API_PASS = 'must2024';

// Função auxiliar para criar headers de autenticação
function getAuthHeaders() {
    const credentials = btoa(`${API_USER}:${API_PASS}`);
    return {
        'Authorization': `Basic ${credentials}`
    };
}

// Formatar modo do inversor
function formatInverterMode(mode) {
    const modes = {
        0: { text: 'Power On', class: 'status-standby' },
        1: { text: 'Standby', class: 'status-standby' },
        2: { text: 'Mains', class: 'status-line' },
        3: { text: 'Off-Grid', class: 'status-battery' },
        4: { text: 'Bypass', class: 'status-standby' },
        5: { text: 'Charging', class: 'status-online' },
        6: { text: 'Fault', class: 'status-fault' }
    };
    return modes[mode] || { text: 'Desconhecido', class: 'status-offline' };
}

// Atualizar dados do dashboard
async function updateData() {
    try {
        const response = await fetch(`${API_BASE}/api/sensors`, {
            headers: getAuthHeaders()
        });
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }
        
        const data = await response.json();
        
        // Bateria
        document.getElementById('battery-soc').textContent = data.battery.soc + '%';
        document.getElementById('battery-voltage').textContent = data.battery.voltage.toFixed(1) + ' V';
        document.getElementById('battery-current').textContent = data.battery.current.toFixed(1) + ' A';
        document.getElementById('battery-power').textContent = data.battery.power.toFixed(0) + ' W';
        document.getElementById('battery-temp').textContent = data.battery.temperature.toFixed(1) + ' °C';
        
        // Solar
        const solarPower = data.pv.power;
        document.getElementById('solar-power').textContent = solarPower.toFixed(0) + ' W';
        document.getElementById('solar-voltage').textContent = data.pv.voltage.toFixed(1) + ' V';
        document.getElementById('solar-current').textContent = data.pv.current.toFixed(1) + ' A';
        
        // Carregador
        document.getElementById('charger-voltage').textContent = data.charger.voltage.toFixed(1) + ' V';
        document.getElementById('charger-current').textContent = data.charger.current.toFixed(1) + ' A';
        document.getElementById('charger-power').textContent = data.charger.power.toFixed(0) + ' W';
        document.getElementById('charger-accumulated').textContent = data.charger.accumulated_power.toFixed(2) + ' kWh';
        
        // Inversor
        const modeInfo = formatInverterMode(data.inverter.mode_id);
        const modeBadge = document.getElementById('inverter-mode');
        modeBadge.textContent = modeInfo.text;
        modeBadge.className = 'status-badge ' + modeInfo.class;
        
        document.getElementById('ac-voltage').textContent = data.inverter.ac_voltage.toFixed(1) + ' V';
        document.getElementById('ac-current').textContent = data.inverter.ac_current.toFixed(1) + ' A';
        document.getElementById('ac-frequency').textContent = data.inverter.ac_frequency.toFixed(1) + ' Hz';
        document.getElementById('ac-power').textContent = data.inverter.ac_power.toFixed(0) + ' W';
        document.getElementById('load-percent').textContent = data.inverter.load_percent + '%';
        
        // Energia
        document.getElementById('total-charged').textContent = data.totals.total_charged.toFixed(2) + ' kWh';
        document.getElementById('total-discharged').textContent = data.totals.total_discharged.toFixed(2) + ' kWh';
        
        // Sistema
        document.getElementById('device-temp').textContent = data.totals.device_temperature.toFixed(1) + ' °C';
        
        // Timestamp
        const now = new Date();
        document.getElementById('timestamp').textContent = 
            `Última atualização: ${now.toLocaleTimeString('pt-BR')}`;
        
        // Remover indicador de erro se existir
        const errorDiv = document.getElementById('error-message');
        if (errorDiv) {
            errorDiv.remove();
        }
        
    } catch (error) {
        console.error('Erro ao buscar dados:', error);
        showError('Erro ao comunicar com o dispositivo. Tentando reconectar...');
    }
}

// Mostrar mensagem de erro
function showError(message) {
    let errorDiv = document.getElementById('error-message');
    if (!errorDiv) {
        errorDiv = document.createElement('div');
        errorDiv.id = 'error-message';
        errorDiv.className = 'error';
        document.querySelector('.container').insertBefore(errorDiv, document.querySelector('.grid'));
    }
    errorDiv.textContent = message;
}

// Resetar para modo AP
async function resetToAP() {
    if (!confirm('Tem certeza que deseja resetar as configurações de WiFi?\n\nO dispositivo voltará ao modo de configuração (AP).')) {
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE}/api/reset`, {
            method: 'POST',
            headers: getAuthHeaders()
        });
        
        if (response.ok) {
            alert('Reset iniciado! O dispositivo irá reiniciar em modo AP.\n\nConecte-se à rede "MUST-PV19-CONFIG" para configurar novamente.');
            setTimeout(() => {
                window.location.href = '/';
            }, 2000);
        } else {
            alert('Erro ao realizar reset. Tente novamente.');
        }
    } catch (error) {
        console.error('Erro ao resetar:', error);
        alert('Erro ao comunicar com o dispositivo.');
    }
}

// Inicializar quando a página carregar
document.addEventListener('DOMContentLoaded', () => {
    // Atualizar dados imediatamente
    updateData();
    
    // Atualizar a cada 5 segundos
    setInterval(updateData, 5000);
    
    // Event listeners dos botões
    const resetBtn = document.getElementById('reset-btn');
    if (resetBtn) {
        resetBtn.addEventListener('click', resetToAP);
    }
});
