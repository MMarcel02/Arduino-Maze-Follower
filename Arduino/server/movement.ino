// --- Motor Pin Definitions ---
// Assign PWM (speed) and DIR (direction) pins for each motor
const int FL_PWM = 6,  FL_DIR = 5;     // Front Left Motor
// test comment
const int FR_PWM =9, FR_DIR=10;        // Front Right 
const int BL_PWM = A4, BL_DIR = A5;    // Back Left
const int BR_PWM=11, BR_DIR = 12;      // Back Right

// --- Variables ---
int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed + 20;
char lastMotionCmd = 'x';      // Stores the last direction command (e.g., 'f' for forward)

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
  Serial.begin(9600); // for logging
    
  setupMotor(FL_PWM, FL_DIR);
  setupMotor(FR_PWM,FR_DIR);
  setupMotor(BL_PWM,BL_DIR);
  setupMotor(BR_PWM,BR_DIR);
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

void driveBackward() {
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}

void driveRight () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void driveLeft () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}
void turnOnSpotRight () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, false);
}
void turnOnSpotLeft () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}
void crabWalkRight () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}
void crabWalkLeft () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, false);
}