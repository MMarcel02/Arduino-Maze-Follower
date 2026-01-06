

void setupUltraSonicSensor () {
	pinMode(TRIG_PIN, OUTPUT);  
	pinMode(ECHO_PIN, INPUT);  
}

void setupIRSensors () {
	pinMode(IR_DIGITAL_LEFT_SENSOR_PIN, INPUT);  
	pinMode(IR_DIGITAL_RIGHT_SENSOR_PIN, INPUT);  
  pinMode(IR_ANALOG_LEFT_SENSOR_PIN, INPUT);
  pinMode(IR_ANALOG_RIGHT_SENSOR_PIN, INPUT);
}

void readUltrasonicSensor() {
  digitalWrite(TRIG_PIN, LOW);  
	delayMicroseconds(2);  
	digitalWrite(TRIG_PIN, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(TRIG_PIN, LOW);  

  // Time out after 5000 gives us a distance of about 80cm
  duration = pulseIn(ECHO_PIN, HIGH, 5000);  

  // 0.343 speed of sound in centimeters per microsecond
  distance = (duration*.0343)/2; 
}

void checkEmergencyStop() {
  // we check for more than 0 because it displays 0 if it times out
  if (0 < distance && distance < emergencyStopDistance) {
    stopAllMotors();
  }
}

void readIRSensors() {
  leftDigitalIRReading = digitalRead(IR_DIGITAL_LEFT_SENSOR_PIN);
  rightDigitalIRReading = digitalRead(IR_DIGITAL_RIGHT_SENSOR_PIN);
  leftAnalogIRReading = analogRead(IR_ANALOG_LEFT_SENSOR_PIN);
  rightAnalogIRReading = analogRead(IR_ANALOG_RIGHT_SENSOR_PIN);
}

void checkIRAnalogOverThreshold() {
  leftIRAnalog = (leftAnalogIRReading > leftIRThreshold);
  rightIRAnalog = (rightAnalogIRReading > rightIRThreshold);
}

void updateSensors() {
  // Read IR every cycle (for proper line following)
  readIRSensors();

  // We need to wait for the previous Ultrasonic waves to clear the area before reading again to get cleaner data
  unsigned long currentMillis = millis();
  // We check if 60ms has elapsed yet, if it has we send a packet with data
  if (currentMillis - lastUltrasonicReadTime >= ULTRASONIC_READ_INTERVAL) {
    readUltrasonicSensor();
    lastUltrasonicReadTime = currentMillis;
  }
}

String buildSensorMessage() {
  return String(distance, 2) + "," + String(leftDigitalIRReading) + "," + String(rightDigitalIRReading)+ "," + String(leftAnalogIRReading) + "," + String(rightAnalogIRReading);
}
