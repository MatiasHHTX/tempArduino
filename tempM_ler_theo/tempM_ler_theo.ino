#include <Wire.h>
#include "rgb_lcd.h"

#define buttonPin 2
#define rotaryPin A1

rgb_lcd lcd;

int state=0;
int reset = true;
String temp = "Temp: ";
String scrClear = "                ";
int rotValue = 0, minTemp = 0, maxTemp = 0, currTemp=0;


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
    lcd.clear();
    lcd.setRGB(0, 0, 255);
    temp = "Was:  " + String(minTemp);
    
  }

  else if (state == 1) {
    minTemp = rotValue;
    state = 2;
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setRGB(255, 0, 0);
    temp = "Was: " + String(maxTemp);
  }
  
  else if (state==2) {
    maxTemp = rotValue;
    if (minTemp<maxTemp){ //No error: Switch back to state 0
      state = 0;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.setRGB(0, 255, 0);
      temp = String(minTemp) + "゜C   " + String(maxTemp) + "゜C";  


      
    } else { //Error
      state=3;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.setRGB(255, 0, 255);
      temp = "be less than max"; 
    }
    
  } 
  else {
    state = 1;
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setRGB(0, 0, 255);
    temp = "Was: " + String(minTemp);
    minTemp = rotValue;
  }
   reset = false;
 }
 else if(digitalRead(buttonPin)==false){
  reset = true;
 }
}


void loop() {
  // put your main code here, to run repeatedly:
    rotValue = analogRead(rotaryPin);
    //Print temp: \n XX degrees
    
    lcd.setCursor(0, 0);
    if (state==1) {
      lcd.print("Min temp:" + String(rotValue) + (char)223 + "C       ");
    } else if (state == 2) {
      lcd.print("Max temp: " + String(rotValue) + (char)223 + "C       ");
    } else if (state == 3) {
      lcd.print("Err: min can't");
    } else { //State 0
      if (minTemp>currTemp) {
        
      } else if (maxTemp<currTemp) {
        
      } else {
        lcd.print("Curr Temp: " + String(currTemp) + (char)223 + "C     ");
      }
    }
    
    lcd.setCursor(0,1);
    lcd.print(temp);
    lcd.print("         ");
    changeState();
      
}

void clearScreen() {
  lcd.print(scrClear);
}
