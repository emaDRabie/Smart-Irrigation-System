// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD interface pin with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables declarations
const String defaultPassword = "1234";
String currentPassword = defaultPassword;
bool isAuthenticated = false;
enum State {
  ENTER_PASSWORD,
  MENU,
  CHANGE_PASSWORD,
  ADD_MODE,
  START_MODE
};
State currentState = ENTER_PASSWORD;

// Function declarations
void handleInput(const String &);
void enterPassword(const String &);
void menu(const String &);
void changePassword(const String &);

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
    else {
      inputBuffer += receivedChar;
      lcd.print(inputBuffer);
    }
  }
  lcd.setCursor(0,1);
  lcd.print("=>");
}

// Function definitions
void handleInput(const String &input) {
  if(input=="reset" || input=="clear") currentState=ENTER_PASSWORD;
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
      // Handle ADD_MODE
      break;
    case START_MODE:
      // Handle START_MODE
      break;      
    default:
      // Handle other states
      break;
  }
}
void enterPassword(const String &password) {
  if (password == currentPassword) {
    lcd.clear();
    lcd.print("Access granted!");
    delay(500);
    currentState = MENU;
    lcd.clear();
    lcd.print("C:1 A:2 S:3 R:4");
    // for (int shift = 0; shift < 3; shift++) {
    //   lcd.scrollDisplayLeft();
    //   delay(500);
    // }
    // lcd.clear();
    // lcd.print("(2)Add Mode? (3)Start Mode?");
    // delay(1000);
    // for (int shift = 0; shift < 11; shift++) {
    //   lcd.scrollDisplayLeft();
    //   delay(500);
    // }
    // Serial.println("Change password? (1)");
    // Serial.println("Add Mode?(2)");
    // Serial.println("Start Mode? (3)");
  } else {
    currentState = ENTER_PASSWORD;
    lcd.clear();
    lcd.print("Access denied!");
    delay(500);
    lcd.clear();
    lcd.print("Enter password:");
  }
}
void menu(const String &menuSelection) {
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
    break;
  case 3:
    currentState=START_MODE;
    lcd.clear();
    lcd.print("START MODE");
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
void changePassword(const String &newpassword) {
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