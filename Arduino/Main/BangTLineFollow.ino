
void bangTLineFollow() {
    //If doesn't see black at all we assume we are following the line so we go forward
    if (leftDigitalIRReading == 0 && rightDigitalIRReading == 0){
        moveForward();
    }

    //Detect on left so turn left
    else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 0) {
        turnOnSpotLeft();
    }
    //Detect on right so turn right
    else if (leftDigitalIRReading == 0 && rightDigitalIRReading == 1){
        turnOnSpotRight();
    }

    //Detect on both so lets use last direction we were headed in (probably on a 90 degree turn)
    else if (leftDigitalIRReading == 1 && rightDigitalIRReading == 1) {
        stopAllMotors();
        delay(1500);
       
    }

}