package com.project1.services;


public final class ArduinoEndpoints {

    // These are the things we attach at the end of the IP address. e.g. 192.168.1.1/forward for our HTTP request in ArduinoClient
    public static final String FORWARD            = "/forward";
    public static final String BACKWARD           = "/backward";
    public static final String TURN_ON_SPOT_RIGHT = "/turnOnSpotRight";
    public static final String TURN_ON_SPOT_LEFT  = "/turnOnSpotLeft";
    public static final String LEFT               = "/left";
    public static final String RIGHT              = "/right";
    public static final String STOP               = "/stop";
    public static final String CRAB_WALK_LEFT     = "/crabWalkLeft";
    public static final String CRAB_WALK_RIGHT    = "/crabWalkRight";
    public static final String TOGGLE_EMERGENCY_STOP    = "/toggleEmergencyStop";
    public static final String TOGGLE_LINE_FOLLOWING    = "/toggleLineFollowing";

    public static final String MANUAL               = "/manual";
    public static final String LINE_FOLLOW_BANGBANG = "/lineFollowBangBang";
    public static final String LINE_FOLLOW_PD       = "/lineFollowPD";
    public static final String SOLVE_MAZE_1         = "/solveMaze1";
    public static final String SOLVE_MAZE_2         = "/solveMaze2";
    public static final String LOST_ROBOT           = "/lostRobot";
    public static final String REVERSE_STRAIGHT     = "/reverseStraight";
    public static final String REVERSE_CORNER       = "/reverseCorner";
    public static final String THREE_POINT_TURN     = "/threePointTurn";
    public static final String U_TURN               = "/uTurn";
    public static final String PARKING_IN_BOX       = "/parkingInBox";

    public static String getSpeedEndpoint(int value) {
        return "/setSpeed?s=" + value;
    }

    public static String getEmergencyStopDistanceEndpoint(int value) {
        return "/setEmergencyStopDistance?s=" + value;
    }

    public static String getSensitivityEndpoint(double value) {
        return "/setSensitivity?s=" + value;
    }

    public static String getDampeningEndpoint(double value) {
        return "/setDampening?s=" + value;
    }

    public static String getLeftIRThresholdEndpoint(int value) {
        return "/setLeftIR?s=" + value;
    }

    public static String getRightIRThresholdEndpoint(int value) {
        return "/setRightIR?s=" + value;
    }
    
}
