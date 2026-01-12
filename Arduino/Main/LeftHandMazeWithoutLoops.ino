

void leftHandMazeWithoutLoops() {


  if ( (mazeState == FOLLOW_LINE || mazeState == TURNING_LEFT || mazeState == TURNING_RIGHT || mazeState == BLIND_TURN ) 
        && junctionDetectedTimed() ) {
        
        stopAllMotors();
        stateStartTime = currentTime;
        mazeState = JUNCTION_FOUND;
    }

  switch (mazeState) {
    case FOLLOW_LINE:
      if (obstacleFound()) {
        stopAllMotors();
        stateStartTime = currentTime;
        mazeState = OBJECT_DETECTED;
        break;
      } 
      if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        setLineFollowingSpeed(motorSpeedOutsideLineFollow);
        moveForward();
      } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
        moveBackward(); 
        stateStartTime = currentTime;
        mazeState = TURNING_LEFT;
        break;
      } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
        moveBackward(); 
        stateStartTime = currentTime;
        mazeState = TURNING_RIGHT;
        break;
      }
      break;

    case TURNING_LEFT:
      if (currentTime - stateStartTime >= TURN_START_REVERSE_DURATION) { 
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
      if (currentTime - stateStartTime >= TURN_START_REVERSE_DURATION) {
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
      if (currentTime - stateStartTime >= MINIMUM_TURN_DURATION) {
        mazeState = FOLLOW_LINE;
        break;
      }
      break;

  case JUNCTION_FOUND:
      if (currentTime - stateStartTime >= STOP_TIME_AT_INTERSECTION) { 
        // CHANGE 1: Don't drive yet. Go to squaring state first.
        mazeState = SQUARE_UP_JUNCTION;
        stateStartTime = currentTime; // Reset timer for the next state if needed
      }
      break;

    case SQUARE_UP_JUNCTION: {
      // 1. Perfect Alignment: Both sensors on the line
      if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
          stopAllMotors();
          targetTotalDistance = totalDistance + ROBOT_LENGTH/2; 
          mazeState = DRIVE_THROUGH_INTERSECTION;
      }
      // 2. Twisted Left: Right sensor is on line, Left is off
      else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
          setLineFollowingSpeed(90); // Use a slower speed for precision
          turnOnSpotRight(); 
      }
      // 3. Twisted Right: Left sensor is on line, Right is off
      else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
          setLineFollowingSpeed(90);
          turnOnSpotLeft();
      }
      // 4. THE FIX: We slid off the line! (Both are 0)
      // Since we were just at a junction, we definitely overshot it slightly.
      else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
          setLineFollowingSpeed(80); // Very slow reverse
          moveBackward(); 
      }

    case DRIVE_THROUGH_INTERSECTION: {
      boolean isMoving = moveToDistance(targetTotalDistance);
      if (!isMoving) {
        stopAllMotors();
        targetAngleStart = radToDeg(robotAngle); 
        mazeState = SCAN_LEFT_FOR_LINE;
      }
    } break;

    case SCAN_LEFT_FOR_LINE: {
      setLineFollowingSpeed(100); 
      turnOnSpotLeft();

      float currentDeg = radToDeg(robotAngle);
      float angleDiff = currentDeg - targetAngleStart;

      if (leftDigitalIRReading == 1) { 
          mazeState = ALIGN_EXTRA_LEFT;
      } 
      else if (angleDiff > 120) {
          stopAllMotors();
          mazeState = RECOVER_RIGHT_FIND_BLACK;
      }
    } break;

    case ALIGN_EXTRA_LEFT: {
      turnOnSpotLeft();
      if (rightDigitalIRReading == 1) {
          stopAllMotors();
          mazeState = FOLLOW_LINE;
      }
    } break;

    case RECOVER_RIGHT_FIND_BLACK:
      setLineFollowingSpeed(100);
      turnOnSpotRight();
      if (rightDigitalIRReading == 1) {
          mazeState = RECOVER_RIGHT_FIND_WHITE;
      }
      break;

    case RECOVER_RIGHT_FIND_WHITE:
      turnOnSpotRight();
      if (leftDigitalIRReading == 1) {
          stopAllMotors();
          mazeState = FOLLOW_LINE;
      }
      break;
      
    case OBJECT_DETECTED:
      if (currentTime - stateStartTime >= STOP_TIME_AT_INTERSECTION) {
        targetAngleEnd = radToDeg(robotAngle) + 180;
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
}