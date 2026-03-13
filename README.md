# BLE Proximity 📡

Firmware para detecção de proximidade BLE com ESP32-S3. O dispositivo atua como Central BLE, escaneando o ambiente em busca de um dispositivo específico, indicando a distância via velocidade de piscada de LED baseada no RSSI recebido e conectando via GATT para leitura do nível de bateria do dispositivo alvo.

---

## Hardware

| Item | Detalhe |
|------|---------|
| Placa | LilyGo T-SIMCAM (ESP32-S3) |
| LED | GPIO 21 |
| ESP-IDF | v5.1.1 |
| BLE Stack | NimBLE |


---

## O que é o projeto

Firmware que:
1. Inicializa o stack NimBLE como Central BLE
2. Escaneia continuamente o ambiente em busca de um dispositivo pelo nome
3. Lê o RSSI de cada advertising packet recebido
4. Indica a proximidade via velocidade de piscada do LED
5. Conecta via GATT no dispositivo encontrado e lê o nível de bateria via Battery Service

---

## Arquitetura em camadas

```
┌─────────────────────────────────────────┐
│           APP LAYER (app_main)          │  inicializa e orquestra
├─────────────────────────────────────────┤
│            BLE LAYER                    │
│   (ble_scan + ble_gatt)                 │  descoberta e conexão BLE
├─────────────────────────────────────────┤
│           DRIVER LAYER                  │
│         (driver_led)                    │  controle do LED de status
├─────────────────────────────────────────┤
│         HAL / ESP-IDF / FreeRTOS        │  usado, não escrito
└─────────────────────────────────────────┘
```

| Camada | Responsabilidade |
|--------|-----------------|
| `app_main` | Inicializa todos os componentes e define o dispositivo alvo |
| `ble_scan` | Inicializa NimBLE, realiza scan ativo, filtra por nome e classifica RSSI |
| `ble_gatt` | Conecta no dispositivo encontrado e lê characteristics via GATT |
| `driver_led` | Controla o LED de status. Não sabe de BLE |

---

## Estrutura de diretórios

```
BLE_Proximity/
├── components/
│   ├── ble_scan/          ← scan ativo, filtro por nome, RSSI
│   ├── ble_gatt/          ← conexão GATT, leitura de characteristics
│   └── driver_led/        ← controle do LED de status
├── main/
│   └── main.c
└── CMakeLists.txt
```

---

## Lógica de Proximidade

O RSSI (Received Signal Strength Indicator) é medido em dBm. Valores menos negativos indicam sinal mais forte e maior proximidade.

| Faixa | RSSI | Comportamento do LED |
|-------|------|----------------------|
| Perto | > -60 dBm | Pisca rápido (100ms) |
| Médio | -60 a -80 dBm | Pisca médio (300ms) |
| Longe | < -80 dBm | Pisca lento (600ms) |

---

## Entregas

- [x] **Entrega 1 — BLE Scan + RSSI + LED**
  - ESP32 escaneia dispositivo por nome, classifica RSSI em 3 faixas e indica proximidade via velocidade de piscada do LED

- [ ] **Entrega 2 — Conexão GATT + Battery Service**
  - ESP32 conecta no dispositivo encontrado e lê nível de bateria via GATT Battery Service

---

## Como rodar

### Firmware

```bash
idf.py build flash monitor
```

Antes de buildar, configure o nome do dispositivo alvo no `main/main.c`:

```c
ble_scan_start("NOME_DO_DISPOSITIVO");
```

### Dispositivo de teste

Para testes sem um Peripheral BLE dedicado, use o app **nRF Connect** (Android/iOS):
1. Aba **Advertiser** → criar novo advertiser
2. Adicionar record **Complete Local Name** com o nome configurado no firmware
3. Iniciar o advertising

---

## Conceitos Aplicados

- **GAP (Generic Access Profile)** — camada BLE responsável por descoberta e advertising
- **Scan ativo vs passivo** — scan ativo envia `SCAN_REQ` para obter o nome completo do dispositivo via scan response
- **RSSI** — métrica de potência do sinal recebido usada para estimar proximidade
- **NimBLE** — stack BLE leve do ESP-IDF baseado em eventos e callbacks
- **NVS** — Non-Volatile Storage, necessário para inicialização do stack BLE

---

## Tecnologias

- ESP-IDF 5.1.1 (ESP32-S3)
- FreeRTOS
- NimBLE BLE Stack