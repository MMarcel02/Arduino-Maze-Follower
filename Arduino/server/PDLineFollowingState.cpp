#include "PDLineFollowingState.h"
#include "hardware.h"

RobotState* PDLineFollowingState::handle(WiFiClient& client) {
  updateLineSensors();
  
  int dir = 0;

  // -1 to turn left, +1 right
  if(leftLine)  dir -= 1;
  if(rightLine) dir += 1;
  
  double correction = sensitivity * dir + dampening * (dir - previousDir);
  
  int leftSpeed   = (int)(1 - correction) * motorSpeed;
  int rightSpeed  = (int)(1 + correction) * motorSpeed;
  
  setMotor(FL_PWM, FL_DIR, leftSpeed);
  setMotor(FR_PWM, FR_DIR, rightSpeed);
  setMotor(BR_PWM, BR_DIR, rightSpeed); 
  setMotor(BL_PWM, BL_DIR, leftSpeed);
}