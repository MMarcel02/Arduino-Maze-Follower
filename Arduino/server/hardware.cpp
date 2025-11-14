#include <Arduino.h>
#include "hardware.h"

// --- Variables ---
int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed - 20;
char lastMotionCmd = 'x';      // Stores the last direction command (e.g., 'f' for forward)

float getDistanceCM() {
  long duration;
  float distance;
  
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2.0;

  return distance;
}

// --- Read both IR sensor ---
String getIRoutput() {
  int leftDigital = digitalRead(IR_Left_Digital);
  int rightDigital = digitalRead(IR_Right_Digital);
  int LeftAnalog = analogRead(IR_Left_Analog);
  int rightAnalog = analogRead(IR_Right_Analog);

  String IRdata = "Left IR -> D: " + String(leftDigital) + " | A: " + String(leftAnalog);
  IRdata += " || Right IR -> D: " + String(rightDigital) + " | A: " + String(rightAnalog);
  return IRdata;
}

// --- Function to Drive a Motor ---
// 'speed' determines how fast, 'forward' determines direction
void setMotor(int pwm, int dir, int speed, bool forward) {
  digitalWrite(dir, forward ? HIGH : LOW);  // Set direction
  analogWrite(pwm, speed);                  // Set speed using PWM
}

// --- Setup Function for Each Motor ---
// Configures the direction and PWM pins for a motor
void setupMotor(int pwm, int dir) {
  pinMode(pwm, OUTPUT);  // Set PWM pin as output
  pinMode(dir, OUTPUT);  // Set direction pin as output
}

// --- Arduino Setup Function ---
// Runs once when the board powers up or resets
void setupAllMotors() {    
  setupMotor(FL_PWM, FL_DIR);
  setupMotor(FR_PWM,FR_DIR);
  setupMotor(BL_PWM,BL_DIR);
  setupMotor(BR_PWM,BR_DIR);
}

void setSpeed(int newSpeed) {
  motorSpeed = newSpeed;
  motorTurningSpeed = motorSpeed - 20;
}

void stopAllMotors() {
  setMotor(FL_PWM, FL_DIR, 0, true);
  setMotor(FR_PWM, FR_DIR, 0, true);
  setMotor(BL_PWM, BL_DIR, 0, true);
  setMotor(BR_PWM, BR_DIR, 0, true); 
}

void moveForward() {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}

void moveBackward() {
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}

void moveRight () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}

void moveLeft () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void turnOnSpotRight () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}
void turnOnSpotLeft () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void crabWalkRight () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void crabWalkLeft () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}