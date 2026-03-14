# Tradução de Sensores — Inglês → Português BR

Arquivo: `esp32-pv19.yaml`

---

## binary_sensor — CE: Erros do Carregador (address 15213)

| Inglês | Português BR |
|--------|-------------|
| CE: Charger hardware protection | CE: Proteção hardware carregador |
| CE: Over current | CE: Sobrecorrente |
| CE: Current sensor error | CE: Erro sensor corrente |
| CE: Over temperature | CE: Superaquecimento |
| CE: PV voltage is too high | CE: Tensão PV muito alta |
| CE: PV voltage is too low | CE: Tensão PV muito baixa |
| CE: Battery voltage is too high | CE: Tensão bateria muito alta |
| CE: Battery voltage is too low | CE: Tensão bateria muito baixa |
| CE: Current is uncontrollable | CE: Corrente incontrolável |
| CE: Parameter error | CE: Erro de parâmetro |

---

## binary_sensor — CW: Avisos do Carregador (address 15214)

| Inglês | Português BR |
|--------|-------------|
| CW: Fan error | CW: Erro no ventilador |

---

## binary_sensor — IE: Erros do Inversor 1 (address 25261)

| Inglês | Português BR |
|--------|-------------|
| IE: Fan is locked when inverter is off | IE: Ventilador travado inversor desligado |
| IE: Inverter transformer over temperature | IE: Transformador inversor superaquecido |
| IE: Battery voltage is too high | IE: Tensão bateria muito alta |
| IE: Battery voltage is too low | IE: Tensão bateria muito baixa |
| IE: Output short circuited | IE: Curto-circuito na saída |
| IE: Inverter output voltage is high | IE: Tensão saída inversor alta |
| IE: Overload time out | IE: Timeout de sobrecarga |
| IE: Inverter bus voltage is too high | IE: Tensão barramento muito alta |
| IE: Bus soft start failed | IE: Falha soft start barramento |
| IE: Main relay failed | IE: Falha no relé principal |
| IE: Inverter output voltage sensor error | IE: Erro sensor tensão saída inversor |
| IE: Inverter grid voltage sensor error | IE: Erro sensor tensão rede inversor |
| IE: Inverter output current sensor error | IE: Erro sensor corrente saída inversor |
| IE: Inverter grid current sensor error | IE: Erro sensor corrente rede inversor |
| IE: Inverter load current sensor error | IE: Erro sensor corrente carga inversor |
| IE: Inverter grid over current error | IE: Erro sobrecorrente rede inversor |

---

## binary_sensor — IE: Erros do Inversor 2 (address 25262)

| Inglês | Português BR |
|--------|-------------|
| IE: Inverter radiator over temperature | IE: Radiador inversor superaquecido |
| IE: Solar charger battery voltage class error | IE: Erro classe tensão bateria solar |
| IE: Solar charger current sensor error | IE: Erro sensor corrente solar |
| IE: Solar charger current is uncontrollable | IE: Corrente solar incontrolável |
| IE: Inverter grid voltage is low | IE: Tensão rede inversor baixa |
| IE: Inverter grid voltage is high | IE: Tensão rede inversor alta |
| IE: Inverter grid under frequency | IE: Subfrequência rede inversor |
| IE: Inverter grid over frequency | IE: Sobrefrequência rede inversor |
| IE: Inverter over current protection error | IE: Erro proteção sobrecorrente inversor |
| IE: Inverter bus voltage is too low | IE: Tensão barramento muito baixa |
| IE: Inverter soft start failed | IE: Falha soft start inversor |
| IE: Over DC voltage in AC output | IE: Sobretensão DC na saída AC |
| IE: Battery connection is open | IE: Conexão bateria aberta |
| IE: Inverter control current sensor error | IE: Erro sensor corrente controle |
| IE: Inverter output voltage is too low | IE: Tensão saída inversor muito baixa |

---

## binary_sensor — IW: Avisos do Inversor (address 25265)

| Inglês | Português BR |
|--------|-------------|
| IW: Fan is locked when inverter is on | IW: Ventilador travado inversor ligado |
| IW: Fan2 is locked when inverter is on | IW: Ventilador2 travado inversor ligado |
| IW: Battery is over-charged | IW: Bateria sobrecarregada |
| IW: Low battery | IW: Bateria baixa |
| IW: Overload | IW: Sobrecarga |
| IW: Output power derating | IW: Redução potência saída |
| IW: Solar charger stops due to low battery | IW: Solar parado por bateria baixa |
| IW: Solar charger stops due to high PV voltage | IW: Solar parado tensão PV alta |
| IW: Solar charger stops due to over load | IW: Solar parado por sobrecarga |
| IW: Solar charger over temperature | IW: Carregador solar superaquecido |
| IW: PV charger communication error | IW: Erro comunicação carregador PV |

---

## sensor

| Inglês | Português BR |
|--------|-------------|
| State of Charge | Nível de carga |
| Charger workstate | Estado do carregador |
| MPPT state | Estado MPPT |
| Charging state | Estado de carregamento |
| PV1 voltage | Tensão PV1 |
| PV2 voltage | Tensão PV2 |
| Battery voltage (charger side) | Tensão bateria (lado carregador) |
| PV1 Charger Current | Corrente carregador PV1 |
| PV2 Charger Current | Corrente carregador PV2 |
| PV1 Charger power | Potência carregador PV1 |
| PV2 Charger power | Potência carregador PV2 |
| Battery Relay | Relé da bateria |
| PV Relay | Relé PV |
| Accumulated charger power | Energia acumulada carregador |
| Inverter Work state | Estado de trabalho inversor |
| Battery voltage | Tensão da bateria |
| Inverter voltage | Tensão do inversor |
| Grid voltage | Tensão da rede |
| Grid frequency | Frequência da rede |
| Inverter power | Potência do inversor |
| Grid power | Potência da rede |
| Load power | Potência da carga |
| System load | Carga do sistema |
| AC radiator temp | Temp. radiador AC |
| Transformer temp | Temp. transformador |
| DC Radiator temp | Temp. radiador DC |
| Accumulated discharger power | Energia acumulada descarga |
| Accumulated INV charger power | Energia acumulada INV carregador |
| Accumulated buy power | Energia comprada acumulada |
| Accumulated sell power | Energia vendida acumulada |
| Accumulated load power | Energia da carga acumulada |
| Accumulated grid charge power | Energia carga rede acumulada |
| Battery power | Potência da bateria |
| Battery current | Corrente da bateria |

---

## select

| Inglês | Português BR |
|--------|-------------|
| Energy use mode | Modo de uso de energia |
| Charger source priority | Prioridade fonte carregador |
| Solar Use Aim | Objetivo uso solar |

---

## number

| Inglês | Português BR |
|--------|-------------|
| INV Enable grid charge | INV Habilitar carga rede |
| INV Disable grid charge | INV Desabilitar carga rede |
| Float voltage | Tensão de flutuação |
| Absorb voltage | Tensão de absorção |
| Battery stop discharging voltage | Tensão parar descarga bateria |
| Battery stop charging voltage | Tensão parar carga bateria |
| Battery low voltage | Tensão mínima da bateria |
| Battery high voltage | Tensão máxima da bateria |
| Max Grid Charger current | Corrente máx. carregador rede |
| Max Combine Charger current | Corrente máx. carregador combinado |
