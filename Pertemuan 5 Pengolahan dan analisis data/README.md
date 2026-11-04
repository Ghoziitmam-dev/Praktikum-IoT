# Praktikum IoT Modul 5 - Pengolahan dan Analisis Data IoT

Repositori ini berisi dokumentasi dan kode sumber (source code) untuk Modul 5 yang mempelajari proses akuisisi data mentah dari sensor DHT22, pra-pemrosesan data menggunakan filter di sisi *edge* (ESP32), hingga analisis statistik, visualisasi, dan deteksi anomali di sisi server (Python)[cite: 10, 11].

## 1. Penjelasan Singkat Mengenai Detail Percobaan
Praktikum ini dibagi menjadi dua percobaan utama:
*   **Percobaan 5A (Edge Processing - ESP32):** Mikrokontroler ESP32 membaca data suhu secara langsung dari DHT22 dan melakukan pra-pemrosesan data menggunakan filter *Moving Average*. ESP32 juga mencatat nilai minimum, maksimum, serta simpangan baku dari pembacaan data, mengurangi jumlah derau (*noise*) sebelum data dikirimkan[cite: 10, 11].
*   **Percobaan 5B (Server Processing - Python):** Laptop/PC digunakan sebagai server untuk berlangganan (*subscribe*) data dari ESP32 via MQTT. Data diakuisisi sebanyak 30 sampel, disimpan ke *file* `data_suhu.csv`, dihitung rata-rata dan deviasi standarnya dengan `pandas`, dan dilakukan deteksi anomali menggunakan perhitungan z-score[cite: 14, 15]. Data tersebut lalu digambar menjadi grafik (*plot*) secara visual menggunakan `matplotlib`[cite: 15].

## 2. Library atau Dependencies yang Diperlukan
**Untuk ESP32 (Arduino IDE):**
1.  **DHT sensor library**: Untuk membaca data dari sensor DHT22[cite: 11].

**Untuk Python (Komputer):**
1.  **paho-mqtt**: Pustaka untuk menghubungkan skrip Python dengan broker MQTT (menerima data suhu dari ESP32)[cite: 11].
2.  **pandas**: Pustaka analisis data untuk mengelola data tabular (DataFrame CSV), menghitung *moving average*, *mean*, dan *standard deviation* secara efisien[cite: 11].
3.  **matplotlib**: Pustaka pembuat grafik (*plotting*) untuk visualisasi tren data sensor dan menandai lokasi terjadinya anomali suhu[cite: 11].

## 3. Skematik / Diagram Rangkaian
*   **ESP32 & DHT22:**[cite: 12]
    *   Pin VCC DHT22 dihubungkan ke 3V3 ESP32.
    *   Pin GND DHT22 dihubungkan ke GND ESP32.
    *   Pin DATA DHT22 dihubungkan ke GPIO 4 (Pin D4) ESP32.

## 4. Penjelasan Code & Modifikasi Tugas Pendahuluan

### Kode Percobaan 5A (Modifikasi: Tambahan Simpangan Baku)
Program ditambahkan fungsi baru `hitungSimpanganBaku(float mean)`. Fungsi ini memanfaatkan data yang berada di dalam `buffer` (sejarah pembacaan terakhir) untuk mengukur seberapa jauh data-data tersebut menyimpang dari nilai rata-ratanya (`mean`). Di dalam fungsi `loop()`, nilai *standard deviation* ini dicetak berdampingan dengan hasil filter.

### Kode Percobaan 5B (Modifikasi: Moving Average sebelum Z-Score)
Program Python dimodifikasi menggunakan fungsi `.rolling(window=5).mean()` bawaan dari *pandas* untuk menerapkan filter *Moving Average* pada DataFrame `df["suhu"]`. 
*   **Tujuan:** Dengan meloloskan data melalui MA terlebih dahulu, fluktuasi/derau minor dihaluskan. 
*   **Hasil:** Perhitungan parameter statistik `rata2` dan `std` diambil dari data MA, bukan data mentah. Ini berakibat nilai *z-score* menjadi jauh lebih peka terhadap perubahan beruntun, sehingga jumlah anomali yang dideteksi umumnya lebih sedikit dari derau sesaat, tapi akan akurat mendeteksi adanya *trend* suhu gangguan panas buatan.

## 5. Penjelasan Setiap Fungsi
*   **ESP32 - `hitungMovingAverage()`:** Fungsi ini bertugas menyimpan nilai baru ke dalam array `buffer` sebagai sebuah *circular buffer*. Fungsi ini menjumlahkan semua *item* yang sudah terisi dan membaginya dengan total item untuk meratakan kurva data[cite: 12].
*   **Python - `on_connect()` & `on_message()`:** Adalah fungsi *callback* MQTT. `on_connect` dipanggil saat program berhasil login ke broker, lalu mengeksekusi `client.subscribe()`. `on_message` mem-parsing data JSON setiap ada kiriman pesan masuk, lalu menambahkannya ke array Python `data[]` hingga mencapai batasan 30 sampel[cite: 14].

## 6. Penjelasan Percabangan / Conditional
*   **ESP32 - `if (isnan(suhu))`:** Percabangan ini bertindak sebagai penjaga batas (*guard clause*) untuk memvalidasi apakah sensor gagal dibaca atau kabel putus. Jika *Not a Number*, proses filter dilompati agar program tidak *crash*[cite: 13].
*   **Python - `try-except` (Pada `on_message`):** Karena lingkungan MQTT bersifat publik, sangat mungkin masuk pesan dengan format yang rusak atau *spam* string. Blok ini mencegah *runtime error* (`ValueError` atau `KeyError`) dengan mengabaikan pesan tersebut dan program tetap hidup untuk pesan berikutnya[cite: 14].
*   **Python - `if std == 0:`:** Percabangan pengaman matematis. Rumus *Z-Score* memerlukan pembagian dengan simpangan baku. Jika semua nilai sensor konstan (100% sama), deviasi akan 0. Tanpa *if* ini, program akan melempar `ZeroDivisionError`[cite: 15].

## 7. Jawaban Pertanyaan Praktikum yang Berkaitan dengan Code

### Pertanyaan 5.5.4 (ESP32 - Edge Processing)
1.  **Gambarkan diagram alur (flowchart) perhitungan moving average!**
    *   (Mulai) -> Input `nilaiBaru` -> Simpan nilai ke `buffer[indeks]` -> Ubah nilai indeks menggunakan modulus: `indeks = (indeks + 1) % N` -> Apakah `jumlah < N`? (Jika ya, `jumlah++`) -> Hitung `total` dari seluruh `buffer` -> Hitung rata-rata: `total / jumlah` -> `Return` rata-rata -> (Selesai).
2.  **Jelaskan fungsi operasi (indeks + 1) % N !**
    *   Operasi ini digunakan untuk membuat struktur data *Circular Buffer*. Dengan modulus `% N`, apabila nilai `indeks` mencapai batas akhir panjang array (N), nilainya akan langsung dipaksa kembali menjadi 0[cite: 12]. Ini membuat memori yang dipakai selalu tetap dan data terlama akan selalu tertimpa oleh data baru.
3.  **Dampak jika nilai N diperbesar (20) atau diperkecil (2)?**
    *   Jika **N = 20**: Hasil penyaringan (*filter*) akan menjadi sangat halus dan stabil, karena merata-ratakan sejarah yang sangat panjang. Namun, kekurangannya, respons terhadap lonjakan suhu aktual akan sangat lambat[cite: 11].
    *   Jika **N = 2**: Respons sistem akan sangat cepat dalam melacak perubahan suhu asli, tapi kekurangannya data akan tetap kasar/kasar karena derau acak masih sangat mempengaruhi perhitungan rata-rata.

### Pertanyaan 5.6.4 (Python - Analisis dan Visualisasi)
1.  **Fungsi blok try-except pada fungsi `on_message`!**
    *   Digunakan untuk menangani *error exception*. Jika pesan yang diterima bukan format JSON (`ValueError`) atau JSON valid tapi tidak memiliki kunci *key* `"suhu"` (`KeyError`), program tidak akan keluar (*force close*), melainkan sekadar mencetak "Data tidak valid" dan melanjutkan mendengarkan sisa data lainnya[cite: 14].
2.  **Arti z-score dan batasan |z| > 2 !**
    *   *Z-score* adalah nilai yang menyatakan seberapa jauh suatu sampel pembacaan menyimpang/melenceng dari titik rata-ratanya, diukur dalam satuan simpangan baku (standar deviasi)[cite: 11]. Dalam distribusi statistik probabilitas normal, 95% data selalu berada dalam batas `z = -2` hingga `z = 2`. Data apa pun yang melebihinya sangat wajar dikategorikan sebagai anomali (pembacaan tidak masuk akal).
3.  **Kelemahan z-score pada tren tajam?**
    *   *Z-score* sangat bergantung pada "rata-rata tunggal" dari seluruh himpunan data (`mean` global). Jika suhu naik perlahan namun terus menerus sepanjang hari (misal jam 7 pagi sampai 12 siang), suhu siang hari otomatis akan bernilai *z-score* di atas 2 dibanding rata-rata harian, padahal hal ini adalah wajar (bukan anomali sistem).

## 8. Foto Proses Praktikum / Perangkaian
<img width="1000" height="400" alt="WhatsApp Image 2026-10-20 at 14 12 16" src="https://github.com/user-attachments/assets/a96940c3-5b1d-4167-ae11-b649d420a849" />


<img width="1041" height="690" alt="WhatsApp Image 2026-10-20 at 14 14 47" src="https://github.com/user-attachments/assets/0418f93d-9c89-4f72-8b15-51865cd6446a" />




