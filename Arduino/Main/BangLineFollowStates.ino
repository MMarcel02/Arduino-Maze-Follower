void bangLineFollowStates() {

    unsigned long currentTime = millis();

    switch (mazeState) {

        case FOLLOW_LINE:
        if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
            setLineFollowingSpeed(motorSpeedOutsideLineFollow);
            moveForward(); 
        } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
            setLineFollowingSpeed(150);
            turnOnSpotLeft();
        } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
            setLineFollowingSpeed(150);
            turnOnSpotRight();
        } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
            setLineFollowingSpeed(motorSpeedOutsideLineFollow);
            moveForward();
            stateStartTime = currentTime; // the timer now starts
            mazeState = CLEAR_JUNCTION;
        }
        break;

        case CLEAR_JUNCTION:
            moveForward();
            if (currentTime - stateStartTime >= CLEAR_JUNCTION_TIME) {
                stopAllMotors();
                stateStartTime = currentTime;
                mazeState = CHECK_LEFT_130DEG;
            }
            break;
        // turn left till you find a line in a certain time period this is Phase 1 of the left search
        case CHECK_LEFT_130DEG:
            setLineFollowingSpeed(150);
            turnOnSpotLeft();
            if (leftDigitalIRReading == 1){
                stateStartTime = currentTime;
                mazeState = CHECK_LEFT_DONE_CENTERING_LINE; // implement this
                break;
            }
            if (currentTime - stateStartTime >= CHECK_LEFT_130DEG_TIME) {
                stateStartTime = currentTime;
                mazeState = RETURN_FROM_LEFT_TO_LINE;  // implement this
            }
            break;
            
        case CHECK_LEFT_DONE_CENTERING_LINE:
            setLineFollowingSpeed(150);
            turnOnSpotLeft();
            if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
                stopAllMotors();
                mazeState = FOLLOW_LINE;
                break;
            }
            if (currentTime - stateStartTime >= CENTERING_TIMEOUT) {
                stateStartTime = currentTime;
                mazeState = RETURN_FROM_LEFT_TO_LINE;
            }
            break;

        case RETURN_FROM_LEFT_TO_LINE:
            setLineFollowingSpeed(150);
            turnOnSpotRight();
            if (leftDigitalIRReading == 1){
                stopAllMotors();
                mazeState = FOLLOW_LINE;
                break;
            }
            break;            
                  
    }
}