// Ultrasound pins
const int trigPin = 0;
const int echoPin = 1; 

float duration, distance;  
int leftReading, rightReading;

// IR sensor pins (using analogue values to be more accurate)
// Analog pins dont need to be setup with pinMode()
const int LEFT_SENSOR_PIN = A0; 
const int RIGHT_SENSOR_PIN = A2; 

void setupUltraSonicSensor () {
	pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
}

void setupIRSensors () {
	pinMode(LEFT_SENSOR_PIN, INPUT);  
	pinMode(RIGHT_SENSOR_PIN, INPUT);  
}

void readUltrasonicSensor() {
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  

  duration = pulseIn(echoPin, HIGH, 5000);  

  distance = (duration*.0343)/2; 
}

void checkEmergencyStop() {
  // we check for more than 0.00 because sometimes it displays 0.00 if it times out
  if (0.00 < distance && distance < emergencyStopDistance) {
    stopAllMotors();
    currentState = STOPPED;
  }
}

void readIRSensors() {
  leftReading = digitalRead(LEFT_SENSOR_PIN);
  rightReading = digitalRead(RIGHT_SENSOR_PIN);
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

