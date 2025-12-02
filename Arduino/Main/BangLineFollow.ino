
int previousDir ;


void bangLineFollow() {
    //If doesn't see black at all we assume we are following the line so we go forward
    if (leftReading == 0 && rightReading == 0){
        setLineFollowingSpeed(motorSpeedOutsideLineFollow);
        moveForward();
    }

    //Detect on left so turn left
    else if (leftReading == 1 && rightReading == 0) {
        setLineFollowingSpeed(255);
        turnOnSpotLeft();
        previousDir = -1;
    }
    //Detect on right so turn right
    else if (leftReading == 0 && rightReading == 1){
        setLineFollowingSpeed(255); 
        turnOnSpotRight();
        previousDir = 1;
    }

    //Detect on both so lets use last direction we were headed in (probably on a 90 degree turn)
    else if (leftReading == 1 && rightReading == 1) {
         if (previousDir == -1) {
             setLineFollowingSpeed(255);
             turnOnSpotLeft();
         } else {
             setLineFollowingSpeed(255);
             turnOnSpotRight();
         }
    }

}