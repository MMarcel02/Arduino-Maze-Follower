void lostRobot() {
    switch (lostRobotState) {
        
        case SEARCHING_FOR_THE_LINE:
            if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
                setLineFollowingSpeed(motorSpeedOutsideLineFollow);
                spinLeftLostRobot();
            } else if (leftDigitalIRReading == 1 || rightDigitalIRReading == 1) {
                stopAllMotors();
                lostRobotState = FOUND_THE_LINE;
                break;
            }
            break;
    
        case FOUND_THE_LINE:
            leftHandMazeWithoutLoops();
            break;
    }
}
