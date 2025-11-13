#pragma once

#include "RobotState.h"

class ManualControlState : public RobotState {
  public:
    void enter(WiFiClient& client) override;
    RobotState* handle(WiFiClient& client) override;
};
