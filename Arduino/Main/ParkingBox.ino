void parkingBox() {

    if ( (parkingBoxState == APPROACHING_PARKING_BOX || parkingBoxState == TURNING_LEFT || parkingBoxState == TURNING_RIGHT || parkingBoxState == BLIND_TURN ) 
        && junctionDetectedTimed() ) {

        stopAllMotors();
        stateStartTime = currentTime;
        mazeState = SELF_ALIGN_90_DEGREES;
    }

  switch (parkingBoxState) {

  case APPROACHING_PARKING_BOX:
    if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
      setLineFollowingSpeed(motorSpeedOutsideLineFollow);
      moveForward();
    } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
      moveBackward();
      stateStartTime = currentTime;
      parkingBoxState = PARKING_TURNING_LEFT;
      break;
    } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
      moveBackward();
      stateStartTime = currentTime;
      parkingBoxState = PARKING_TURNING_RIGHT;
      break;
    }
    break;

  case TURNING_LEFT:
    if (currentTime - stateStartTime >= TURN_START_REVERSE_DURATION) {
      setLineFollowingSpeed(100);
      turnOnSpotLeft();
      if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        stateStartTime = currentTime;
        parkingBoxState = BLIND_TURN;
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
        parkingBoxState = BLIND_TURN;
        break;
      }
    }
    break;

  case BLIND_TURN:
    if (currentTime - stateStartTime >= MINIMUM_TURN_DURATION) {
      parkingBoxState = APPROACHING_PARKING_BOX;
      break;
    }
    break;

  case SELF_ALIGN_90_DEGREES: {
      boolean isTurning = turnToAbsoluteAngle(90);
      if (!isTurning) {
        stopAllMotors();
        targetTotalDistance = totalDistance + 10; // Going to scan 10cm ahead after alligning to locate the intersection again
        parkingBoxState = PARKING_SEARCH_START_LINE;
      }
      break;
  }

    case PARKING_SEARCH_START_LINE:
      setLineFollowingSpeed(motorSpeedOutsideLineFollow);
      moveForward();

      if (junctionDetectedTimed()) {
          targetTotalDistance = totalDistance + ROBOT_LENGTH;
          parkingBoxState = PARKING_DRIVE_INTO_BOX;
      }
      
      else if (totalDistance >= targetTotalDistance) {
          parkingBoxState = PARKING_FALLBACK_SEARCH_FOR_END;
      }
      break;

  case PARKING_DRIVE_INTO_BOX:
      setLineFollowingSpeed(motorSpeedOutsideLineFollow);
      moveForward();

      if (totalDistance >= targetTotalDistance) {
          stopAllMotors();
          parkingBoxState = END_OF_PARKING_BOX;
      }
      break;

      case PARKING_FALLBACK_SEARCH_FOR_END:
      setLineFollowingSpeed(motorSpeedOutsideLineFollow);
      moveForward();

      if (junctionDetectedTimed()) {
          stopAllMotors();
          parkingBoxState = END_OF_PARKING_BOX;
      }
      break;

  case END_OF_PARKING_BOX:
      stopAllMotors();
      break;
  }
}