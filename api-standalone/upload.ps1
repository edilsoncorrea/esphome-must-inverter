#!/usr/bin/env pwsh
# Script de upload automatizado para ESP32
# Faz upload do filesystem (LittleFS) e do firmware

param(
    [string]$Port = "",
    [switch]$OnlyFilesystem,
    [switch]$OnlyFirmware,
    [switch]$Help
)

function Show-Help {
    Write-Host "Upload Automatizado - ESP32 MUST Inverter API" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Uso:" -ForegroundColor Yellow
    Write-Host "  .\upload.ps1                    # Upload completo (filesystem + firmware)"
    Write-Host "  .\upload.ps1 -Port COM3         # Especificar porta"
    Write-Host "  .\upload.ps1 -OnlyFilesystem    # Apenas arquivos HTML/CSS/JS"
    Write-Host "  .\upload.ps1 -OnlyFirmware      # Apenas código C++"
    Write-Host ""
    Write-Host "Exemplos:" -ForegroundColor Yellow
    Write-Host "  .\upload.ps1                    # Auto-detecta porta, faz upload completo"
    Write-Host "  .\upload.ps1 -Port COM5         # Upload completo na porta COM5"
    Write-Host "  .\upload.ps1 -OnlyFilesystem    # Atualiza apenas interface web"
    Write-Host ""
}

if ($Help) {
    Show-Help
    exit 0
}

Write-Host "==================================" -ForegroundColor Cyan
Write-Host "   ESP32 Upload Automático" -ForegroundColor Cyan
Write-Host "==================================" -ForegroundColor Cyan
Write-Host ""

$portArg = ""
if ($Port -ne "") {
    $portArg = "--upload-port $Port"
    Write-Host "Porta especificada: $Port" -ForegroundColor Yellow
} else {
    Write-Host "Porta: Auto-detect" -ForegroundColor Yellow
}
Write-Host ""

# Verificar se estamos no diretório correto
if (-not (Test-Path "platformio.ini")) {
    Write-Host "❌ Erro: platformio.ini não encontrado!" -ForegroundColor Red
    Write-Host "Execute este script do diretório api-standalone/" -ForegroundColor Red
    exit 1
}

$uploadSuccess = $true

# Upload do Filesystem
if (-not $OnlyFirmware) {
    Write-Host "📁 Etapa 1: Upload do Filesystem (HTML/CSS/JS)..." -ForegroundColor Green
    Write-Host "----------------------------------------" -ForegroundColor Gray
    
    $cmd = "pio run --target uploadfs $portArg"
    Write-Host "Executando: $cmd" -ForegroundColor Gray
    
    Invoke-Expression $cmd
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ Falha no upload do filesystem!" -ForegroundColor Red
        $uploadSuccess = $false
    } else {
        Write-Host "✅ Filesystem enviado com sucesso!" -ForegroundColor Green
    }
    Write-Host ""
}

# Upload do Firmware
if (-not $OnlyFilesystem -and $uploadSuccess) {
    Write-Host "⚡ Etapa 2: Upload do Firmware (código C++)..." -ForegroundColor Green
    Write-Host "----------------------------------------" -ForegroundColor Gray
    
    $cmd = "pio run --target upload $portArg"
    Write-Host "Executando: $cmd" -ForegroundColor Gray
    
    Invoke-Expression $cmd
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ Falha no upload do firmware!" -ForegroundColor Red
        $uploadSuccess = $false
    } else {
        Write-Host "✅ Firmware enviado com sucesso!" -ForegroundColor Green
    }
    Write-Host ""
}

# Resumo final
Write-Host "==================================" -ForegroundColor Cyan
if ($uploadSuccess) {
    Write-Host "✅ Upload concluído com sucesso!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Próximos passos:" -ForegroundColor Yellow
    Write-Host "  1. Abra o monitor serial: pio device monitor" -ForegroundColor White
    Write-Host "  2. Aguarde o ESP32 inicializar" -ForegroundColor White
    Write-Host "  3. Conecte ao WiFi 'must-inverter-api'" -ForegroundColor White
    Write-Host "  4. Acesse http://192.168.4.1" -ForegroundColor White
} else {
    Write-Host "❌ Upload falhou!" -ForegroundColor Red
    Write-Host "Verifique a conexão USB e tente novamente." -ForegroundColor Yellow
}
Write-Host "==================================" -ForegroundColor Cyan
