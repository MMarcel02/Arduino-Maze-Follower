void bangTLineFollowState() {

    if (mazeState == 0) {
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
            stopAllMotors();
            delay(2000);
            setLineFollowingSpeed(motorSpeedOutsideLineFollow);
            moveForward();
            delay(1000);
            mazeState = 1;
        }
    } else if (mazeState == 1) {
        setLineFollowingSpeed(150); 
        turnOnSpotLeft();
        if (rightDigitalIRReading == 0) {
            mazeState = 2;
        }
    } else if (mazeState == 2) {
        setLineFollowingSpeed(150); 
        turnOnSpotLeft();
        if (rightDigitalIRReading == 1) {
            mazeState = 0;
        }
    }
    
}