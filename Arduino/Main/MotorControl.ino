
// --- Variables ---
int motorSpeed = 80;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed - 20;

// --- Function to Drive a Motor ---
void setMotor(int pwm, int dir, int speed, bool forward) {
  digitalWrite(dir, forward ? HIGH : LOW);  // Set direction
  analogWrite(pwm, speed);                  // Set speed using PWM
}

// --- Setup Function for Each Motor ---
void setupMotor(int pwm, int dir) {
  pinMode(pwm, OUTPUT);  // Set PWM pin as output (for speed)
  pinMode(dir, OUTPUT);  // Set direction pin as output
}

// --- Arduino Setup Function ---
void setupAllMotors() {    
  setupMotor(FL_PWM, FL_DIR);
  setupMotor(FR_PWM,FR_DIR);
  setupMotor(BL_PWM,BL_DIR);
  setupMotor(BR_PWM,BR_DIR);
}

void setSpeed(int newSpeed) {
  motorSpeed = newSpeed;
  motorSpeedOutsideLineFollow = newSpeed;
  motorTurningSpeed = motorSpeed*0.8;
}

// This one we use when we're changing speed on the fly in the line following
void setLineFollowingSpeed(int newSpeed) {
  motorSpeed = newSpeed;
}

void stopAllMotors() {
  currentMovementState = STOPPED;
  setMotor(FL_PWM, FL_DIR, 0, true);
  setMotor(FR_PWM, FR_DIR, 0, true);
  setMotor(BL_PWM, BL_DIR, 0, true);
  setMotor(BR_PWM, BR_DIR, 0, true); 
}

void moveForward() {
  currentMovementState = FORWARD;
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}

void moveBackward() {
  currentMovementState = BACKWARD;
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}

void moveRight () {
  currentMovementState = RIGHT;
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}

void moveLeft () {
  currentMovementState = LEFT;
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void turnOnSpotRight () {
  currentMovementState = TURN_SPOT_RIGHT;
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}
void turnOnSpotLeft () {
  currentMovementState = TURN_SPOT_LEFT;
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void crabWalkRight () {
  currentMovementState = CW_RIGHT;
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void crabWalkLeft () {
  currentMovementState = CW_LEFT;
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}