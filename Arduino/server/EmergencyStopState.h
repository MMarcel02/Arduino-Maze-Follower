#pragma once

#include "RobotState.h"

class EmergencyStopState : public RobotState {
  public:
    void enter(WiFiClient& client) override;
    RobotState* handle(WiFiClient& client) override;
};
