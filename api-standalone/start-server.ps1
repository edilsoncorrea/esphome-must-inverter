#!/usr/bin/env pwsh
# Script para iniciar o servidor de desenvolvimento
# Uso: .\start-server.ps1

Write-Host ""
Write-Host "🚀 Iniciando MUST Inverter API - Development Server..." -ForegroundColor Cyan
Write-Host ""

# Navega para a pasta do script
Set-Location $PSScriptRoot

# Verifica se node_modules existe
if (-not (Test-Path "node_modules")) {
    Write-Host "📦 Instalando dependências..." -ForegroundColor Yellow
    npm install
    Write-Host ""
}

# Inicia o servidor
Write-Host "▶️  Executando servidor..." -ForegroundColor Green
Write-Host ""
npm run dev
