// Ultrasound pins

const int trigPin = 0;
const int echoPin = 1; 

float duration, distance;  
int leftReading, rightReading;


// IR sensor pins (using analogue values to be more accurate)
// Analog pins dont need to be setup with pinMode()
const int LEFT_SENSOR_PIN = A0; 
const int RIGHT_SENSOR_PIN = A2; 


//Found in testing that when its 150 - 400 that when it is on black
const int BLACK_TAPE_THRESHOLD = 150; 

void setupUltraSonicSensor () {
	pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
}

void setupIRSensors () {
	pinMode(LEFT_SENSOR_PIN, INPUT);  
	pinMode(RIGHT_SENSOR_PIN, INPUT);  
}

float readUltrasonicSensor() {
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  

  duration = pulseIn(echoPin, HIGH, 5000);  

  distance = (duration*.0343)/2; 
  return distance;
}

void checkEmergencyStop() {
  if (0.00 < distance && distance < emergencyStopDistance) {
    stopAllMotors();
    currentState = STOPPED;
  }
}

String readIRSensors() {
	// IR Sensors loop
  leftReading = digitalRead(LEFT_SENSOR_PIN);
  rightReading = digitalRead(RIGHT_SENSOR_PIN);

  return String(leftReading) + "," + String(rightReading);
}

bool checkLeftIRSensor() {
  return (leftReading == 1);
}

bool checkRightIRSensor() {
  return (rightReading == 1);
}



String buildSensorMessage() {
  return String(readUltrasonicSensor(), 2) + "," + readIRSensors();
}

