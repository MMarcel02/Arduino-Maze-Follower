// Main file that Arduino compiles first
// All other files added alphabetically below this
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

// setup() is automatically ran by Arduino on startup
void setup() {
  Serial.begin(115200); // USB Port for debugging
  delay(1000); // delay for the server to setup
  setupWifiPins(); 
  wifiSafetyCheck();
  startWifiAp();
  setupAllMotors();
  stopAllMotors();
  setupUltraSonicSensor();
  setupIRSensors();
}

// loop() is automatically run by Arduino as its running
void loop() {

  // Updates the IR and UltraSonic values
  updateSensors();

  // Checks for emergency stop / line following
  manageRobotMovementState();

  // Checks if received a new HTTP command, e.g. /forward
  handleHTTPCommands();

  // Sends Sensor Data back to GUI
  handleTCPData();
}


