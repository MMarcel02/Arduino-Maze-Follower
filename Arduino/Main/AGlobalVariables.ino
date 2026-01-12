// Has A at front so we know we compile this right after Main

// Pins that describe the WIFI module. Pins used from the Arduino Wi-Fi library
const int WINC_CS  = 8, WINC_IRQ = 7, WINC_RST = 4, WINC_EN = 2;

// Wifi name and password
const char ssid[] = "Team 36";
const char pass[] = "Team36Rules";     // >= 8 chars for WPA2

// Assign PWM (speed) and DIR (direction) pins for each motor
const int FL_PWM = 6,  FL_DIR = 5;     // Front Left Motor
const int FR_PWM =9, FR_DIR=10;        // Front Right 
const int BL_PWM = A4, BL_DIR = A5;    // Back Left
const int BR_PWM=11, BR_DIR = 12;      // Back Right

// IR Digital Sensor Pins
const int IR_DIGITAL_LEFT_SENSOR_PIN = A0; 
const int IR_DIGITAL_RIGHT_SENSOR_PIN = A2; 

// Pins for encoders
const int leftEncA = A3; // Pin A3
const int leftEncB = A1; // Pin A1
const int rightEncA = 21; // Pin SCL
const int rightEncB = 20; // Pin SDA

// Ultrasonic pins
const int TRIG_PIN = 0;
const int ECHO_PIN = 1; 

// Robot physical constants
const float WHEEL_RADIUS = 3.25;   // 3.25 cm 
const float TRACK_WIDTH  = 33;     // 22 cm (multiplied by 1.5x to account for slippage)
const int TICKS_PER_REV  = 225;      // Ticks for one full spin
const float DISTANCE_PER_TICK = (2 * PI * WHEEL_RADIUS) / TICKS_PER_REV; 

// Robot Position and Speed
int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed*0.8;
int motorSpeedOutsideLineFollow = motorSpeed;

// Ultrasonic
float duration, ultrasonicDistance;  

// We have HC-SR04 sensor, 60 ms is needed to avoid bad data from ghost echoes
const int ULTRASONIC_READ_INTERVAL = 60;
unsigned long lastUltrasonicReadTime = 0;

// IR
int leftDigitalIRReading, rightDigitalIRReading;

float robotAngle = PI/2; // angle in Radians (initial is 90 degrees for facing up on the map)
float totalDistance = 0; // in centimetres

// Target values  
float targetAngleStart = 0;
float targetAngleEnd = 0;
float targetTotalDistance = 0;

long stateStartTime = 0;
long lastLeftBlackTime = 0;
long lastRightBlackTime = 0;

boolean isMoving;

const unsigned int SmallStopAfterSensorDetection = 100;
const unsigned int BlindTime = 150;
const unsigned int ObjectFoundTime = 500;
const unsigned long JUNCTION_TIME_DELTA = 200;

// These states are here because they are also passed to the GUI
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
RobotMovementState currentMovementState = STOPPED;

enum RobotControlState {
  MANUAL,
  LINE_FOLLOW_BANGBANG,
  SOLVE_MAZE_1,
  SOLVE_MAZE_2,
  LOST_ROBOT,
  EMERGENCY_STOP,
  U_TURN,
  PARKING_IN_BOX
};
RobotControlState currentControlState = MANUAL;

enum mazeState {
  FOLLOW_LINE,
  TURNING_LEFT, 
  BLIND_TURN,   
  TURNING_RIGHT,
  OBJECT_DETECTED,
  TURNING_180_DEGREES,
  AFTER_180_RIGHT_SENSOR_SEARCH,
  JUNCTION_FOUND,
  DRIVE_THROUGH_INTERSECTION, 
  SCAN_LEFT_FOR_LINE,         
  ALIGN_EXTRA_LEFT,           
  RECOVER_RIGHT_FIND_BLACK,   
  RECOVER_RIGHT_FIND_WHITE,   
  LOST_ROBOT1
};
mazeState mazeState = FOLLOW_LINE;

enum LostRobotState {
  SEARCHING_FOR_THE_LINE,
  FOUND_THE_LINE,
};
LostRobotState lostRobotState = SEARCHING_FOR_THE_LINE;

enum EmergencyStopState {
  BANG_LINE_FOLLOWING,
  STOPPED
};
EmergencyStopState emergencyStopState = BANG_LINE_FOLLOWING;
int emergencyStopDistance = 20;

enum ParkingBoxState {
    APPROACHING_PARKING_BOX,
    PARKING_TURNING_LEFT,
    PARKING_TURNING_RIGHT,
    PARKING_BLIND_TURN,
    START_OF_PARKING_BOX,
    SELF_ALIGN_90_DEGREES,
    FIND_THE_END_OF_PARKING_BOX,
    END_OF_PARKING_BOX
};
ParkingBoxState parkingBoxState = APPROACHING_PARKING_BOX;

    
    
