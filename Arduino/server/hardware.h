#pragma once

#include <Arduino.h>

// --- Motor Pin Definitions ---
// Assign PWM (speed) and DIR (direction) pins for each motor
constexpr int FL_PWM =  6, FL_DIR =  5;     // Front Left Motor
constexpr int FR_PWM =  9, FR_DIR = 10;        // Front Right 
constexpr int BL_PWM = A4, BL_DIR = A5;    // Back Left
constexpr int BR_PWM = 11, BR_DIR = 12;      // Back Right

// --- Ultrasonic Sensor Pins ---
constexpr int trigPin = 3;
constexpr int echoPin = 2;

// --- IR Sensor Pins ---
constexpr int IR_Left_Digital = A0;
constexpr int IR_Left_Analog  = A1;
constexpr int IR_Right_Digital = A2;
constexpr int IR_Right_Analog  = A3;

// --- Variables ---
extern int motorSpeed;           // Default speed for all motors (range: 0–255)
extern int motorTurningSpeed;
extern char lastMotionCmd;      // Stores the last direction command (e.g., 'f' for forward)

// --- Read both IR sensor ---
String getIRoutput();

float getDistanceCM();

void setMotor(int pwm, int dir, int speed, bool forward);
void setupMotor(int pwm, int dir);
void setupAllMotors();
void setSpeed(int newSpeed);
void stopAllMotors();
void moveForward();
void moveBackward();
void moveRight();
void moveLeft();
void turnOnSpotRight();
void turnOnSpotLeft();
void crabWalkRight();
void crabWalkLeft();
