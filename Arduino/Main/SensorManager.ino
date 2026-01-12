

void setupUltraSonicSensor () {
	pinMode(TRIG_PIN, OUTPUT);  
	pinMode(ECHO_PIN, INPUT);  
}

void setupIRSensors () {
	pinMode(IR_DIGITAL_LEFT_SENSOR_PIN, INPUT);  
	pinMode(IR_DIGITAL_RIGHT_SENSOR_PIN, INPUT);  

  // Commented out for now since not using analog because of Encoders (no pins left)
  // pinMode(IR_ANALOG_LEFT_SENSOR_PIN, INPUT);
  // pinMode(IR_ANALOG_RIGHT_SENSOR_PIN, INPUT);
}

void readUltrasonicSensor() {
  digitalWrite(TRIG_PIN, LOW);  
	delayMicroseconds(2);  
	digitalWrite(TRIG_PIN, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(TRIG_PIN, LOW);  

  // Time out after 5000 gives us a ultrasonicDistance of about 80cm
  duration = pulseIn(ECHO_PIN, HIGH, 5000);  

  // 0.343 speed of sound in centimeters per microsecond
  ultrasonicDistance = (duration*.0343)/2; 
}

void checkEmergencyStop() {
  // we check for more than 0 because it displays 0 if it times out
  if (0 < ultrasonicDistance && ultrasonicDistance < emergencyStopDistance) {
    stopAllMotors();
  }
}

void readIRSensors() {
  leftDigitalIRReading = digitalRead(IR_DIGITAL_LEFT_SENSOR_PIN);
  rightDigitalIRReading = digitalRead(IR_DIGITAL_RIGHT_SENSOR_PIN);

  // Commented out for now since not using analog because of Encoders (no pins left)
  // leftAnalogIRReading = analogRead(IR_ANALOG_LEFT_SENSOR_PIN);
  // rightAnalogIRReading = analogRead(IR_ANALOG_RIGHT_SENSOR_PIN);
}

// Not used for now as got rid of analog, keeping in case we bring it back
void checkIRAnalogOverThreshold() {
  leftIRAnalog = (leftAnalogIRReading > leftIRThreshold);
  rightIRAnalog = (rightAnalogIRReading > rightIRThreshold);
}

void updateSensors() {
  // Read IR every cycle (for proper line following)
  readIRSensors();
  updateLineSensorTimes();

  // We need to wait for the previous Ultrasonic waves to clear the area before reading again to get cleaner data
  if (currentTime - lastUltrasonicReadTime >= ULTRASONIC_READ_INTERVAL) {
    readUltrasonicSensor();
    lastUltrasonicReadTime = currentTime;
  }
}
// new helper functions for junctions id

void updateLineSensorTimes() {
  if (leftDigitalIRReading == 1) {
    lastLeftBlackTime = currentTime;
  }
  if (rightDigitalIRReading == 1) {
    lastRightBlackTime = currentTime;
  }
}

bool junctionDetectedTimed() {
  boolean doubleSensorDetectedDelta = abs(lastLeftBlackTime - lastRightBlackTime) < JUNCTION_TIME_DELTA;
  boolean isRecent = (currentTime - lastLeftBlackTime < JUNCTION_TIME_DELTA || currentTime - lastRightBlackTime < JUNCTION_TIME_DELTA);
  return (doubleSensorDetectedDelta && isRecent);
}

String buildSensorMessage() {
  return String(ultrasonicDistance, 2) + "," + String(leftDigitalIRReading) + "," + String(rightDigitalIRReading);
}

boolean obstacleFound(){
  if (0 < ultrasonicDistance && ultrasonicDistance < emergencyStopDistance) {
    return true;
  }
  return false;
}
