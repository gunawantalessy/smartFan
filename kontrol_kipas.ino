#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2           // Pin untuk DHT11 sensor
#define relay 3            // Pin untuk relay (jika diperlukan)
#define DHTTYPE DHT11      // Jenis sensor DHT11
#define fanPin 6           // Pin PWM untuk mengendalikan kecepatan kipas

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C untuk LCD

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  pinMode(fanPin, OUTPUT);  // Set pin PWM untuk kipas
  digitalWrite(relay, 1);
  lcd.setCursor(0, 0);
  lcd.print("Rio_201020100020");
  delay(3000);
  lcd.clear();
}

void loop() {
  float t = dht.readTemperature();  // Membaca suhu dari sensor DHT11

  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Suhu : ");
  lcd.print(t);
  lcd.print((char)223);  // Menampilkan simbol derajat Celsius
  lcd.print("C");
  lcd.print("    ");

  // Menentukan kecepatan kipas berdasarkan suhu
  if (t > 35) {
    analogWrite(fanPin, 255);  // Kecepatan maksimal (PWM 255)
    digitalWrite(relay, 0);    // Menghidupkan kipas
    lcd.setCursor(0, 1);
    lcd.print("Kipas Kecepatan 3");
  } else if (t > 28 && t <= 35) {
    analogWrite(fanPin, 170);  // Kecepatan sedang (PWM 170)
    digitalWrite(relay, 0);    // Menghidupkan kipas
    lcd.setCursor(0, 1);
    lcd.print("Kipas Kecepatan 2");
  } else if (t > 20 && t <= 28) {
    analogWrite(fanPin, 85);   // Kecepatan rendah (PWM 85)
    digitalWrite(relay, 0);    // Menghidupkan kipas
    lcd.setCursor(0, 1);
    lcd.print("Kipas Kecepatan 1");
  } else {
    analogWrite(fanPin, 0);    // Kipas mati
    digitalWrite(relay, 1);    // Mematikan kipas
    lcd.setCursor(0, 1);
    lcd.print("Kipas Mati");
  }

  delay(1000);  // Delay 1 detik untuk pembaruan data
}
