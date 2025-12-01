void stopTurnLineFollow () {
  readIRSensors();

  bool leftLine = checkLeftIRSensor()
  bool rightLine = checkRightIRSensor();
  
  int leftSpeed  = motorSpeed;
  int rightSpeed = motorSpeed;

  if (leftLine && !rightLine) {
    leftSpeed *= -1;
  }
  else if (!leftLine && rightLine) {
    rightSpeed *= -1;
  }

  setSmartMotor2(FL_PWM, FL_DIR, leftSpeed);
  setSmartMotor2(FR_PWM, FR_DIR, rightSpeed);
  setSmartMotor2(BL_PWM, BL_DIR, leftSpeed);
  setSmartMotor2(BR_PWM, BR_DIR, rightSpeed);
}

void setSmartMotor2(int pwmPin, int dirPin, int speedVal) {
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