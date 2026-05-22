// =====================================================
// PROJETO: Controle Remoto ESP32 para Robô Espacial
// =====================================================

// ----------------------
// PINOS DO JOYSTICK
// ----------------------
const int pinoVRx = 34;   // Eixo X
const int pinoVRy = 35;   // Eixo Y

// ----------------------
// PINOS DOS LEDs
// ----------------------
const int pinoLedVerde = 25;
const int pinoLedVermelho = 26;

// ----------------------
// PINO DO BOTÃO
// ----------------------
const int pinoBotao = 32;

// ----------------------
// LIMITES DO JOYSTICK
// ----------------------
// Valores analógicos do ESP32 vão de 0 até 4095

const int limiteInferior = 1000;
const int limiteSuperior = 3000;

// ----------------------
// VARIÁVEIS DE ESTADO
// ----------------------
bool roboLigado = true;

bool ultimoEstadoBotao = HIGH;

String ultimoComando = "";

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // ----------------------
  // CONFIGURAÇÃO DOS PINOS
  // ----------------------

  pinMode(pinoVRx, INPUT);
  pinMode(pinoVRy, INPUT);

  pinMode(pinoLedVerde, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);

  // INPUT_PULLUP:
  // botão pressionado = LOW
  // botão solto = HIGH

  pinMode(pinoBotao, INPUT_PULLUP);

  // ----------------------
  // MENSAGENS INICIAIS
  // ----------------------

  Serial.println("=================================");
  Serial.println("CONTROLE REMOTO ESP32 INICIADO");
  Serial.println("=================================");

  Serial.println("Status: Robô LIGADO");
  Serial.println("LED Verde ACESO");

  atualizarLEDs();
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  lerBotaoDesligar();

  processarJoystick();

  delay(50);
}

// =====================================================
// FUNÇÃO: BOTÃO DE DESLIGAR
// =====================================================

void lerBotaoDesligar() {

  bool estadoAtual = digitalRead(pinoBotao);

  // Detecta clique do botão
  if (ultimoEstadoBotao == HIGH && estadoAtual == LOW) {

    // Alterna estado do robô
    roboLigado = !roboLigado;

    // ----------------------
    // ROBÔ DESLIGADO
    // ----------------------

    if (!roboLigado) {

      Serial.println();
      Serial.println(">>> COMANDO ENVIADO: DESLIGAR <<<");

      Serial.println("Motores PARADOS");
      Serial.println("Status: Robô DESLIGADO");
      Serial.println("LED Vermelho ACESO");
    }

    // ----------------------
    // ROBÔ LIGADO
    // ----------------------

    else {

      Serial.println();
      Serial.println(">>> COMANDO ENVIADO: LIGAR <<<");

      Serial.println("Status: Robô LIGADO");
      Serial.println("LED Verde ACESO");
    }

    atualizarLEDs();

    delay(200); // debounce
  }

  ultimoEstadoBotao = estadoAtual;
}

// =====================================================
// FUNÇÃO: CONTROLE DOS LEDs
// =====================================================

void atualizarLEDs() {

  if (roboLigado) {

    digitalWrite(pinoLedVerde, HIGH);
    digitalWrite(pinoLedVermelho, LOW);
  }

  else {

    digitalWrite(pinoLedVerde, LOW);
    digitalWrite(pinoLedVermelho, HIGH);
  }
}

// =====================================================
// FUNÇÃO: PROCESSAR JOYSTICK
// =====================================================

void processarJoystick() {

  // Se robô desligado, não envia comandos
  if (!roboLigado) {
    return;
  }

  // Leitura analógica
  int valorX = analogRead(pinoVRx);
  int valorY = analogRead(pinoVRy);

  String comando = "";

  // =================================================
  // MOVIMENTOS
  // =================================================

  // FRENTE
  if (valorY < limiteInferior) {

    comando = "Frente";
  }

  // TRÁS
  else if (valorY > limiteSuperior) {

    comando = "Trás";
  }

  // ESQUERDA
  else if (valorX < limiteInferior) {

    comando = "Esquerda";
  }

  // DIREITA
  else if (valorX > limiteSuperior) {

    comando = "Direita";
  }

  // =================================================
  // ENVIA SOMENTE SE O COMANDO MUDAR
  // =================================================

  if (comando != "" && comando != ultimoComando) {

    Serial.println();
    Serial.println("Comando enviado: " + comando);

    // Simulação lógica dos motores
    if (comando == "Frente") {

      Serial.println("Motores: Frente");
    }

    else if (comando == "Trás") {

      Serial.println("Motores: Ré");
    }

    else if (comando == "Esquerda") {

      Serial.println("Motor direito ATIVO");
    }

    else if (comando == "Direita") {

      Serial.println("Motor esquerdo ATIVO");
    }

    ultimoComando = comando;
  }

  // =================================================
  // JOYSTICK VOLTOU AO CENTRO
  // =================================================

  if (comando == "") {

    ultimoComando = "";
  }
}
