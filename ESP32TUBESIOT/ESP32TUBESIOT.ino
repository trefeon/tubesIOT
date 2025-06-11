// Kode untuk ESP32 sebagai Konsentrator
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h> // Library untuk sensor DHT
#include <ArduinoJson.h>

// --- KONFIGURASI WIFI ---
const char* ssid = "PROLiNK"; // Ganti dengan SSID WiFi kamu
const char* password = "prolink12345"; // Ganti dengan password WiFi kamu

// --- KONFIGURASI MQTT ---
const char* mqtt_server = "192.168.123.6"; // Contoh IP dari dokumen Anda, sesuaikan!
const int mqtt_port = 1883;
const char* mqtt_topic_out = "iot/concentrator/data";
char mqtt_client_id[30]; // Client ID dinamis

// --- KONFIGURASI SENSOR ---
#define DHT_PIN 4     // Pin data DHT11 terhubung ke GPIO4
#define DHT_TYPE DHT11 // Tipe sensor DHT diubah menjadi DHT11
#define PIR_PIN 2     // Pin output sensor PIR HC-SR501 terhubung ke GPIO2
#define LED_PIN 5     // Pin LED indikator (opsional)

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long intervalPesan = 5000; // Kirim data setiap 5 detik

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke WiFi: ");
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
    Serial.println("WiFi terhubung!");
    Serial.print("Alamat IP ESP32: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Gagal terhubung ke WiFi.");
  }
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima [callback tidak terpakai]: ");
  Serial.println(topic);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Mencoba koneksi MQTT ulang...");
    snprintf(mqtt_client_id, sizeof(mqtt_client_id), "ESP32Client-%04X", (int)random(0xffff)); 
    if (client.connect(mqtt_client_id)) {
      Serial.println("terhubung ke MQTT Broker!");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { delay(10); }
  Serial.println("Booting ESP32 Konsentrator (dengan DHT11 & HC-SR501)...");

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 

  dht.begin(); // Inisialisasi sensor DHT
  
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(onMqttMessage); 
  randomSeed(analogRead(0)); 
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Koneksi WiFi terputus. Mencoba menghubungkan ulang...");
    setup_wifi(); 
    return; 
  }

  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > intervalPesan) {
    lastMsg = now;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Baca suhu dalam Celsius

    bool motion = digitalRead(PIR_PIN); // Pembacaan PIR HC-SR501

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Gagal membaca data dari sensor DHT11!");
    }

    StaticJsonDocument<256> doc; 
    doc["deviceId"] = mqtt_client_id;
    doc["temperature_celsius"] = isnan(temperature) ? (float)NULL : temperature;
    doc["humidity_percent"] = isnan(humidity) ? (float)NULL : humidity;
    doc["motion_detected"] = motion;
    doc["timestamp"] = millis();

    char jsonString[256];
    serializeJson(doc, jsonString);

    if (client.publish(mqtt_topic_out, jsonString)) {
        Serial.print("Data terkirim ke MQTT (DHT11 & HC-SR501): ");
        Serial.println(jsonString);
        digitalWrite(LED_PIN, HIGH); 
        delay(100);
        digitalWrite(LED_PIN, LOW);
    } else {
        Serial.println("Gagal mengirim data ke MQTT.");
    }
  }
}
