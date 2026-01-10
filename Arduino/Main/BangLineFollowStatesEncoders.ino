void bangLineFollowStatesEncoders() {

    // how many states will i have 
    // 1. follow line
    // 2. turning left
    // 3. turning right
    // 4. clear junction
    // 5. check 90 degrees left
    // 6. Left sensor found line
    // 7. Right sensor found line - robot centralised
    // 8. Object detected - robot stops
    // 7. Rotate robot after object detected

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
            break;
        }
        break;

        case TURNING_LEFT:
        if (currentTime - stateStartTime >= SmallStopAfterSensorDetection) { // check to fix the short stop time to half a second
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
        if (currentTime - stateStartTime >= SmallStopAfterSensorDetection) { // check to fix the short stop time to half a second
            setLineFollowingSpeed(100);
            turnOnSpotRight();
            if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
                stateStartTime = currentTime;
                mazeState = BLIND_TURN;
                break;
            }
        }
        break;
       

    }

}
