// This file contains all the global variables used in our project
// Has A at front so we know we compile this right after Main

// Pins that describe the WIFI module. Pins used from the Arduino Wi-Fi library
const int WINC_CS  = 8, WINC_IRQ = 7, WINC_RST = 4, WINC_EN = 2;

// Wifi name and password
const char ssid[] = "Team 36";
const char pass[] = "Team36Rules";     // >= 8 chars for WPA2

// Pins for the control of the motors
// Assign PWM (speed) and DIR (direction) pins for each motor
const int FL_PWM = 6,  FL_DIR = 5;     // Front Left Motor
const int FR_PWM =9, FR_DIR=10;        // Front Right 
const int BL_PWM = A4, BL_DIR = A5;    // Back Left
const int BR_PWM=11, BR_DIR = 12;      // Back Right

// IR Digital Sensor Pins
const int IR_DIGITAL_LEFT_SENSOR_PIN = A0; 
const int IR_DIGITAL_RIGHT_SENSOR_PIN = A2; 

// IR Analog Sensor Pins
const int IR_ANALOG_LEFT_SENSOR_PIN = A1;
const int IR_ANALOG_RIGHT_SENSOR_PIN = A3;

// Ultrasonic
float duration, distance;  

// Interval (in ms) which determines how often we read ultrasonic sensor
const int ULTRASONIC_READ_INTERVAL = 60;
// Timer variable which we cross check with SENSOR_SEND_INTERVAL
unsigned long lastUltrasonicReadTime = 0;

// IR
int leftDigitalIRReading, rightDigitalIRReading;
int leftAnalogIRReading, rightAnalogIRReading;

// Ultrasonic pins
const int TRIG_PIN = 0;
const int ECHO_PIN = 1; 

// These are the default values, GUI overwrites
bool emergencyStop = false;

int emergencyStopDistance = 20;

// enum assigns numbers to these words (less mistakes than using strings (typos) and easier and faster to compare numbers) 
enum RobotMovementState {
  STOPPED, // Compiler assigns this = 0
  FORWARD, // Compiler assigns this = 1
  BACKWARD,
  LEFT,
  RIGHT,
  TURN_SPOT_LEFT,
  TURN_SPOT_RIGHT,
  CW_LEFT,
  CW_RIGHT
};

enum RobotControlState {
  MANUAL,
  LINE_FOLLOW_BANGBANG,
  LINE_FOLLOW_PD,
  SOLVE_MAZE_1,
  SOLVE_MAZE_2,
  LOST_ROBOT,
  REVERSE_STRAIGHT,
  REVERSE_CORNER,
  THREE_POINT_TURN,
  U_TURN,
  PARKING_IN_BOX
};

// Default at start
RobotMovementState currentMovementState = STOPPED;
RobotControlState currentControlState = MANUAL;

// --- Variables ---
int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed - 20;
// Motor speed passed down from GUI
int motorSpeedOutsideLineFollow = motorSpeed;


int previousDir = 0;

// For PD algorithm
double sensitivity = 5.0;
double dampening = 1.0;

// Time it keeps turning after it stopped seeing a line
int lineMemoryLatency = 150;

