/**********
  Written by Tyler James
**********/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int SWITCH = 16;
const int BUTTON = 17;
const int POT = 4;  //Analog Read Range: 0-4095

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Brain Icon
byte customCharBrain0[8] = {0b00000,0b00000,0b00000,0b00111,0b01111,0b11111,0b11111,0b11111};
byte customCharBrain1[8] = {0b00000,0b00000,0b00000,0b11000,0b11100,0b11111,0b11111,0b11111};
byte customCharBrain2[8] = {0b11111,0b11111,0b01111,0b01111,0b00110,0b00110,0b00100,0b00000};
byte customCharBrain3[8] = {0b11111,0b11110,0b11100,0b10000,0b00000,0b00000,0b00000,0b00000};

//Wifi Icon
byte customCharWifi0[8] = {0b00000,0b00011,0b01111,0b11100,0b11000,0b00011,0b00111,0b00110};
byte customCharWifi1[8] = {0b00000,0b11000,0b11110,0b00111,0b00011,0b11000,0b11100,0b01100};
byte customCharWifi2[8] = {0b00001,0b00011,0b00010,0b00000,0b00001,0b00001,0b00000,0b00000};
byte customCharWifi3[8] = {0b10000,0b11000,0b01000,0b00000,0b10000,0b10000,0b00000,0b00000};

int readPot(){
  return analogRead(POT);
}

bool readSwitch(){
  return digitalRead(SWITCH);
}

bool readButton(){
  return digitalRead(BUTTON);
}

void displayInit(){
  //LCD display setup
  lcd.init(); // initialize the lcd
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GO WROOM!!!");
  lcd.createChar(0, customCharBrain0);
  lcd.createChar(1, customCharBrain1);
  lcd.createChar(2, customCharBrain2);
  lcd.createChar(3, customCharBrain3);
  lcd.createChar(4, customCharWifi0);
  lcd.createChar(5, customCharWifi1);
  lcd.createChar(6, customCharWifi2);
  lcd.createChar(7, customCharWifi3);

  pinMode(SWITCH, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(POT, INPUT);
}

void updateIdleDisplay(){
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Mode: ");
  if (digitalRead(SWITCH)){
    lcd.print(" WIFI");
    lcd.setCursor(0,0);
    lcd.write((byte)4);
    lcd.write((byte)5);
    lcd.setCursor(0,1);
    lcd.write((byte)6);
    lcd.write((byte)7);
  }
  else {
    lcd.print("BRAIN");
    lcd.setCursor(0,0);
    lcd.write((byte)0);
    lcd.write((byte)1);
    lcd.setCursor(0,1);
    lcd.write((byte)2);
    lcd.write((byte)3);
  }
  lcd.setCursor(3, 1);
  lcd.print("Speed: ");
  lcd.print(analogRead(POT)/410);

//   Serial.print("SWITCH: ");
//   Serial.println(digitalRead(SWITCH));
//   Serial.print("BUTTON: ");
//   Serial.println(digitalRead(BUTTON));
//   Serial.print("Pot: ");
//   Serial.println(analogRead(POT));
}

void updateWIFIDisplay(String ip){
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Mode: ");

  lcd.print(" WIFI");
  lcd.setCursor(0,0);
  lcd.write((byte)4);
  lcd.write((byte)5);
  lcd.setCursor(0,1);
  lcd.write((byte)6);
  lcd.write((byte)7);
  
  lcd.setCursor(3, 1);
  lcd.print(ip);
  // lcd.print(analogRead(POT)/410);
}

void updateBLEDisplay(){
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Mode: ");

  lcd.print("BRAIN");
  lcd.setCursor(0,0);
  lcd.write((byte)0);
  lcd.write((byte)1);
  lcd.setCursor(0,1);
  lcd.write((byte)2);
  lcd.write((byte)3);
  
  lcd.setCursor(3, 1);
  lcd.print("Speed: ");
  lcd.print(analogRead(POT)/410);
  lcd.print("  ");
  lcd.print(dirR);
}