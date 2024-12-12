#include "Arduino.h"
#include "Enes100.h"
#define PI 3.1415926535897932384626433832795

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

// Motor control functions
void turnLeft(int time) {
    analogWrite(enA, 200);
    analogWrite(enB, 200);
    digitalWrite(ln1, HIGH);
    digitalWrite(ln2, LOW);
    digitalWrite(ln3, LOW);
    digitalWrite(ln4, HIGH);
    delay(time);
    stopMotors();
}

void turnRight(int time) {
    analogWrite(enA, 200);
    analogWrite(enB, 230);
    digitalWrite(ln1, LOW);
    digitalWrite(ln2, HIGH);
    digitalWrite(ln3, HIGH);
    digitalWrite(ln4, LOW);
    delay(time);
    stopMotors();
}

void startMotorsFWD(int time) {
    analogWrite(enA, 210);
    analogWrite(enB, 190);
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

// Turn the robot to a specific angle
void turn(float goalDegrees) {
    float goal = normalizeAngle(goalDegrees);
    const float tolerance = 2;  // Tolerance range in degrees
    bool turnTo = turnDirection(goal);

    while (true) {
        float current = normalizeAngle(Enes100.getTheta() * (180 / PI));
        float difference = normalizeAngle(goal - current);
        // Diagnostic output for debugging
        Enes100.println("Current: " + String(current) + " Goal: " + String(goal) + " Diff: " + String(difference));

        if (difference < tolerance || (360 - difference) < tolerance) {
            stopMotors();
            break;
        }

        // Decide turning direction and move
        if (turnTo) {
          if(difference>100){
            turnRight(500);
            continue;
          } else if(difference<=100 && difference>60){
            turnRight(300);
            continue;
          }  else if(difference<=60 && difference>30){
            turnRight(100);
          }

        } else {
            if(difference>100){
            turnLeft(500);
            continue;
          } else if(difference<=100 && difference>60){
            turnLeft(300);
            continue;
          }  else if(difference<=60 && difference>30){
            turnLeft(300);
            continue;
          }
        }

        // Stop if within tolerance
        if (difference < tolerance || (360 - difference) < tolerance) {
            stopMotors();
            break;
        }
        delay(500);  // Short delay to stabilize sensor reading

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

    t = Enes100.getTheta() * (180 / PI);
    x = Enes100.getX();
    y = Enes100.getY();

    stopMotors();
    delay(2000);

    if (Enes100.getY() < 1.00) {
        Enes100.println("Bottom spawn, turning to 90°.");
        turn(90);
        startMotorsFWD(6000);
        stopMotors();
        turn(0);
        startMotorsFWD(12000);
    } else {
        Enes100.println("Top spawn, turning to 270°.");
        turn(0);
        startMotorsFWD(25000);
        stopMotors();
    }
}

void loop() {
    // Add any loop logic if needed
}
