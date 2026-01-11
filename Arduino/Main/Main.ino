
#include <WiFi101.h>


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
void setUpEncoders();


void setup() {
  Serial.begin(115200); 
  delay(1000); 
  setupWifiPins(); 
  wifiSafetyCheck();
  startWifiAp();
  setupAllMotors();
  setUpEncoders();
  stopAllMotors();
  setupUltraSonicSensor();
  setupIRSensors();
}

// Time
unsigned long currentTime = 0;

void loop() {
  currentTime = millis();
  
  // Update sensor values
  updateSensors();

  // Check distance and angle every 50ms
  updateOdometry();
  leftHandMazeWithoutLoops();
  
  // Checks for emergency stop and line following etc.
  // manageRobotMovementState();

  // // Checks if received a new HTTP command, e.g. /forward
  // handleHTTPCommands();

  // // Sends Sensor Data back to GUL
  // handleTCPData();
  
}

