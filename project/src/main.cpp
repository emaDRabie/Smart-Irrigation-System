// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#define DHTPIN 10
#define DHTTYPE DHT11
#define In_1 8
#define In_2 7
#define EN_A 9
#define myAnalogMoisture A0
DHT dht(DHTPIN, DHTTYPE);
uint8_t value =0 ,SOIL_MOISTURE =0;
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
// LCD interface pin with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables declarations
const String defaultPassword = "1234";
String currentPassword = defaultPassword;
enum State {
  ENTER_PASSWORD,
  MENU,
  CHANGE_PASSWORD,
  ADD_MODE,
  START_MODE,
  RESET
};
enum Add {
  Name,
  minSoilMoisture,
  maxSoilMoisture,
  minTemp,
  maxTemp
};
State currentState = ENTER_PASSWORD;
Add currentAdd = Name;
struct mode{
  String name;
  int minSoilMoisture;
  int maxSoilMoisture;
  int minTemp;
  int maxTemp;
};
int Iterate = 0; 
mode modeData[5];
static int modeIndex;

// Function declarations
void handleInput(const String & );
void enterPassword(const String & );
void menu(const String & );
void changePassword(const String & );
void addMode(const String & );
void startMode(const String & );
void DHT_CODE_and_MOISTURE_CODE();
void terminateSystem();
boolean validName(const String & );
boolean validSoilMoisture(const String & );
boolean validMinTemp(const String & );
boolean validMaxTemp(const String & );

void setup() {
  lcd.begin(16, 2);
  lcd.print("Enter password:");
  dht.begin();
  lcd.createChar(0, degree_symbol);
  Serial.begin(9600);
  pinMode(In_1, OUTPUT);
  pinMode(In_2, OUTPUT);
  pinMode(EN_A, OUTPUT);
}

void loop() {
  static String inputBuffer = "";
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    lcd.setCursor(2,1);
    if (receivedChar == '\r' || receivedChar == '\t') {
      handleInput(inputBuffer);
      inputBuffer = "";
    }
    else if (receivedChar == '\b' && inputBuffer.length() > 0) {
      inputBuffer = inputBuffer.substring(0, inputBuffer.length() - 1);
      lcd.setCursor(0,1);
      lcd.print(">>");
      lcd.print(inputBuffer+" ");
    }
    else if (receivedChar != '\b'){
      inputBuffer += receivedChar;
      lcd.print(inputBuffer);
    }
  }
  if(currentState!=START_MODE){
    lcd.setCursor(0,1);
    lcd.print(">>");
  }
}

// Function definitions
void handleInput(const String & input) {
  if(input=="") currentState=RESET;
  switch (currentState) {
    case ENTER_PASSWORD :
      enterPassword(input);
      break;
    case MENU:
      menu(input);
      break;
    case CHANGE_PASSWORD:
      changePassword(input);
      break;
    case ADD_MODE:
      addMode(input);
      break;
    case START_MODE:
      startMode(input);
      break;
    case RESET:
      terminateSystem();
      break;
    default:
      currentState = ENTER_PASSWORD;
      lcd.clear();
      lcd.print("Enter password:");
      break;
  }
}
void enterPassword(const String & password) {
  if (password == currentPassword) {
    lcd.clear();
    lcd.print("Entering Sys");
    for(int i =0; i< 3 ; i++){
    lcd.setCursor(12, 0);
    lcd.print("    ");
    lcd.setCursor(12, 0);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    }
    delay(500);
    currentState = MENU;
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
  } else {
    currentState = ENTER_PASSWORD;
    lcd.clear();
    lcd.print("Access denied!");
    delay(500);
    lcd.clear();
    lcd.print("Enter password:");
  }
}
void menu(const String & menuSelection) {
  switch (menuSelection.toInt()){
  case 1:
    currentState=CHANGE_PASSWORD;
    lcd.clear();
    lcd.print("CHANGE PASSWORD");
    delay(1000);
    lcd.clear();
    lcd.print("New password:");
    break;
  case 2:
    currentState=ADD_MODE;
    lcd.clear();
    lcd.print("ADD MODE");
    delay(1000);
    lcd.clear();
    lcd.print("Mode Name:");
    break;
  case 3:
    currentState=START_MODE;
    lcd.clear();
    lcd.print("START MODE");
    for(int i=0; i<Iterate; i++){
      Serial.print("===========(");
      Serial.print(i+1);
      Serial.println(")===========");
      Serial.print("Mode name: ");
      Serial.println(modeData[i].name);
      Serial.print("Min moisture: ");
      Serial.println(modeData[i].minSoilMoisture);
      Serial.print("Max moisture: ");
      Serial.println(modeData[i].maxSoilMoisture);
      Serial.print("Min temp: ");
      Serial.println(modeData[i].minTemp);
      Serial.print("Max temp: ");
      Serial.println(modeData[i].maxTemp);
    }
    break;
  case 4:
    currentState=ENTER_PASSWORD;
    lcd.clear();
    lcd.print("Restarting");
    for(int i =0; i< 3 ; i++){
    lcd.setCursor(10, 0);
    lcd.print("    ");
    lcd.setCursor(10, 0);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    }
    delay(200);
    lcd.clear();
    lcd.print("Enter password:");
    break;
  default:
    currentState=MENU;
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
    break;
  }
}
void changePassword(const String & newpassword) {
  if(newpassword.length() >= 4){
    currentPassword=newpassword;
    currentState=MENU;
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
  }
  else{
    currentState=CHANGE_PASSWORD;
    lcd.clear();
    lcd.print("Bad Password!");
    delay(500);
    lcd.clear();
    lcd.print("New password:");
  }
}
void addMode(const String & data) {
  switch (currentAdd){
  case Name:
    if(validName(data)){
      currentAdd=minSoilMoisture;
      lcd.clear();
      lcd.print("Name Done");
      delay(500);
      lcd.clear();
      modeData[Iterate].name=data;
      lcd.print("min Moisture:");
    }else{
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("Mode Name:");
    }
    break;
  case minSoilMoisture:
    if(validSoilMoisture(data)){
      currentAdd=maxSoilMoisture;
      modeData[Iterate].minSoilMoisture=data.toInt();
      lcd.clear();
      lcd.print("min Moisture Done");
      delay(1000);
      lcd.clear();
      lcd.print("max Moisture:");
    } else{
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("min Moisture:");
    }
    break;
  case maxSoilMoisture:
    if(validSoilMoisture(data)){
      currentAdd=minTemp;
      modeData[Iterate].maxSoilMoisture=data.toInt();
      lcd.clear();
      lcd.print("max Moisture Done");
      delay(1000);
      lcd.clear();
      lcd.print("min Temp:");
    } else{
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("max Moisture:");
    }
    break;
  case minTemp:
    if(validSoilMoisture(data)){
      currentAdd=maxTemp;
      lcd.setCursor(0, 0);
      modeData[Iterate].minTemp=data.toInt();
      lcd.clear();
      lcd.print("min Temp Done");
      delay(1000);
      lcd.clear();
      lcd.print("max Temp:");
    } else {
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("min Temp:");
    }
    break;
  case maxTemp:
    if(validSoilMoisture(data)){
      currentState=MENU;
      currentAdd=Name;
      modeData[Iterate].maxTemp=data.toInt();
      Iterate++;
      lcd.clear();
      lcd.print("max Temp Done");
      delay(1000);
      lcd.clear();
      lcd.print("C:1 A:2 S:3 R:4");
    }else{
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("max Temp:");
    }
    break;
  default:
    currentState=MENU;
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
    break;
  }
}
void startMode(const String & data) {
  modeIndex = data.toInt() - 1;
  if (modeIndex >= 0 && modeIndex < Iterate) {
    lcd.clear();
    lcd.print(modeData[modeIndex].name);
    lcd.setCursor(0, 1);
    lcd.print("Starting");
    for(int i =0; i< 3 ; i++){
      lcd.setCursor(8, 1);
      lcd.print("    ");
      lcd.setCursor(8, 1);
      lcd.print(".");
      delay(200);
      lcd.print(".");
      delay(200);
      lcd.print(".");
      delay(200);
      lcd.print(".");
      delay(200);
    }
    delay(2000);
    lcd.clear();
    DHT_CODE_and_MOISTURE_CODE();
  } else {
    lcd.clear();
    lcd.print("Invalid Mode!");
    delay(1000);
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
    currentState = MENU;
  }
}

void DHT_CODE_and_MOISTURE_CODE() {
  while (1) {
    ///////////////// DHT_11 SENSOR /////////////////
    byte HUM = dht.readHumidity();
    byte TEMP = dht.readTemperature();
    if (isnan(HUM) || isnan(TEMP))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    lcd.setCursor(0, 0);
    lcd.print("T = ");
    lcd.print(TEMP);
    lcd.write((uint8_t)0);
    lcd.print("C");
    lcd.print("|");
    lcd.print("H = ");
    lcd.print(HUM);
    lcd.print("%");
    if (TEMP > modeData[modeIndex].maxTemp){
      analogWrite(EN_A, 220);
      digitalWrite(In_1, HIGH);
      digitalWrite(In_2, LOW);
    }
    else if (TEMP < modeData[modeIndex].minTemp){
      analogWrite(EN_A, 150);
      digitalWrite(In_1, LOW);
      digitalWrite(In_2, HIGH);
    }
    else{
      analogWrite(EN_A, 0);
      digitalWrite(In_1, LOW);
      digitalWrite(In_2, LOW);
    }
    //************************* TEST *************************
    ///////////////// SOIL MOISTURE SENSOR /////////////////
    value = analogRead(myAnalogMoisture);
    SOIL_MOISTURE = map(value, 1023, 0, 0, 100);
    lcd.setCursor(0, 1);
    lcd.print("M = ");
    lcd.print(SOIL_MOISTURE);
    lcd.print("% |");
    lcd.print("V = ");
    lcd.print((value* 5 )/1023);
    delay(1000);
  }
}

boolean validName(const String & name){
  if(name.length() < 3) return false;
  for(int i=0; i<=Iterate; i++) {
    if(name == modeData[i].name) return false;
  }
  return true;
}
boolean validSoilMoisture(const String & Moisture){
  volatile int myMoisture = Moisture.toInt();
  if(Moisture.length() > 4) return false;
  for (int i = 0; i < Moisture.length(); i++){
    if(!isDigit(Moisture.charAt(i))) return false;
  }
  if(myMoisture > 100 || myMoisture < 0) return false;
  return true;
} 
void terminateSystem() {
  exit(0);
}
