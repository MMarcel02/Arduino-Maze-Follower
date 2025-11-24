#pragma once

#include "RobotState.h"

class PDLineFollowState : public RobotState {
  public:
    double sensitivity = 0.5;
    double dampening = 2.0;
    int previousDir = 0;
    RobotState* handle(WiFiClient& client) override;
};