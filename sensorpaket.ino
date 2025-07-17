#define BLYNK_TEMPLATE_ID "TMPL6vfehEyy-"
#define BLYNK_TEMPLATE_NAME "FSR"
#define BLYNK_AUTH_TOKEN "Gz2QuwScE1AGAY5bXIJDQFvbgujmIf-T"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "yakali gabisa";
char pass[] = "12345678";

#define FSR_PIN 34    // pin analog FSR
#define LED_PIN 33     // LED onboard ESP32 (atau bisa ganti ke pin lain)

bool sudahKirim = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 mulai booting...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Koneksi WiFi manual agar tidak hang
  WiFi.begin(ssid, pass);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Koneksi ke Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

void loop() {
  Blynk.run();

  int nilaiFSR = analogRead(FSR_PIN);  // Baca nilai analog dari FSR

  // Kalibrasi kasar konversi ke "berat"
  float berat = (float)(nilaiFSR - 500) * 2000.0 / (3000.0 - 500.0);
  berat = constrain(berat, 0, 2000); // batasi antara 0–2000 gram

  // Tampilkan di Serial Monitor
  Serial.print("FSR: ");
  Serial.print(nilaiFSR);
  Serial.print(" | Berat: ");
  Serial.println(berat);

  // Kirim berat ke Virtual Pin V1 (gunakan widget "Gauge" atau "Label")
  Blynk.virtualWrite(V3, berat);

  // Kirim notifikasi status paket ke V0 (gunakan widget "Label")
  if (nilaiFSR > 1000 && !sudahKirim) {
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V2, "📦 Paket baru telah datang!");
    sudahKirim = true;
  }

  // Reset saat tidak ada paket
  if (nilaiFSR < 500) {
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V2, "❌ Tidak ada paket.");
    sudahKirim = false;
  }

  delay(300);
}
