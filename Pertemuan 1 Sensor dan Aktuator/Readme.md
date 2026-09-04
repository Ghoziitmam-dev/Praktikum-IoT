## **Pertanyaan praktikum percobaan 1A**
Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali 
pembacaan sebelum ditampilkan

          #include <DHT.h>
          #define DHTPIN 4
          #define DHTTYPE DHT22
          
          DHT dht(DHTPIN, DHTTYPE);
          
          void setup() {
            Serial.begin(115200);
            dht.begin();
            Serial.println("Memulai akuisisi data rata-rata (menunggu 10 detik)...");
          }
          
          void loop() {
            float totalSuhu = 0;
            float totalKelembaban = 0;
            int bacaanValid = 0; // Menghitung berapa kali pembacaan berhasil
          
            // Melakukan 5 kali pengambilan sampel
            for (int i = 0; i < 5; i++) {
              float suhu = dht.readTemperature();
              float kelembaban = dht.readHumidity();
          
              // Memastikan hanya data yang valid yang dijumlahkan
              if (!isnan(suhu) && !isnan(kelembaban)) {
                totalSuhu += suhu;
                totalKelembaban += kelembaban;
                bacaanValid++;
              } else {
                Serial.println("Gagal 1 sampel, melanjutkan...");
              }
              
              delay(2000); // Tetap butuh jeda perangkat keras 2 detik per sampel
            }
          
            // Menghitung dan menampilkan rata-rata jika ada data valid
            if (bacaanValid > 0) {
              float rataSuhu = totalSuhu / bacaanValid;
              float rataKelembaban = totalKelembaban / bacaanValid;
              
              Serial.print("Rata-rata Suhu: ");
              Serial.print(rataSuhu);
              Serial.print(" °C, Rata-rata Kelembaban: ");
              Serial.print(rataKelembaban);
              Serial.println("%");
            } else {
              Serial.println("Gagal membaca 5 data berturut-turut!");
            }
          }
<img width="1280" height="720" alt="WhatsApp Image 2026-09-03 at 16 16 31" src="https://github.com/user-attachments/assets/7162e393-b8b3-4975-9339-719430b4db81" />

## **Pertanyaan Praktikum 2A**
Modifikasi program agar menggunakan dua ambang batas (histerisis), misalnya aktuator 
menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah 28°C, dan berikan 
penjelasan di setiap baris kode nya dalam bentuk README.md!

          #include <DHT.h>
          #define DHTPIN 4
          #define DHTTYPE DHT22
          #define RELAYPIN 26
          
          DHT dht(DHTPIN, DHTTYPE);
          
          // Mendefinisikan dua ambang batas untuk histerisis
          const float suhuBatasAtas = 30.0;
          const float suhuBatasBawah = 28.0;
          
          void setup() {
            Serial.begin(115200);
            dht.begin();
            pinMode(RELAYPIN, OUTPUT);
            digitalWrite(RELAYPIN, LOW); // pastikan aktuator mati di awal
          }
          
          void loop() {
            float suhu = dht.readTemperature();
          
            if (isnan(suhu)) {
              Serial.println("Gagal membaca data sensor!");
            } else {
              Serial.print("Suhu: ");
              Serial.print(suhu);
              Serial.print(" °C -> ");
          
              // Kendali aktuator menggunakan Histerisis
              if (suhu > suhuBatasAtas) {
                digitalWrite(RELAYPIN, HIGH); // aktifkan relay
                Serial.println("Aktuator: ON");
              } 
              else if (suhu < suhuBatasBawah) {
                digitalWrite(RELAYPIN, LOW); // matikan relay
                Serial.println("Aktuator: OFF");
              } 
              else {
                // Kondisi suhu di antara batas bawah dan batas atas (28 - 30)
                // Relay mempertahankan status terakhirnya (tidak ada perubahan)
                Serial.println("Aktuator: Mempertahankan Status (Stabil)");
              }
            }
            delay(2000);
          }

<img width="900" height="1600" alt="WhatsApp Image 2026-09-01 at 22 40 12" src="https://github.com/user-attachments/assets/a99e7497-9964-43ed-9608-33cc979c1d42" />

## 1. Penjelasan Singkat Mengenai Detail Percobaan
Praktikum ini bertujuan untuk membangun dan mengimplementasikan sistem Internet of Things (IoT) dasar melalui dua tahapan percobaan utama:
* **Percobaan 1A (Akuisisi Data Sensor):** Mikrokontroler ESP32 diinstruksikan untuk membaca data lingkungan fisik, yakni suhu dan kelembaban, melalui sensor digital DHT22[cite: 1]. Hasil pembacaan ini kemudian diproses dan ditampilkan melalui layar Serial Monitor secara berkala setiap dua detik[cite: 1].
* **Percobaan 2A (Kendali Aktuator):** Mengembangkan sistem akuisisi sebelumnya menjadi otomasi terpadu[cite: 1]. ESP32 tidak hanya membaca data sensor, tetapi juga menggunakannya sebagai acuan untuk mengendalikan aktuator mekanis berupa relay (yang disimulasikan dengan LED)[cite: 1]. Aktuator ini diprogram untuk menyala dan mati secara otomatis berdasarkan parameter suhu ambang batas yang telah ditetapkan[cite: 1].

## 2. Library atau Dependencies yang Diperlukan
Untuk menjalankan seluruh kode program (source code) pada praktikum ini, diperlukan instalasi beberapa komponen pada Arduino IDE:
* **Board Manager ESP32:** Modul inti yang wajib diinstal agar perangkat lunak Arduino IDE mengenali, mengkompilasi, dan mengunggah kode program ke dalam board mikrokontroler ESP32 DevKit[cite: 1].
* **DHT Sensor Library (`DHT.h`):** Pustaka eksternal yang diwajibkan untuk menjembatani protokol komunikasi data antara mikrokontroler dengan sensor DHT11 maupun DHT22[cite: 1].

## 3. Penjelasan Setiap Fungsi
Program ini memanfaatkan fungsi-fungsi esensial, baik bawaan Arduino maupun dari library pihak ketiga:
* `Serial.begin(115200)`: Berfungsi membuka jalur komunikasi serial dengan komputer pada *baud rate* 115200 agar mikrokontroler dapat mengirim teks dan angka ke antarmuka Serial Monitor[cite: 1].
* `dht.begin()`: Menginisialisasi koneksi perangkat keras (*hardware*) sensor DHT22 agar bersiap melakukan proses pengambilan sampel data lingkungan[cite: 1].
* `pinMode(RELAYPIN, OUTPUT)`: Mendeklarasikan pin GPIO (dalam hal ini GPIO 26) sebagai jalur transmisi sinyal keluar (output) yang menuju ke rangkaian relay aktuator[cite: 1].
* `dht.readTemperature()`: Mengeksekusi pembacaan dari library DHT untuk mengambil nilai besaran suhu murni dalam satuan derajat Celsius[cite: 1].
* `dht.readHumidity()`: Mengeksekusi pembacaan nilai persentase (%) kelembaban relatif (RH) dari udara sekitar[cite: 1].
* `isnan(nilai)`: Fungsi diagnostik yang bertugas menginspeksi hasil pembacaan numerik untuk memverifikasi apakah variabel tersebut valid atau mengalami *error/Not a Number*[cite: 1].
* `digitalWrite(pin, HIGH / LOW)`: Mengirimkan perintah tegangan secara digital ke pin relay. `HIGH` untuk mengalirkan arus (menyalakan relay) dan `LOW` untuk memutus arus (mematikan relay)[cite: 1].
* `delay(2000)`: Menahan proses berjalannya eksekusi program (jeda) selama 2000 milidetik (2 detik) sebelum mengulang baris kode berikutnya[cite: 1].

## 4. Penjelasan Percabangan / Conditional
Logika bersyarat digunakan secara intensif untuk menavigasi alur program bergantung pada status perangkat keras:
* **Kondisi Validasi (Percobaan 1A & 2A):** Program menggunakan struktur `if (isnan(kelembaban) || isnan(suhu))` untuk menyaring data cacat[cite: 1]. Jika fungsi pembacaan mengembalikan sinyal NaN (gagal), blok kode ini akan mencegat proses dan memunculkan peringatan "Gagal membaca data dari sensor DHT22!" ke pengguna[cite: 1].
* **Kondisi Kendali Aktuator Tunggal (Percobaan 2A):** Struktur `if (suhu > suhuThreshold)` bertindak sebagai gerbang logika. Jika variabel suhu memuat angka yang lebih besar dari batas parameter *threshold* (30.0°C), program masuk ke kondisi *true* dan memanggil instruksi `HIGH` pada relay[cite: 1]. Jika suhu lebih rendah atau sama, program mengeksekusi blok `else` dan mempertahankan sinyal `LOW`[cite: 1].
* **Kondisi Kendali Histerisis (Modifikasi 2A):** Penggunaan rantai kondisional `if` (batas atas) dan `else if` (batas bawah). Jika suhu berada tepat di antara dua kondisi tersebut, blok evaluasi dilewati, dan mikrokontroler mempertahankan *state* aktuator terakhir (tidak ada instruksi `digitalWrite` yang dieksekusi).

## 5. Penjelasan Code (Modifikasi Khusus Praktikum)
* **Kode Modifikasi Akuisisi (Rata-rata 5 Sampel):** Bagian kode ini mengandalkan blok pengulangan `for (int i = 0; i < 5; i++)`. ESP32 dijebak dalam siklus baca-tahan (*read-delay*) sebanyak 5 kali. Setiap kali fungsi `!isnan()` menyatakan datanya sah, nilai tersebut ditambahkan (diakumulasikan) ke dalam variabel total. Hal ini dilakukan untuk memuluskan data akhir sebelum dicetak ke monitor.
* **Kode Modifikasi Aktuator (Histerisis):** Kode tidak lagi menggunakan satu variabel `suhuThreshold`, melainkan memperkenalkan variabel baru: `suhuBatasAtas` (30.0) dan `suhuBatasBawah` (28.0). Teknik modifikasi pada baris ini bertujuan memberikan *deadband* (area mati/toleransi) agar aktuator elektromekanis tidak hidup-mati terlalu cepat ketika suhu sedang tidak stabil di satu titik krisis.

## 6. Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code
* **Pentingnya fungsi `isnan()`:** Fungsi ini bertindak sebagai perisai program dari variabel kosong. Tanpa perlindungan baris kode fungsi tersebut, pencetakan dan kalkulasi matematis terhadap sinyal sensor yang putus akan mengakibatkan sistem *crash* atau menghasilkan parameter perintah yang salah.
* **Alasan teknis penulisan perintah `delay(2000)`:** Jeda baris kode ini bersifat wajib (mandatory) sesuai dengan dokumentasi spesifikasi (datasheet) sensor DHT22. Sensor memerlukan waktu pemulihan perangkat keras minimal sekitar 2 detik per siklus untuk menghasilkan paket konversi analog ke digital yang sempurna[cite: 1].
* **Peran krusial variabel ambang batas (*threshold*):** Pendeklarasian konstanta ini sangat krusial karena mikrokontroler tidak memiliki akal untuk menilai sendiri kapan suatu lingkungan dianggap panas. Nilai *threshold* ini ditanamkan dalam memori kode untuk memberikan referensi matematis absolut (trigger) bagi program `if` saat mengaktifkan relay[cite: 1].

