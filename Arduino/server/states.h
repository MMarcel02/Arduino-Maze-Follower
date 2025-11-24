#pragma once

#include "EmptyState.h"
#include "ManualControlState.h"
#include "RobotState.h"
#include "LineFollowState.h"

// Add states below, like so:
extern EmptyState EMPTY_STATE;
extern ManualControlState MANUAL_CONTROL_STATE;
extern LineFollowState LINE_FOLLOW_STATE;


// current state variable, accessible globally
extern RobotState* currentState;

void changeState(RobotState* newState, WiFiClient& client);
