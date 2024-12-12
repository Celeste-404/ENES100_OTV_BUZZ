#include "Arduino.h"
#include "Enes100.h"
#define PI 3.1415926535897932384626433832795

//Pin Setup 
//Motor Left:
int enA = 9;
int ln1 = 10;
int ln2 = 11;
//Motor Right:
int enB = 3;
int ln3 = 12;
int ln4 = 13;

// XY & Theta Coordinates
float t = 0;
float x = 0;
float y = 0;

//Utility Methods:
void turn(float goal){
  float current = Enes100.getTheta() * (180/PI);
  do{
    float current = Enes100.getTheta() * (180/PI);
    bool negativeTheta = Enes100.getTheta()>0? false : true;
    int sign = negativeTheta? -1: 1;

    if(abs(current-goal)>75){
     turnLeft(750);
    } else if(abs(current-goal)<=75 && abs(current-goal)>40){
      turnLeft(500);
    } else if(abs(current-goal)<=40 && abs(current-goal)>15){
      turnLeft(250);
    } else if(abs(current-goal)<=15 && abs(current-goal)>7){
      turnLeft(100);
    } 
  
  }while(abs(current - goal) > 5);
}

void turnLeft(int time){
  analogWrite(enA, 100);
  analogWrite(enB, 100);
  digitalWrite (ln1, HIGH);
  digitalWrite (ln2, LOW);
  digitalWrite (ln3, LOW);
  digitalWrite (ln4, HIGH);
  delay(500);
  stopMotors();
}

void stopMotors(){
  digitalWrite(ln1, LOW);
  digitalWrite(ln2, LOW);
  digitalWrite(ln3, LOW);
  digitalWrite(ln4, LOW);
}

void setup() {
  Enes100.begin("Team Dewaltt", SEED, 566 , 8, 7);
  Enes100.isConnected();
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(ln2, OUTPUT);
  pinMode(ln1, OUTPUT);
  pinMode(ln2, OUTPUT);
  pinMode(ln3, OUTPUT);
  pinMode(ln4, OUTPUT);
  
  stopMotors();
 
   
  t= Enes100.getTheta() * (180/PI);
  x= Enes100.getX();
  y= Enes100.getY();
 
  //mission site
  //first conditional checking starting location
  
}
void loop(){
    t= Enes100.getTheta() * (180/PI);
    x= Enes100.getX();
    y= Enes100.getY();
    if(y < 1){
    turn(90);
    }
  
}

