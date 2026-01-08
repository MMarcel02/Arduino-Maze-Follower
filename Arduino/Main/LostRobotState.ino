void searchForLine(){
// keep going forward and left untill we detect the line    
    if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0) {
        moveLeft();
    }
    else if (leftDigitalIRReading == 1 || rightDigitalIRReading == 1) {
        stopAllMotors();
    // fix for a possible problem that might appear: if a sensor falsely reads black 
    // it'll stop the robot without reaching the line. 
    // This way the sensors will have to read black at least 2 times before stopping
    /* static int blackCount = 0;
    if (leftDigitalIRReading == 1 || rightDigitalIRReading == 1) {
    blackCount++;
    if (blackCount >= 2) stopAllMotors();
    } else { blackCount = 0; } */
    
    }

}