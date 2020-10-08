#include <Wire.h>
#include "rgb_lcd.h"
#include <Math.h>

#define buttonPin 4
#define rotaryPin A0
#define tempPin A1
#define buzzPin 3

rgb_lcd lcd;

//State variables for lcd Screen
int state=0;
int reset = true;


const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k

//Function that returns current temp in degrees celsius
float getTemp() {
  float a = analogRead(tempPin);
  float R = 1023.0/a-1.0;
  R = R0*R;
  return 1.0/(log(R/R0)/B+1/298.15)-273.15;

}

//Temperature values
int rotValue = 0, minTemp = -20, maxTemp = 30;
float currTemp = getTemp();

//Bottom line of text
String bottomLine = "Init min&max";

byte degC[] = { //Costum Degree sign
  B01000,
  B10100,
  B01000,
  B00011,
  B00100,
  B00100,
  B00100,
  B00011
};


void setup() {
  lcd.begin(16, 2); //Init lcd
  lcd.createChar((byte)0,degC); //Init degree symbol
  lcd.setRGB(0, 255, 0); //set background of lcd

  //Init pins and sensors
  pinMode(buttonPin, INPUT);
  pinMode(rotaryPin, INPUT);
  pinMode(tempPin, INPUT);
  pinMode(buzzPin, OUTPUT);


}

//Function to change state of program
void changeState() {
 if (digitalRead(buttonPin) and reset) { //if button is pressed
  if(state == 0) {
    //Change variables and colors to appropriate values for state 1
    state = 1;
    lcd.setCursor(0, 0);
    lcd.clear(); //Clear Screen
    lcd.setRGB(0, 0, 255); //Blue
    bottomLine = "Was: " + String(minTemp);
    
  }

  else if (state == 1) {
    //Set minimum temperature to current rotary temperature
    minTemp = rotValue;
    //Change variables and colors to appropriate values for state 2
    state = 2;
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setRGB(255, 0, 0);
    bottomLine = "Was: " + String(maxTemp);
  }
  
  else if (state==2) {
    //Set maximum temperature to current rotary temperature
    maxTemp = rotValue;
    if (minTemp<maxTemp){ //No error: Switch back to state 0
      //Change variables and colors to appropriate values for state 0
      state = 0;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.setRGB(0, 255, 0);
      


      
    } else { //Error
      //Change variables and colors to appropriate values for state 3 (error state)
      state=3;
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.setRGB(255, 0, 255);
      bottomLine = "be less than max                            "; 
    }
    
  } 
  else { //Currently in error state
    //Change variables and colors to appropriate values for state 1 (minimum temp)
    state = 1;
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setRGB(0, 0, 255);
    bottomLine = "Was: " + String(minTemp);
    minTemp = rotValue;
  }
   reset = false; //Make sure the function wont be called twice from same button press
 }
 else if(digitalRead(buttonPin)==false){
  reset = true; //Make sure the function will be callable when button isn't pressed
 }
}


void loop() {
    //Read analog values:
    rotValue = (int)(analogRead(rotaryPin)/17.05)-20;
    currTemp = getTemp();

    //Make sure lcd writes from right pos
    lcd.setCursor(0, 0);

    if (state==1) { //Write the top line, dependent on states
      lcd.print("Min temp: " + String(rotValue));
      lcd.write((byte)0);
      lcd.print("       ");
    } else if (state == 2) {
      lcd.print("Max temp: " + String(rotValue));
      lcd.write((byte)0);
      lcd.print("        ");
    } else if (state == 3) {
      lcd.print("Err: min can't"); //Error state
    } else { //State 0
      lcd.print("Current: " + (String)currTemp);
      lcd.write((byte)0);
      checkTemp(); //Function that checks if min/max exceeds currentTemp
    }

    //Write bottom line:
    lcd.setCursor(0,1);
    lcd.print(bottomLine);
    lcd.write((byte)0);
    lcd.print("          ");
    
    changeState(); //Change state - check above.
      
}


void checkTemp(){
  if (currTemp<minTemp) { //If the temperature is too cold
    bottomLine = "OUCH! TOO COLD!         ";
    lcd.setRGB(0, 0 ,255);

    //Alarm noise
    digitalWrite(3, HIGH);
    delay(400);
    digitalWrite(3, LOW);
    delay(400);
  } else if (currTemp>maxTemp) { //If the temperature is too hot
    bottomLine = "OUCH! TOO WARM!        ";
    lcd.setRGB(255,0,0);

    //Alarm noise
    digitalWrite(3, HIGH);
    delay(400);
    digitalWrite(3, LOW);
    delay(400);
  } else { //If the temperature is fine
    bottomLine = "Must be comfy!         ";
    digitalWrite(3, LOW);
    lcd.setRGB(0, 255, 0);
  }

}
