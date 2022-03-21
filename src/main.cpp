
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

//Fonction d'initialisation de l'arduino
void setup() {


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

  
}

void loop() {
  
}