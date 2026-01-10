
void reverseBangLineFollow() {
    int backingTurningSpeed = 60;
    int backingForwardSpeed = 50;
    
    //If doesn't see black at all we assume we are following the line so we go forward
    if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0){
        setLineFollowingSpeed(motorSpeedOutsideLineFollow);
        moveBackward();
    }

    //Detect on left so steer left, with backwheels rotating and frontwheels pushing
    else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
        motorSpeed = backingForwardSpeed;
        motorTurningSpeed = backingTurningSpeed;
        
        steerBackWheelsRight();
        
        previousDir = -1;
    }
    //Detect on right so steer right, with backwheels rotating and frontwheels pushing
    else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1){
        motorSpeed = backingForwardSpeed;
        motorTurningSpeed = backingTurningSpeed;
        
        steerBackWheelsLeft();
        
        previousDir = 1;
    }

    // I don't know about the below case yet
    // Let's test the part before first.
    
    //Detect on both so lets use last direction we were headed in (probably on a 90 degree turn)
    else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
         if (previousDir == -1) {
             setLineFollowingSpeed(255);
             turnOnSpotRight();
         } else {
             setLineFollowingSpeed(255);
             turnOnSpotLeft();
         }
    }

}