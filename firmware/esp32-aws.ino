#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "secrets.h" // Memanggil file kredensial

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();    // Menjaga koneksi MQTT tetap hidup

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    if (isnan(suhu) || isnan(kelembapan)) {
      Serial.println("Gagal membaca DHT!");
      return;
    }

    String payload = "{";
    payload += "\"suhu\":";
    payload += suhu;
    payload += ",\"kelembapan\":";
    payload += kelembapan;
    payload += "}";

    client.publish("sensor/data", payload.c_str());
    Serial.println(payload);
  }
}
