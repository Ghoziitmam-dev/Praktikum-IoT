#include <DHT.h>

// ===============================
// KONFIGURASI PIN
// ===============================
#define DHTPIN 4          // Pin DATA DHT22 ke GPIO 4
#define DHTTYPE DHT22     // Jenis sensor DHT
#define RELAYPIN 6       // Pin relay/LED ke GPIO 06

// Membuat objek DHT
DHT dht(DHTPIN, DHTTYPE);

// ===============================
// KONFIGURASI SUHU
// ===============================
const float suhuThreshold = 30.0; // Batas suhu dalam °C

// ===============================
// SETUP
// ===============================
void setup() {
  // Memulai komunikasi Serial
  Serial.begin(115200);

  // Memulai sensor DHT22
  dht.begin();

  // Mengatur pin relay sebagai OUTPUT
  pinMode(RELAYPIN, OUTPUT);

  // Memastikan aktuator dalam kondisi OFF saat awal
  digitalWrite(RELAYPIN, LOW);

  // Pesan awal
  Serial.println("=================================");
  Serial.println(" SISTEM AKUISISI & KENDALI SUHU");
  Serial.println("=================================");
  Serial.println("DHT22 siap digunakan.");
  Serial.print("Batas suhu: ");
  Serial.print(suhuThreshold);
  Serial.println(" °C");
  Serial.println();
}

// ===============================
// LOOP
// ===============================
void loop() {

  // Membaca suhu dalam Celsius
  float suhu = dht.readTemperature();

  // ===============================
  // CEK DATA SENSOR
  // ===============================
  if (isnan(suhu)) {

    Serial.println("ERROR: Gagal membaca data sensor DHT22!");

    // Jika sensor gagal dibaca, aktuator dimatikan
    digitalWrite(RELAYPIN, LOW);

  } else {

    // Menampilkan hasil pembacaan suhu
    Serial.print("Suhu: ");
    Serial.print(suhu, 2);
    Serial.print(" °C -> ");

    // ===============================
    // KENDALI AKTUATOR
    // ===============================
    if (suhu > suhuThreshold) {

      // Jika suhu lebih dari 30°C
      digitalWrite(RELAYPIN, HIGH);

      Serial.println("Aktuator: ON");

    } else {

      // Jika suhu 30°C atau kurang
      digitalWrite(RELAYPIN, LOW);

      Serial.println("Aktuator: OFF");
    }
  }

  // DHT22 dibaca setiap 2 detik
  delay(2000);
}
