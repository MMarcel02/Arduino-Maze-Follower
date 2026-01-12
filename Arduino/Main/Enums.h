// These states are here because they are also passed to the GUI
// enum assigns numbers to these words (less mistakes than using strings (typos) and easier and faster to compare numbers) 
enum RobotMovementState {
  STOPPED, // Compiler assigns this = 0
  FORWARD, // Compiler assigns this = 1
  BACKWARD,
  LEFT,
  RIGHT,
  TURN_SPOT_LEFT,
  TURN_SPOT_RIGHT,
  CW_LEFT,
  CW_RIGHT
};
RobotMovementState currentMovementState = STOPPED;

enum RobotControlState {
  MANUAL,
  LINE_FOLLOW_BANGBANG,
  SOLVE_MAZE_1,
  SOLVE_MAZE_2,
  LOST_ROBOT,
  EMERGENCY_STOP,
  U_TURN,
  PARKING_IN_BOX
};
RobotControlState currentControlState = MANUAL;

enum MazeState {
  FOLLOW_LINE,
  TURNING_LEFT, 
  BLIND_TURN,   
  TURNING_RIGHT,
  OBJECT_DETECTED,
  TURNING_180_DEGREES,
  AFTER_180_RIGHT_SENSOR_SEARCH,
  JUNCTION_FOUND,
  SQUARE_UP_JUNCTION,
  DRIVE_THROUGH_INTERSECTION, 
  SCAN_LEFT_FOR_LINE,         
  ALIGN_EXTRA_LEFT,           
  RECOVER_RIGHT_FIND_BLACK,   
  RECOVER_RIGHT_FIND_WHITE,   
  LOST_ROBOT1
};
MazeState mazeState = FOLLOW_LINE;

enum LostRobotState {
  SEARCHING_FOR_THE_LINE,
  FOUND_THE_LINE,
};
LostRobotState lostRobotState = SEARCHING_FOR_THE_LINE;

enum EmergencyStopState {
  BANG_LINE_FOLLOWING,
  EMERGENCY_STOPPED
};
EmergencyStopState emergencyStopState = BANG_LINE_FOLLOWING;

enum ParkingBoxState {
    APPROACHING_PARKING_BOX,
    PARKING_TURNING_LEFT,
    PARKING_TURNING_RIGHT,
    PARKING_BLIND_TURN,
    SELF_ALIGN_90_DEGREES,
    PARKING_SEARCH_START_LINE,
    PARKING_DRIVE_INTO_BOX,
    PARKING_FALLBACK_SEARCH_FOR_END,
    END_OF_PARKING_BOX
};
ParkingBoxState parkingBoxState = APPROACHING_PARKING_BOX;
