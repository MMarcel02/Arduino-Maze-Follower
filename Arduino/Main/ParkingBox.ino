void parkingBox() {

  switch (parkingBoxState) {


    /*case 
    APPROACHING_PARKING_BOX
    PARKING_TURNING_LEFT
    PARKING_TURNING_RIGHT
    PARKING_BLIND_TURN
    START_OF_PARKING_BOX
    SELF_ALIGN_90_DEGREES
    FIND_THE_END_OF_PARKING_BOX
    END_OF_PARKING_BOX

    */

  case APPROACHING_PARKING_BOX:
    if (junctionDetectedTimed()) {
      stopAllMotors();
      stateStartTime = currentTime;
      parkingBoxState = START_OF_PARKING_BOX;
      break;
    } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
      setLineFollowingSpeed(motorSpeedOutsideLineFollow);
      moveForward();
    } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
      moveBackward(); // Or standard turn logic
      stateStartTime = currentTime;
      parkingBoxState = PARKING_TURNING_LEFT;
      break;
    } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
      moveBackward(); // Or standard turn logic
      stateStartTime = currentTime;
      parkingBoxState = PARKING_TURNING_RIGHT;
      break;
    }
    break;

  // --- Existing Turn Logic (Preserved for standard corners) ---
  case PARKING_TURNING_LEFT:
    if (junctionDetectedTimed()) {
      stopAllMotors();
      stateStartTime = currentTime;
      parkingBoxState = START_OF_PARKING_BOX;
      break;
    }
    if (currentTime - stateStartTime >= SmallStopAfterSensorDetection) {
      setLineFollowingSpeed(100);
      turnOnSpotLeft();
      if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        stateStartTime = currentTime;
        parkingBoxState = PARKING_BLIND_TURN;
        break;
      }
    }
    break;

  case PARKING_TURNING_RIGHT:
    if (junctionDetectedTimed()) {
      stopAllMotors();
      stateStartTime = currentTime;
      parkingBoxState = START_OF_PARKING_BOX;
      break;
    }
    if (currentTime - stateStartTime >= SmallStopAfterSensorDetection) {
      setLineFollowingSpeed(100);
      turnOnSpotRight();
      if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        stateStartTime = currentTime;
        parkingBoxState = PARKING_BLIND_TURN;
        break;
      }
    }
    break;

  case PARKING_BLIND_TURN:
    if (junctionDetectedTimed()) {
      stopAllMotors();
      stateStartTime = currentTime;
      parkingBoxState = START_OF_PARKING_BOX;
      break;
    }
    if (currentTime - stateStartTime >= BlindTime) {
      parkingBoxState = APPROACHING_PARKING_BOX;
      break;
    }
    break;

    // Parking space starts get to 90 degrees and then move forward a bit 
    case START_OF_PARKING_BOX:
    if (currentTime - stateStartTime >= ObjectFoundTime) {
      targetTotalDistance = totalDistance + 30; // instruct the bot to move max 0.5 meters
      parkingBoxState = SELF_ALIGN_90_DEGREES;
      break;
    }
    break;

    case SELF_ALIGN_90_DEGREES:{
        // self adjust to strict 90 degrees
      boolean isTurning = turnToAbsoluteAngle(90);
      if (!isTurning) {
        stopAllMotors();
        parkingBoxState = FIND_THE_END_OF_PARKING_BOX;
        break;
        }   
      }
    break;

    case FIND_THE_END_OF_PARKING_BOX:
         isMoving = moveToDistance(targetTotalDistance);
        if(!isMoving){
            stopAllMotors();
            // targetTotalDistance = totalDistance - 5; // instruct the bot to move max 0.5 meters
            parkingBoxState = END_OF_PARKING_BOX;
            break;
        }
        
        break;

    case END_OF_PARKING_BOX:
      //  isMoving = moveToDistance(targetTotalDistance);
      // if(!isMoving){
      //     stopAllMotors();
      // }
      break;
      
  }
}