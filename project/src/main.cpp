#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#define modeControl 13
#define countControl 6
#define myVoltageAnalogSignal A0
#define myTempAnalogSignal A1
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
int A = 10, B = 9, C = 8, D = 7, count = 0, an = 0;
int A_Cases[10] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
int B_Cases[10] = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
int C_Cases[10] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0};
int D_Cases[10] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1};
void setup()
{
  lcd.createChar(0, degree_symbol);
  lcd.begin(16, 2);
  for (int i = 7; i < 11; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
  lcd.clear();
}
void loop()
{
  if (digitalRead(modeControl) == HIGH)
  {
    //-----------|Print on lcd|--------
    lcd.setCursor(0, 1);
    lcd.print("                              ");
    lcd.setCursor(0, 0);
    delay(100);
    lcd.print("Count = ");
    lcd.print(count);
    digitalWrite(A, A_Cases[count]);
    digitalWrite(B, B_Cases[count]);
    digitalWrite(C, C_Cases[count]);
    digitalWrite(D, D_Cases[count]);
    //---------------------------------
    delay(500);
    if (digitalRead(countControl) == LOW)
    {
      count++;
    }
    lcd.setCursor(8, 0);
    if (count == 10)
      count = 0;
  }
  else
  {
    lcd.setCursor(0, 0);
    delay(100);
    an = analogRead(myVoltageAnalogSignal);
    lcd.print("V = ");
    lcd.setCursor(4, 0);
    lcd.print((5.0 / 1023) * an);
    lcd.print("v");
    lcd.setCursor(0, 1);
    lcd.print("T = ");
    lcd.print(analogRead(myTempAnalogSignal) * (497.85 / 1023.0));
    lcd.write((uint8_t)0);
    lcd.print("C");
  }
}
