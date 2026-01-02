// ==================== INTERNATIONALIZATION (i18n) ====================

const translations = {
    en: {
        // Header
        title: "MUST PV19 Monitor",
        subtitle: "Real-Time Monitoring",
        demoMode: "🔧 DEMO MODE - Inverter not connected. Displaying simulated data for API validation.",
        
        // Language selector
        language: "Language",
        
        // Battery Card
        battery: "Battery",
        voltage: "Voltage",
        current: "Current",
        power: "Power",
        temperature: "Temperature",
        soc: "Charge",
        
        // Solar Card
        solarPanel: "Solar Panel",
        solar: "Solar PV",
        
        // Charger Card
        charger: "Charger",
        sourceVoltage: "Source Voltage",
        accumulated: "Accumulated",
        
        // Inverter Card
        inverter: "AC Inverter",
        mode: "Mode",
        acVoltage: "AC Voltage",
        acCurrent: "AC Current",
        frequency: "Frequency",
        acPower: "AC Power",
        load: "Load",
        
        // Energy Card
        totalEnergy: "Total Energy",
        energy: "Energy",
        totalCharged: "Total Charged",
        totalDischarged: "Total Discharged",
        
        // System Card
        system: "System",
        deviceTemp: "Device Temp",
        settings: "⚙️ Settings",
        statusJson: "Status JSON",
        resetWifi: "Reset WiFi",
        
        // Loading
        loading: "Loading...",
        
        // Inverter Modes
        modes: {
            powerOn: "Power On",
            standby: "Standby",
            mains: "Mains",
            offGrid: "Off-Grid",
            bypass: "Bypass",
            charging: "Charging",
            fault: "Fault",
            unknown: "Unknown"
        },
        
        // Status
        status: "Status",
        online: "Online",
        offline: "Offline",
        
        // Buttons
        settings: "Settings",
        reset: "Reset WiFi",
        
        // Messages
        error: "Error communicating with device. Trying to reconnect...",
        resetConfirm: "Are you sure you want to reset WiFi settings?\n\nThe device will return to configuration mode (AP).",
        resetSuccess: "Reset initiated! The device will restart in AP mode.\n\nConnect to the \"MUST-PV19-CONFIG\" network to configure again.",
        resetError: "Error performing reset. Try again.",
        
        // Footer
        lastUpdate: "Last update",
        ago: "ago",
        seconds: "seconds",
        
        // Settings Page
        settingsTitle: "Settings - MUST PV19 Monitor",
        settingsHeader: "Settings",
        settingsIcon: "⚙️",
        settingsSubtitle: "Change API access credentials",
        apiCredentials: "🔐 API Credentials",
        wifiConfig: "📶 WiFi Configuration",
        currentPassword: "Current Password",
        currentPasswordHelp: "Enter your current password to confirm the change",
        newUsername: "New Username (optional)",
        newUsernamePlaceholder: "Leave blank to keep: admin",
        newUsernameHelp: "Leave blank to keep current username",
        newPassword: "New Password",
        newPasswordHelp: "Minimum 6 characters. Leave blank to keep current password.",
        confirmPassword: "Confirm New Password",
        wifiSSID: "WiFi Network (SSID)",
        wifiSSIDHelp: "Leave blank to keep current network",
        wifiPassword: "WiFi Password",
        wifiPasswordHelp: "Minimum 8 characters. Leave blank to keep current password.",
        wifiScanButton: "🔍 Search WiFi Networks",
        wifiScanning: "⏳ Searching...",
        wifiNoNetworks: "No networks found",
        wifiScanError: "❌ Error searching for WiFi networks",
        wifiSignalExcellent: "📶 Excellent",
        wifiSignalVeryGood: "📶 Very Good",
        wifiSignalGood: "📶 Good",
        wifiSignalWeak: "📶 Weak",
        wifiSignalVeryWeak: "📶 Very Weak",
        saveChanges: "💾 Save Changes",
        backToDashboard: "← Back to Dashboard",
        importantTitle: "⚠️ Important:",
        importantNote1: "Keep your new credentials in a safe place",
        importantNote2: "You will need to log in again after saving",
        importantNote3: "On dev-server, changes are not persisted",
        saving: "⏳ Saving...",
        successTitle: "✅ Success!",
        errorTitle: "❌ Error",
        infoTitle: "ℹ️ Info",
        warningTitle: "⚠️ Warning",
        noChanges: "No changes were made.",
        passwordTooShort: "New password must be at least 6 characters!",
        passwordMismatch: "Passwords do not match!",
        currentPasswordRequired: "Current password is required!",
        credentialsUpdated: "Credentials updated successfully!",
        sessionExpired: "You will need to log in again with the new credentials.",
        closeWindow: "Close this window and log in with new credentials.",
        specialCharWarning: "Warning: The '&' character works via API but may not work in HTTP Basic Auth login box of some older browsers."
    },
    
    pt: {
        // Cabeçalho
        title: "Monitor MUST PV19",
        subtitle: "Monitoramento em Tempo Real",
        demoMode: "🔧 MODO DEMONSTRAÇÃO - Inversor não conectado. Exibindo dados simulados para validação da API.",
        
        // Seletor de idioma
        language: "Idioma",
        
        // Card Bateria
        battery: "Bateria",
        voltage: "Tensão",
        current: "Corrente",
        power: "Potência",
        temperature: "Temperatura",
        soc: "Carga",
        
        // Card Solar
        solarPanel: "Painel Solar",
        solar: "Solar PV",
        
        // Card Carregador
        charger: "Carregador",
        sourceVoltage: "Tensão Fonte",
        accumulated: "Acumulado",
        
        // Card Inversor
        inverter: "Inversor AC",
        mode: "Modo",
        acVoltage: "Tensão AC",
        acCurrent: "Corrente AC",
        frequency: "Frequência",
        acPower: "Potência AC",
        load: "Carga",
        
        // Card Energia
        totalEnergy: "Energia Total",
        energy: "Energia",
        totalCharged: "Total Carregado",
        totalDischarged: "Total Descarregado",
        
        // Card Sistema
        system: "Sistema",
        deviceTemp: "Temp. Dispositivo",
        settings: "⚙️ Configurações",
        statusJson: "Status JSON",
        resetWifi: "Reset WiFi",
        
        // Carregamento
        loading: "Carregando...",
        
        // Modos do Inversor
        modes: {
            powerOn: "Ligando",
            standby: "Em Espera",
            mains: "Rede",
            offGrid: "Off-Grid",
            bypass: "Bypass",
            charging: "Carregando",
            fault: "Falha",
            unknown: "Desconhecido"
        },
        
        // Status
        status: "Status",
        online: "Online",
        offline: "Offline",
        
        // Botões
        settings: "Configurações",
        reset: "Resetar WiFi",
        
        // Mensagens
        error: "Erro ao comunicar com o dispositivo. Tentando reconectar...",
        resetConfirm: "Tem certeza que deseja resetar as configurações de WiFi?\n\nO dispositivo voltará ao modo de configuração (AP).",
        resetSuccess: "Reset iniciado! O dispositivo irá reiniciar em modo AP.\n\nConecte-se à rede \"MUST-PV19-CONFIG\" para configurar novamente.",
        resetError: "Erro ao realizar reset. Tente novamente.",
        
        // Rodapé
        lastUpdate: "Última atualização",
        ago: "atrás",
        seconds: "segundos",
        
        // Página de Configurações
        settingsTitle: "Configurações - MUST PV19 Monitor",
        settingsHeader: "Configurações",
        settingsIcon: "⚙️",
        settingsSubtitle: "Altere as credenciais de acesso à API",
        apiCredentials: "🔐 Credenciais da API",
        wifiConfig: "📶 Configuração WiFi",
        currentPassword: "Senha Atual",
        currentPasswordHelp: "Digite sua senha atual para confirmar a alteração",
        newUsername: "Novo Usuário (opcional)",
        newUsernamePlaceholder: "Deixe em branco para manter: admin",
        newUsernameHelp: "Deixe em branco para manter o usuário atual",
        newPassword: "Nova Senha",
        newPasswordHelp: "Mínimo 6 caracteres. Deixe em branco para manter a senha atual.",
        confirmPassword: "Confirmar Nova Senha",
        wifiSSID: "Rede WiFi (SSID)",
        wifiSSIDHelp: "Deixe em branco para manter a rede atual",
        wifiPassword: "Senha do WiFi",
        wifiPasswordHelp: "Mínimo 8 caracteres. Deixe em branco para manter a senha atual.",
        wifiScanButton: "🔍 Procurar Redes WiFi",
        wifiScanning: "⏳ Procurando...",
        wifiNoNetworks: "Nenhuma rede encontrada",
        wifiScanError: "❌ Erro ao buscar redes WiFi",
        wifiSignalExcellent: "📶 Excelente",
        wifiSignalVeryGood: "📶 Muito Bom",
        wifiSignalGood: "📶 Bom",
        wifiSignalWeak: "📶 Fraco",
        wifiSignalVeryWeak: "📶 Muito Fraco",
        saveChanges: "💾 Salvar Alterações",
        backToDashboard: "← Voltar ao Dashboard",
        importantTitle: "⚠️ Importante:",
        importantNote1: "Guarde suas novas credenciais em local seguro",
        importantNote2: "Você precisará fazer login novamente após salvar",
        importantNote3: "No dev-server, as alterações não são persistidas",
        saving: "⏳ Salvando...",
        successTitle: "✅ Sucesso!",
        errorTitle: "❌ Erro",
        infoTitle: "ℹ️ Info",
        warningTitle: "⚠️ Aviso",
        noChanges: "Nenhuma alteração foi feita.",
        passwordTooShort: "A nova senha deve ter no mínimo 6 caracteres!",
        passwordMismatch: "As senhas não coincidem!",
        currentPasswordRequired: "A senha atual é obrigatória!",
        credentialsUpdated: "Credenciais atualizadas com sucesso!",
        sessionExpired: "Você precisará fazer login novamente com as novas credenciais.",
        closeWindow: "Feche esta janela e faça login com as novas credenciais.",
        specialCharWarning: "Atenção: O caractere '&' funciona via API mas pode não funcionar na caixa de login HTTP Basic Auth de alguns navegadores antigos."
    }
};

// Idioma padrão
let currentLanguage = 'en';

// Carregar idioma salvo
function loadLanguage() {
    const saved = localStorage.getItem('language');
    if (saved && translations[saved]) {
        currentLanguage = saved;
    }
    return currentLanguage;
}

// Salvar idioma
function saveLanguage(lang) {
    localStorage.setItem('language', lang);
    currentLanguage = lang;
}

// Obter tradução
function t(key) {
    const keys = key.split('.');
    let value = translations[currentLanguage];
    
    for (const k of keys) {
        if (value && typeof value === 'object') {
            value = value[k];
        } else {
            return key; // Retorna a chave se não encontrar tradução
        }
    }
    
    return value || key;
}

// Aplicar traduções na página
function applyTranslations() {
    // Atualizar todos os elementos com data-i18n
    document.querySelectorAll('[data-i18n]').forEach(element => {
        const key = element.getAttribute('data-i18n');
        const translation = t(key);
        
        if (element.tagName === 'INPUT' || element.tagName === 'TEXTAREA') {
            if (element.hasAttribute('placeholder')) {
                element.placeholder = translation;
            } else {
                element.value = translation;
            }
        } else {
            element.textContent = translation;
        }
    });
    
    // Atualizar placeholders específicos
    document.querySelectorAll('[data-i18n-placeholder]').forEach(element => {
        const key = element.getAttribute('data-i18n-placeholder');
        element.placeholder = t(key);
    });
    
    // Atualizar títulos
    document.querySelectorAll('[data-i18n-title]').forEach(element => {
        const key = element.getAttribute('data-i18n-title');
        element.title = t(key);
    });
}

// Trocar idioma
function changeLanguage(lang) {
    if (!translations[lang]) {
        console.error('Language not supported:', lang);
        return;
    }
    
    saveLanguage(lang);
    applyTranslations();
    
    // Atualizar seletor de idioma
    const selector = document.getElementById('language-selector');
    if (selector) {
        selector.value = lang;
    }
    
    // Disparar evento customizado
    window.dispatchEvent(new CustomEvent('languageChanged', { detail: { language: lang } }));
}

// Inicializar quando o DOM carregar
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', () => {
        loadLanguage();
        applyTranslations();
    });
} else {
    loadLanguage();
    applyTranslations();
}
