# Panduan Instalasi dan Setup Node-RED di AWS EC2

Node-RED adalah *flow-based programming tool* yang berjalan di atas Node.js. Dokumentasi resmi Node-RED menyarankan instalasi global menggunakan `npm` (Node Package Manager). Setelah berhasil berjalan, antarmuka editor Node-RED dapat diakses melalui browser pada port `1880`.

---

## 1. Pastikan Security Group AWS Membuka Port 1880
Sebelum menginstal aplikasi di server, pastikan AWS Firewall mengizinkan akses ke *port* tersebut.
1. Masuk ke **AWS Console** → **EC2** → **Instances**.
2. Pilih instance kamu, lalu klik tab **Security**.
3. Klik tautan pada **Security groups**, lalu pilih **Edit inbound rules**.
4. Tambahkan/pastikan *rule* ini ada:
   * **Type**: `Custom TCP`
   * **Protocol**: `TCP`
   * **Port range**: `1880`
   * **Source**: `My IP` *(Sangat disarankan menggunakan 'My IP' alih-alih `0.0.0.0/0` agar editor Node-RED kamu tidak terekspos ke publik).*
   * **Description**: `Node-RED editor`

*(Pastikan port 22 untuk SSH juga masih terbuka dengan konfigurasi Source: My IP).*

---

## 2. Login ke Server EC2 via SSH
1. Buka terminal (PowerShell / Git Bash / Command Prompt) di laptopmu.
2. Masuk ke direktori tempat file `.pem` disimpan (contoh: folder Downloads).
 ```bash
   cd Downloads
 ```


3. Lakukan SSH ke server menggunakan *Public IP* atau *Elastic IP* milikmu:
```bash
ssh -i nama-key-kamu.pem ec2-user@<IP_PUBLIC_EC2>

```


*(Contoh: `ssh -i iot-monitoring-key.pem ec2-user@13.238.95.89`)*
4. Jika muncul prompt *"Are you sure you want to continue connecting?"*, ketik `yes`.

---

## 3. Update Sistem Operasi Server

Sangat penting untuk memperbarui *package* sistem sebelum menginstal *software* baru.

1. Jalankan perintah update:
```bash
sudo dnf update -y

```


*(Catatan: Jika server menggunakan Amazon Linux 2 (versi lama) dan `dnf` tidak tersedia, gunakan `sudo yum update -y`).*
2. Verifikasi versi OS yang terinstal:
```bash
cat /etc/os-release

```


Pastikan *output* menunjukkan bahwa kamu menggunakan **Amazon Linux 2023** (atau varian yang sesuai).

---

## 4. Instalasi Node.js dan npm

Node-RED membutuhkan Node.js. Periksa apakah sistem sudah memilikinya.

1. Cek versi saat ini:
```bash
node -v
npm -v

```


2. Jika *output* memunculkan `command not found`, lakukan instalasi Node.js dan npm:
```bash
sudo dnf install nodejs npm -y

```


3. Cek kembali versinya untuk memastikan instalasi berhasil. Pastikan muncul angka versi seperti `v20.x.x` dan `10.x.x`.

---

## 5. Instalasi Node-RED Global

Instal paket Node-RED secara global di seluruh sistem:

```bash
sudo npm install -g --unsafe-perm node-red

```

*(Catatan: Jika muncul teks "warning" berwarna kuning selama instalasi, hal tersebut normal dan bisa diabaikan. Namun, jika ada tulisan "ERR!", instalasi gagal dan perlu diperbaiki).*

Verifikasi instalasi:

```bash
node-red --version

```

---

## 6. Uji Coba Node-RED (Mode Manual)

1. Jalankan server Node-RED secara manual di terminal:
```bash
node-red

```


2. Biarkan terminal tetap terbuka. Buka *browser* di laptopmu dan akses alamat berikut:
`http://<IP_PUBLIC_EC2>:1880`
3. Jika halaman editor *flow* terbuka, artinya instalasi sukses.
4. **Hentikan Node-RED manual ini** dengan kembali ke terminal dan menekan `CTRL + C`. *(Kita akan menjalankannya sebagai service di latar belakang pada Langkah 11).*

### Troubleshooting Jika Gagal Dibuka:

* Pastikan proses `node-red` tidak di-stop di terminal.
* Cek apakah server mendengarkan port 1880: `sudo ss -tulnp | grep 1880`.
* Periksa kembali IP Publik kamu. Jika internet/WiFi rumah di-restart, *Source IP* di Security Group AWS perlu diubah kembali ke *My IP* yang baru.

---

## 7. Instalasi Node-RED Dashboard

Untuk membuat antarmuka visual (seperti grafik/gauge), kamu perlu menginstal *module* Dashboard.

1. Pindah ke direktori konfigurasi pengguna Node-RED:
```bash
cd ~/.node-red

```


2. Instal *package* dashboard (paket UI resmi klasik Node-RED):
```bash
npm install node-red-dashboard

```


3. Restart Node-RED jika sedang berjalan, lalu buka ulang editor di browser (`http://<IP_PUBLIC_EC2>:1880`).
4. Di *palette* sebelah kiri layar, *scroll* ke bawah dan pastikan *node* khusus UI seperti `ui_button`, `ui_gauge`, dan `ui_chart` sudah muncul.

---

## 8. Membuat Flow Dashboard Sederhana (Test UI)

Langkah ini untuk memastikan URL `/ui` bisa diakses.

1. Tarik (*Drag*) 3 node ini ke *canvas* tengah:
* **Inject** (berada di grup *Common*)
* **Function** (berada di grup *Function*)
* **Gauge** (berada di grup *Dashboard*)


2. Sambungkan titik-titiknya menjadi: `[Inject] -> [Function] -> [Gauge]`.
3. Konfigurasi **Inject Node** (Klik 2x):
* **Payload**: Ubah formatnya ke `number`, lalu isi dengan angka `30`.
* **Repeat**: Pilih `interval`.
* **Every**: `5` seconds.
* Klik **Done**.


4. Konfigurasi **Function Node** (Klik 2x), masukkan script berikut:
```javascript
msg.payload = Number(msg.payload);
return msg;

```


* Klik **Done**.


5. Konfigurasi **Gauge Node** (Klik 2x):
* Pada kolom **Group**, klik ikon pensil (Edit) untuk membuat grup UI baru.
* **Tab Name**: `IoT Monitoring`
* **Group Name**: `Sensor Data`
* **Label**: `Suhu`
* **Value format**: `{{value}} °C`
* **Units**: `°C`
* **Range**: min `0`, max `100`
* Klik **Done**.


6. Klik tombol **Deploy** (berwarna merah) di pojok kanan atas layar.

---

## 9. Akses Halaman UI Dashboard

Buka tab *browser* baru dan tambahkan `/ui` di akhir URL servermu:
`http://<IP_PUBLIC_EC2>:1880/ui`

Jika sukses, kamu akan melihat *dashboard* sederhana berbentuk spidometer yang menunjukkan angka 30.

---

## 10. Konfigurasi Node-RED sebagai Systemd Service (Background)

Agar Node-RED tetap hidup meski terminal SSH ditutup, kita harus menjadikannya *service* sistem (daemon).

1. Buat file *service* baru:
```bash
sudo nano /etc/systemd/system/nodered.service

```


2. Salin dan tempel blok konfigurasi berikut (khusus untuk `ec2-user`):
```ini
[Unit]
Description=Node-RED
After=network.target

[Service]
Type=simple
User=ec2-user
Group=ec2-user
WorkingDirectory=/home/ec2-user
ExecStart=/usr/bin/env node-red
Restart=on-failure
RestartSec=10
Environment=NODE_OPTIONS=--max_old_space_size=256

[Install]
WantedBy=multi-user.target

```


3. Simpan dan keluar dari editor `nano`:
* Tekan `CTRL + O` lalu tekan `Enter` (Simpan).
* Tekan `CTRL + X` (Keluar).


4. *Reload* daemon systemd agar mendeteksi file baru:
```bash
sudo systemctl daemon-reload

```


5. Jalankan layanan Node-RED:
```bash
sudo systemctl start nodered

```


6. Aktifkan agar *service* berjalan otomatis *(auto-start)* setiap kali server *restart*:
```bash
sudo systemctl enable nodered

```


7. Cek status untuk memastikan layanan hidup:
```bash
sudo systemctl status nodered

```


*(Harus muncul teks berwarna hijau: `active (running)`. Untuk keluar dari tampilan log tekan huruf `q`).*

---

## 11. Daftar Command Penting Node-RED

Simpan perintah-perintah ini untuk manajemen *server* di masa mendatang:

* `sudo systemctl start nodered` (Menjalankan layanan)
* `sudo systemctl stop nodered` (Menghentikan layanan)
* `sudo systemctl restart nodered` (Restart sistem, wajib dilakukan setelah memasang *package npm* baru)
* `sudo systemctl status nodered` (Cek status layanan)
* `journalctl -u nodered -f` (Melihat log/aktivitas Node-RED secara *live*)
