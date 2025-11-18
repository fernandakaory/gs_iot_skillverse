
/*
 * Projeto: Monitor de Frequência Cardíaca com ESP32 e MQTT
 * Disciplina: Global Solutions - O Futuro do Trabalho
 * Descrição: Sistema que monitora frequência cardíaca usando um potenciômetro (simulação)
 *            e envia alertas via MQTT quando detecta frequência elevada
 */

#include <WiFi.h>
#include <PubSubClient.h>

// Configurações de WiFi
const char* ssid = "Wokwi-GUEST";          // Substitua pelo nome da sua rede WiFi
const char* password = "";     // Substitua pela senha da sua rede WiFi

// Configurações do Broker MQTT
const char* mqtt_server = "broker.hivemq.com";  // Broker MQTT público (ou use seu próprio)
const int mqtt_port = 1883;
const char* mqtt_user = "";              // Deixe vazio se não usar autenticação
const char* mqtt_password = "";          // Deixe vazio se não usar autenticação

// Tópicos MQTT
const char* topic_bpm = "fiap/gs/heartrate/bpm";           // Tópico para publicar BPM
const char* topic_status = "fiap/gs/heartrate/status";     // Tópico para publicar status
const char* topic_alert = "fiap/gs/heartrate/alert";       // Tópico para alertas

// Configuração do Potenciômetro (simulando sensor de frequência cardíaca)
const int POT_PIN = 34;  // GPIO34 - Pino analógico do ESP32

// Configuração dos LEDs de status
const int LED_RED = 32;    // LED vermelho - frequência alta
const int LED_GREEN = 26;  // LED verde - frequência normal
const int LED_YELLOW = 33; // LED amarelo - frequência moderada

// Variáveis globais
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
const long interval = 2000;  // Intervalo de leitura (2 segundos)

// Limites de frequência cardíaca
const int BPM_MIN = 40;
const int BPM_MAX = 180;
const int BPM_ALERT_HIGH = 120;  // Frequência considerada alta
const int BPM_WARNING = 100;      // Frequência de aviso

// ====================================================================================
// FUNÇÃO: Conectar ao WiFi
// ====================================================================================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Falha ao conectar WiFi!");
  }
}

// ====================================================================================
// FUNÇÃO: Callback para mensagens MQTT recebidas
// ====================================================================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

// ====================================================================================
// FUNÇÃO: Reconectar ao Broker MQTT
// ====================================================================================
void reconnect() {
  // Loop até reconectar
  while (!client.connected()) {
    Serial.print("Conectando ao Broker MQTT...");

    // Cria um ID único para o cliente
    String clientId = "ESP32-HeartRate-";
    clientId += String(random(0xffff), HEX);

    // Tenta conectar
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("Conectado!");

      // Subscreve aos tópicos necessários (se houver)
      // client.subscribe("fiap/gs/heartrate/command");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

// ====================================================================================
// FUNÇÃO: Ler potenciômetro e converter para BPM
// ====================================================================================
int readHeartRate() {
  // Lê o valor do potenciômetro (0-4095)
  int potValue = analogRead(POT_PIN);

  // Converte para BPM (40-180 BPM)
  int bpm = map(potValue, 0, 4095, BPM_MIN, BPM_MAX);

  return bpm;
}

// ====================================================================================
// FUNÇÃO: Atualizar LEDs de acordo com o BPM
// ====================================================================================
void updateLEDs(int bpm) {
  if (bpm >= BPM_ALERT_HIGH) {
    // Frequência alta - LED vermelho
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
  } else if (bpm >= BPM_WARNING) {
    // Frequência moderada - LED amarelo
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    // Frequência normal - LED verde
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

// ====================================================================================
// FUNÇÃO: Publicar dados via MQTT
// ====================================================================================
void publishData(int bpm) {
  char bpmStr[10];
  sprintf(bpmStr, "%d", bpm);

  // Publica o valor de BPM
  client.publish(topic_bpm, bpmStr);
  Serial.print("BPM publicado: ");
  Serial.println(bpmStr);

  // Define o status e publica
  String status;
  if (bpm >= BPM_ALERT_HIGH) {
    status = "ALTA - ALERTA!";

    // Publica mensagem de alerta
    String alertMsg = "ATENCAO! Frequencia cardiaca elevada: " + String(bpm) + " BPM. Faca uma pausa!";
    client.publish(topic_alert, alertMsg.c_str());

    Serial.println("⚠️  ALERTA: Frequência cardíaca alta!");
    Serial.println("💡 Sugestão: Faça uma pausa e relaxe!");
  } else if (bpm >= BPM_WARNING) {
    status = "MODERADA";
  } else {
    status = "NORMAL";
  }

  client.publish(topic_status, status.c_str());
  Serial.print("Status: ");
  Serial.println(status);
}

// ====================================================================================
// SETUP
// ====================================================================================
void setup() {
  // Inicializa Serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("========================================");
  Serial.println("Monitor de Frequência Cardíaca - ESP32");
  Serial.println("Global Solutions - O Futuro do Trabalho");
  Serial.println("========================================");

  // Configura pinos dos LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // Teste inicial dos LEDs
  digitalWrite(LED_RED, HIGH);
  delay(300);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  delay(300);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);

  // Conecta ao WiFi
  setup_wifi();

  // Configura MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Sistema iniciado!");
  Serial.println("Aguardando leituras...");
  Serial.println();
}

// ====================================================================================
// LOOP PRINCIPAL
// ====================================================================================
void loop() {
  // Verifica conexão MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lê e publica dados a cada intervalo definido
  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;

    // Lê frequência cardíaca do potenciômetro
    int bpm = readHeartRate();

    // Imprime no Serial Monitor
    Serial.println("========================================");
    Serial.print("Frequência Cardíaca: ");
    Serial.print(bpm);
    Serial.println(" BPM");

    // Atualiza LEDs
    updateLEDs(bpm);

    // Publica dados via MQTT
    publishData(bpm);

    Serial.println("========================================");
    Serial.println();
  }

  delay(3000);  // Pequeno delay para estabilidade
}
