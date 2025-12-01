
int previousDir ;

void bangLineFollow() {
  readIRSensors();

    if (leftReading == 0 && rightReading == 0){
        setSpeed(85);
        moveForward();
    }

    //Left Turn
    else if (leftReading == 1 && rightReading == 0) {
        setSpeed(250);
        turnOnSpotLeft();
        
        previousDir = -1;
    }
    //Right Turn
    else if (leftReading == 0 && rightReading == 1){
        setSpeed(250); 
        turnOnSpotRight();
        
        previousDir = 1;
    }

    else if (leftReading == 1 && rightReading == 1) {
         if (previousDir == -1) {
             setSpeed(250);
             turnOnSpotLeft();
         } else {
             setSpeed(250);
             turnOnSpotRight();
         }
    }

}