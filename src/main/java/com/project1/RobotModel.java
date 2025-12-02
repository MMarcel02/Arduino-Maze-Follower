package com.project1;

// In MVC, Model holds all the values of our actual Robot

public class RobotModel {

    private int speed = 80;
    private double sensitivity = 5.0;
    private double dampening = 1.0;
    private int emergencyStopDistance = 20;
    private RobotState state = RobotState.STOPPED;
    private boolean followingLine = false;


    public int getSpeed() { return speed; }
    public void setSpeed(int speed) {
        if (speed < 0) speed = 0;
        if (speed > 255) speed = 255;
        this.speed = speed;
    }

    public void updateSpeed(int changeInSpeed) {
        setSpeed(this.speed + changeInSpeed);
    }

    public double getSensitivity() { return sensitivity; }
    public void setSensitivity(double sensitivity) { this.sensitivity = sensitivity; }

    public double getDampening() { return dampening; }
    public void setDampening(double dampening) { this.dampening = dampening; }

    public int getEmergencyStopDistance() { return emergencyStopDistance; }
    public void setEmergencyStopDistance(int emergencyStopDistance) { this.emergencyStopDistance = emergencyStopDistance; }

    public void setState(RobotState state) {
        this.state = state;
    }
    public RobotState getState() {return state;}

    public void toggleLineFollowing() {
        followingLine = !followingLine;
    }

    public boolean getFollowingLineStatus() {
        return followingLine;
    }
}