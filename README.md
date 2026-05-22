# Robô Espacial IoT com ESP32

## Objetivo

Sistema IoT utilizando ESP32-S3 para monitoramento ambiental e detecção de probabilidade de vida através de sensores.

---

## Componentes

- ESP32-S3
- DHT22
- Sensor PIR
- LDR
- Servo Motor
- LED Verde
- LED Vermelho

---

## Funcionalidades

- Leitura de temperatura e umidade
- Detecção de presença
- Medição de luminosidade
- Cálculo da probabilidade de vida
- Alertas via WhatsApp (CallMeBot)
- Envio de dados para banco de dados
- Buffer local para falha de conexão

---

## Estrutura do Projeto

```txt
esp32/
backend/
database/
```

---

## Como rodar ESP32

1. Abrir Arduino IDE
2. Instalar bibliotecas:
   - ESP32Servo
   - DHT sensor library
3. Configurar WiFi
4. Enviar código para ESP32

---

## Como rodar Backend Python

```bash
pip install -r requirements.txt
python app.py
```

---

## Banco de Dados

Executar:

```sql
database/create_table.sql
```

---

## API

### POST /leituras

Salva leitura dos sensores.

### GET /leituras

Retorna últimas 100 leituras.

---

## Autor

Davi Dorea