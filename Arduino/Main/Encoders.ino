const int leftEncA = 17; // Pin A3
const int leftEncB = 15; // Pin A1
volatile long leftTick = 0;

const int rightEncA = 21 // Pin SCL
const int rightEncB = 20 // Pin SDA
volatile long rightTick = 0;

setup()
{
  Serial.begin(115200);

  pinMode(leftEncA, INPUT_PULLUP);
  pinMode(leftEncB, INPUT_PULLUP);
  pinMode(rightEncA, INPUT_PULLUP);
  pinMode(rightEncB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(leftEncA), handleLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(rightEncA), handleRightEncoder, RISING);

}

void loop() {
  // Print values every 200ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 200) {
    Serial.print("L: "); Serial.print(leftTicks);
    Serial.print(" | R: "); Serial.println(rightTicks);
    lastPrint = millis();
  }
}

// ISR for Left Motor
void handleLeftEncoder() {
  if (digitalRead(leftEncB) == LOW) leftTicks++;
  else leftTicks--;
}

// ISR for Right Motor
void handleRightEncoder() {
  if (digitalRead(rightEncB) == LOW) rightTicks++;
  else rightTicks--;
}