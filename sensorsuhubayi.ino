#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 8

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);

  if (!mlx.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor gagal!");
    while (1); // Stop program
  }
}

void loop() {
  float suhu = mlx.readObjectTempC();

  lcd.setCursor(0, 0);
  lcd.print("Suhu Bayi:");
  lcd.setCursor(0, 1);
  lcd.print(suhu);
  lcd.print((char)223);
  lcd.print("C     ");

  Serial.print("Suhu: ");
  Serial.println(suhu);

  if (suhu < 36.0) {
    tone(BUZZER_PIN, 500); delay(200);
    tone(BUZZER_PIN, 800); delay(200);
    tone(BUZZER_PIN, 1200); delay(200);
    noTone(BUZZER_PIN);
  } else {
    noTone(BUZZER_PIN);
  }

  delay(1000);
}
