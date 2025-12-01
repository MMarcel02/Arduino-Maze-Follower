
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
        while (rightReading == 0 && leftReading == 1) { 
             turnOnSpotLeft();
             readIRSensors();
        }
        previousDir = -1;
    }
    //Right Turn
    else if (leftReading == 0 && rightReading == 1){
        setSpeed(250); 
        while (leftReading == 0 && rightReading == 1) { 
             turnOnSpotRight();
             readIRSensors();
        }
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