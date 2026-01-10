

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

#pragma region END_OF_LINE_DETECTION

const unsigned long MIN_STRAIGHT_TIME = 500; // 500ms
// e.g. compare against last 10 bounces
const int AVERAGE_BOUNCE_COUNT = 10;
unsigned long bounceTimes[AVERAGE_BOUNCE_COUNT];
int bounceIndex = 0;
int bounceFilled = 0;

unsigned long lastBounceTime = 0;
unsigned long straightStartTime = 0;

unsigned long getAvgBounceTime() {
  unsigned long sum = 0;
  for (int i = 0; i < bounceFilled; i++) {
    sum += bounceTimes[i];
  }
  return (bounceFilled > 0) ? (sum / bounceFilled) : 0;
}

bool detectEndOfLine() {
  unsigned long now = millis();

  bool isStraight = (leftDigitalIRReading == 0 && rightDigitalIRReading == 0);
  bool isBounce   = !isStraight;
  
  // We bang banged
  if (isBounce) {

    // measure the time and calculate the average time between bounces
    if (lastBounceTime != 0) {
      unsigned long dt = now - lastBounceTime;
      
      // don't store micro movements
      if (dt >= 10) {
        bounceTimes[bounceIndex] = dt;
        bounceIndex = (bounceIndex + 1) % AVERAGE_BOUNCE_COUNT;
        
        if (bounceFilled < AVERAGE_BOUNCE_COUNT) {
          ++bounceFilled;
        }  
      }
    }

    lastBounceTime = now;
    straightStartTime = 0; // reset straight timer
  }
  
  else {

    if (straightStartTime == 0)
      straightStartTime = now;

    unsigned long avgBounceTime = getAvgBounceTime();
      
    // require at least 5 bounces for now
    if (bounceFilled >= 5 && avgBounceTime > 0) {
      // we check if the current time without bouncing
      // is more than 15 times the calculated average
      if ((now - straightStartTime) > max(avgBounceTime * 15, MIN_STRAIGHT_TIME)) {
        return true;
      }
    }
  }
  
  return false;
}

void resetEndOfLineDetection() {
  lastBounceTime = 0;
  straightStartTime = 0;

  bounceIndex = 0;
  bounceFilled = 0;
}

#pragma endregion