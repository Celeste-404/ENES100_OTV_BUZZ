#include "Arduino.h"
#include "Enes100.h"
#define PI 3.1415926535897932384626433832795

#define ULTRASONICTRIG_LEFT 8
#define ULTRASONICTRIG_FRONT 7
#define ULTRASONICTRIG_RIGHT 2
#define ULTRASONICECHO_LEFT A5
#define ULTRASONICECHO_FRONT A4
#define ULTRASONICECHO_RIGHT A3

// Pin Setup
// Motor Left:
int enA = 9;
int ln1 = 10;
int ln2 = 11;
// Motor Right:
int enB = 3;
int ln3 = 12;
int ln4 = 13;

// XY & Theta Coordinates
float t = 0;
float x = 0;
float y = 0;

// Ultrasonic Variables
long duration_front, cm_front;
long duration_left, cm_left;
long duration_right, cm_right;
bool turnLeftNext = true; // Variable to alternate turns


// ULTRASONIC SENSOR METHODS
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long cm = duration / 29 / 2;  // Convert to centimeters
  return cm;
}

void ultrasonicSensor_left(){
  digitalWrite(ULTRASONICTRIG_LEFT, LOW);
  delayMicroseconds(5);
  digitalWrite(ULTRASONICTRIG_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONICTRIG_LEFT, LOW);
  pinMode(ULTRASONICECHO_LEFT, INPUT);
  duration_left = pulseIn(ULTRASONICECHO_LEFT, HIGH);
  cm_left = (duration_left/2) / 29.1; 
  Serial.print(cm_left);
  Serial.print("cm   ");
}

// TURNING MECHANISM 
// Normalize angle to 0-360 degrees
float normalizeAngle(float angle) {
    angle = fmod(angle, 360);
    if (angle < 0) angle += 360;
    return angle;
}

// Decide turn direction (true = right, false = left)
bool turnDirection(float goalDegrees) {
    float current = normalizeAngle(Enes100.getTheta() * (180 / PI));
    float goal = normalizeAngle(goalDegrees);

    // Calculate the shortest turn direction
    float difference = normalizeAngle(goal - current);

    // Turn right if difference is <= 180, otherwise turn left
    return difference <= 180; // true = turn right, false = turn left
}

// Turn the robot to a specific angle
void turn(float goalDegrees) {
    float goal = normalizeAngle(goalDegrees);
    const float tolerance = 5;  // Tolerance range in degrees
    bool turnTo = turnDirection(goal);

    while (true) {
        float current = normalizeAngle(Enes100.getTheta() * (180 / PI));
        float difference = normalizeAngle(goal - current);
        // Diagnostic output for debugging
       // Enes100.println("Current: " + String(current) + " Goal: " + String(goal) + " Diff: " + String(difference));

        if (difference < tolerance || (360 - difference) < tolerance) {
            stopMotors();
            break;
        }

        // Decide turning direction and move
        if (turnTo) {
          if(difference>100){
            turnRight(500,200);
            continue;
          } else if(difference<=100 && difference>60){
            turnRight(300,100);
            continue;
          }  else if(difference<=60 && difference>30){
            turnRight(100,50);
          }

        } else {
            if(difference>100){
            turnLeft(500, 200);
            continue;
          } else if(difference<=100 && difference>60){
            turnLeft(300, 100);
            continue;
          }  else if(difference<=60 && difference>30){
            turnLeft(300, 50);

          }
        }

        // Stop if within tolerance
        if (difference < tolerance || (360 - difference) < tolerance) {
            stopMotors();
            break;
        }
        delay(500);  // Short delay to stabilize sensor reading

    }
    stopMotors();
}

//GET TO X/Y COORDINATE FUNCTIONS
void getToX(float goal){
  const float tolerance = 0.05;
  bool fwd = Enes100.getX() < goal? true : false;

  do{
    float currentX = Enes100.getX();
    float difference = abs(goal - currentX);

    if(difference<= tolerance){
      stopMotors();
      break;
    }

    if(fwd){
      if(difference>0.5){
        startMotorsFWD(300, 200);
        continue;
      } else if(difference<=0.5 && difference>0.2){
        startMotorsFWD(300,130);
        continue;
      } else if(difference<=0.2 && difference>0.05){
        startMotorsFWD(300,100);
        continue;
      }
    } else {
      if(difference>0.5){
        startMotorsBKWD(300, 200);
        continue;
      } else if(difference<=0.5 && difference>0.2){
        startMotorsBKWD(300,130);
        continue;
      } else if(difference<=0.2 && difference>0.05){
        startMotorsBKWD(300,100);
        continue;
      }
    }
  
  } while(true);
}

void getToY(float goal, bool fwd){
  const float tolerance = 0.05;
  if(fwd){
    Enes100.println("Going up!");
  } else{
    Enes100.println("Going down!");
  }

  do{
    float currentY = Enes100.getY();
    float difference = abs(goal - currentY);

    if(difference<= tolerance){
      stopMotors();
      break;
    }

    if(fwd){
      if(difference>0.5){
        startMotorsFWD(300, 200);
        continue;
      } else if(difference<=0.5 && difference>0.2){
        startMotorsFWD(300,130);
        continue;
      } else if(difference<=0.2 && difference>0.05){
        startMotorsFWD(300,100);
        continue;
      }
    } else {
      if(difference>0.5){
        startMotorsBKWD(300, 200);
        continue;
      } else if(difference<=0.5 && difference>0.2){
        startMotorsBKWD(300,130);
        continue;
      } else if(difference<=0.2 && difference>0.05){
        startMotorsBKWD(300,100);
        continue;
      }
    }
  } while(true);

  stopMotors();
}


// Motor control functions
void turnLeft(int time, int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed+30);
    digitalWrite(ln1, HIGH);
    digitalWrite(ln2, LOW);
    digitalWrite(ln3, LOW);
    digitalWrite(ln4, HIGH);
    delay(time);
    delay(200);
}

void turnRight(int time, int speed) {
    analogWrite(enA, speed);
    analogWrite(enB, speed+30);
    digitalWrite(ln1, LOW);
    digitalWrite(ln2, HIGH);
    digitalWrite(ln3, HIGH);
    digitalWrite(ln4, LOW);
    delay(time);
    delay(200);
}


void startMotorsFWD(int time, int power) {
    analogWrite(enA, power + 20);
    analogWrite(enB, power);
    digitalWrite(ln1, LOW);
    digitalWrite(ln2, HIGH);
    digitalWrite(ln3, LOW);
    digitalWrite(ln4, HIGH);
    delay(time);
}

void startMotorsFWDINDEF() { 
    analogWrite(enA, 180);
    analogWrite(enB, 150);
    digitalWrite(ln1, LOW);
    digitalWrite(ln2, HIGH);
    digitalWrite(ln3, LOW);
    digitalWrite(ln4, HIGH);
}

void startMotorsBKWD(int time, int power) {
    analogWrite(enA, power + 20);
    analogWrite(enB, power);
    digitalWrite(ln1, HIGH);
    digitalWrite(ln2, LOW);
    digitalWrite(ln3, HIGH);
    digitalWrite(ln4, LOW);
    delay(time);
}


void stopMotors() {
    digitalWrite(ln1, LOW);
    digitalWrite(ln2, LOW);
    digitalWrite(ln3, LOW);
    digitalWrite(ln4, LOW);
}
/*
#define ULTRASONICTRIG_LEFT 8
#define ULTRASONICTRIG_FRONT 7
#define ULTRASONICTRIG_RIGHT 2
#define ULTRASONICECHO_LEFT A5
#define ULTRASONICECHO_FRONT A4
#define ULTRASONICECHO_RIGHT A3
*/

void obstacleLoop(){
  stopMotors();
  //front measureDistance
  if(measureDistance(7,A4)>=30){
      delay(500);
      Enes100.println("It is safe to continue forward");
      delay(500);
      startMotorsFWD(300,200);
  } else{
    if(measureDistance(8, A5) < measureDistance(2,A3)){ //more distance on the right 
      delay(500);
      Enes100.println("Turning to the right\n");
      delay(1000);
      turn(270);
    } else if(measureDistance(8, A5) > measureDistance(2,A3)){
      delay(500);
      Enes100.println("Turning to the left\n");
      delay(1000);
      turn(90);
    } else {
      turn(90);
      startMotorsFWD(300,200);
    }
  }
}

void setup() {
    Enes100.begin("Team Dewaltt", SEED, 566, 5, 4);
    Enes100.isConnected();
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(ln2, OUTPUT);
    pinMode(ln1, OUTPUT);
    pinMode(ln3, OUTPUT);
    pinMode(ln4, OUTPUT);
    pinMode(ULTRASONICTRIG_LEFT, OUTPUT);
    pinMode(ULTRASONICECHO_LEFT, INPUT);
    pinMode(ULTRASONICTRIG_FRONT, OUTPUT);
    pinMode(ULTRASONICECHO_FRONT, INPUT);
    pinMode(ULTRASONICTRIG_RIGHT, OUTPUT);
    pinMode(ULTRASONICECHO_RIGHT, INPUT);


    t = Enes100.getTheta() * (180 / PI);
    x = Enes100.getX();
    y = Enes100.getY();

    stopMotors();
    delay(2000);
  
    if (Enes100.getY() < 1.00) {
        Enes100.println("Bottom spawn, turning to 90°.");
        turn(90);
        delay(300);
        getToY(1.55, true);
        getToY(1.00, false);
        turn(90);
        Enes100.println("Mission Loop Complete!");
    } else {
        Enes100.println("Top spawn, turning to 270°.");
        turn(270);
        delay(300);
        getToY(0.55, true);
        getToY(1.00, false);
        turn(90);
        Enes100.println("Mission Loop Complete!");
    }
    
}

void loop() {
    // obstacleLoop();
}
