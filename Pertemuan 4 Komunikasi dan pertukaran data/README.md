# Praktikum IoT Modul 4 - Komunikasi dan Pertukaran Data (Dua Arah / Full Duplex)

Repositori ini berisi dokumentasi dan source code untuk Modul 4: Komunikasi Pertukaran Data Dua Arah (Publish dan Subscribe) menggunakan protokol MQTT pada ESP8266.

## 1. Penjelasan Singkat Mengenai Detail Percobaan
Pada praktikum ini, diimplementasikan sistem IoT yang memiliki kemampuan komunikasi dua arah (bidirectional) menggunakan protokol MQTT:
*   **Percobaan 4A (Subscribe & Deserialisasi JSON):** Mengimplementasikan ESP8266 sebagai *subscriber* yang menerima perintah dalam format JSON dari broker MQTT. Pesan JSON dideserialisasi untuk menghidupkan/mematikan LED dan mengatur intensitas cahayanya (PWM).
*   **Percobaan 4B (Full Duplex - Publish & Subscribe):** Menggabungkan komunikasi satu arah (publish data suhu dari sensor DHT11) dengan komunikasi arah sebaliknya (subscribe perintah kontrol LED dan Buzzer) secara bersamaan. Pendekatan **non-blocking** menggunakan `millis()` digunakan sebagai pengganti `delay()` agar penerimaan pesan dan pengiriman data tidak saling mengganggu.

## 2. Library atau Dependencies yang Diperlukan
Untuk menjalankan program pada ESP8266 ini, diperlukan beberapa *library* berikut yang harus diinstal melalui Library Manager di Arduino IDE:
1.  **ESP8266WiFi** (Bawaan board manager ESP8266): Untuk menghubungkan mikrokontroler ke jaringan WiFi.
2.  **PubSubClient** (oleh Nick O'Leary): Untuk mengimplementasikan klien MQTT (melakukan *publish* dan *subscribe*).
3.  **ArduinoJson** (oleh Benoit Blanchon): Untuk melakukan serialisasi (membuat) dan deserialisasi (memecah/parsing) pesan dengan format JSON.
4.  **DHT sensor library** (oleh Adafruit): Untuk membaca data suhu dan kelembaban dari sensor DHT11.

## 3. Skematik/Diagram Rangkaian
*   **ESP8266 & DHT11:**
    *   VCC DHT11 -> 3V3 ESP8266
    *   GND DHT11 -> GND ESP8266
    *   DATA DHT11 -> Pin D2 (GPIO 4) ESP8266
*   **ESP8266 & LED:**
    *   Kaki Anoda LED -> Resistor 220 Ohm -> Pin D0 (GPIO 26) ESP8266
    *   Kaki Katoda LED -> GND ESP8266
*   **ESP8266 & Buzzer (Tugas 4.6.4):**
    *   VCC Buzzer -> Pin D1 (GPIO 5) ESP8266
    *   GND Buzzer -> GND ESP8266

## 4. Penjelasan Code (Termasuk Modifikasi Tugas)

### Modifikasi Percobaan 4A (Tambahan Intensitas PWM)
Kode dimodifikasi untuk menjawab pertanyaan 4.5.4 poin 4, yaitu menerima JSON `{"perintah": "ON", "intensitas": 200}` untuk mengatur kecerahan LED menggunakan `analogWrite()`. Jika atribut "intensitas" kosong, maka nilai *default* `255` akan digunakan.

### Modifikasi Percobaan 4B (Aktuator Tambahan/Buzzer)
Kode dimodifikasi untuk menjawab pertanyaan 4.6.4 poin 4, dengan menambahkan 2 topik subscribe untuk membedakan LED dan Buzzer. Di dalam fungsi `callback`, terdapat pengecekan parameter `topic` menggunakan *if-else* agar ESP8266 mengetahui perangkat mana yang harus diaktifkan/dimatikan berdasarkan alamat pesannya.

## 5. Penjelasan Setiap Fungsi
1.  **`setup()`**: Fungsi yang dieksekusi sekali saat awal program. Digunakan untuk inisialisasi baud rate Serial, mengatur pin (INPUT/OUTPUT), memulai pembacaan sensor DHT, menghubungkan WiFi, mengatur server broker MQTT, dan mendaftarkan fungsi `callback`.
2.  **`loop()`**: Fungsi utama yang berjalan berulang-ulang secara terus-menerus. Di dalamnya terdapat pengecekan koneksi MQTT, pemanggilan `client.loop()`, dan eksekusi pembacaan serta *publish* data secara periodik dengan *non-blocking timer* `millis()`.
3.  **`hubungkanWiFi()`**: Fungsi untuk menghubungkan mikrokontroler dengan access point/WiFi.
4.  **`hubungkanMQTT()`**: Fungsi untuk menyambungkan ulang (reconnect) *client* ESP8266 ke broker MQTT jika terputus, dan **secara krusial** melakukan *subscribe* ulang ke topik-topik perintah yang dibutuhkan.
5.  **`callback(char* topic, byte* payload, unsigned int length)`**: Fungsi *event-driven* yang dipanggil secara otomatis oleh pustaka PubSubClient setiap kali ada pesan masuk pada topik yang telah disubscribe. Fungsi ini merakit *payload byte* menjadi *String*, mendeserialisasinya, dan menjalankan aksi aktuator.

## 6. Penjelasan Percabangan/Conditional
*   **`if (error)` atau `if (deserializeJson(doc, pesan))`**: Mengecek apakah data string yang diterima berhasil di-parsing menjadi objek JSON. Jika gagal/corrupt, blok percabangan menghentikan eksekusi (dengan `return`) agar program tidak crash.
*   **`if (String(perintah) == "ON") { ... } else if (...)`**: Mengecek isi dari *key* `"perintah"` untuk menentukan status aktuator (HIGH untuk menyalakan, LOW untuk mematikan).
*   **`if (String(topic) == topicPerintahLed)` (Pada Modifikasi 4B)**: Karena callback menerima pesan dari semua topik yang disubscribe, percabangan ini digunakan untuk memilah pesan masuk berdasarkan topiknya, sehingga perintah aktuator (LED vs Buzzer) tepat sasaran.
*   **`if (millis() - waktuTerakhirPublish > intervalPublish)`**: Logika *non-blocking delay*. Mengecek apakah waktu berjalan (`millis()`) dikurangi waktu terakhir publish sudah melebihi 5000 ms. Jika ya, lakukan publish; jika tidak, lewati (sehingga proses `client.loop()` di bawahnya tetap berjalan tanpa henti).

## 7. Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code

### Pertanyaan 4.5.4 (Subscribe & Deserialisasi)
1.  **Gambarkan diagram alur (flowchart) proses penerimaan pesan pada fungsi callback!**
    *   *(Secara deskriptif)*: Pesan masuk dari Broker -> Konversi array `byte` ke `String` -> Deserialisasi `String` menjadi objek `JsonDocument` -> Apakah format JSON valid? -> Jika Error: cetak pesan error dan Return (berhenti) -> Jika Valid: ambil data dari key `"perintah"` -> Apakah nilainya "ON"? -> Jika ya, aktuator HIGH. Jika tidak (OFF), aktuator LOW -> Selesai.
2.  **Apa yang akan terjadi apabila pesan yang dipublikasikan bukan merupakan format JSON yang valid?**
    *   Fungsi `deserializeJson()` akan gagal dan mendeteksi adanya error. Karena program telah dilengkapi blok kondisional `if (error)`, maka program tidak akan *crash/freeze*. ESP8266 hanya akan mencetak pesan gagal di Serial Monitor dan keluar dari fungsi callback.
3.  **Mengapa fungsi `client.subscribe()` dipanggil di dalam fungsi `hubungkanMQTT()`, bukan di dalam `setup()`?**
    *   Untuk mengamankan fitur *auto-reconnect*. Jika koneksi WiFi atau MQTT terputus, sesi *subscribe* pada broker akan hilang. Dengan meletakkannya di `hubungkanMQTT()`, ESP8266 secara otomatis akan mendaftar (subscribe) kembali ke topik terkait setiap kali ia berhasil menyambung ulang ke broker.

### Pertanyaan 4.6.4 (Pertukaran Data Dua Arah)
1.  **Mengapa penggunaan delay() yang lama sebaiknya dihindari pada program yang menggabungkan proses publish dan subscribe secara bersamaan?**
    *   Fungsi `delay()` bersifat *blocking*, yang berarti memberhentikan seluruh proses mikrokontroler. Jika terjadi jeda lama, fungsi `client.loop()` tidak akan terpanggil. Akibatnya, perintah *subscribe* bisa terlewat, dan broker dapat memutus koneksi karena klien dianggap terputus (Ping Timeout).
2.  **Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi `millis()` pada program di atas!**
    *   Perangkat merekam waktu berjalannya program (`millis()`). Di dalam `loop()`, mikrokontroler secara konstan menghitung selisih antara waktu saat ini dengan catatan waktu terakhir dikirim (`waktuTerakhirPublish`). Hanya ketika selisihnya lebih dari interval (5 detik), blok pengiriman data dieksekusi. Karena hanya menggunakan operasi pengurangan matematika sederhana, proses *subscribe* dapat terus "mendengarkan" pesan masuk tanpa hambatan.
3.  **Apa yang akan terjadi apabila fungsi `client.loop()` jarang dipanggil?**
    *   ESP8266 tidak akan responsif terhadap perintah kontrol yang masuk (*lagging*), pesan yang masuk bisa ditolak oleh *buffer*, dan berujung pada pemutusan koneksi otomatis dari pihak broker.

## 8. Foto Proses Praktikum / Perangkaian

<img width="900" height="1600" alt="WhatsApp Image 2026-09-29 at 13 58 38" src="https://github.com/user-attachments/assets/2b5c9b32-1793-44bd-828f-bf2396282be3" />
