// --- Motor Pin Definitions ---
// Assign PWM (speed) and DIR (direction) pins for each motor
const int FL_PWM = 6,  FL_DIR = 5;     // Front Left Motor
// test comment

// --- Variables ---
int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
char lastMotionCmd = 'x';      // Stores the last direction command (e.g., 'f' for forward)

// --- Function to Drive a Motor ---
// 'speed' determines how fast, 'forward' determines direction
void setMotor(int pwm, int dir, int speed, bool forward) {
  digitalWrite(dir, forward ? HIGH : LOW);  // Set direction
  analogWrite(pwm, speed);                  // Set speed using PWM
}

// --- Arduino Setup Function ---
// Runs once when the board powers up or resets
void setupAllMotors() {
  Serial.begin(9600); // for logging
  // Initialize all four motors
  
  setupMotor(FL_PWM, FL_DIR); // Front left
  
  // ...
}

// --- Function to Stop All Motors ---
void stopAllMotors() {
  setMotor(FL_PWM, FL_DIR, 0, true);  // Speed 0 = stop
  //Add your code to control the other motors.
}

void moveForward() {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  //Add your code to control the other motors.
  Serial.println("Moving forward");
}
