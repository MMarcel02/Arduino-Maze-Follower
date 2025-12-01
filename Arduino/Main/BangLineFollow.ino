

// --- Function for the decision making logic in bang-bang line following ---
void bangLineFollow (){
  readIRSensors();
  bool leftLine = checkLeftIRSensor();
  bool rightLine = checkRightIRSensor();

// if both sensors aren't seeing the line we assume its between them and move forward 
  if (!leftLine && !rightLine ) { 
      moveForward();
  }      

// Robot drifted right -> steer left
  else if (leftLine && !rightLine ) { 
      turnOnSpotLeft(); 
  }

// Robot drifted left -> steer right
  else if ( !leftLine && rightLine ) { 
    turnOnSpotRight();
  }  

// Both sensors on black -> We are on a hard curve 
  else { 
    moveBackward(); 
  }
}


