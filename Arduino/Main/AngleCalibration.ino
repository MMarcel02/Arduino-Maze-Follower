unsigned long startTime = 0;

int[] testedSpeeds = { 25, 50, 80, 100, 150, 200, 255 };
int testedSpeedsCount = sizeof(testedSpeeds)/sizeof(testedSpeeds[0]);

unsigned long[] testedSpeedsTimes = new long[testedSpeedsCount];
int currentTestingSpeedIdx = 0;

int calibrationState = 0;

/// Measures how long it takes the robot to rotate 90 degrees at different speeds.
void calibrate() {
  
  // Initialization state
  if (calibrationState == 0) {
    startTime = millis();
    calibrationState = 1;
    motorSpeed = testedSpeeds[currentTestingSpeedIdx];
    return;
  }
  
  // Turn until first sensor reaches line.
  if (calibrationState == 1) {
    turnOnSpotLeft();
    
    // First sensor passed line, keep turning
    if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
      calibrationState = 2;
    }
    
    return;
  }
  
  // Turn until second sensor reaches line.
  if (calibrationState == 2) {
    turnOnSpotLeft();
    
    // Second sensor reached line, end measuring time.
    if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
      unsigned long endTime = millis();
      testedSpeedsTimes[currentTestingSpeedIdx++] = endTime - startTime;
      
      // If we reached the end of the array, stop calibrating
      if (currentTestingSpeedIdx == testedSpeedsCount) {
        calibrationState = 3;
      }
      else {
        calibrationState = 0;
      }
    }
    
    return;
  }
}