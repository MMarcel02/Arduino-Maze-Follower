void bangLineFollowMaze() {

  unsigned long currentTime = millis();

  switch (mazeState) {
  case FOLLOW_LINE:
    if (obstacleFound()) {
      stopAllMotors();
      stateStartTime = currentTime;
      mazeState = OBJECT_DETECTED;
      break;
    } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
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
    } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
      stopAllMotors();
      stateStartTime = currentTime;
      mazeState = JUNCTION_FOUND;
      break;
    }
    break;

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

  case BLIND_TURN:
    if (currentTime - stateStartTime >= BlindTime) {
      mazeState = FOLLOW_LINE;
      break;
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

  case JUNCTION_FOUND:
    if (currentTime - stateStartTime >= ObjectFoundTime) {
      targetTotalDistance = totalDistance + 0.05;
      mazeState = CLEAR_JUNCTION;
      break;
    }
    break;

  case CLEAR_JUNCTION: {
    boolean isMoving = moveToDistance(targetTotalDistance);
    if (!isMoving) {
      stateStartTime = currentTime;
      mazeState = SEARCHING_FOR_LINE_90_DEG_LEFT;
    }
  } break;

  case SEARCHING_FOR_LINE_90_DEG_LEFT:
    if (currentTime - stateStartTime >= ObjectFoundTime) {
      targetAngleEnd = radToDeg(robotAngle) + 110;
      targetAngleStart = radToDeg(robotAngle) - 5;
      mazeState = TURNING_100_DEGREES_LEFT;
      break;
    }
    break;

  case TURNING_100_DEGREES_LEFT: {
    boolean isTurning = turnToAbsoluteAngle(targetAngleEnd);
    if (!isTurning) {
      stopAllMotors();
      mazeState = RETURN_RIGHT;
    }
  } break;

  case RETURN_RIGHT: {
    if (currentTime - stateStartTime >= ObjectFoundTime) {
      boolean isTurning = turnToAbsoluteAngle(targetAngleStart);
      if (isTurning) {
        if (leftDigitalIRReading == 1) {
          stopAllMotors();
          mazeState = FOLLOW_LINE;
        }
      } else if (!isTurning) { // the robot never is lost
        stopAllMotors();
        mazeState = LOST_ROBOT1;
      }
    }
  } break;

  case LOST_ROBOT1:
    stopAllMotors();
    break;
  }
}
