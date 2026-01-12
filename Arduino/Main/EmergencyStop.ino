
void emergencyStop() {

  switch (bangBangLineFollowState) {

  case (BANG_LINE_FOLLOWING):
    bangLineFollow();

    if (obstacleFound()) {
      stopAllMotors();
      bangBangLineFollowState = STOPPED;
      break;
    }

  case (STOPPED):
    stopAllMotors();
    break;
  }
}