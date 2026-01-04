package com.project1.model;

// We can add the other control states here (like maze following, PD, U turn etc)
public enum RobotControlState {
    MANUAL,
    LINE_FOLLOW_BANGBANG,
    LINE_FOLLOW_PD,
    SOLVE_MAZE_1,
    SOLVE_MAZE_2,
    LOST_ROBOT,
    REVERSE_STRAIGHT,
    REVERSE_CORNER,
    THREE_POINT_TURN,
    U_TURN,
    PARKING_IN_BOX
}
