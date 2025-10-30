// Add states below, like so:
EmptyState EMPTY_STATE;
ManualControlState MANUAL_CONTROL_STATE;



// current state variable, accessible globally
RobotState* currentState = &EMPTY_STATE;

void changeState(RobotState* newState) {
  // check if nullptr, just in case
  if (currentState) currentState->exit();
  
  currentState = newState;
  
  if (currentState) currentState->enter();
}
