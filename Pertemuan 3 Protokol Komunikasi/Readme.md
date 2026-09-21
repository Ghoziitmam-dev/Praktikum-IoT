# Percobaan 3A: Komunikasi Data Menggunakan HTTP

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

# 6. Dokumentasi
*(Ganti teks di bawah ini dengan link/foto praktikum Anda)*
* **Skematik:** *[Hanya menggunakan ESP32 langsung colok USB]*
