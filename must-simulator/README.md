# Simulador MUST Inverter para ESP32-C3

Este projeto simula um inversor MUST PV18/PV19 para testar configurações ESPHome sem necessidade do hardware real.

## Hardware Necessário

- ESP32-C3 DevKit (simulador)
- ESP32 DevKit (com ESPHome)
- Cabos jumper

## Conexões

```
ESP32-C3 (Simulador)    <-->    ESP32 DevKit (ESPHome)
========================        =======================
GPIO21 (TX)             <-->    GPIO18 (RX)
GPIO20 (RX)             <-->    GPIO19 (TX)
GND                     <-->    GND
```

**IMPORTANTE:** 
- Conecte TX do C3 ao RX do DevKit
- Conecte RX do C3 ao TX do DevKit
- Conecte os GNDs juntos
- NÃO conecte VCC/5V entre os dois (alimente cada um separadamente)

## Configuração

### 1. Compilar e fazer upload no ESP32-C3

```bash
cd must-simulator
pio run -t upload -t monitor
```

### 2. Verificar funcionamento

No monitor serial do ESP32-C3, você deve ver:
```
MUST Inverter Modbus Simulator
Device Address: 0x04
Baud Rate: 19200
✓ Simulator ready!
Waiting for Modbus requests...
```

### 3. Testar com ESPHome

Faça upload da configuração ESPHome no ESP32 DevKit. O simulador responderá aos comandos Modbus e você verá no Home Assistant:

- SOC: ~85%
- PV1 voltage: ~95V
- Battery voltage: ~52.5V
- Potências variadas
- Todos os sensores funcionando

## Valores Simulados

O simulador retorna valores realistas:
- **SOC**: 85% (varia lentamente)
- **Tensão PV**: 95V ± 10V
- **Tensão Bateria**: 52.5V ± 4V
- **Potência**: Valores positivos e negativos
- **Temperaturas**: 38-45°C
- **Energia acumulada**: Valores de exemplo

Os valores variam automaticamente a cada 5 segundos para simular condições reais.

## Debug

### ESP32-C3 (Simulador)
- Monitor serial mostra requisições recebidas
- Exibe valores atualizados a cada 5s

### ESP32 DevKit (ESPHome)
- Monitor logs do ESPHome
- Verifique se vê "Duplicate modbus command"
- Aguarde ~20s para primeira leitura

## Troubleshooting

### "No response received"
- Verifique as conexões TX/RX
- Confirme que GND está conectado
- Verifique se ambos ESP estão ligados
- Teste trocar TX/RX se não funcionar

### "Valores não atualizam"
- Normal demorar até 20s (update_interval)
- Verifique logs do ESPHome
- Confirme baud rate 19200 em ambos

### Modificar valores
Edite `setupModbusRegisters()` em `main.cpp` para ajustar valores iniciais.

## Customização

Para simular erros/warnings, modifique:
```cpp
holdingRegs[25261] = 0x0001;  // Bit 0 = Fan locked
holdingRegs[15213] = 0x0008;  // Bit 3 = Over temperature
```

Para mudar taxa de atualização:
```cpp
const unsigned long UPDATE_INTERVAL = 2000; // 2 segundos
```
