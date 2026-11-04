
# Praktikum IoT Modul 2 - Konfigurasi Jaringan (WiFi Mode Station & Access Point)

Repositori ini berisi dokumentasi dan *source code* untuk Modul 2, yang mempelajari dasar-dasar konfigurasi jaringan nirkabel pada ESP32, termasuk mengatur peran mikrokontroler sebagai *Client* (Station), penyedia jaringan (Access Point), maupun keduanya secara bersamaan (AP+STA)[cite: 18, 19].

## 1. Penjelasan Singkat Mengenai Detail Percobaan
Konfigurasi jaringan adalah langkah esensial dalam sistem IoT agar perangkat dapat saling bertukar data[cite: 18]. Percobaan ini terbagi menjadi:
*   **Percobaan 2A (Station/STA):** ESP32 diposisikan sebagai klien yang terhubung ke *router* atau *hotspot* yang sudah ada untuk mendapatkan akses ke jaringan lokal atau internet[cite: 18, 20]. Program juga mendeteksi IP address, MAC Address, dan kekuatan sinyal (RSSI)[cite: 19].
*   **Percobaan 2B (Access Point/AP):** ESP32 dikonfigurasi sebagai penyedia jaringan (*hotspot* mandiri) tanpa perlu router eksternal, sehingga *smartphone* atau *laptop* pengguna dapat langsung terhubung kepadanya[cite: 18, 22].

## 2. Library atau Dependencies yang Diperlukan
*   **`WiFi.h`**: Pustaka standar bawaan *board manager* ESP32 pada Arduino IDE yang menyediakan seluruh fungsi untuk mengatur parameter jaringan nirkabel (menghubungkan perangkat, membuat AP, dan membaca status jaringan)[cite: 18, 19].

## 3. Skematik/Diagram Rangkaian
*   **ESP32 terhubung via Kabel USB** ke Laptop/PC untuk keperluan program dan *Serial Monitoring*[cite: 19].
*   **LED Indikator (Opsional):** 
    *   Kaki Anoda LED -> Resistor 220 Ohm -> Pin GPIO 2 ESP32[cite: 20, 21].
    *   Kaki Katoda LED -> Pin GND ESP32.

## 4. Penjelasan Code & Modifikasi Tugas Pendahuluan

### Modifikasi Percobaan 2A (Auto-Reconnect)
Berdasarkan pertanyaan praktikum (Tugas 2.5.4 poin 4), program ditambahkan logika pengecekan di dalam blok `loop()`. Apabila `WiFi.status()` tidak lagi bernilai `WL_CONNECTED` (terputus), program akan mematikan LED (LOW), mengeksekusi `WiFi.disconnect()` untuk mereset *state*, dan kembali memanggil `WiFi.begin(ssid, password)` dalam struktur `while` sampai koneksi berhasil tersambung ulang.

### Modifikasi Percobaan 2B (Mode AP+STA)
Berdasarkan pertanyaan praktikum (Tugas 2.6.4 poin 4), program diubah dari sekadar mode AP menjadi mode kombinasi. Kode `WiFi.mode(WIFI_AP)` diubah menjadi `WiFi.mode(WIFI_AP_STA)`[cite: 19]. Di dalam blok `setup()`, terdapat dua tahapan inisialisasi: pertama mengeksekusi `WiFi.softAP()` untuk memancarkan jaringan buatan ESP32, dan yang kedua mengeksekusi `WiFi.begin()` untuk menyambungkan ESP32 tersebut ke jaringan *router* rumah. Keduanya berjalan serentak.

## 5. Penjelasan Setiap Fungsi
*   **`WiFi.mode()`**: Mengatur mode operasi perangkat, bisa berupa `WIFI_STA` (Station), `WIFI_AP` (Access Point), atau `WIFI_AP_STA` (Gabungan)[cite: 19, 20].
*   **`WiFi.begin(ssid, password)`**: Mengirim permintaan ( *request*) koneksi ke *router* atau *access point* luar[cite: 19].
*   **`WiFi.softAP(ssid, password)`**: Mengaktifkan ESP32 sebagai pemancar jaringan (*hotspot*) dengan nama (SSID) dan kata sandi yang telah ditentukan[cite: 19, 22].
*   **`WiFi.localIP()`**: Mengambil dan menampilkan IP Address yang didapatkan ESP32 dari *router* luar pada mode Station[cite: 19].
*   **`WiFi.softAPIP()`**: Menampilkan alamat IP statis dari Access Point yang baru saja dibuat oleh ESP32[cite: 19].
*   **`WiFi.softAPgetStationNum()`**: Membaca jumlah total perangkat/klien (*smartphone*/*laptop*) yang sedang terhubung ke sinyal AP buatan ESP32 saat itu[cite: 22].

## 6. Penjelasan Percabangan/Conditional
*   **`while (WiFi.status() != WL_CONNECTED)`**: Blok perulangan ini menahan laju eksekusi kode (bersifat *blocking*) dengan mencetak titik (`.`) secara terus menerus ke Serial Monitor. Program hanya akan keluar dari blok ini dan melanjutkan eksekusi ke baris berikutnya jika ESP32 telah sukses terkoneksi ke jaringan[cite: 20].
*   **`if (WiFi.status() == WL_CONNECTED) { ... } else { ... }`**: Struktur percabangan di dalam `loop()` yang secara konstan memantau kesehatan koneksi setiap 5 detik. Jika statusnya terhubung, ESP32 mencetak "Status: Terhubung", dan jika masuk ke blok `else`, status terputus dicetak dan memicu prosedur *reconnect*[cite: 21].

## 7. Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code

### Pertanyaan 2.5.4 (Mode Station - STA)
1.  **Gambarkan diagram alur (flowchart) proses koneksi ESP32 ke jaringan WiFi!**
    *   (Mulai) -> Inisialisasi Serial dan Pin LED -> Set Mode WiFi ke `WIFI_STA` -> Panggil `WiFi.begin()` -> Apakah status koneksi = terhubung? -> Jika Tidak: Tunggu 500ms, cetak titik, periksa status lagi -> Jika Ya: Cetak IP Address & MAC, Nyalakan LED -> Masuk ke proses pengecekan konstan di `loop()` -> (Selesai).
2.  **Apa fungsi perintah `WiFi.mode(WIFI_STA)`?**
    *   Perintah ini digunakan untuk memaksa *chip* WiFi pada ESP32 agar bertindak secara eksklusif sebagai klien (Station)[cite: 18, 20]. Mode ini membuat ESP32 berfokus mencari dan menyambung ke jaringan eksternal (seperti laptop biasa) tanpa memancarkan sinyal *hotspot* buatan.
3.  **Apa yang terjadi jika SSID atau Password salah?**
    *   Fungsi `WiFi.begin()` akan gagal menyelesaikan prosedur *handshake*. Akibatnya, nilai `WiFi.status()` tidak akan pernah menjadi `WL_CONNECTED`. Program akan terjebak secara permanen di dalam *looping* `while`, dan Serial Monitor akan terus-menerus mencetak karakter titik (`....`) tanpa batas waktu, dan baris kode menyalakan LED tidak akan pernah tereksekusi.

### Pertanyaan 2.6.4 (Mode Access Point - AP)
1.  **Mengapa alamat IP default Access Point pada ESP32 umumnya bernilai 192.168.4.1?**
    *   Nilai `192.168.4.1`[cite: 23] merupakan pengaturan bawaan *default subnet* yang telah diprogram oleh pabrikan Espressif di dalam inti *firmware* (ESP-IDF) khusus untuk antarmuka Soft-AP. Hal ini ditujukan untuk meminimalisasi terjadinya konflik alamat IP, karena router rumah pada umumnya sering menggunakan *subnet* `192.168.1.1` atau `192.168.0.1`.
2.  **Perbedaan mendasar mode Station dan Access Point?**
    *   Pada **Mode Station**, ESP32 berperan sebagai klien (penerima) yang meminta akses masuk ke jaringan yang sudah tersedia[cite: 18]. Pada **Mode Access Point**, ESP32 berperan sebagai router/penyedia jaringan itu sendiri, memancarkan sinyal WiFi (*hotspot*) yang memungkinkan klien lain untuk meminta akses dan menumpang masuk ke jaringannya[cite: 18].
3.  **Risiko keamanan jika password Access Point tidak diberikan/terlalu sederhana?**
    *   Jaringan ESP32 dapat disusupi (*unauthorized access*). Pihak luar dapat terhubung dengan bebas ke mikrokontroler, mengeksploitasi celah program, melakukan pemalsuan data (*spoofing*), atau mengirim beban *request* tinggi (DDoS) yang menyebabkan ESP32 menjadi *overheat* dan mati (*crash*).

## 8. Foto Proses Praktikum / Perangkaian

<img width="685" height="293" alt="WhatsApp Image 2026-09-08 at 14 31 44" src="https://github.com/user-attachments/assets/6ea43e11-8cf9-4779-b29f-d825bf005b91" />
