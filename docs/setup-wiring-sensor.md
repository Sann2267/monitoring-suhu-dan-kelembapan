# Dokumentasi Wiring & Setup Sensor DHT22 ke ESP32

Dokumentasi ini menjelaskan cara melakukan *wiring* (penyambungan) sensor DHT22 3-pin ke ESP32 untuk *project* monitoring suhu dan kelembapan *real-time* berbasis AWS, MQTT, dan Node-RED.

Sensor DHT22 digunakan untuk membaca nilai suhu dan kelembapan dari lingkungan sekitar. Data dari sensor akan dibaca oleh ESP32, lalu dikirim ke MQTT Broker yang berjalan di AWS EC2 dan ditampilkan melalui *dashboard* Node-RED.

---

## 1. Komponen yang Dibutuhkan
Komponen yang digunakan pada *project* ini adalah:
1. ESP32 Development Board
2. Sensor DHT22 (Modul 3 pin)
3. Kabel jumper *female-to-female*
4. Kabel USB Micro/Type-C (untuk ESP32)
5. Laptop atau komputer
6. Arduino IDE

*(Catatan: Sensor yang digunakan adalah DHT22 versi modul 3 pin. Sensor jenis ini biasanya sudah memiliki resistor pull-up bawaan, sehingga tidak perlu menambahkan resistor eksternal secara manual).*

---

## 2. Fungsi Setiap Komponen
* **ESP32**: Mikrokontroler yang memiliki fitur WiFi terintegrasi. Pada *project* ini, ESP32 digunakan untuk membaca data dari sensor, terhubung ke jaringan internet, dan mengirim data ke MQTT Broker.
* **Sensor DHT22**: Sensor yang digunakan untuk membaca suhu dan kelembapan. Sensor ini memiliki akurasi dan rentang ukur yang lebih baik dibandingkan tipe DHT11.
* **Kabel Jumper**: Digunakan untuk menghubungkan pin-pin fisik pada sensor DHT22 ke pin pada ESP32.
* **Arduino IDE**: *Software* yang digunakan untuk menulis, melakukan *compile*, dan mengunggah (*upload*) program C++ ke dalam ESP32.

---

## 3. Pin pada Sensor DHT22 (3 Pin)
Sensor DHT22 versi 3 pin biasanya memiliki susunan pin utama sebagai berikut:
* **VCC** : Sumber tegangan masukan
* **DATA** : Jalur pengiriman sinyal/data sensor
* **GND** : *Ground* (Arus negatif)

Pada beberapa merek modul, pin **DATA** bisa juga dicetak dengan label lain, seperti: `OUT`, `SIGNAL`, atau `S`. Jika pada sensormu tertulis label tersebut, itu adalah pin data.

---

## 4. Wiring Sensor DHT22 ke ESP32
Pada *project* ini, sensor DHT22 dihubungkan ke ESP32 menggunakan pin **GPIO 15** sebagai pin penerima data.

### Tabel Wiring:
| Pin DHT22 (Modul) | Pin ESP32 | Keterangan |
| :---: | :---: | :--- |
| **VCC** | **3V3** | Sumber tegangan 3.3 Volt |
| **DATA / OUT / S** | **GPIO 15** | Jalur data |
| **GND** | **GND** | Ground |

### Diagram Skema Sederhana:
```text
  DHT22 3 Pin                  ESP32
  -----------                  -----
  VCC        ----------------> 3V3
  DATA/OUT   ----------------> GPIO 15
  GND        ----------------> GND

```

**⚠️ PERINGATAN PENTING:** Pastikan kabel terhubung dengan benar. Kesalahan paling umum adalah menukar posisi `VCC` dan `GND`. Jika kedua pin ini tertukar, sensor bisa gagal terbaca, *overheating* (panas berlebih), atau bahkan rusak permanen.

---

## 5. Kenapa Menggunakan Pin GPIO 15?

Pada kode program *project* ini, pin data sensor secara spesifik didefinisikan di GPIO 15:

```cpp
#define DHTPIN 15

```

Artinya, kabel `DATA` dari sensor DHT22 **harus** benar-benar terhubung ke pin `D15` / `GPIO 15` pada papan ESP32. Jika kamu memasang kabel `DATA` ke pin lain (misalnya GPIO 4), maka kode di atas wajib diubah menjadi `#define DHTPIN 4`.

---

## 6. Tegangan yang Digunakan

Sangat disarankan agar VCC sensor DHT22 dihubungkan ke pin **3V3** ESP32, bukan pin 5V (VIN).
Alasannya, pin GPIO pada ESP32 beroperasi menggunakan logika **3.3V**. Menggunakan tegangan 3.3V memastikan sinyal data yang masuk ke ESP32 aman dan tidak berisiko merusak pin mikrokontroler.

---

## 7. Checklist Sebelum Upload Program

Sebelum mengunggah program ke ESP32, pastikan hal-hal berikut sudah benar:

* [x] Kabel `VCC` sensor terhubung ke `3V3` ESP32.
* [x] Kabel `GND` sensor terhubung ke `GND` ESP32.
* [x] Kabel `DATA` sensor terhubung ke `GPIO 15` ESP32.
* [x] Tidak ada sambungan kabel jumper yang longgar.
* [x] Board ESP32 sudah terhubung ke laptop via USB.
* [x] Arduino IDE sudah mendeteksi Port (COM) ESP32.
* [x] Library sensor DHT sudah terinstal di Arduino IDE.

---

## 8. Library yang Dibutuhkan (Arduino IDE)

Agar ESP32 bisa membaca DHT22 dan terhubung ke server, instal *library* berikut melalui menu **Sketch** → **Include Library** → **Manage Libraries**:

1. `DHT sensor library` (oleh Adafruit)
2. `Adafruit Unified Sensor` (Dibutuhkan oleh library DHT)
3. `PubSubClient` (oleh Nick O'Leary - untuk koneksi MQTT)

Kode ini memanggil *library* tersebut di awal program:

```cpp
#include <WiFi.h>           // Memungkinkan ESP32 terhubung ke WiFi
#include <PubSubClient.h>   // Memungkinkan protokol pengiriman MQTT
#include <DHT.h>            // Memungkinkan pembacaan sensor DHT

```

---

## 9. Konfigurasi Board di Arduino IDE

Gunakan pengaturan berikut pada menu **Tools**:

* **Board**: `ESP32 Dev Module`
* **Port**: *(Pilih port COM yang terdeteksi)*
* **Upload Speed**: `115200` atau `921600`
* **CPU Frequency**: `240MHz (WiFi/BT)`
* **Flash Frequency**: `80MHz`
* **Flash Mode**: `QIO` *(Jika gagal upload, coba ubah ke `DIO`)*

*💡 Tips: Jika saat proses upload muncul tulisan `Connecting...` terlalu lama, tekan dan tahan tombol **BOOT** fisik pada papan ESP32 sampai proses persentase upload berjalan, lalu lepaskan.*

---

## 10. Program Test Sensor (Tanpa WiFi/MQTT)

Sebelum masuk ke sistem cloud, uji coba pembacaan sensor menggunakan kode dasar ini.

```cpp
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Testing sensor DHT22...");
}

void loop() {
  delay(2000);

  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C | Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println(" %");
}

```

**Hasil Test**: Buka **Serial Monitor** (Ikon kaca pembesar di kanan atas Arduino IDE) dan ubah baud rate ke `115200`. Jika sukses, data akan muncul setiap 2 detik.

---

## 11. Program Final: ESP32 + DHT22 + MQTT AWS

File ini adalah kode utama (`firmware/esp32-dht22-mqtt.ino`) yang membaca sensor dan mengirimkannya ke EC2.

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "secrets.h"

// Pin dan tipe sensor
#define DHTPIN 15
#define DHTTYPE DHT22

// Topic MQTT
#define MQTT_TOPIC "sensor/data"

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const unsigned long interval = 5000;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi berhasil terhubung");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT Broker... ");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;

    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    if (isnan(suhu) || isnan(kelembapan)) {
      Serial.println("Gagal membaca data dari sensor DHT22!");
      return;
    }

    // Format Payload JSON
    String payload = "{";
    payload += "\"suhu\":";
    payload += String(suhu, 2);
    payload += ",";
    payload += "\"kelembapan\":";
    payload += String(kelembapan, 2);
    payload += "}";

    client.publish(MQTT_TOPIC, payload.c_str());

    Serial.print("Data terkirim ke topic ");
    Serial.print(MQTT_TOPIC);
    Serial.print(": ");
    Serial.println(payload);
  }
}

```

---

## 12. Manajemen File Konfigurasi (secrets.h)

File `secrets.h` digunakan untuk menyimpan kredensial WiFi dan IP Server agar terpisah dari kode utama. **Hanya buat file ini di komputer lokalmu.**

Contoh isi `secrets.h`:

```cpp
#ifndef SECRETS_H
#define SECRETS_H

const char* ssid = "II";
const char* password = "Qwerty123";
const char* mqtt_server = "13.238.95.89"; // IP EC2
const int mqtt_port = 1883;

#endif

```

Untuk diunggah ke GitHub, buatlah file *template* bernama `secrets.example.h` agar orang lain tahu strukturnya tanpa melihat data aslimu:

```cpp
#ifndef SECRETS_H
#define SECRETS_H

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";
const char* mqtt_server = "IP_PUBLIC_EC2";
const int mqtt_port = 1883;

#endif

```

---

## 13. Format Data JSON MQTT

ESP32 akan mengirim (Publish) *payload* teks berformat JSON ke dalam *Topic* `sensor/data`. Bentuk *output*-nya akan seperti ini:

```json
{
  "suhu": 30.20,
  "kelembapan": 78.50
}

```

---

## 14. Troubleshooting (Pemecahan Masalah)

### A. Sensor Tidak Terbaca (`Gagal membaca data dari sensor DHT22!`)

* Kabel `DATA` belum masuk ke port `D15`/`GPIO 15`.
* Kabel VCC dan GND tertukar posisinya.
* Kabel jumper *loss* / longgar / terputus di dalam.
* Salah mengatur jenis sensor pada kode program (Cek baris `#define DHTTYPE DHT22`. Jika kamu menggunakan DHT11, ubah menjadi `DHT11`).

### B. WiFi Tidak Terhubung (Hanya Muncul Titik `.....`)

* Nama/Password WiFi salah (perhatikan huruf besar/kecil).
* WiFi yang digunakan menggunakan frekuensi 5 GHz (ESP32 **hanya** mendukung jaringan 2.4 GHz).

### C. MQTT Gagal Terhubung (`rc=-2`)

* Kode error `rc=-2` pada `PubSubClient` menandakan **Koneksi Ditolak** oleh server.
* IP Public EC2 salah ketik di `secrets.h`.
* Layanan *Mosquitto MQTT* di server AWS belum di-start atau *crash*.
* Port `1883` belum dibuka di konfigurasi *Security Group Inbound Rules* AWS EC2.

---

## 15. Keamanan Repository (.gitignore)

Agar file rahasia tidak terunggah ke GitHub, pastikan kamu membuat file bernama `.gitignore` di dalam folder *repository*-mu dan mengisinya dengan baris berikut:

```text
# Arduino secrets file
secrets.h

# Arduino IDE build directory
build/
*.bin
*.elf
*.map

```

---

## 16. Kesimpulan

Pada tahap ini, *hardware* ESP32 telah berhasil di-*wiring* dengan sensor DHT22 dan diprogram untuk membaca data lingkungan. Data tersebut telah distrukturkan ke dalam format JSON dan dikirim melalui protokol MQTT secara *real-time* ke server *cloud* AWS. Tahap selanjutnya adalah menangkap data ini menggunakan **Node-RED** untuk divisualisasikan.
