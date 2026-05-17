---

```markdown
# Dokumentasi Setup AWS

## 1. Kredensial & Autentikasi (Key Pair)
**Key Pair** adalah kredensial keamanan utama yang digunakan untuk autentikasi yang aman saat mengakses instance Amazon EC2 (Virtual Machine) secara *remote* melalui protokol SSH.

### Manajemen Key Pair
Berikut adalah perintah-perintah dasar untuk mengelola *private key* (`flask.pem`) dan *public key* (`flask.pub`):

* **Melihat Detail File Key**
    ```bash
    ls -la flask.pem
    ```
    *Output contoh: `-rw-r--r-- 1 Ibnu 197121 1674 Apr 14 14:26 flask.pem`*
    *Fungsi: Menampilkan detail file termasuk hak akses (permission), pemilik, dan ukuran file. (Perintah ini spesifik untuk environment Linux/Unix/Git Bash).*

* **Mengubah Hak Akses (Permissions)**
    ```bash
    chmod 400 flask.pem
    ```
    *Fungsi: Mengamankan private key. Angka `400` berarti file ini berstatus *read-only* (hanya bisa dibaca) dan murni hanya pemilik file yang memiliki akses tersebut. AWS mewajibkan pengaturan ini agar key tidak bisa diakses oleh *user* lain di komputer yang sama.*

* **Mengekstrak Public Key**
    ```bash
    ssh-keygen -y -f flask.pem > flask.pub
    ```
    *Fungsi: Menghasilkan/mengekstrak *public key* (`flask.pub`) berdasarkan *private key* (`flask.pem`) yang sudah ada.*
    * `-y` : Membaca file private key dan mencetak public key-nya.
    * `-f` : Menentukan lokasi/nama file private key yang akan dibaca.

### Cara Melakukan SSH ke Instance
```bash
ssh -i flask.pem ubuntu@<IP_ADDRESS>

```

*(Catatan: `<IP_ADDRESS>` bisa diisi dengan Public IP jika diakses dari luar jaringan AWS, atau Private IP jika diakses dari dalam jaringan/VPC yang sama. `ubuntu` adalah default username untuk OS Ubuntu).*

---

## 2. Konfigurasi IP Address

Berikut adalah daftar IP yang digunakan dalam environment ini:

* **IP Amazon (untuk SSH)**: `16.176.170.94` *(Elastic IP / Public IP spesifik)*
* **Public IP EC2**: `52.63.74.223` *(IP yang bisa diakses dari internet)*
* **Private IP EC2**: `172.31.13.190` *(IP internal di dalam VPC AWS)*
* **IP Komputer Lokal (User)**: `103.47.134.76`

---

## 3. Pengaturan Jaringan & Firewall

### Subnet (Jaringan Internal)

* **CIDR Block**: `172.31.0.0/20` (Berada di IP Class B)
* **Kapasitas Host**: Menyediakan hingga 4091 alamat IP yang tersedia untuk *resource* AWS di dalam subnet ini.

### Security Group (Firewall)

Pengaturan *Inbound Rules* (aturan masuk) untuk membatasi akses ke server:

* **Tipe**: SSH
* **Protokol**: TCP
* **Rentang Port**: 22 (Port default SSH)
* **Tipe Sumber**: Custom
* **Sumber (Source)**: `192.168.20.0/24`
*(Hanya mengizinkan akses masuk dari rentang IP Class C ini yang memiliki total 254 host).*

---

## 4. Sistem File & Penyimpanan (S3)

**Amazon S3 (Simple Storage Service)** adalah layanan *Object Storage*, yaitu tempat menyimpan data dalam bentuk objek (seperti file, gambar, video, atau *backup*) di dalam sebuah wadah logis yang disebut **Bucket**. Data di S3 dapat diakses kapan saja melalui API atau URL.

### Detail Bucket

* **Nama Bucket**: `s3://flaskbuckets01`
*(Nama bucket bersifat unik secara global di seluruh AWS dan memiliki kapasitas penyimpanan yang hampir tak terbatas).*

### Konfigurasi Sistem File S3

Integrasi S3 sebagai sistem file *(file system)*:

* **File System ID**: `fs-01dec2fe4e2358e5f`
* **Tags**: `Key: flask`, `Value: fs-flaskbuckets01-1776154684741`
* **ARN (Amazon Resource Name)**: `arn:aws:s3files:ap-southeast-2:171643300383:file-system/fs-01dec2fe4e2358e5f`
*(ARN berfungsi sebagai pengidentifikasi unik berstandar AWS untuk melacak atau memanggil sumber daya/resource secara spesifik di seluruh ekosistem cloud).*

---

## 5. Endpoint Layanan

* **EC2 Instance Connect Endpoint (EICE)**: `eice-0e37708774190f429`
*(Digunakan untuk terhubung ke instance melalui SSH menggunakan private IP tanpa perlu mengekspos instance ke public internet).*

```

***

```
