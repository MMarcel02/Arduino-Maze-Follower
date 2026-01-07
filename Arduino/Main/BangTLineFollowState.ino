void bangTLineFollowState() {

//TODO DELAY FOR THE RIGHT TURN 

    if (mazeState == 0) {
        if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
            setLineFollowingSpeed(motorSpeedOutsideLineFollow);
            moveForward();
        } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
            setLineFollowingSpeed(150);
            turnOnSpotLeft();
            delay(10);
        } else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1) {
            setLineFollowingSpeed(150); 
            turnOnSpotRight();
            delay(10);
        } else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
            stopAllMotors();
            delay(1500);
            setLineFollowingSpeed(motorSpeedOutsideLineFollow);
            //moveForward();
            //delay(800);
            mazeState = 1;
        }
    } else if (mazeState == 1) {
        moveForward();
        if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
            delay(600);
            mazeState = 2;
        }
    } else if (mazeState == 2) {
        setLineFollowingSpeed(150);
        turnOnSpotLeft();
        if (rightDigitalIRReading == 1) {
            mazeState = 0;
        }
    }
    // } else if (mazeState == 1) {
    //     setLineFollowingSpeed(150); 
    //     turnOnSpotLeft();
    //     if (rightDigitalIRReading == 0) {
    //         mazeState = 2;
    //     }
    // } else if (mazeState == 2) {
    //     setLineFollowingSpeed(150); 
    //     turnOnSpotLeft();
    //     if (rightDigitalIRReading == 1) {
    //         mazeState = 0;
    //     }
    // }
    
}