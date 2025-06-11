// Kode untuk NodeMCU ESP8266 sebagai Bridge MQTT-HTTP
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> // Hanya jika perlu parsing/modifikasi JSON sebelum kirim

// --- KONFIGURASI WIFI ---
const char* ssid = "PROLiNK"; // Ganti dengan SSID WiFi kamu
const char* password = "prolink12345"; // Ganti dengan password WiFi kamu

// --- KONFIGURASI MQTT ---
const char* mqtt_server = "192.168.123.6"; // Contoh IP dari dokumen Anda, sesuaikan!
const int mqtt_port = 1883;
const char* mqtt_topic_in = "iot/concentrator/data"; // Topik yang di-subscribe
char mqtt_client_id_bridge[30]; // Client ID dinamis

// --- KONFIGURASI HTTP ---
const char* http_endpoint = "http://192.168.123.6:5000/data"; // Contoh IP & port, sesuaikan!

WiFiClient espNodeClient; 
PubSubClient mqttNodeClient(espNodeClient); 
HTTPClient http;

void setup_wifi_bridge() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan NodeMCU ke WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("NodeMCU WiFi terhubung!");
    Serial.print("Alamat IP NodeMCU: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("NodeMCU gagal terhubung ke WiFi.");
  }
}

void sendToHTTPApi(String jsonData) {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Meneruskan data ke HTTP endpoint: ");
    Serial.println(http_endpoint);
    Serial.print("Data JSON: ");
    Serial.println(jsonData);

    http.begin(espNodeClient, http_endpoint); 
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Kode Respons HTTP: ");
      Serial.println(httpResponseCode);
      Serial.print("Isi Respons HTTP: ");
      Serial.println(response);
    } else {
      Serial.print("Error saat mengirim HTTP POST, kode: ");
      Serial.println(httpResponseCode);
      Serial.printf("[HTTP] POST... gagal, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi tidak terhubung. Tidak bisa mengirim HTTP POST.");
  }
}

void onMqttMessage_bridge(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima oleh Bridge dari topik: ");
  Serial.println(topic);
  
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Payload (String): " + message);

  sendToHTTPApi(message);
}

void reconnectMQTT_bridge() {
  while (!mqttNodeClient.connected()) {
    Serial.print("Mencoba koneksi MQTT ulang untuk Bridge...");
    snprintf(mqtt_client_id_bridge, sizeof(mqtt_client_id_bridge), "NodeMCUClient-%04X", (int)random(0xffff));
    if (mqttNodeClient.connect(mqtt_client_id_bridge)) {
      Serial.println("terhubung ke MQTT Broker (Bridge)!");
      mqttNodeClient.subscribe(mqtt_topic_in);
      Serial.print("Berlangganan ke topik: ");
      Serial.println(mqtt_topic_in);
    } else {
      Serial.print("gagal, rc=");
      Serial.print(mqttNodeClient.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { delay(10); }
  Serial.println("Booting NodeMCU Bridge...");
  
  setup_wifi_bridge();
  
  mqttNodeClient.setServer(mqtt_server, mqtt_port);
  mqttNodeClient.setCallback(onMqttMessage_bridge);
  randomSeed(analogRead(0)); 
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Koneksi WiFi NodeMCU terputus. Mencoba menghubungkan ulang...");
    setup_wifi_bridge();
    return;
  }

  if (!mqttNodeClient.connected()) {
    reconnectMQTT_bridge();
  }
  mqttNodeClient.loop();
  
  delay(100); 
}