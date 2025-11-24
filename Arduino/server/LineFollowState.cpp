#include "LineFollowState.h"
#include "states.h" 
#include "hardware.h"



// --- Function for the decision making logic in bang-bang line following ---
void bangLineFollow (bool leftLine, bool rightLine){

// if both sensors aren't seeing the line we assume its between them and move forward 
  if (!leftLine && !rightLine ) { 
      moveForward();
  }      

// Robot drifted right -> steer left
  else if (leftLine && !rightLine ) { 
    moveLeft(); 
  }

// Robot drifted left -> steer right
  else if ( !leftLine && rightLine ) { 
    moveRight();
  }  

// Both sensors on black -> We are on a hard curve 
  else { moveForward(); 
  }
}

void LineFollowState::enter(WiFiClient& client) {
  
    client.print("Entered line following state");

}

void LineFollowState::exit(WiFiClient& client) {
  stopAllMotors();
}

RobotState* LineFollowState::handle(WiFiClient& client) {
  // First we'll read the sensors every cycle
  updateLineSensors();
  
 // client.println("L=" + String(leftLine) + " R=" + String(rightLine)); to see what the robot is doing

  // Second we run the logic
  bangLineFollow();

  return this;
}
