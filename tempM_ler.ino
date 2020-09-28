#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
int state=0;
int reset = true;
String temp = "Temp: ";
String scrClear = "                ";
#define buttonPin 4
#define rotaryPin 0

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0);
  pinMode(buttonPin, INPUT);
  pinMode(rotaryPin, INPUT);
}

void changeState() {
 if (digitalRead(buttonPin) and reset) {
  if(state == 0) {
    state = 1;
    lcd.setCursor(0, 0);
    clearScreen();
    lcd.setRGB(0, 0, 255);
    temp = "Lowest temp: ";
  }

  else if (state == 1) {
    state = 2;
    lcd.setCursor(0, 0);
    clearScreen();
    lcd.setRGB(255, 0, 0);
    temp = "Highest temp: ";
  }
  
   else {
    state = 0;
    lcd.setCursor(0, 0);
    clearScreen();
    lcd.setRGB(0, 255, 0);
    temp = "Temp: ";
   }
   reset = false;
 }
 else if(digitalRead(buttonPin)==false){
  reset = true;
 }
}

void state1(){
  
}

void loop() {
  // put your main code here, to run repeatedly:
    int rotValue = analogRead(rotaryPin);
    lcd.setCursor(0, 0);
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print(rotValue), char(223), "C";
    lcd.print("         ");
    changeState();
      
}

void clearScreen() {
  lcd.print(scrClear);
}
