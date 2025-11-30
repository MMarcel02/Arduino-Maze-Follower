int previousDir = 0;

unsigned long lastLeftSeeTime = 0;
unsigned long lastRightSeeTime = 0;

void pdLineFollow () {
  readIRSensors();

  bool leftLine = checkLeftIRSensor()
  bool rightLine = checkRightIRSensor();
  
  unsigned long currentTime = millis();

  if(leftLine)  lastLeftSeeTime = currentTime;
  if (rightLine) lastRightSeeTime = currentTime;

  // We make it still turn for a bit after it stopped seeing the line
  bool stickyLeft = (currentTime - lastLeftSeeTime < lineMemoryLatency);
  bool stickyRight = (currentTime - lastRightSeeTime < lineMemoryLatency);
  
  int dir = 0;

  // If it still has both in its memory then we are probably on a curve or zigzagging
  if (stickyLeft && stickyRight) dir = previousDir;
  else if (stickyLeft)  dir -= 1;
  else if (stickyRight) dir += 1;
  
  double correction = sensitivity * dir + dampening * (dir - previousDir);
  
  previousDir = dir;

  int leftSpeed   = (int)((1 + correction) * motorSpeed);
  int rightSpeed  = (int)((1 - correction) * motorSpeed);

  setSmartMotor(FL_PWM, FL_DIR, leftSpeed);
  setSmartMotor(FR_PWM, FR_DIR, rightSpeed);
  setSmartMotor(BL_PWM, BL_DIR, leftSpeed);
  setSmartMotor(BR_PWM, BR_DIR, rightSpeed);
}

void setSmartMotor(int pwmPin, int dirPin, int speedVal) {
  bool forward = true;
  
  // If speed is negative, we need to go BACKWARD
  if (speedVal < 0) {
      forward = false;
      speedVal = -speedVal;
  }
  
  // Max speed of motor during line following
  if (speedVal > 150) speedVal = 150;

  // Pass values to the motor
  setMotor(pwmPin, dirPin, speedVal, forward);
}