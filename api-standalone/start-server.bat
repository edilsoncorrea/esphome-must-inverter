@echo off
REM Script para iniciar o servidor de desenvolvimento
REM Uso: start-server.bat

echo.
echo 🚀 Iniciando MUST Inverter API - Development Server...
echo.

cd /d "%~dp0"

REM Verifica se node_modules existe
if not exist "node_modules" (
    echo 📦 Instalando dependências...
    call npm install
    echo.
)

REM Inicia o servidor
echo ▶️  Executando servidor...
echo.
call npm run dev

pause
