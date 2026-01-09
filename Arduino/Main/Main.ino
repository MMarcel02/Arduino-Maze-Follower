// Main file that Arduino compiles first
// All other files added alphabetically below this
#include <WiFi101.h>
const int leftEncA = A3; // Pin A3
const int leftEncB = A1; // Pin A1
volatile long leftTick = 0;

const int rightEncA = 21; // Pin SCL
const int rightEncB = 20; // Pin SDA
volatile long rightTick = 0;


void setupWifiPins(); 
void wifiSafetyCheck();
void startWifiAp();
void setupAllMotors();
void stopAllMotors();
void setupUltraSonicSensor();
void setupIRSensors();
void updateSensors();
void manageRobotMovementState();
void handleHTTPCommands();
void handleTCPData();


// setup() is automatically ran by Arduino on startup
void setup() {
  Serial.begin(115200); // USB Port for debugging
  delay(1000); // delay for the server to setup
  setupWifiPins(); 
  wifiSafetyCheck();
  startWifiAp();


  pinMode(leftEncA, INPUT);
  pinMode(leftEncB, INPUT);
  pinMode(rightEncA, INPUT);
  pinMode(rightEncB, INPUT);

  attachInterrupt(digitalPinToInterrupt(leftEncA), handleLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(rightEncA), handleRightEncoder, RISING);


  setupAllMotors();
  stopAllMotors();
  setupUltraSonicSensor();
  setupIRSensors();
}

// loop() is automatically run by Arduino as its running
void loop() {

  // Updates the IR and UltraSonic values
  updateSensors();

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 50) {
    Serial.print("L: "); Serial.print(leftTick);
    Serial.print(" | R: "); Serial.println(rightTick);
    lastPrint = millis();
  }
  //bangLineFollowStates();
  // bangLineFollowStates();
  // // Checks for emergency stop / line following
  manageRobotMovementState();

  // // Checks if received a new HTTP command, e.g. /forward
  handleHTTPCommands();

  // // Sends Sensor Data back to GUI
  handleTCPData();
}


