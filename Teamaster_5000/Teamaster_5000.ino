#include <LiquidCrystal.h>
#include <Servo.h>
#include "pitches.h"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int startButtonPin = 6;
const int stopButtonPin = 10;
const int speakerPin = 8;
const int sensorPin = A0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo teaServo;


int potiToSec (int value){
  float ergebnis = value;
  ergebnis = ergebnis / 1023 * 900;
  value = ergebnis;
  return value;
}

int secToMin (int value){
  return value / 60;
}
int secToSecRemainder (int value){
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
    tone(speakerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
}
void playSingleSound(int Tone, int duration) {
  tone(speakerPin, Tone, duration);
  delay(duration * 1.30);
  noTone(speakerPin);
}


void setup() {
  createCustomChars();
  lcd.begin(16, 2);
  lcd.print("Teamaster 5000");
  playWindows();
  lcd.clear();  
  pinMode(startButtonPin, INPUT_PULLUP);
  teaServo.attach(9);
  teaServo.write(62);
  pinMode(stopButtonPin, INPUT_PULLUP);
}

void loop() {
  int selectedSeconds = 0;
  int sensorValue = 0;
  int valStartButton = 0;
  int valStopButton = 0;
  int seconds = 0;
  lcd.setCursor(0, 0);
  lcd.print("Ziehzeit:");
  valStartButton = digitalRead(startButtonPin);
  while(valStartButton == HIGH){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    sensorValue = analogRead(sensorPin);
    seconds = potiToSec(sensorValue);
    lcd.print("min: ");
    lcd.print(secToMin(seconds));
    lcd.print(" sec: ");
    lcd.print(secToSecRemainder(seconds));
    delay(250);
    valStartButton = digitalRead(startButtonPin);
  }
  selectedSeconds = seconds;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("Bereit");
  playSingleSound (NOTE_C3, 250);
  playSingleSound (NOTE_G3, 250);
  playSingleSound (NOTE_C4, 250);
  teaServo.write(2);
  delay(15);
  valStopButton = digitalRead(stopButtonPin);
  int counter = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Restzeit: ");
  lcd.print(secToMin(seconds));
  lcd.print(":");
  lcd.print(secToSecRemainder(seconds));
  lcd.setCursor(0,1);
  writeProgressbar(getPercentage(selectedSeconds,seconds));
  lcd.setCursor(12,1);
  lcd.print(getPercentage(selectedSeconds,seconds));
  lcd.print("%");
  while(seconds > 0 and valStopButton == HIGH){
    if (counter < 3) {
      counter = counter + 1;
      valStopButton = digitalRead(stopButtonPin);
      delay(250);
    }
    else {
      counter = 0;
      seconds = seconds - 1;
      valStopButton = digitalRead(stopButtonPin);
      //lcd.setCursor(0,1);
      lcd.clear();   
      lcd.setCursor(0,0);
      lcd.print("Restzeit: ");
      lcd.print(secToMin(seconds));
      lcd.print(":");
      if (sectoSecRemainder(seconds) < 10) {
        lcd.print("0");
      }
      lcd.print(secToSecRemainder(seconds));
      lcd.setCursor(0,1);
      writeProgressbar(getPercentage(selectedSeconds,seconds));
      lcd.setCursor(12,1);
      lcd.print(getPercentage(selectedSeconds,seconds));
      lcd.print("%");
      delay(250);
    }
  }
  if (valStopButton == LOW){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Abbruch");
    playSingleSound (NOTE_G3, 250);
    playSingleSound (NOTE_C3, 250);
    lcd.clear();
    teaServo.write(62);
  }
  else {
   playZelda1();
   teaServo.write(62);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Tee ist fertig!");
   playZelda2();
   delay(7000);
   lcd.clear();
   lcd.setCursor(0,0); 
  }    
}

void createCustomChars() {
  byte progressbar0[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  };
  byte progressbar1[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  };
  byte progressbar2[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  };
  byte progressbar3[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  };
  byte progressbar4[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  };
  byte progressbar5[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  };
  lcd.createChar(0, progressbar0);
  lcd.createChar(1, progressbar1);
  lcd.createChar(2, progressbar2);
  lcd.createChar(3, progressbar3);
  lcd.createChar(4, progressbar4);
  lcd.createChar(5, progressbar5);
}

int getPercentage(int maximum, int current) {
  float fCurrent = current;
  float fMaximum = maximum;
  return 100 - ((fCurrent / fMaximum) * 100);
}
void writeProgressbar(int percentage) {
  int filledChars = percentage / 10;
  int remainder = (percentage % 10) / 2;
  for (int i=0; i < filledChars; i++) {
    lcd.write(byte(5));
  }
  lcd.write(byte(remainder)); 
}

