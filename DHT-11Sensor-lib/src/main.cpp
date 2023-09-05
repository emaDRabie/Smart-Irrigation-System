#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
void DHT_SETUP();
void DHT_CODE();
#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte degree_symbol[8] =
    {
        0b00111,
        0b00101,
        0b00111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000};
void setup() {
  DHT_SETUP();
}

void loop() {
  DHT_CODE();
}

void DHT_SETUP(){
  lcd.begin(16, 2);
  lcd.print("T = ");
  lcd.setCursor(0, 1);
  lcd.print("H = ");
  lcd.setCursor(9, 0);
  lcd.write((uint8_t)0);
  lcd.print("C");
  lcd.setCursor(9, 1);
  lcd.print("%");
  dht.begin();
  lcd.createChar(0, degree_symbol);
}

void DHT_CODE(){
  delay(2000);
  float HUM = dht.readHumidity();
  float TEMP = dht.readTemperature();
  if (isnan(HUM) || isnan(TEMP) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  lcd.setCursor(4,0);
  lcd.print(TEMP);
  lcd.setCursor(4,1);
  lcd.print(HUM);
}