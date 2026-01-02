#!/usr/bin/env pwsh
# Auto-restart dev-server script
# Reinicia automaticamente o servidor Node.js quando ele para

param(
    [string]$Port = "5000"
)

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ServerScript = Join-Path $ScriptDir "dev-server.js"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  🔄 Auto-Restart Dev Server" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "📁 Directory: $ScriptDir" -ForegroundColor Yellow
Write-Host "📄 Script: $ServerScript" -ForegroundColor Yellow
Write-Host "🔌 Port: $Port" -ForegroundColor Yellow
Write-Host ""
Write-Host "ℹ️  O servidor será reiniciado automaticamente quando:" -ForegroundColor Green
Write-Host "   • Você alterar credenciais via /api/credentials" -ForegroundColor Green
Write-Host "   • Ocorrer um erro e o processo parar" -ForegroundColor Green
Write-Host ""
Write-Host "⚠️  Para parar completamente: Pressione Ctrl+C" -ForegroundColor Yellow
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$restartCount = 0

while ($true) {
    if ($restartCount -gt 0) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Yellow
        Write-Host "  🔄 RESTART #$restartCount" -ForegroundColor Yellow
        Write-Host "========================================" -ForegroundColor Yellow
        Write-Host ""
        Start-Sleep -Seconds 1
    }
    
    $restartCount++
    
    # Executar o servidor
    & node $ServerScript
    
    # Se chegou aqui, o processo terminou
    Write-Host ""
    Write-Host "⚠️  Servidor parou. Reiniciando em 3 segundos..." -ForegroundColor Yellow
    Start-Sleep -Seconds 3
}

