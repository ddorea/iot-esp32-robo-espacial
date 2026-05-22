#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <time.h>

// ======================================
// WIFI
// ======================================

const char* ssid = "Rafa Crusoé";
const char* password = "12345689";

// ======================================
// SUPABASE
// ======================================

const char* supabaseUrl =
  "https://aemjcsucyrcbukbthtaq.supabase.co/rest/v1/leituras";

const char* supabaseKey =
  "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFlbWpjc3VjeXJjYnVrYnRodGFxIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Nzk0NTk5MTIsImV4cCI6MjA5NTAzNTkxMn0.8q_Q0j3AT9Zt0be5tJv6HPaJicH5yWJLdXoXM4bd74s";

// ======================================
// CALLMEBOT
// ======================================

String telefone = "557199410521";

String apiKey = "7544306";

// ======================================
// PINOS ESP32-S3
// ======================================

const int ledVerde = 4;
const int ledVermelho = 5;

const int pir = 6;
const int ldr = 7;

const int servoPin = 15;

// ======================================
// DHT22
// ======================================

#define DHTPIN 16
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ======================================
// SERVO
// ======================================

Servo motor;

// ======================================
// VARIÁVEIS
// ======================================

int probabilidadeVida = 0;

String estadoRobo = "NORMAL";

bool mensagemEnviada = false;

// ======================================
// BUFFER LOCAL
// ======================================

String bufferJSON[20];

int bufferIndex = 0;

// ======================================
// TIMESTAMP
// ======================================

String obterTimestampISO() {

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {

    return "2026-01-01T00:00:00Z";
  }

  char buffer[30];

  strftime(
    buffer,
    sizeof(buffer),
    "%Y-%m-%dT%H:%M:%SZ",
    &timeinfo
  );

  return String(buffer);
}

// ======================================
// WHATSAPP
// ======================================

void enviarWhatsApp() {

  String mensagem =
    "ALERTA! Alta probabilidade de vida detectada no planeta.";

  mensagem.replace(" ", "%20");

  String url =
    "https://api.callmebot.com/whatsapp.php?phone=" +
    telefone +
    "&text=" +
    mensagem +
    "&apikey=" +
    apiKey;

  HTTPClient http;

  http.begin(url);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {

    Serial.println("Mensagem enviada via WhatsApp!");
  }

  else {

    Serial.println("Erro ao enviar mensagem.");
  }

  http.end();
}

// ======================================
// REENVIO BUFFER
// ======================================

void reenviarBuffer() {

  if (bufferIndex == 0) {

    return;
  }

  Serial.println("Tentando reenviar buffer...");

  for (int i = 0; i < bufferIndex; i++) {

    HTTPClient http;

    http.begin(supabaseUrl);

    http.addHeader("Content-Type", "application/json");

    http.addHeader("apikey", supabaseKey);

    http.addHeader(
      "Authorization",
      String("Bearer ") + supabaseKey
    );

    int codigo = http.POST(bufferJSON[i]);

    if (codigo > 0) {

      Serial.println("Leitura do buffer reenviada!");
    }

    else {

      Serial.println("Falha ao reenviar buffer.");
    }

    http.end();
  }

  bufferIndex = 0;
}

// ======================================
// ENVIO SUPABASE
// ======================================

void enviarLeituraSupabase(
  float temperatura,
  float umidade,
  int luminosidade,
  int presenca,
  int probabilidade
) {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi desconectado.");

    return;
  }

  reenviarBuffer();

  HTTPClient http;

  http.begin(supabaseUrl);

  http.addHeader("Content-Type", "application/json");

  http.addHeader("apikey", supabaseKey);

  http.addHeader(
    "Authorization",
    String("Bearer ") + supabaseKey
  );

  String timestamp = obterTimestampISO();

  String json = "{";

  json += "\"timestamp\":\"" + timestamp + "\",";
  json += "\"temperatura_c\":" + String(temperatura) + ",";
  json += "\"umidade_pct\":" + String(umidade) + ",";
  json += "\"luminosidade\":" + String(luminosidade) + ",";
  json += "\"presenca\":" + String(presenca) + ",";
  json += "\"probabilidade_vida\":" + String(probabilidade);

  json += "}";

  Serial.println("JSON enviado:");
  Serial.println(json);

  int httpResponseCode = http.POST(json);

  Serial.print("Resposta HTTP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {

    String resposta = http.getString();

    Serial.println("Dados enviados ao Supabase!");

    Serial.println(resposta);
  }

  else {

    Serial.println("Falha no envio.");

    if (bufferIndex < 20) {

      bufferJSON[bufferIndex] = json;

      bufferIndex++;

      Serial.println("Dados armazenados no buffer.");
    }
  }

  http.end();
}

// ======================================
// SETUP
// ======================================

void setup() {

  Serial.begin(115200);

  // LEDs

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  // PIR

  pinMode(pir, INPUT);

  // DHT

  dht.begin();

  // Servo

  motor.attach(servoPin);

  // ======================================
  // WIFI
  // ======================================

  Serial.println("Conectando ao WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println("");

  Serial.println("WiFi conectado!");

  Serial.print("IP ESP32: ");

  Serial.println(WiFi.localIP());

  // ======================================
  // NTP
  // ======================================

  //
  configTime(0, 0, "pool.ntp.org");

  Serial.print("Sincronizando horario");

  struct tm timeinfo;

  while (!getLocalTime(&timeinfo)) {

    Serial.print(".");

    delay(500);
  }

  Serial.println("");
  Serial.println("Horario sincronizado!");

  // ======================================
  // ESTADO INICIAL
  // ======================================

  digitalWrite(ledVerde, HIGH);

  digitalWrite(ledVermelho, LOW);

  Serial.println("=================================");

  Serial.println("ROBO ESPACIAL INICIADO");

  Serial.println("=================================");
}

// ======================================
// LOOP
// ======================================

void loop() {

  // ======================================
  // LEITURA DOS SENSORES
  // ======================================

  float temperatura = dht.readTemperature();

  float umidade = dht.readHumidity();

  int luminosidade = analogRead(ldr);

  int presenca = digitalRead(pir);

  // ======================================
  // VERIFICA DHT
  // ======================================

  if (isnan(temperatura) || isnan(umidade)) {

    Serial.println("Erro ao ler DHT22!");

    delay(2000);

    return;
  }

  // ======================================
  // CÁLCULO
  // ======================================

  probabilidadeVida = 0;

  // Temperatura

  if (temperatura >= 15 && temperatura <= 30) {

    probabilidadeVida += 25;
  }

  // Umidade

  if (umidade >= 40 && umidade <= 70) {

    probabilidadeVida += 25;
  }

  // Luminosidade

  if (luminosidade > 2000) {

    probabilidadeVida += 20;
  }

  // Presença

  if (presenca == HIGH) {

    probabilidadeVida += 30;
  }

  // ======================================
  // ALERTA
  // ======================================

  if (probabilidadeVida > 75) {

    estadoRobo = "ALERTA";

    digitalWrite(ledVerde, LOW);

    digitalWrite(ledVermelho, HIGH);

    motor.write(90);

    Serial.println("");

    Serial.println(
      "ALERTA! Alta probabilidade de vida detectada!"
    );

    if (!mensagemEnviada) {

      enviarWhatsApp();

      mensagemEnviada = true;
    }
  }

  // ======================================
  // NORMAL
  // ======================================

  else {

    estadoRobo = "LIGADO";

    digitalWrite(ledVerde, HIGH);

    digitalWrite(ledVermelho, LOW);

    mensagemEnviada = false;

    Serial.println("");

    Serial.println(
      "Exploracao normal. Nenhum indicio relevante detectado."
    );

    motor.write(0);

    delay(500);

    motor.write(180);

    delay(500);
  }

  // ======================================
  // MONITOR SERIAL
  // ======================================

  Serial.println("---------------------------------");

  Serial.print("Temperatura: ");

  Serial.print(temperatura);

  Serial.println(" C");

  Serial.print("Umidade: ");

  Serial.print(umidade);

  Serial.println(" %");

  Serial.print("Intensidade da luz: ");

  Serial.println(luminosidade);

  Serial.print("Sensor de presenca: ");

  if (presenca == HIGH) {

    Serial.println("Presenca detectada");
  }

  else {

    Serial.println("Sem presenca");
  }

  Serial.print("Estado do robo: ");

  Serial.println(estadoRobo);

  Serial.print("Probabilidade de vida: ");

  Serial.print(probabilidadeVida);

  Serial.println("%");

  Serial.println("---------------------------------");

  // ======================================
  // ENVIO PARA SUPABASE
  // ======================================

  enviarLeituraSupabase(
    temperatura,
    umidade,
    luminosidade,
    presenca,
    probabilidadeVida
  );

  delay(2000);
}