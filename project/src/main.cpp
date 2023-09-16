// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>

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
  START_MODE
};
enum Add {
  Name,
  SoilMoisture
};
State currentState = ENTER_PASSWORD;
Add currentAdd = Name;
struct mode{
  String name;
  int SoilMoisture;
};
int Iterate = 0; 
mode modeData[5];

// Function declarations
void handleInput(const String & );
void enterPassword(const String & );
void menu(const String & );
void changePassword(const String & );
void addMode(const String & );
boolean validName(const String & );
boolean validSoilMoisture(const String & );

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);  
  lcd.print("Enter password:");
}

void loop() {
  static String inputBuffer = "";
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    lcd.setCursor(2,1);
    if (receivedChar == '\n' || receivedChar == '\r') {
      handleInput(inputBuffer);
      inputBuffer = "";
    }
    else if (receivedChar == '\b' && inputBuffer.length() > 0) {
      inputBuffer = inputBuffer.substring(0, inputBuffer.length() - 1);
      lcd.clear();
      lcd.print("Enter password:");
      lcd.setCursor(0,1);
      lcd.print("=>");
      lcd.print(inputBuffer);
    }
    else {
      inputBuffer += receivedChar;
      lcd.print(inputBuffer);
    }
  }
  lcd.setCursor(0,1);
  lcd.print("=>");
}

// Function definitions
void handleInput(const String & input) {
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
      // Handle START_MODE
      break;      
    default:
      // Handle other states
      break;
  }
}
void enterPassword(const String & password) {
  if (password == currentPassword) {
    lcd.clear();
    lcd.print("Access granted!");
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
    for(int i=0; i<=Iterate; i++){
      Serial.println(modeData[i].name);
      Serial.println(modeData[i].SoilMoisture);
    }
    break;
  case 4:
    currentState=ENTER_PASSWORD;
    lcd.clear();
    lcd.print("Restarting....");
    delay(500);
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
      currentAdd=SoilMoisture;
      lcd.clear();
      lcd.print("Name Done");
      delay(500);
      lcd.clear();
      lcd.print("Soil Moisture:");
      modeData[Iterate].name=data;
    }else{
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("Mode Name:");
    }
    break;
  case SoilMoisture:
    if(validSoilMoisture(data)){
      currentState=MENU;
      currentAdd=Name;
      modeData[Iterate].SoilMoisture=data.toInt();
      Iterate++;
      lcd.clear();
      lcd.print("Moisture Done");
      delay(1000);
      lcd.clear();
      lcd.print("C:1 A:2 S:3 R:4");
    }else{
      lcd.print("Invalid!");
      delay(500);
      lcd.clear();
      lcd.print("Soil Moisture:");
    }
    break;
  default:
    currentState=MENU;
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
    break;
  }
}
boolean validName(const String & name){
  if(name.length() < 3) return false;
  for(int i=0; i<=Iterate; i++){
    if(name == modeData[i].name) return false;
  }
  return true;
}
boolean validSoilMoisture(const String & Moisture){
  if(Moisture.length() > 4) return false;
  for (int i = 0; i < Moisture.length(); i++){
    if(!isDigit(Moisture.charAt(i))) return false;
  }
  return true;
}
