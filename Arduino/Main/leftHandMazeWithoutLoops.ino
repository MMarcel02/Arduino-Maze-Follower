void leftHandMazeWithoutLoops() {

  switch (mazeState) {
  case FOLLOW_LINE:
    if (obstacleFound()) {
      stopAllMotors();
      stateStartTime = currentTime;
      mazeState = OBJECT_DETECTED;
      break;
    } 
    // Trigger Intersection Logic
    else if (junctionDetectedTimed()) {
      stopAllMotors();
      stateStartTime = currentTime;
      mazeState = JUNCTION_FOUND;
      break;
    } 
    // Standard Line Following Logic
    else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
      setLineFollowingSpeed(motorSpeedOutsideLineFollow);
      moveForward();
    } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
      moveBackward(); // Or standard turn logic
      stateStartTime = currentTime;
      mazeState = TURNING_LEFT;
      break;
    } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
      moveBackward(); // Or standard turn logic
      stateStartTime = currentTime;
      mazeState = TURNING_RIGHT;
      break;
    }
    break;

  // --- Existing Turn Logic (Preserved for standard corners) ---
  case TURNING_LEFT:
    if (currentTime - stateStartTime >= SmallStopAfterSensorDetection) { 
      setLineFollowingSpeed(100);
      turnOnSpotLeft();
      if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        stateStartTime = currentTime;
        mazeState = BLIND_TURN;
        break;
      }
    }
    break;

  case TURNING_RIGHT:
    if (currentTime - stateStartTime >= SmallStopAfterSensorDetection) {
      setLineFollowingSpeed(100);
      turnOnSpotRight();
      if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        stateStartTime = currentTime;
        mazeState = BLIND_TURN;
        break;
      }
    }
    break;

  case BLIND_TURN:
    if (currentTime - stateStartTime >= BlindTime) {
      mazeState = FOLLOW_LINE;
      break;
    }
    break;

  // --- NEW INTERSECTION LOGIC START ---

  case JUNCTION_FOUND:
    // Pause briefly, then setup the 8cm move
    if (currentTime - stateStartTime >= ObjectFoundTime) { // or reduced time
       // Calculate target distance: Current + 0.08m (8cm)
       targetTotalDistance = totalDistance + 0.08; 
       mazeState = DRIVE_THROUGH_INTERSECTION;
    }
    break;

  case DRIVE_THROUGH_INTERSECTION: {
    // moveForward until 8cm is reached
    boolean isMoving = moveToDistance(targetTotalDistance);
    if (!isMoving) {
       // Movement complete
       stopAllMotors();
       // Record the angle before we start scanning left
       targetAngleStart = radToDeg(robotAngle); 
       mazeState = SCAN_LEFT_FOR_LINE;
    }
  } break;

  case SCAN_LEFT_FOR_LINE: {
    // 1. Rotate Left
    setLineFollowingSpeed(100); // Slow speed for scanning
    turnOnSpotLeft();

    // Calculate how much we have turned
    float currentDeg = radToDeg(robotAngle);
    float angleDiff = currentDeg - targetAngleStart;

    // 2. Check Conditions
    if (leftDigitalIRReading == 1) { 
        // Condition A: Found the line on the left sensor
        // Setup to rotate an "extra few degrees" (e.g., 20 degrees)
        targetAngleEnd = currentDeg + 20; 
        mazeState = ALIGN_EXTRA_LEFT;
    } 
    else if (angleDiff > 120) {
        // Condition B: Rotated > 120 deg without seeing line.
        // It was a 90 deg right turn that triggered the junction sensors.
        stopAllMotors();
        mazeState = RECOVER_RIGHT_FIND_BLACK;
    }
  } break;

  case ALIGN_EXTRA_LEFT: {
    // Finish the extra rotation to align fully
    boolean isTurning = turnToAbsoluteAngle(targetAngleEnd);
    if (!isTurning) {
        stopAllMotors();
        mazeState = FOLLOW_LINE;
    }
  } break;

  case RECOVER_RIGHT_FIND_BLACK:
    // Rotate Right until sensor sees black (Finding the line)
    setLineFollowingSpeed(100);
    turnOnSpotRight();
    if (rightDigitalIRReading == 1) {
        mazeState = RECOVER_RIGHT_FIND_WHITE;
    }
    break;

  case RECOVER_RIGHT_FIND_WHITE:
    // Keep rotating right until sensor goes white (Centered on line/ready to follow)
    turnOnSpotRight();
    if (rightDigitalIRReading == 0) {
        stopAllMotors();
        mazeState = FOLLOW_LINE;
    }
    break;

  // --- NEW INTERSECTION LOGIC END ---

  // --- Existing Obstacle Logic (Preserved) ---
  case OBJECT_DETECTED:
    if (currentTime - stateStartTime >= ObjectFoundTime) {
      targetAngleEnd = radToDeg(robotAngle) + 170;
      mazeState = TURNING_180_DEGREES;
      break;
    }
    break;

  case TURNING_180_DEGREES: {
    boolean isTurning = turnToAbsoluteAngle(targetAngleEnd);
    if (!isTurning) {
      stopAllMotors();
      mazeState = AFTER_180_RIGHT_SENSOR_SEARCH;
    }
  } break;

  case AFTER_180_RIGHT_SENSOR_SEARCH:
    setLineFollowingSpeed(100);
    turnOnSpotLeft();
    if (rightDigitalIRReading == 1) {
      mazeState = FOLLOW_LINE;
      break;
    }
    break;

  case LOST_ROBOT1:
    stopAllMotors();
    break;
  }
}