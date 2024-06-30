#include <WiFi.h>
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Configurações de WiFi - INSIRA SEU SSID E PASSWORD AQUI
const char* ssid = "SEU_SSID_WIFI";
const char* password = "SUA_SENHA_WIFI";

WiFiClient esp32WiFiClient;

// Configurações de MQTT - INSIRA SEU BROKER, CLIENT ID, USERNAME E PASSWORD AQUI
const char* mqttBroker = "SEU_ENDERECO_MQTT";
const int mqttPort = 1883; // Porta padrão MQTT
const char* mqttTopic = "topico/scan";
const char* mqttClientID = "mqttx_6b28939b";
const char* mqttUsername = "seu_usuario_mqtt";
const char* mqttPassword = "sua_senha_mqtt";

// Configurações do BLE
int scanTime = 5;
int levelRSSI = -75; // RSSI de referência para 1 metro
int N = 2; // Expoente da perda de caminho

// Configurações do filtro de média
const int windowSize = 5;
int rssiBuffer[windowSize];
int rssiIndex = 0;
int rssiSum = 0;

void setup() {
  Serial.begin(115200);
  connectWiFi();
  mqttClient.setServer(mqttBroker, mqttPort);
  BLEDevice::init("");
  Serial.println("Escaneando...");
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String foundDevices = advertisedDevice.getAddress().toString().c_str();
    if (foundDevices == "dd:f8:44:df:fa:ad") { // Se for o dispositivo autorizado
      Serial.println("Dispositivo IDENTIFICADO!");
      Serial.print("Nome do dispositivo: ");
      Serial.println(advertisedDevice.getName().c_str());
      int rssi = advertisedDevice.getRSSI();
      Serial.print("RSSI: ");
      Serial.println(rssi);

      // Aplicar filtro de média no RSSI
      int filteredRSSI = meanFilter(rssi);

      // Cálculo da distância usando a fórmula
      double distance = pow(10, ((levelRSSI - filteredRSSI) / (10.0 * N)));

      String payload = "{\"Nome do dispositivo\":\"" + String(advertisedDevice.getName().c_str()) +
                       "\",\"MAC\":\"" + foundDevices +
                       "\",\"RSSI\":" + String(rssi) +
                       "\",\"Distancia\":" + String(distance, 2) + " metros" + "}"; // Distância com 2 casas decimais e unidade de medida

      // Enviar mensagem MQTT apenas se o RSSI filtrado não for zero
      if (filteredRSSI != 0) {
        mqttClient.publish(mqttTopic, payload.c_str(), true);
      }
    }
  }
};

void scanBLE() {
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  Serial.print("Dispositivos encontrados: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Escaneamento concluído");
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientID, mqttUsername, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.print("Falha na conexão MQTT, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

int meanFilter(int newValue) {
  // Subtrai o valor mais antigo do buffer da soma
  rssiSum = rssiSum - rssiBuffer[rssiIndex] + newValue;

  // Adiciona o novo valor ao buffer
  rssiBuffer[rssiIndex] = newValue;

  // Avança para o próximo índice circular
  rssiIndex = (rssiIndex + 1) % windowSize;

  // Retorna a média
  return rssiSum / windowSize;
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  scanBLE();
  delay(2000);
}

