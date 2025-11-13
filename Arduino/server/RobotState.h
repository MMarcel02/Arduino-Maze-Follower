#pragma once
#include <WiFi101.h>

class RobotState {
  public:
      virtual void enter(WiFiClient& client) {}
      virtual void exit(WiFiClient& client) {}
      
      /// The handle function is called each frame
      ///
      /// Must return a RobotState pointer to the next state;
      /// if you want the state to remain the same,
      /// return the reference to 'this' (see EmptyState.ino for example).
      virtual RobotState* handle(WiFiClient& client) = 0;
};
