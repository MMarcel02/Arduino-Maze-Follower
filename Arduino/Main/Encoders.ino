

// ISR for Left Motor
void handleLeftEncoder() {
  if (digitalRead(leftEncB) > 0) {
    leftTick++;
  }
  else {
    leftTick--;
  }
}

// ISR for Right Motor
void handleRightEncoder() {
  if (digitalRead(rightEncB) > 0) {
    rightTick++;
  }
  else {
    rightTick--;
  }
}