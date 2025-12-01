
int previousDir ;

// --- Function for the decision making logic in bang-bang line following ---
void bangLineFollow (){
  readIRSensors();
  bool leftLine = checkLeftIRSensor();
  bool rightLine = checkRightIRSensor();
  
  // if both sensors aren't seeing the line we assume its between them and move forward 
  if (!leftLine && !rightLine ) { 
      setSpeed(85);
      moveForward();
  }      

// Robot drifted right -> steer left
  else if (leftLine && !rightLine ) { 
    setSpeed(250);
    while (checkLeftIRSensor && !checkRightIRSensor) { 
      turnOnSpotLeft();
      readIRSensors();
    }
    previousDir = -1;
  }

// Robot drifted left -> steer right
  else if ( !leftLine && rightLine ) { 
    setSpeed(250);
    while (!checkLeftIRSensor && checkRightIRSensor) { 
      turnOnSpotRight();
      readIRSensors();
    }
    previousDir = 1;
  }  

// Both sensors on black, we try to repeat last move
  else { 
    if (previousDir == -1) {
        setSpeed(250);
        turnOnSpotLeft();
    } else {
        setSpeed(250);
        turnOnSpotRight();
    }
  }
}