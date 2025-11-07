#pragma once

#include "RobotState.h"

class EmptyState : public RobotState {
  public:
    virtual RobotState* handle(WiFiClient& client) override;
};
