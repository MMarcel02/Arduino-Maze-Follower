// Main file that calls all different functions to just setup an access point
#include <Arduino.h>
#include <WiFi101.h>


void setup() {
  Serial.begin(115200);
  delay(1000); // delay for the server to setup
  setupWifiPins(); // 
  wifiSafetyCheck();
  startWifiAp();
  setupAllMotors();
  stopAllMotors();
}

void loop() {
 acceptClients();
 float dist = getDistanceCM();
 if (dist<20){stopAllMotors();}
 
}


