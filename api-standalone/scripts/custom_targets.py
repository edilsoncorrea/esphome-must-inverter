"""
PlatformIO Custom Targets
Adiciona comandos customizados para upload automatizado
"""

Import("env")
import sys

def upload_all_callback(*args, **kwargs):
    """Upload completo: filesystem primeiro, depois firmware"""
    print("\n" + "="*60)
    print("  UPLOAD COMPLETO - Filesystem + Firmware")
    print("="*60 + "\n")
    
    # Etapa 1: Upload do Filesystem
    print("📁 Etapa 1: Uploading filesystem (HTML/CSS/JS)...")
    print("-" * 60)
    ret = env.Execute("$UPLOADFSCMD")
    if ret != 0:
        print("\n❌ ERRO: Falha no upload do filesystem!")
        sys.exit(ret)
    print("✅ Filesystem enviado com sucesso!\n")
    
    # Etapa 2: Upload do Firmware
    print("⚡ Etapa 2: Uploading firmware (C++ code)...")
    print("-" * 60)
    ret = env.Execute("$UPLOADCMD")
    if ret != 0:
        print("\n❌ ERRO: Falha no upload do firmware!")
        sys.exit(ret)
    print("✅ Firmware enviado com sucesso!\n")
    
    # Mensagem final
    print("="*60)
    print("✅ Upload completo realizado com sucesso!")
    print("="*60)
    print("\n📌 Próximos passos:")
    print("  1. Monitor serial: pio device monitor")
    print("  2. Conecte ao WiFi: must-inverter-api")
    print("  3. Acesse: http://192.168.4.1")
    print("")

# Registrar custom target
env.AddCustomTarget(
    name="uploadall",
    dependencies=None,
    actions=upload_all_callback,
    title="Upload All",
    description="Upload filesystem and firmware sequentially"
)

print("✨ Custom targets disponíveis:")
print("  • pio run -t uploadall  → Upload completo (filesystem + firmware)")
