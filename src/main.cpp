
#include <Arduino.h>

//Define the pins used by the displays
#define Aff1 2
#define Aff2 3
#define Aff3 4
#define Aff4 5

//Define the pins used by each segment
#define segA 30
#define segB 31
#define segC 32
#define segD 33
#define segE 34
#define segF 35
#define segG 36
#define dotP 37

//Define the pin used by the buzzer
#define speaker 8


//Binary representation of each digit
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

//array with the binary representation of each digit
const byte digits[10] = {dig0,dig1,dig2,dig3,dig4,dig5,dig6,dig7,dig8,dig9};

//array with each segment reversed
const byte segments[7] = {segG,segF,segE,segD,segC,segB,segA};

//Define the variables used by the program
const String time_at_compilation = __TIME__; //__TIME__ is a macro that returns the time at compilation
byte hours; //Used in the program to store the hour part of the time
byte minutes; //Used in the program to store the minute part of the time
int fullTime; //Used in the program to store the full time as an integer
const byte secondsInAMinute = 60; //Variable used by the program to know when to change the time
const int blinkCountdown = 800; //Blink the decimal point every XX milliseconds
unsigned long time_now_1 = 0; //Used for the first countdown
unsigned long time_now_2; //Used for the second countdown

bool dpState = false; //Used to store the state of the decimal point


/* 
Function to display a digit on a display.
Parameters :
- display : the display to display the digit on
- digit : the digit to display
- dp : the state of the decimal point
*/
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
  
  for(int i=6; i>=0; i--){
    digitalWrite(segments[i], !bitRead(digits[digit], i));
  }
  digitalWrite(dotP, dp);
}

/*
Function to display a 4-digit number on the displays.
Parameters :
- number : the number to display
- wait_delay : the delay between each refresh of the display
- time : boolean to indicate if a time format is required
*/
void displayNumber(int number, int wait_delay, bool time){
  
  byte chiffres[4];

  //Split the number into 4 digits
  chiffres[0] = number/1000;
  chiffres[1] = (number%1000)/100;
  chiffres[2] = (number%100)/10;
  chiffres[3] = (number%10);

  //Display each digit
  displayDigit(1,chiffres[0], false);
  delayMicroseconds(wait_delay);
  displayDigit(2,chiffres[1], time);
  delayMicroseconds(wait_delay);
  displayDigit(3,chiffres[2], time);
  delayMicroseconds(wait_delay);
  displayDigit(4,chiffres[3], false);
  delayMicroseconds(wait_delay);
}


/*
Initialization of the program.
*/
void setup() {

  Serial.begin(115200); //Start the serial port at 115200 bauds

  for(int i=Aff1; i<=Aff4; i++){ //Initialize the displays
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }

  for(int i=segA; i<=dotP; i++){ //Initialize the segments
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  pinMode(speaker, OUTPUT);   //Initialize the speaker
  digitalWrite(speaker, HIGH); //Turn the speaker off

  hours = time_at_compilation.substring(0,2).toInt();  //Get the hour part of the time at compilation
  minutes = time_at_compilation.substring(3,5).toInt(); //Get the minute part of the time at compilation
  fullTime = (hours*100)+minutes; //Convert the time to an integer
}

void loop() {
  if(millis()/1000 >= time_now_1 + secondsInAMinute){ //If a minute has passed
        time_now_1 += secondsInAMinute; //Restart the countdown
        minutes++; //Increment the minutes
        if(minutes >= 60){  //If the minutes are greater than 59
          minutes = 0; //Reset the minutes
          hours++; //Increment the hours
        }
        if(hours > 24){ //If the hours are greater than 23
          hours=0; //Reset the time;
        }
  }
  if(millis() >= time_now_2 + blinkCountdown){ //If the blink countdown has passed
    time_now_2 += blinkCountdown; dpState=!dpState; //Restart the countdown and change the state of the decimal point
  }
  fullTime = (hours*100)+minutes; //Convert the time to an integer
  displayNumber(fullTime, 3500, dpState); //Display the time
}