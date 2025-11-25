int previousDir = 0;

void pdLineFollow () {
  readIRSensors();
  bool leftLine = checkLeftIRSensor();
  bool rightLine = checkRightIRSensor();
  
  int dir = 0;

  // -1 to turn left, +1 right
  if (leftLine && rightLine) dir = previousDir;
  if(leftLine)  dir -= 1;
  if(rightLine) dir += 1;
  
  double correction = sensitivity * dir + dampening * (dir - previousDir);
  
  int leftSpeed   = (int)((1 + correction) * motorSpeed);
  int rightSpeed  = (int)((1 - correction) * motorSpeed);
  

  setSmartMotor(FL_PWM, FL_DIR, leftSpeed);
  setSmartMotor(FR_PWM, FR_DIR, rightSpeed);
  setSmartMotor(BL_PWM, BL_DIR, leftSpeed);
  setSmartMotor(BR_PWM, BR_DIR, rightSpeed);
  
  previousDir = dir;
}

void setSmartMotor(int pwmPin, int dirPin, int speedVal) {
  bool forward = true;
  
  // If speed is negative, we need to go BACKWARD
  if (speedVal < 0) {
      forward = false;
      speedVal = -speedVal; // Make positive for analogWrite
  }
  
  // Clamp speed to max 255
  if (speedVal > 255) speedVal = 255;

  // Call your original setMotor function
  setMotor(pwmPin, dirPin, speedVal, forward);
}