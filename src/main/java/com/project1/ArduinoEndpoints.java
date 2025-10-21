package com.project1;

public final class ArduinoEndpoints {

    // Movement commands
    public static final String FORWARD            = "/forward";
    public static final String BACKWARD           = "/backward";
    public static final String TURN_ON_SPOT_RIGHT = "/turnOnSpotRight";
    public static final String TURN_ON_SPOT_LEFT  = "/turnOnSpotLeft";
    public static final String LEFT               = "/left";
    public static final String RIGHT              = "/right";
    public static final String STOP               = "/stop";
    public static final String CRAB_WALK_LEFT     = "/crabWalkLeft";
    public static final String CRAB_WALK_RIGHT    = "/crabWalkRight";

    public static String getSpeedEndpoint(int value) {
        return "/setSpeed?s=" + value;
    }

    private ArduinoEndpoints() {}
}
