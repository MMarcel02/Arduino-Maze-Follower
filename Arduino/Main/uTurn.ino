bool uTurnCompleted = false;
unsigned long lastBounceTime = 0;
unsigned long straightStartTime = 0;
unsigned long avgBounceTime = 0;
int bounceCount = 0;

void resetUTurn() {
  lastBounceTime = 0;
  avgBounceTime = 0;
  bounceCount = 0;
  straightStartTime = 0;
  uTurnCompleted = false;
}

void uTurn() {
  if (uTurnCompleted) {
    stopAllMotors();
    return;
  }

  unsigned long now = millis();

  bool isStraight = (leftDigitalIRReading == 0 && rightDigitalIRReading == 0);
  bool isBounce   = !isStraight;

  // We bang banged
  if (isBounce) {
    // keep following the line, we dont care
    bangLineFollow();

    // measure the time and calculate the average time between bounces
    if (lastBounceTime != 0) {
      unsigned long dt = now - lastBounceTime;

      // running weighted average
      bounceCount++;
      avgBounceTime = ((avgBounceTime * (bounceCount - 1)) + dt) / bounceCount;
    }

    lastBounceTime = now;
    straightStartTime = 0; // reset straight timer
  }
  
  else {
    moveForward();

    if (straightStartTime == 0)
      straightStartTime = now;

    // if we are moving straight for longer than the average time between bounces,
    // assume there is no line left
    if (bounceCount > 3 && avgBounceTime > 0) {
      if ((now - straightStartTime) > (avgBounceTime * 2)) {
        uTurnCompleted = true;
        stopAllMotors();
      }
    }
  }
}