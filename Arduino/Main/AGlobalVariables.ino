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

// Robot physical constants
const float WHEEL_RADIUS = 3.35;   // 3.35 cm in meters
const float TRACK_WIDTH  = 33;     // 22 cm in meters (multiplied by 1.5x to account for slippage)
const int TICKS_PER_REV  = 225;      // Ticks for one full spin
const float DISTANCE_PER_TICK = (2 * PI * WHEEL_RADIUS) / TICKS_PER_REV; // ~0.000935 m/tick

// Robot Position and Speed

int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed*0.8;
int motorSpeedOutsideLineFollow = motorSpeed;

float robotAngle = PI/2; // angle in Radians (initial is 90 degrees for facing up on the map)
float totalDistance = 0; // in centimetres

// Target angles  
float targetAngleEnd = 0;
float targetAngleEnd2 = 0;
float targetAngleStart = 0;
float targetTotalDistance = 0;



// Ultrasonic
float duration, distance;  

// we have HC-SR04 sensor, 60 ms is needed to avoid bad data from ghost echoes
const int ULTRASONIC_READ_INTERVAL = 60;
unsigned long lastUltrasonicReadTime = 0;

// IR
int leftDigitalIRReading, rightDigitalIRReading;
int leftAnalogIRReading, rightAnalogIRReading;




int leftIRThreshold = 37;
int rightIRThreshold = 37;
boolean leftIRAnalog, rightIRAnalog;

// These are the default values, GUI overwrites
bool emergencyStop = false;
int emergencyStopDistance = 20;

int previousDir = 0;

// For PD algorithm (not used anymore)
double sensitivity = 5.0;
double dampening = 1.0;


//MAZE SOLVING STATES
enum mazeState {
    FOLLOW_LINE,
    TURNING_LEFT,
    BLIND_TURN,
    TURNING_RIGHT,
    OBJECT_DETECTED,
    TURNING_180_DEGREES,
    AFTER_180_RIGHT_SENSOR_SEARCH,
    JUNCTION_FOUND,
    CLEAR_JUNCTION,
    SEARCHING_FOR_LINE_90_DEG_LEFT,
    TURNING_100_DEGREES_LEFT,
    RETURN_RIGHT,
    LOST_ROBOT1
};

enum lostRobotAlgoState {
    SEARCHING_FOR_THE_LINE,
    FOUND_THE_LINE,
};

lostRobotAlgoState lostRobotAlgoState = SEARCHING_FOR_THE_LINE;

mazeState mazeState = FOLLOW_LINE;
unsigned long stateStartTime = 0;
unsigned long lastLeftBlackTime = 0;
unsigned long lastRightBlackTime = 0;



const unsigned int SmallStopAfterSensorDetection = 100;
const unsigned int BlindTime = 150;
const unsigned int ObjectFoundTime = 500;
const unsigned long JUNCTION_TIME_DELTA = 200;

    
    
