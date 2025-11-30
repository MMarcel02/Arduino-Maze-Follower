// Ultrasonic
float duration, distance;  

// IR
int leftReading, rightReading;

void setupUltraSonicSensor () {
	pinMode(TRIG_PIN, OUTPUT);  
	pinMode(ECHO_PIN, INPUT);  
}

void setupIRSensors () {
	pinMode(IR_LEFT_SENSOR_PIN, INPUT);  
	pinMode(IR_RIGHT_SENSOR_PIN, INPUT);  
}

void readUltrasonicSensor() {
  digitalWrite(TRIG_PIN, LOW);  
	delayMicroseconds(2);  
	digitalWrite(TRIG_PIN, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(TRIG_PIN, LOW);  

  // Time out after 5000 gives us a distance of about 80cm
  duration = pulseIn(ECHO_PIN, HIGH, 5000);  

  distance = (duration*.0343)/2; 
}

void checkEmergencyStop() {
  // we check for more than 0 because it displays 0 if it times out
  if (0 < distance && distance < emergencyStopDistance) {
    stopAllMotors();
    currentState = STOPPED;
  }
}

void readIRSensors() {
  leftReading = digitalRead(IR_LEFT_SENSOR_PIN);
  rightReading = digitalRead(IR_RIGHT_SENSOR_PIN);
}

bool checkLeftIRSensor() {
  return (leftReading == 1);
}

bool checkRightIRSensor() {
  return (rightReading == 1);
}



String buildSensorMessage() {
  return String(distance, 2) + "," + String(leftReading) + "," + String(rightReading);;
}

