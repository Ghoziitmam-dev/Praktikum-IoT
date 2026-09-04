#include <DHT.h>

// ===============================
// KONFIGURASI SENSOR DHT22
// ===============================
#define DHTPIN 4
#define DHTTYPE DHT22

// Membuat objek sensor
DHT dht(DHTPIN, DHTTYPE);

// ===============================
// SETUP
// ===============================
void setup() {
  // Memulai komunikasi Serial
  Serial.begin(115200);

  // Memulai sensor DHT22
  dht.begin();

  // Pesan awal
  Serial.println("=================================");
  Serial.println("   SISTEM AKUISISI DATA DHT22");
  Serial.println("=================================");
  Serial.println("Sensor sedang diinisialisasi...");
  
  // Memberikan waktu sensor untuk stabil
  delay(2000);

  Serial.println("Sensor DHT22 siap digunakan!");
  Serial.println();
}

// ===============================
// LOOP
// ===============================
void loop() {

  // Membaca kelembaban
  float kelembaban = dht.readHumidity();

  // Membaca suhu dalam Celsius
  float suhu = dht.readTemperature();

  // ===============================
  // CEK HASIL PEMBACAAN
  // ===============================
  if (isnan(kelembaban) || isnan(suhu)) {

    Serial.println("ERROR: Gagal membaca data dari sensor DHT22!");
    Serial.println("Periksa koneksi VCC, GND, dan DATA sensor.");
    
  } else {

    // ===============================
    // MENAMPILKAN DATA
    // ===============================
    Serial.println("---------- DATA SENSOR ----------");

    Serial.print("Suhu       : ");
    Serial.print(suhu, 2);
    Serial.println(" °C");

    Serial.print("Kelembaban : ");
    Serial.print(kelembaban, 2);
    Serial.println(" %");

    Serial.println("---------------------------------");
  }

  // DHT22 sebaiknya tidak dibaca terlalu cepat
  delay(2000);
}
