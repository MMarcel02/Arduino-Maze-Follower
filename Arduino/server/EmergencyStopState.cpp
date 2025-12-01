#include "EmergencyStopState.h"
#include "hardware.h"
#include "states.h"

void EmergencyStopState::enter(WiFiClient& client) {
    // Notifications can be sent here if required
    stopAllMotors(); // Ensure all motors are stopped when entering this state
}

RobotState* EmergencyStopState::handle(WiFiClient& client) {
    // Check if the emergency stop condition is still active
    if (getDistanceCM() >= 20) {
        return &MANUAL_CONTROL_STATE; // Transition back to manual control if the way is clear
    }
    // If still close, remain in the emergency stop state
    return this;
}
