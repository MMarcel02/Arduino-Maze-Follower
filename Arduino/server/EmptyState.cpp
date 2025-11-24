#include "EmptyState.h"

RobotState* EmptyState::handle(WiFiClient& client) {
  // no state changes, return THIS state
  return this;
}
