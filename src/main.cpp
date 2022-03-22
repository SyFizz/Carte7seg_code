
#include <Arduino.h>

//On définit les branchements
#define Aff1 2
#define Aff2 3
#define Aff3 4
#define Aff4 5

#define segA 30
#define segB 31
#define segC 32
#define segD 33
#define segE 34
#define segF 35
#define segG 36
#define dotP 37

#define speaker 8

const byte dig0 = 0b0000001;
const byte dig1 = 0b1001111;
const byte dig2 = 0b0010010;
const byte dig3 = 0b0000110;
const byte dig4 = 0b1001100;
const byte dig5 = 0b0100100;
const byte dig6 = 0b0100000;
const byte dig7 = 0b0001111;
const byte dig8 = 0b0000000;
const byte dig9 = 0b0000100;


const byte digits[10] = {dig0,dig1,dig2,dig3,dig4,dig5,dig6,dig7,dig8,dig9};

const String time_at_compilation = __TIME__;
int hours;
int minutes;
int fullTime;
int secondsInAMinute = 60;
int blinkCountdown = 800;
unsigned long time_now_1 = 0;
unsigned long time_now_2;

bool dpState = false;

void displayDigit(int display, int digit, bool dp){
  switch(display){
    case 1:
      digitalWrite(Aff1,LOW); 
      digitalWrite(Aff2,HIGH); 
      digitalWrite(Aff3,HIGH); 
      digitalWrite(Aff4,HIGH);
      break;
    case 2:
      digitalWrite(Aff1,HIGH); 
      digitalWrite(Aff2,LOW); 
      digitalWrite(Aff3,HIGH); 
      digitalWrite(Aff4,HIGH);
      break;
    case 3:
      digitalWrite(Aff1,HIGH); 
      digitalWrite(Aff2,HIGH); 
      digitalWrite(Aff3,LOW); 
      digitalWrite(Aff4,HIGH);
      break;
    case 4: 
      digitalWrite(Aff1,HIGH); 
      digitalWrite(Aff2,HIGH); 
      digitalWrite(Aff3,HIGH); 
      digitalWrite(Aff4,LOW);
      break;
    default:
      Serial.println("Valeur d'afficheur incorrecte");
      break;
  }
  digitalWrite(segA, !bitRead(digits[digit], 6));
  digitalWrite(segB, !bitRead(digits[digit], 5));
  digitalWrite(segC, !bitRead(digits[digit], 4));
  digitalWrite(segD, !bitRead(digits[digit], 3));
  digitalWrite(segE, !bitRead(digits[digit], 2));
  digitalWrite(segF, !bitRead(digits[digit], 1));
  digitalWrite(segG, !bitRead(digits[digit], 0));
  digitalWrite(dotP, dp);
}

void displayNumber(int number, int wait_delay, bool time){
  
  byte chiffres[4];

  chiffres[0] = number/1000;
  chiffres[1] = (number%1000)/100;
  chiffres[2] = (number%100)/10;
  chiffres[3] = (number%10);
  displayDigit(1,chiffres[0], false);
  delayMicroseconds(wait_delay);
  displayDigit(2,chiffres[1], time);
  delayMicroseconds(wait_delay);
  displayDigit(3,chiffres[2], time);
  delayMicroseconds(wait_delay);
  displayDigit(4,chiffres[3], false);
  delayMicroseconds(wait_delay);
}

//Fonction d'initialisation de l'arduino
void setup() {

  Serial.begin(115200);

  for(int i=Aff1; i<=Aff4; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }

  for(int i=segA; i<=dotP; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  pinMode(speaker, OUTPUT);
  digitalWrite(speaker, HIGH);

  hours = time_at_compilation.substring(0,2).toInt();
  minutes = time_at_compilation.substring(3,5).toInt();
  fullTime = (hours*100)+minutes;
}

void loop() {
  if(millis()/1000 >= time_now_1 + secondsInAMinute){
        time_now_1 += secondsInAMinute;
        minutes++;
        if(minutes >= 60){
          minutes = 0;
          hours++;
        }
        if(hours > 24){
          hours=0;
        }
  }
  if(millis() >= time_now_2 + blinkCountdown){ time_now_2 += blinkCountdown; dpState=!dpState; }
  fullTime = (hours*100)+minutes;
  Serial.println(dpState);
  displayNumber(fullTime, 3500, dpState);
}