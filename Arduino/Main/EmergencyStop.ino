
void emergencyStop() {

  switch (emergencyStopState) {

  case (BANG_LINE_FOLLOWING):
    bangLineFollow();

    if (obstacleFound()) {
      stopAllMotors();
      emergencyStopState = EMERGENCY_STOPPED;
    }
    break;

  case (EMERGENCY_STOPPED):
    stopAllMotors();
    break;
  }
}