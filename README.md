# Robô Espacial IoT com ESP32-S3

## Objetivo do Projeto

Desenvolver um sistema IoT utilizando ESP32-S3 para simular um robô espacial capaz de:

- Monitorar o ambiente utilizando sensores
- Detectar presença
- Medir luminosidade
- Estimar probabilidade de vida
- Enviar alertas via WhatsApp
- Armazenar dados em banco de dados
- Simular controle remoto no Wokwi

---

# Tecnologias Utilizadas

- ESP32-S3
- Arduino IDE
- Wokwi
- Flask (Python)
- SQLite
- Supabase
- CallMeBot
- GitHub

---

# Estrutura do Projeto

```txt
iot-esp32-robo-espacial/
│
├── esp32/
│   ├── robo_espacial.ino
│   └── controle_remoto_wokwi.ino
│
├── backend/
│   ├── app.py
│   └── requirements.txt
│
├── database/
│   └── create_table.sql
│
└── README.md
```

---

# Componentes Utilizados

## Robô físico

- ESP32-S3
- DHT22
- Sensor PIR
- LDR
- Servo Motor
- LED Verde
- LED Vermelho

## Controle remoto (Wokwi)

- ESP32
- Joystick analógico
- LEDs
- Botão

---

# Funcionalidades

## Controle Remoto (Wokwi)

- Controle de direção com joystick
- Comandos:
  - Frente
  - Trás
  - Esquerda
  - Direita
- Botão remoto de desligamento
- LEDs de status
- Monitor serial

Arquivo:

```txt
esp32/controle_remoto_wokwi.ino
```

---

## Robô Físico

- Leitura de temperatura
- Leitura de umidade
- Detecção de presença
- Medição de luminosidade
- Controle de servo motor
- LEDs de status
- Probabilidade de vida
- Envio de alertas via WhatsApp
- Envio de dados para banco de dados
- Buffer local para falhas de conexão

Arquivo:

```txt
esp32/robo_espacial.ino
```

---

# Lógica da Probabilidade de Vida

A probabilidade é calculada utilizando:

| Condição | Pontuação |
|---|---|
| Temperatura entre 15°C e 30°C | +25% |
| Umidade entre 40% e 70% | +25% |
| Luminosidade adequada | +20% |
| Presença detectada | +30% |

## Resultado

- ≤ 75%:
  - LED verde ligado
  - Exploração normal

- > 75%:
  - LED vermelho ligado
  - Servo parado
  - Alerta via WhatsApp

---

# Banco de Dados

## Estrutura SQL

Arquivo:

```txt
database/create_table.sql
```

Tabela:

```sql
create table leituras (
  id bigint generated always as identity primary key,
  timestamp timestamptz not null,
  temperatura_c float,
  umidade_pct float,
  luminosidade int,
  presenca int,
  probabilidade_vida float
);
```

---

# Backend Python

Arquivo:

```txt
backend/app.py
```

## Instalação

```bash
pip install -r requirements.txt
```

## Execução

```bash
python app.py
```

---

# API

## POST /leituras

Salva dados dos sensores.

Exemplo:

```json
{
  "timestamp": "2025-09-02T14:35:00Z",
  "temperatura_c": 24.3,
  "umidade_pct": 55,
  "luminosidade": 723,
  "presenca": 1,
  "probabilidade_vida": 78.0
}
```

---

## GET /leituras

Retorna as últimas 100 leituras registradas.

---

# Como Executar o Projeto

## ESP32

1. Abrir Arduino IDE
2. Instalar bibliotecas:
   - ESP32Servo
   - DHT sensor library
   - Adafruit Unified Sensor
3. Configurar WiFi
4. Enviar código para ESP32-S3

---

## Backend

```bash
cd backend
pip install -r requirements.txt
python app.py
```

---

# Simulação Wokwi

O projeto possui uma versão simulada utilizando Wokwi.

Arquivo:

```txt
esp32/controle_remoto_wokwi.ino
```

---

# Autor

Davi Dorea
