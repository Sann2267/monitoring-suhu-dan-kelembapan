# MOSQUITTO / MQTT

**PORT : 1880** *(Catatan kecil: Pastikan tidak tertukar, port 1880 biasanya digunakan untuk Node-RED, sedangkan Mosquitto MQTT berjalan di port 1883 sesuai dengan isi konfigurasi Anda di bawah).*

Berikut adalah langkah langkah menginstall mqtt di amazon linux 2023 dengan cara source(compile) step-by-step. Cara nya agak ribet karena menggunakan library yang berbeda dengan amazon linux 2.

### A. Persiapan dan Download File

**1. Install dependency dulu**

```bash
sudo dnf install gcc gcc-c++ make cmake openssl-devel libuuid-devel c-ares-devel -y

```

**2. Download source Mosquitto**

```bash
wget https://mosquitto.org/files/source/mosquitto-2.0.18.tar.gz

```

**3. Extract file**

```bash
tar -xvzf mosquitto-2.0.18.tar.gz

```

---

### B. Instalasi Requirement Khusus (cJSON)

Sebelum lanjut install, anda perlu mendownload source cJSON untuk requirement saat menginstall mqtt nanti.

**4. Install dependency git/dasar**

```bash
sudo dnf install gcc gcc-c++ make cmake git openssl-devel libuuid-devel c-ares-devel -y

```

**5. Download source cJSON**

```bash
git clone https://github.com/DaveGamble/cJSON.git

```

Masuk ke foldernya:

```bash
cd cJSON

```

**6. Compile dan install cJSON**

```bash
mkdir build
cd build
cmake ..
make
sudo make install

```

**7. Update library path**

```bash
sudo ldconfig

```

---

### C. Kompilasi dan Instalasi Mosquitto

**8. Buka Directory Mosquitto**
*(Karena posisi saat ini ada di folder `cJSON/build`, kita mundur dulu 2 folder ke belakang baru masuk ke mosquitto)*

```bash
cd ../..
cd mosquitto-2.0.18

```

**9. Lalu compile file nya**

```bash
make clean
make

```

**10. Lanjut Install MQTT**

```bash
sudo make install

```

---

### D. Konfigurasi Akses Luar (Penting untuk IoT)

**11. Menjalankan MQTT Secara Lokal**
Anda bisa menjalankan langsung mqtt nya tapi hanya running di local/server amazon itu sendiri. Artinya server/mqtt dari luar tidak akan bisa terhubung dengan mqtt amazon anda.
Untuk menjalankan mqtt:

```bash
mosquitto -v

```

> **Saran saya:** Sebelum mengetes, anda harus mengkonfigurasi kan mosquitto nya terlebih dahulu agar dapat diakses dari luar, seperti dari mosquitto di komputer lain atau bahkan IoT ESP32, ini penting!!!

**12. Buat config file**

```bash
sudo nano /etc/mosquitto.conf

```

Isi dengan:

```text
listener 1883
allow_anonymous true

```

Simpan dengan cara:

* Tekan `CTRL + O`
* Tekan `Enter`
* Tekan `CTRL + X`

**13. Jalankan mosquitto dengan config yang sudah anda bikin**

```bash
mosquitto -c /etc/mosquitto.conf -v

```

Kalau sudah berhasil, text *"local only mode"* akan menghilang dan cuma muncul:

> Opening ipv4 listen socket on port 1883.
> Opening ipv6 listen socket on port 1883.
> mosquitto version 2.0.18 running

---

### E. Menghubungkan PC Lokal ke Server MQTT

**Catatan:** Untuk menghubungkan mosquitto local PC/Laptop dengan mqtt di amazon anda harus mendownload mqtt di PC/Laptop anda dengan cara buka website resmi nya.

Setelah selesai mendownload pastikan anda setting/tambahkan PATH di Environment Variables:

**1. Buka Environment Variables**

* Tekan `Windows + S`
* Ketik: `Environment Variables`
* Klik **Edit the system environment variables**
* Akan muncul jendela System Properties

**2. Masuk ke Environment Variables**

* Klik tombol: **Environment Variables**
* Di bagian System Variables cari: **Path**
* Lalu klik **Edit**.

**3. Tambahkan folder Mosquitto**

* Klik: **New**
* Lalu masukkan path berikut (biasanya lokasi default):
`C:\Program Files\mosquitto`
* Kemudian klik: **OK** → **OK** → **OK**

**4. Restart terminal**
Tutup Git Bash / CMD / PowerShell, lalu buka lagi.

**5. Cek apakah PATH berhasil**

```bash
mosquitto -h

```

atau

```bash
mosquitto -v

```
