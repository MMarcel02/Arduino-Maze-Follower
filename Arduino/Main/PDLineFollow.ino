double lastError = 0;
unsigned long lastPDTime = 0;

void pdLineFollow () {
  
  
  double error = rightAnalogIRReading - leftAnalogIRReading;
  
  unsigned long currentTime = millis();
  double changeInTime = (double)(currentTime - lastPDTime);

  double derivative = 0;

  if (changeInTime > 0 && lastPDTime != 0) {
    derivative = (error - lastError) / changeInTime;
  }

  double correction = (sensitivity * error) + (dampening * derivative);
  
  lastError = error;
  lastPDTime = currentTime;

  int leftSpeed  = motorSpeed + (int)correction;
  int rightSpeed = motorSpeed - (int)correction;

  leftSpeed  = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

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

  // Pass values to the motor
  setMotor(pwmPin, dirPin, speedVal, forward);
}
void runReversePDLineFollow() {
  double error = 0;

  if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
    error = -1
  } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
    error = 1;
  } else {
    error = 0;
  }

  if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
    stopAllMotors();
    return;
  }

  double correction = error * reverseKp;

  if (REVERSE_INVERT_STEERING) {
    correction = -correction;
  }

  int leftSpd = -reverseBaseSpeed + correction;
  int rightSpd = -reverseBaseSpeed - correction;

  leftSpd = constrain(leftSpd, -255, 255);
  rightSpd = constrain(rightSpd, -255, 255);

  setSignedMotorSpeeds(leftSpd, rightSpd);
}