
void emergencyStop() {

  switch (bangBangLineFollowState) {

  case (BangBangLineFollowing):
    bangLineFollow();

    if (obstacleFound()) {
      stopAllMotors();
      bangBangLineFollowState = Stopped;
      break;
    }

  case (Stopped):
    stopAllMotors();
    break;
  }
}