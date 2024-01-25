//Camping Alarm project
//With: Arduino Nano, 6 PIR sensors, DFPlayer Mini, Buzzer, LED
//Final fine wiring with 6 pir active

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include <Wire.h> 
#include <LiquidCrystal.h>

SoftwareSerial mySoftwareSerial(5, 4); ///tx4 rx5 ( VCC RX TX )
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

int pirSensors[] = {7, 8, 9, 10, 11, 12};
int buzzerPin = 3;
int ledPin = 2;

const char *directions[] = {"2 o'clock", "4 o'clock", "6 o'clock", "8 o'clock", "10 o'clock", "12 o'clock"};
unsigned long lastMotionTime = 0;

//For LED blinking when no motion detected
unsigned long previousMillis = 0;
const long interval = 500;  // Interval for LED flashing
int ledState = LOW;
//..........

void setup()
{
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30); 
  for (int i = 0; i < 6; i++) 
  {
    pinMode(pirSensors[i], INPUT);
  }
  pinMode(ledPin, OUTPUT);    
  pinMode(buzzerPin, OUTPUT); 
   

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("System      ");
  lcd.setCursor(0,1);
  lcd.print("initiating...     ");
}
//===================================================================================
void printDetection(int directionPir) 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Motion detected!");
  lcd.setCursor(0,1);
  lcd.print("At: ");
  lcd.print(directions[directionPir]);
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
}

void noDetection() //Keep the led blinks to prevent mobile changer from sleeping
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    digitalWrite(buzzerPin, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    No sign     ");
    lcd.setCursor(0,1);
    lcd.print("  of movement    ");
  }
}

void playDF(int songNum)        
{
  myDFPlayer.play(songNum);
}

void stopDF()
{
  myDFPlayer.pause();
}
//==================================================================================
void loop() 
{
  for (int i = 0; i < 6; i++)
  {
     if(digitalRead(pirSensors[i]) == HIGH)
     {
        printDetection(i);
        playDF(i);
        lastMotionTime = millis();
     }
     else
     {
      if(millis() - lastMotionTime >= 3000)
      {
        noDetection();
        stopDF();
      }
     }
   }
}    
  


    
  

 
