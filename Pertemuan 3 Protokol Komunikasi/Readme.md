# Percobaan 3A: Komunikasi Data Menggunakan HTTP

#  Dokumentasi

![Uploading WhatsApp Image 2026-09-15 at 13.45.04.jpeg…]()


## 1. Detail Percobaan Singkat
Percobaan ini bertujuan untuk mengimplementasikan pengiriman data sensor (dummy) dari ESP32 ke sebuah server pengujian (`httpbin.org/post`) menggunakan protokol HTTP dengan metode POST. Data yang dikirimkan diformat menggunakan JSON. Modifikasi dilakukan dengan menambahkan data `waktu_nyala_ms` menggunakan fungsi `millis()`.

## 2. Library (Dependencies)
* `WiFi.h`: Untuk menghubungkan ESP32 ke jaringan WiFi.
* `HTTPClient.h`: Untuk membuat dan mengelola request HTTP (GET, POST, dll).
* `ArduinoJson.h`: (Oleh Benoit Blanchon) Untuk membuat, memanipulasi, dan menyerialisasi data ke dalam format JSON.

## 3. Penjelasan Code & Fungsi
* `setup()`: Menginisialisasi komunikasi Serial dan menghubungkan ESP32 ke jaringan WiFi.
* `loop()`: Fungsi utama yang berjalan berulang. Mengecek koneksi WiFi, membuat format data JSON, dan mengirimkannya via HTTP POST setiap 10 detik.
* `http.begin(serverUrl)`: Membuka inisiasi koneksi ke URL tujuan.
* `http.addHeader(...)`: Menyiapkan *header* paket data.
* `serializeJson(doc, requestBody)`: Mengonversi struktur objek JSON (`doc`) menjadi format teks murni (`requestBody`) agar bisa ditransmisikan.
* `http.POST(requestBody)`: Mengeksekusi pengiriman data ke server dan mengembalikan nilai kode status (*response code*).
* `millis()`: Fungsi bawaan mikrokontroler untuk menghitung waktu (dalam milidetik) sejak board dinyalakan.

## 4. Penjelasan Percabangan / Conditional
* `while (WiFi.status() != WL_CONNECTED)`: Menahan program (looping) selama WiFi belum terhubung, ditandai dengan print ".".
* `if (WiFi.status() == WL_CONNECTED)`: Memastikan bahwa request HTTP hanya dijalankan JIKA ESP32 terhubung ke internet.
* `if (httpResponseCode > 0)`: Mengecek apakah server memberikan respon yang valid. Jika `> 0` berarti ada balasan dari server (berhasil), jika `< 0` biasanya menandakan kegagalan jaringan atau timeout.

## 5. Jawaban Pertanyaan Praktikum (3.5.4)


Berikut adalah potongan kode pada fungsi `loop()` yang telah ditambahkan fitur kalkulasi waktu menggunakan fungsi `millis()` sebelum diserialisasi ke dalam format JSON:

  ```cpp
        // 1. Membuat objek data dalam format JSON
        JsonDocument doc;
        doc["suhu"] = 28.5;         // Data suhu dummy (°C)
        doc["kelembaban"] = 65.0;   // Data kelembaban dummy (%)
        
        // --- MODIFIKASI: Penambahan Data Waktu (millis) ---
        unsigned long waktuMilis = millis(); 
        doc["waktu_nyala_ms"] = waktuMilis; 
        // ---------------------------------------------------
        
        // 2. Mengubah objek JSON menjadi String
        String requestBody;
        serializeJson(doc, requestBody);

```

# Percobaan 3B: Komunikasi MQTT
## 7. Dokumentasi
<img width="1531" height="320" alt="WhatsApp Image 2026-09-15 at 13 56 25" src="https://github.com/user-attachments/assets/6093f12e-de74-4d7c-a17c-ffe5e397dd56" />



## 1. Detail Percobaan Singkat
Percobaan ini mengimplementasikan protokol MQTT (*Message Queuing Telemetry Transport*) untuk mengirimkan data secara *publish-subscribe*. ESP32 memublikasikan data (suhu dan kelembaban dalam format JSON) ke broker publik `broker.hivemq.com` melalui sebuah *topic* spesifik setiap 5 detik.

## 2. Library (Dependencies)
* `WiFi.h`: Mengelola koneksi jaringan nirkabel.
* `PubSubClient.h`: (Oleh Nick O'Leary) Digunakan untuk membuat ESP32 bertindak sebagai MQTT Client, memungkinkan proses publish dan subscribe ke broker.
* `ArduinoJson.h`: Digunakan untuk membuat struktur data JSON.

## 3. Penjelasan Code & Fungsi
* `hubungkanWiFi()`: Fungsi kustom untuk menghubungkan board ke akses internet.
* `hubungkanMQTT()`: Fungsi kustom untuk melakukan *handshake* dengan broker MQTT. Jika gagal, akan diulang (looping) hingga berhasil terhubung.
* `client.setServer(...)`: Mengatur alamat IP/Domain broker MQTT beserta port-nya (1883).
* `client.publish(topic, payload)`: Fungsi untuk menembakkan (mengirim) data `payload` ke jalur komunikasi `topic` yang telah ditentukan di broker.

## 4. Penjelasan Percabangan / Conditional
* `if (client.connect(clientId.c_str()))`: Mencoba koneksi ke broker. Percabangan `if-else` ini digunakan untuk menampilkan pesan "berhasil" jika koneksi terjalin, atau pesan "gagal beserta kode error" jika ditolak broker.
* `if (!client.connected())`: Berada di dalam blok `loop()`. Kondisi ini berfungsi sebagai *Auto-Reconnect*. Jika di tengah jalan WiFi atau koneksi broker terputus, maka program akan masuk ke fungsi `hubungkanMQTT()` lagi.

## 5. Jawaban Pertanyaan Praktikum (3.6.4)
1. **Fungsi Topic & Alasan Dibuat Unik:** *Topic* berfungsi sebagai "kanal" atau alamat spesifik pengelompokan pesan di dalam broker MQTT. Harus dibuat unik (misal ditambah ID kelompok) karena kita menggunakan *Public Broker*. Jika topic tidak unik, data kita akan bercampur dengan data orang lain yang kebetulan menggunakan nama *topic* yang sama di seluruh dunia.
2. **Fungsi perintah `client.loop()`:** Perintah ini wajib dipanggil di dalam fungsi `loop()` Arduino. Fungsinya adalah untuk menjaga koneksi (*keep-alive*) dengan broker, memproses pesan masuk (jika kita melakukan *subscribe*), dan membersihkan *buffer* jaringan. Tanpa ini, koneksi MQTT akan terputus seketika (*timeout*).
3. **Jika Koneksi Terputus:** Berkat blok percabangan `if (!client.connected())` di dalam `loop()`, ESP32 akan menyadari koneksi terputus dan tidak akan mencoba melakukan `client.publish`. Sebaliknya, program akan memanggil `hubungkanMQTT()` dan menahan program dalam fungsi tersebut sampai koneksi berhasil dipulihkan.

## 6. Jawaban Pertanyaan Analisis (3.7)
1. **Uraian Hasil:** ESP32 berhasil berfungsi sebagai klien pada protokol HTTP maupun MQTT. Pada HTTP, komunikasi dilakukan sekali tembak (*request-response*) ke httpbin. Pada MQTT, koneksi dipertahankan (*persistent*) dan data berhasil dikirim serta dicegat (dilihat) secara *real-time* menggunakan aplikasi pihak ketiga (seperti MQTT Explorer) melalui metode Subscribe.
2. **Perbandingan Overhead:** Protokol HTTP memiliki *overhead* yang sangat besar karena setiap kali mengirim data harus membuka koneksi TCP baru, mengirim *header metadata* yang panjang, lalu menutup koneksi. MQTT memiliki *overhead* yang sangat kecil (header hanya 2 bytes) karena menggunakan koneksi TCP yang selalu terbuka (*persistent*).
3. **Skenario Pengiriman Terus-Menerus:** Untuk jangka panjang dan pengiriman reguler (misal tiap detik), **MQTT** jauh lebih sesuai. Alasannya karena MQTT menghemat *bandwidth* dan baterai perangkat. Koneksi tidak perlu dibongkar-pasang setiap kali mau mengirim data seperti halnya HTTP.
4. **Peran Format JSON:** JSON membuat pertukaran data (interoperabilitas) sangat mudah lintas platform. Karena JSON berbasis teks (*key-value*), data yang dikirim oleh ESP32 (C++) dapat langsung dan mudah diuraikan (*parsing*) oleh *dashboard* web (JavaScript), backend server (Python/PHP), atau aplikasi Android tanpa perlu mengubah struktur datanya.







