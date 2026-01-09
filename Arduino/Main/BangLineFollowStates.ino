void bangLineFollowStates() {

    unsigned long currentTime = millis();

    switch (mazeState) {

        case FOLLOW_LINE:
        if (obstacleFound()) {
            stopAllMotors();
            stateStartTime = currentTime;
            mazeState = SELF_ROTATE_1;
            break;
        } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
            setLineFollowingSpeed(motorSpeedOutsideLineFollow);
            moveForward(); 
        } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
            setLineFollowingSpeed(215);
            turnOnSpotLeft();
        } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
            setLineFollowingSpeed(215);
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
        if (currentTime - stateStartTime >= SHORT_STOP_TIME) {
            setLineFollowingSpeed(150);
            turnOnSpotRight();
            if (leftDigitalIRReading == 1){
                stopAllMotors();
                mazeState = FOLLOW_LINE;
                break;
            }          
        }
        break;

        case SELF_ROTATE_1:
        if (currentTime - stateStartTime >= SHORT_STOP_TIME) {
            setLineFollowingSpeed(125);
            turnOnSpotLeft();
            if (rightDigitalIRReading == 1){
            stateStartTime = currentTime;
            mazeState = SELF_ROTATE_2;
            break;
            }
        }
        break;

        case SELF_ROTATE_2:
        if (currentTime - stateStartTime >= SHORT_STOP_TIME) {
            setLineFollowingSpeed(125);
            turnOnSpotLeft();
            if (leftDigitalIRReading == 1){
            stateStartTime = currentTime;
            mazeState = SELF_ROTATE_3;
            break;
            }
        }
        break;

        case SELF_ROTATE_3:
        if (currentTime - stateStartTime >= SHORT_STOP_TIME) {
            setLineFollowingSpeed(125);
            turnOnSpotLeft();
            if (rightDigitalIRReading == 1){
            mazeState = FOLLOW_LINE;
            break;
            }
        }
        break;
    }
}