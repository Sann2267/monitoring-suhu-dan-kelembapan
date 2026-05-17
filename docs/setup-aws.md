# Panduan Setup AWS EC2 untuk IoT Monitoring

## 1. Masuk ke AWS Console
1. Buka AWS Console dan login ke akun AWS kamu.
2. Cari layanan **EC2** di kolom pencarian atas. EC2 (Elastic Compute Cloud) adalah *virtual server* di AWS. Node-RED dan MQTT broker kamu akan berjalan di dalam server virtual ini. 
3. Pastikan **Region** kamu sudah benar (pojok kanan atas). Pilih region yang terdekat atau sesuai kebutuhan, misalnya `ap-southeast-2` (Sydney) atau `ap-southeast-1` (Singapore). Pastikan kamu konsisten menggunakan satu region ini hingga akhir.

---

## 2. Buat Instance Baru
1. Di *dashboard* EC2, klik tombol **Launch instance**.
2. Pada bagian **Name and tags**, beri label nama agar server mudah dikenali. Contoh: `iot-monitoring-server`.
3. Pada bagian **Application and OS Images (AMI)**, pilih **Amazon Linux 2023 AMI**. Pilih versi standar/default (bukan yang minimal). Sistem operasi ini dioptimalkan khusus oleh AWS untuk berjalan di lingkungan EC2.

---

## 3. Pilih Instance Type
Pilih spesifikasi *hardware* (Instance type) yang sesuai:
* Pilih **t2.micro** (atau **t3.micro** jika t2 tidak tersedia).
* Spesifikasi ini sudah sangat cukup untuk menjalankan *project* monitoring suhu dan kelembapan, dan biasanya masuk dalam cakupan *Free Tier* AWS.

---

## 4. Konfigurasi Key Pair
*Key pair* berfungsi sebagai kredensial keamanan utama (pengganti *password*) untuk membuktikan identitas saat kamu melakukan *remote login* (SSH) ke server Linux.

1. Pada bagian **Key pair**, pilih **Create new key pair** jika belum punya.
2. Isi detail berikut:
   * **Key pair name**: `iot-monitoring-key`
   * **Key pair type**: `RSA`
   * **Private key file format**: `.pem`
3. Klik **Create key pair**.
4. File `.pem` akan otomatis terunduh. Simpan file ini di lokasi yang aman. **Peringatan: Jangan pernah mengunggah file `.pem` ini ke GitHub publik!**

---

## 5. Buat Security Group (Firewall)
*Security Group* berfungsi mengatur *traffic* jaringan apa saja yang boleh masuk (*inbound*) dan keluar (*outbound*) dari server kamu.

1. Pada bagian **Network settings**, klik **Edit**.
2. Konfigurasikan pengaturan jaringan dasar:
   * **VPC**: Default VPC
   * **Subnet**: No preference (Default)
   * **Auto-assign public IP**: Enable
3. Pilih **Create security group**.
4. Beri nama **Security group name**: `sg-iot-monitoring`.
5. Beri **Description**: `Security group for IoT monitoring with SSH, MQTT, and Node-RED`.

---

## 6. Pengaturan Inbound Rules
Tambahkan aturan jaringan masuk berikut. Pastikan untuk menyesuaikan kolom *Source* demi keamanan:

| Type | Protocol | Port Range | Source | Description |
| :--- | :--- | :--- | :--- | :--- |
| **SSH** | TCP | 22 | My IP | Akses SSH untuk remote server dari laptop lokal. (Penting: Hindari `0.0.0.0/0` untuk SSH). |
| **Custom TCP** | TCP | 1880 | My IP | Akses ke Editor Node-RED dari browser (`http://IP:1880`). |
| **Custom TCP** | TCP | 1883 | 0.0.0.0/0 | Akses ke MQTT broker untuk ESP32/IoT device. Diatur publik agar device dari jaringan berbeda bisa masuk. |

*(Catatan Keamanan MQTT: Karena port 1883 dibuka ke publik, pastikan di tahap selanjutnya kamu mengatur username/password pada konfigurasi Mosquitto MQTT).*

---

## 7. Pengaturan Outbound Rules
Biarkan konfigurasi keluar (*outbound*) pada pengaturan *default*:
* **Type**: All traffic
* **Destination**: `0.0.0.0/0`
*Artinya, server diizinkan untuk mengakses internet luar, yang diperlukan untuk mengunduh package, install Node.js, Node-RED, dan pembaruan sistem.*

---

## 8. Konfigurasi Storage
Biarkan pada pengaturan *default*:
* **Ukuran**: 8 GiB
* **Tipe**: gp3
*(Kapasitas 8 GB sudah lebih dari cukup untuk sistem operasi, Node-RED, Mosquitto, dan kode project IoT sederhana).*

---

## 9. Launch Instance & Cek Status
1. Klik tombol **Launch instance** di pojok kanan bawah.
2. Setelah sukses, klik **View all instances**.
3. Pastikan server kamu telah menyala dengan mengecek dua indikator ini:
   * **Instance state**: `Running`
   * **Status checks**: `2/2 checks passed`
*(Jika status belum 2/2, tunggu 1-2 menit hingga proses booting selesai sebelum mencoba login).*

---

## 10. Mengidentifikasi Public IP Awal
Klik pada *Instance* kamu yang sedang berjalan, lalu cari bagian **Public IPv4 address** di panel detail bawah (Contoh: `13.xxx.xxx.xxx`). Ini adalah IP publik dinamis awal sebelum kita menguncinya menggunakan Elastic IP pada langkah berikutnya.

---

## 11. Alokasi dan Asosiasi Elastic IP (Sangat Direkomendasikan untuk IoT)
Secara default, AWS akan mengubah Public IP EC2 kamu setiap kali server dimatikan (*Stop*) dan dinyalakan kembali (*Start*). Fitur **Elastic IP** digunakan untuk mengunci sebuah IP publik statis agar tidak berubah-ubah, sehingga kamu tidak perlu mengedit ulang kode C++ pada ESP32 setiap kali server di-restart.

### Langkah Alokasi IP Statis:
1. Pada menu sebelah kiri dasbor EC2, gulir ke bawah ke bagian **Network & Security**, lalu klik **Elastic IPs**.
2. Klik tombol **Allocate Elastic IP address** di pojok kanan atas.
3. Biarkan pengaturan jaringan pada pilihan default (*Amazon's pool of IPv4 addresses*), lalu klik **Allocate** di bagian bawah.

### Langkah Menghubungkan IP ke EC2:
1. Pilih alamat Elastic IP yang baru saja kamu buat dari daftar.
2. Klik menu drop-down **Actions** di pojok kanan atas, lalu pilih **Associate Elastic IP address**.
3. Pada bagian **Resource type**, pilih **Instance**.
4. Pada kolom **Instance**, klik dan pilih nama server kamu (`iot-monitoring-server`).
5. Pada kolom **Private IP address**, pilih alamat IP internal yang muncul otomatis.
6. Klik **Associate**.

*Catatan: Mulai saat ini, gunakan alamat Elastic IP baru ini sebagai Endpoint utama untuk semua koneksi luar.*

---

## 12. Daftar Endpoint Layanan (Menggunakan Elastic IP)
Ganti `<ELASTIC_IP>` di bawah ini dengan alamat Elastic IP statis yang sudah kamu dapatkan:

* **Terminal SSH**: `ec2-user@<ELASTIC_IP>`
* **Node-RED Editor**: `http://<ELASTIC_IP>:1880`
* **Node-RED Dashboard**: `http://<ELASTIC_IP>:1880/ui`
* **MQTT Broker**: `<ELASTIC_IP>` (pada Port 1883)

---

## 13. Connect ke Instance melalui SSH
Jika kamu menggunakan OS Windows:
1. Buka PowerShell atau Git Bash di dalam folder tempat file `.pem` tersimpan.
2. Jalankan perintah SSH berikut (ganti dengan Elastic IP milikmu):
   ```bash
   ssh -i iot-monitoring-key.pem ec2-user@<ELASTIC_IP>

3. Jika muncul konfirmasi *Are you sure you want to continue connecting?*, ketik `yes` lalu tekan Enter.
4. Jika berhasil, *prompt* terminal akan berubah menjadi `[ec2-user@ip-xxx-xxx-xxx-xxx ~]$`.

---

## 14. Checklist Validasi AWS

Sebelum melanjutkan instalasi Node-RED dan Mosquitto, pastikan semua item ini sudah terpenuhi:

* [x] Instance menggunakan Amazon Linux 2023.
* [x] Instance state "Running" dan Status checks "2/2 passed".
* [x] Elastic IP telah sukses dialokasikan dan diasosiasikan ke instance.
* [x] Security Group membuka port 22 (SSH), 1880 (Node-RED), dan 1883 (MQTT).
* [x] File Key Pair `.pem` tersimpan dengan aman di lokal.
* [x] Berhasil masuk (SSH) ke dalam server sebagai `ec2-user` menggunakan Elastic IP.
