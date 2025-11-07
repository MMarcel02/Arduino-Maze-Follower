#include "states.h"

// Add states below, like so:
EmptyState EMPTY_STATE;
ManualControlState MANUAL_CONTROL_STATE;



// current state variable, accessible globally
RobotState* currentState = &EMPTY_STATE;

void changeState(RobotState* newState, WiFiClient& client) {
  // check if nullptr, just in case
  if (currentState) currentState->exit(client);
  
  currentState = newState;
  
  if (currentState) currentState->enter(client);
}
