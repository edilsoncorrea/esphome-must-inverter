# 🔧 Modo de Demonstração

## Visão Geral

O modo de demonstração permite validar a API e a interface web mesmo sem ter o inversor MUST conectado ao dispositivo. Quando o sistema detecta que não consegue comunicar com o inversor via Modbus, ele automaticamente ativa o modo demo e exibe valores simulados realistas.

## Como Funciona

### Detecção Automática
1. O sistema tenta ler dados do inversor via Modbus
2. Se 3 tentativas consecutivas falharem, o modo demo é ativado automaticamente
3. Um banner laranja aparece na interface indicando claramente que está em modo demonstração

### Indicadores Visuais
- **Banner de Aviso**: Mensagem clara no topo da página informando que o inversor não está conectado
- **Campo `demo_mode`**: Incluído na resposta JSON da API (`/api/sensors`)
- **Logs no Serial**: Mensagens indicando quando o modo demo está ativo

## Dados Simulados

Os valores simulados representam um cenário realista de operação:

### Painel Solar (PV)
- Tensão: ~85V (variação de ±10V)
- Corrente: ~4.5A (variação de ±2A)
- Potência: ~380W

### Bateria
- Tensão: ~52.8V (bateria de 48V carregando)
- Corrente: ~8A
- Potência: ~422W
- Estado de Carga (SOC): ~65%
- Temperatura: ~25°C

### Inversor
- Modo: Off-Grid (3)
- Tensão AC: ~220V
- Corrente AC: ~2.5A
- Frequência: ~50Hz
- Potência: ~550W
- Carga: ~27.5%

### Energia Acumulada
- Total Carregado: 170.7 kWh
- Total Descarregado: 185.9 kWh

## Configuração

### Habilitar/Desabilitar Demo Mode
No arquivo [`config.h`](src/config.h):

```cpp
// ============================================
// Demo Mode Configuration
// ============================================
#define DEMO_MODE_ENABLED true  // true = habilita demo mode
#define DEMO_DETECTION_FAILED_READS 3  // Número de falhas antes de ativar
```

### Parâmetros Configuráveis

- **`DEMO_MODE_ENABLED`**: 
  - `true` = Ativa modo demo quando não houver conexão
  - `false` = Apenas reporta erro, sem dados simulados

- **`DEMO_DETECTION_FAILED_READS`**: 
  - Número de leituras Modbus falhadas antes de ativar o demo
  - Padrão: 3 tentativas
  - Recomendado: 2-5 tentativas

## API Response

Quando em modo demo, a resposta da API inclui o campo `demo_mode`:

```json
{
  "charger": { ... },
  "pv": { ... },
  "battery": { ... },
  "inverter": { ... },
  "totals": { ... },
  "last_update": 12345678,
  "uptime": 123,
  "modbus_error": true,
  "demo_mode": true  // ← Indica modo demonstração
}
```

## Casos de Uso

### 1. Desenvolvimento da Interface
Desenvolva e teste a interface web sem precisar do hardware físico conectado.

### 2. Testes de Integração
Valide integrações com Home Assistant, Node-RED, etc., usando dados consistentes.

### 3. Demonstrações
Mostre o sistema funcionando em apresentações sem necessidade do equipamento real.

### 4. Troubleshooting
Diferencie problemas de comunicação (modo demo ativo) de problemas na interface.

## Desativação do Modo Demo

O modo demo é automaticamente desativado quando:
1. Uma leitura Modbus é bem-sucedida
2. O sistema consegue comunicar com o inversor
3. O contador de falhas é zerado

Não é necessário reiniciar o dispositivo.

## Logs Serial

Exemplos de mensagens no monitor serial:

```
Reading Modbus sensors...
Modbus read failed (attempt 1/3)
Reading Modbus sensors...
Modbus read failed (attempt 2/3)
Reading Modbus sensors...
Modbus read failed (attempt 3/3)
Generating demo data - Inverter not connected
```

Quando a conexão é restaurada:
```
Reading Modbus sensors...
Sensor data updated successfully
```

## Boas Práticas

1. **Sempre verifique o banner**: Certifique-se de saber quando está em modo demo
2. **Valide dados reais**: Use o modo demo apenas para validação da API, não para dados operacionais
3. **Monitore logs**: Acompanhe os logs para entender quando e por que o modo demo foi ativado
4. **Ajuste thresholds**: Se conexões intermitentes causarem ativações indevidas, aumente `DEMO_DETECTION_FAILED_READS`

## Segurança

⚠️ **Importante**: Os dados do modo demo são completamente simulados e não refletem o estado real do sistema. Nunca use dados do modo demo para:
- Decisões operacionais críticas
- Controle automático de cargas
- Sistemas de segurança
- Monitoramento de alarmes

Sempre verifique o campo `demo_mode` antes de confiar nos dados!
