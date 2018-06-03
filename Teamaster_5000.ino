#include <LiquidCrystal.h>
#include <Servo.h>
#include "pitches.h"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int startbutton = 6;
const int stopbutton = 10;
const int speaker = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo;

int sensorPin = A0;
int sensorValue = 0;
int valstartbutton = 0;
int sekunden = 0;
int valstopbutton = 0;

int potizusec (int value){
  float ergebnis = value;
  ergebnis = ergebnis / 1023 * 900;
  value = ergebnis;
  return value;
}

int seczumin (int value){
  return value / 60;
}
int seczusec (int value){
  return value % 60;
}

void playWindows() {
  int melody[] = {
      NOTE_DS4, NOTE_AS3, NOTE_GS3, NOTE_DS4, NOTE_AS3
    };
    int noteDurations[] = {
      4,4,3,6,4
    };
    playSounds(melody, noteDurations,5);
}
void playZelda1() {
  int melody[] = {
      NOTE_G3, NOTE_A3, NOTE_B3, NOTE_CS4, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_CS4,
      NOTE_GS3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_GS3, NOTE_AS3, NOTE_C4, NOTE_D4,
      NOTE_A3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_A3, NOTE_B3, NOTE_CS4, NOTE_DS4,
      NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_E4,
      NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_F4,
      NOTE_C4, NOTE_D4, NOTE_E4, NOTE_FS4,
      NOTE_CS4, NOTE_DS4, NOTE_F4, NOTE_G4,
      NOTE_D4, NOTE_E4, NOTE_FS4, NOTE_GS4
    };
    
    int noteDurations[] = {
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,13,13,13,13
    };
    playSounds(melody, noteDurations,48);
}
void playZelda2() {
  int melody[] = {
      NOTE_A3, NOTE_AS3, NOTE_B3, NOTE_C4
     };
    int noteDurations[] = {
      6,6,6,1
    };
    playSounds(melody, noteDurations,4);
}
void playSounds(int melody[],int noteDurations[], int count) {
  for (int thisNote = 0; thisNote < count; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speaker, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speaker);
  }
}
void playSingleSound(int Tone, int duration) {
  tone(speaker, Tone, duration);
  delay(duration * 1.30);
  noTone(speaker);
}


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print("Teamaster 5000");
  //delay(3000);
  playWindows();
  lcd.clear();  
  pinMode(startbutton, INPUT_PULLUP);
  myservo.attach(9);
  myservo.write(62);
  pinMode(stopbutton, INPUT_PULLUP);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Ziehzeit:");
  valstartbutton = digitalRead(startbutton);
  while(valstartbutton == HIGH){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    sensorValue = analogRead(sensorPin);
    sekunden = potizusec(sensorValue);
    lcd.print("min: ");
    lcd.print(seczumin(sekunden));
    lcd.print(" sec: ");
    lcd.print(seczusec(sekunden));
    delay(250);
    valstartbutton = digitalRead(startbutton);
  }

  playSingleSound (NOTE_C3, 250);
  playSingleSound (NOTE_G3, 250);
  playSingleSound (NOTE_C4, 250);
  myservo.write(2);
  delay(15);
  valstopbutton = digitalRead(stopbutton);
  int counter = 0;
  while(sekunden > 0 and valstopbutton == HIGH){
    if (counter < 3) {
      counter = counter + 1;
      valstopbutton = digitalRead(stopbutton);
      delay(250);
    }
    else {
      counter = 0;
      sekunden = sekunden - 1;
      valstopbutton = digitalRead(stopbutton);
      lcd.setCursor(0,1);
      lcd.print("                ");    
      lcd.setCursor(0,1);
      lcd.print("min: ");
      lcd.print(seczumin(sekunden));
      lcd.print(" sec: ");
      lcd.print(seczusec(sekunden));
      delay(250);
    }
  }
  if (valstopbutton == LOW){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Abbruch");
    playSingleSound (NOTE_G3, 250);
    playSingleSound (NOTE_C3, 250);
    lcd.clear();
    myservo.write(62);
  }
  else {
   playZelda1();
   myservo.write(62);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Tee ist fertig!");
   playZelda2();
   delay(7000);
   lcd.clear();
   lcd.setCursor(0,0); 
  }    
}
  


