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

//BASIC MOVEMENT METHODS:
void turn1(float goal1){
  float current = Enes100.getTheta() * (180/PI);
  float line = 0; //creates a variable for the 180 degree opposing position on the wheel used for calculations
  float goal = goal1;
  if(current < 0)
  {
    current = 360 - abs(current);
  } // this sequence converts the current position on the circle into 0 - 360 rather than -180 - +180 if the goal position is in the bottom half
  if(goal < 0)
  {
    goal = 360 - abs(current);
  } // this does the same as the previosu one, just for the goal as well. 
  if(current > 180){
    line = current - 180;
  }
  else{
    line = current + 180;
  } // that sequence runs through and finds the direction to turn in based on the given direction
  bool goRight = ((goal < current) || (goal > line) || (goal == line))? true:false; // this logic assigns the direction to the goright boolean, false means go left 
 
  
}
void turn(float goal){
  float current = Enes100.getTheta() * (180/PI);
  while(abs(current - goal) > 5){
    float current = Enes100.getTheta() * (180/PI);
    bool negativeTheta = Enes100.getTheta()>0? false : true;
    int sign = negativeTheta? -1: 1;

    if(abs(current-goal)>75){
     turnLeft();
    } else if(abs(current-goal)<=75 && abs(current-goal)>40){
      turnLeft();
    } else if(abs(current-goal)<=40 && abs(current-goal)>15){
      turnLeft();
    } else if(abs(current-goal)<=15 && abs(current-goal)>7){
      turnLeft();
    } 
    Enes100.println(Enes100.getTheta());
    if(abs(current - goal) < 5){
      break;
    }
  }
}

/*
void Move(float distance, bool fwd){
  if(fwd){
    float initialPos = Enes100.getY()
    delay(1000);
    do{ 

    } while()

  } else {

  }

}
*/

/*
//ONLY USE '-n' values as goal
void turnNegative(float goal){
  float current = Enes100.getTheta() * (180/PI);
  do{
    float current = Enes100.getTheta() * (180/PI);
    bool negativeTheta = Enes100.getTheta()>0? false : true;
    int sign = negativeTheta? -1: 1;

    if(current > 0){
    

    } else if(current < 0){
      
    } else{
      Enes100.print("FUCK");
    }

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
*/

//MOTOR UTILITY METHODS
void turnLeft(){
  analogWrite(enA, 100);
  analogWrite(enB, 100);
  digitalWrite (ln1, HIGH);
  digitalWrite (ln2, LOW);
  digitalWrite (ln3, LOW);
  digitalWrite (ln4, HIGH);
  delay(500);
  stopMotors();

}

void startMotorsFWD(int time){
  Enes100.println("going forward");
  analogWrite(enA, 230);
  analogWrite(enB, 200);
  digitalWrite (ln1, LOW);
  digitalWrite (ln2, HIGH);
  digitalWrite (ln3, LOW);
  digitalWrite (ln4, HIGH);
  delay(time);

}

void startMotorsBKWD(int time){
  analogWrite(enA, 200);
  analogWrite(enB, 230);
  digitalWrite (ln1, HIGH);
  digitalWrite (ln2, LOW);
  digitalWrite (ln3, HIGH);
  digitalWrite (ln4, LOW);
  delay(time);
 }


void stopMotors(){
  digitalWrite(ln1, LOW);
  digitalWrite(ln2, LOW);
  digitalWrite(ln3, LOW);
  digitalWrite(ln4, LOW);
}

// SPECIFIC MOVEMENT METHODS

/*
void goToMission(){
  int y = Enes100.getY();
  if(y<1.00){
    Enes100.print("Going to mission site (From bottom spawn)");
    while(Enes100.getY()-1.45 > 0.05){
      if(Enes100.getY()<1.00){
        startMotorsFWD(500);
        delay(500);
       
      } else if(Enes100.getY()<=1.30 || Enes100.getY()>=1.00){
        startMotorsFWD(300);
        delay(500);
        
      } else if(Enes100.getY()<1.40 || Enes100.getY()>=1.30){
        startMotorsFWD(200);
        delay(500);
       
      }
    }   
  } else{
     Enes100.print("Going to mission site (From bottom spawn)");
     while(Enes100.getY()- 0.60 > 0.05){
      if(Enes100.getY()>1.00){
        startMotorsBKWD(500);
        delay(500);
      
      } else if(Enes100.getY()<=1.00 || Enes100.getY()>0.70){
        startMotorsBKWD(300);
        delay(500);
      
      } else if(Enes100.getY()<=0.70 || Enes100.getY()>0.50){
        startMotorsBKWD(200);
        delay(500);
    
      }
    }
  }
}
*/
//TESTS
 void missionCycleBot(){
    Enes100.println("Going to mission site (Bottom Spawn)");
    turn(90);
    stopMotors();
    delay(200);
    startMotorsFWD(9000);
    stopMotors();
  }

  void missionCycleTop(){
    Enes100.println("Going to mission site (Top Spawn)");
    turn(270);
    stopMotors();
    delay(200);
    startMotorsBKWD(9000);
    stopMotors();
  }

void setup() {
  Enes100.begin("Team Dewaltt", SEED, 566 , 5, 4);
  Enes100.isConnected();
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(ln2, OUTPUT);
  pinMode(ln1, OUTPUT);
  pinMode(ln2, OUTPUT);
  pinMode(ln3, OUTPUT);
  pinMode(ln4, OUTPUT);

  t= Enes100.getTheta() * (180/PI);
  x= Enes100.getX();
  y= Enes100.getY();
 
  
  stopMotors();
  delay(2000);

  if(Enes100.getY()<1.00){
    Enes100.println("Hey I'm on the bottom -  my Y is: ");
    missionCycleBot();
  } else {
    Enes100.println("Hey I'm on the top -  my Y is: ");
    missionCycleTop();
  }
}

void loop(){
    t= Enes100.getTheta() * (180/PI);
    x= Enes100.getX();
    y= Enes100.getY();

    
    /*
    if(y < 1){
      Enes100.println("Turning towards mission site");
      turn(90);
      stopMotors();
      delay(200);
      startMotorsFWD(5000);
      
    
    } else {
      Enes100.println("Turning towards mission site");
      turn(90);
      stopMotors();
      delay(200);
      startMotorsBKWD(10000); 
      delay(1000);
    }
    
*/
}

