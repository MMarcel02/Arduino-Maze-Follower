// --- Motor Pin Definitions ---
// Assign PWM (speed) and DIR (direction) pins for each motor
const int FL_PWM = 6,  FL_DIR = 5;     // Front Left Motor
<<<<<<< HEAD:Arduino/dummy_code_car.ino
// test commit
=======
const int FR_PWM =9, FR_DIR=10;        // Front Right 
const int BL_PWM = A4, BL_DIR = A5;    // Back Left
const int BR_PWM=11, BR_DIR = 12;      // Back Right
// test
>>>>>>> movement:Arduino/movementCode.ino

// --- Variables ---
int motorSpeed = 100;           // Default speed for all motors (range: 0–255)
int motorTurningSpeed = motorSpeed + 20;
char lastMotionCmd = 'x';      // Stores the last direction command (e.g., 'f' for forward)

// --- Setup Function for Each Motor ---
// Configures the direction and PWM pins for a motor
void setupMotor(int pwm, int dir) {
  pinMode(pwm, OUTPUT);  // Set PWM pin as output
  pinMode(dir, OUTPUT);  // Set direction pin as output
}

// --- Function to Drive a Motor ---
// 'speed' determines how fast, 'forward' determines direction
void setMotor(int pwm, int dir, int speed, bool forward) {
  digitalWrite(dir, forward ? HIGH : LOW);  // Set direction
  analogWrite(pwm, speed);                  // Set speed using PWM
}

// --- Arduino Setup Function ---
// Runs once when the board powers up or resets
void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 bps

  // Initialize all four motors
  setupMotor(FL_PWM, FL_DIR);
  setupMotor(FR_PWM,FR_DIR);
  setupMotor(BL_PWM,BL_DIR);
  setupMotor(BR_PWM,BR_DIR);
  
  // Stop all motors initially
  stopAllMotors();

  delay(3000);  // Wait for 3 seconds before starting

  printInstructions();  // Print help message to the Serial Monitor
}

// --- Main Loop ---
// Keeps running in a loop after setup() finishes
void loop() {
  
  // Check if data is available from Serial (USB) input
  if (Serial.available()) {
    // Read input string from user until newline
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove whitespace and newline characters

    if (input.length() == 0) return;  // Ignore empty input

    Serial.print("Command: ");
    Serial.println(input);

    // --- Handle Speed Change (e.g., "s150") ---
    if (input.charAt(0) == 's') {
      int newSpeed = input.substring(1).toInt();  // Extract number after 's'

      // Speed must be in the 0–255 range
      if (newSpeed >= 0 && newSpeed <= 255) {
        Serial.print("Received: Setting speed to ");
        Serial.println(newSpeed);
        motorSpeed = newSpeed;
        motorTurningSpeed = motorSpeed + 20;

        // Repeat last movement with new speed
        input = String(lastMotionCmd);  // Reuse the last motion command
      } else {
        Serial.println("Received: Invalid speed value (0–255 allowed)");
        return;
      }
    }

    // Extract the actual command (first character)
    char cmd = input.charAt(0);

    // Save the motion command (so we can reuse it later)
    if (cmd == 'f' || cmd == 'b' || cmd == 'l' || cmd == 'r' ||
        cmd == 'q' || cmd == 'e' || cmd == 'c' || cmd == 'v' || cmd == 'x') {
      lastMotionCmd = cmd;
    }
    // --- Motion Commands Switch ---
    switch (cmd) {
      case 'f': moveForward(); break;
      case 'x':
        stopAllMotors();  // Stop everything
        Serial.println("Stopped.");
        break;
      case 'b': driveBackward(); break;
      case 'l': driveLeft(); break;
      case 'r': driveRight(); break;
      case 'e': turnOnSpotLeft(); break;
      case 'q': turnOnSpotRight(); break;
      case 'c': crabWalkLeft(); break;
      case 'v': crabWalkRight(); break;



      default:
        Serial.println("Unknown command.");  // Unknown input
        printInstructions();                 // Show help again
        break;
    }
  }
}
// --- Function to Stop All Motors ---
void stopAllMotors() {
  setMotor(FL_PWM, FL_DIR, 0, true);
  setMotor(FR_PWM, FR_DIR, 0, true);
  setMotor(BL_PWM, BL_DIR, 0, true);
  setMotor(BR_PWM, BR_DIR, 0, true);  // Speed 0 = stop
  //Add your code to control the other motors.
}

// --- Movement Functions (called when a motion command is received) ---
// Move all motors forward
void moveForward() {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}

void driveBackward() {
  setMotor(FL_PWM, FL_DIR, motorSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorSpeed, false);
}

void driveRight () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorSpeed, true);
}
void driveLeft () {
  setMotor(FL_PWM, FL_DIR, motorSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}
void turnOnSpotRight () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, false);
}
void turnOnSpotLeft () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}
void crabWalkRight () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, true);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, false);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, false);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, true);
}
void crabWalkLeft () {
  setMotor(FL_PWM, FL_DIR, motorTurningSpeed, false);
  setMotor(FR_PWM, FR_DIR, motorTurningSpeed, true);
  setMotor(BL_PWM, BL_DIR, motorTurningSpeed, true);
  setMotor(BR_PWM, BR_DIR, motorTurningSpeed, false);
}


// --- Print Available Commands to the Serial Monitor ---
void printInstructions() {
  Serial.println("\nEnter a command:");
  Serial.println("f = forward");
  Serial.println("b = backward");
  Serial.println("l = move left");
  Serial.println("r = move right");
  Serial.println("q = rotate counter clockwise (CCW)");
  Serial.println("e = rotate clockwise (CW)");
  Serial.println("x = stop all motors");
  Serial.println("s### = set speed (e.g., s200 for speed = 200)");
}