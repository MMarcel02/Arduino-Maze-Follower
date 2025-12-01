#pragma once
#include "RobotState.h"

class LineFollowState : public RobotState {
public:
    void enter(WiFiClient& client) override;
    void exit(WiFiClient& client) override;
    RobotState* handle(WiFiClient& client) override;

};