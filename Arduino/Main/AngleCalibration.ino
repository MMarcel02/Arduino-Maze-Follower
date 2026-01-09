unsigned long startTime = 0;

int testedSpeeds[] = { 25, 50, 80, 100, 150, 200, 255 };
int testedSpeedsCount = sizeof(testedSpeeds)/sizeof(testedSpeeds[0]);

// Calibration
unsigned long testedSpeedsTimes[7];
int currentTestingSpeedIdx = 0;
int calibrationState = 0;

// Preview
int previewCalibrationState = 0;
int currentPreviewSpeedIdx = 0;
unsigned long previewStartTime = 0;

void resetCalibration() {
  calibrationState = 0;
  currentTestingSpeedIdx = 0;
  previewCalibrationState = 0;
  currentPreviewSpeedIdx = 0;
  previewStartTime = 0;
}

/// Measures how long it takes the robot to rotate 90 degrees at different speeds.
void calibrate() {
  
  // Initialization state
  if (calibrationState == 0) {
    startTime = millis();
    
    calibrationState = 1;
    motorSpeed = testedSpeeds[currentTestingSpeedIdx];
    
    // start turning immediately
    turnOnSpotLeft();
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
        previewCalibrationState = 0;
        currentPreviewSpeedIdx = 0;
      }
      else {
        calibrationState = 0;
      }
      
      stopAllMotors();
    }
    
    return;
  }
  
  if (calibrationState == 3) {
    previewCalibration();
  }
}

void previewCalibration() {
  
  // init state
  if (previewCalibrationState == 0) {
    previewStartTime = millis();
    stopAllMotors();
    
    previewCalibrationState = 1;
    return;
  }
  
  // wait state
  if (previewCalibrationState == 1) {
    // wait .25s to start rotating
    if (millis() - previewStartTime < 250) return;
    
    motorSpeed = testedSpeeds[currentPreviewSpeedIdx];
    previewStartTime = millis();
    turnOnSpotLeft();
    previewCalibrationState = 2;
    return;
  }
  
  // rotate state
  if (previewCalibrationState == 2) {
    // wait 2.5 seconds to start rotating
    if (millis() - previewStartTime < testedSpeedsTimes[currentPreviewSpeedIdx]) return;
    
    stopAllMotors();
    previewCalibrationState = 3;
    previewStartTime = millis();
    return;
  }
  
  // change speed state
  if (previewCalibrationState == 3) {
    // wait .25s between changing speeds
    if (millis() - previewStartTime < 250) return;
    ++currentPreviewSpeedIdx;
  
    if (currentPreviewSpeedIdx >= testedSpeedsCount) {
      previewCalibrationState = 4;
    } else {
      previewStartTime = millis();
      previewCalibrationState = 1;
    }
  }
  
  if (previewCalibrationState == 4) {
    stopAllMotors();
  }
  
  
}