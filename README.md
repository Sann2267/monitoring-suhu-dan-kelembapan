# Monitoring Suhu dan Kelembapan Real-Time Berbasis AWS dan Node-RED

Project ini adalah sistem monitoring suhu dan kelembapan secara real-time menggunakan sensor DHT, mikrokontroler ESP32/ESP8266, MQTT Broker di AWS EC2, dan dashboard Node-RED.

Data suhu dan kelembapan dibaca oleh sensor, lalu dikirim melalui jaringan WiFi ke MQTT Broker yang berjalan di server AWS EC2. Setelah itu, Node-RED menerima data tersebut dan menampilkannya dalam bentuk dashboard real-time.

---

## Tujuan Project

Tujuan dari project ini adalah membuat sistem monitoring lingkungan yang dapat membaca data suhu dan kelembapan secara otomatis dan menampilkannya secara real-time melalui dashboard berbasis web.

Project ini cocok digunakan sebagai contoh penerapan Internet of Things, Cloud Computing, MQTT, dan visualisasi data menggunakan Node-RED.

---

## Teknologi yang Digunakan

- ESP32
- Sensor DHT22
- AWS EC2
- Mosquitto MQTT Broker
- Node-RED
- Node-RED Dashboard
- Arduino IDE
- WiFi
- JSON

---

## Alur Kerja Sistem

1. Sensor DHT membaca data suhu dan kelembapan.
2. ESP32 mengambil data dari sensor.
3. Data dikirim ke MQTT Broker menggunakan protokol MQTT.
4. MQTT Broker berjalan di server AWS EC2.
5. Node-RED melakukan subscribe ke topic MQTT.
6. Data diproses oleh Node-RED.
7. Dashboard Node-RED menampilkan suhu dan kelembapan secara real-time.

---

## Arsitektur Sistem

```text
Sensor DHT11/DHT22
        |
        v
ESP32 / ESP8266
        |
        v
WiFi / Internet
        |
        v
AWS EC2 Server
        |
        v
Mosquitto MQTT Broker
        |
        v
Node-RED
        |
        v
Dashboard Monitoring Real-Time
