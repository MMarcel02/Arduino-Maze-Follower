
int outerWheelSpeed = 100; 

int startInnerWheelSpeed = -100;
int endInnerWheelSpeed = 100; 

// Inner speed ramps up from 0-180 in 5 seconds, to make a wider spiral, after that it  just goes straight
long spiralDuration = 5000;

void lostRobot() {
    switch (lostRobotState) {
        
        case SEARCHING_FOR_THE_LINE:
            if (leftDigitalIRReading == 1 || rightDigitalIRReading == 1) {
                stopAllMotors();
                lostRobotState = FOUND_THE_LINE;
                break;
            }

            long elapsedTime = currentTime - stateStartTime;
            int speedRange = endInnerSpeed - startInnerSpeed;
            int innerWheelSpeed = startInnerSpeed + ((elapsedTime * speedRange) / spiralDuration);

            bool spinDirection = (innerWheelSpeed >= 0);
            int innerWheelSpeedAbsolute = abs((int)currentInnerSpeedRaw);
            
            if (innerWheelSpeed > endInnerWheelSpeed) { innerWheelSpeed = endInnerWheelSpeed; };

            setMotor(FL_PWM, FL_DIR, innerWheelSpeedAbsolute, spinDirection);
            setMotor(BL_PWM, BL_DIR, innerWheelSpeedAbsolute, spinDirection);
            setMotor(FR_PWM, FR_DIR, outerWheelSpeed, true);
            setMotor(BR_PWM, BR_DIR, outerWheelSpeed, true);
            
            break;

        case FOUND_THE_LINE:
            leftHandMazeWithoutLoops();
            break;
    }
}