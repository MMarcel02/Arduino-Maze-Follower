package com.project1.services;

import com.project1.model.RobotModel;
import com.project1.model.RobotMovementState;
import com.project1.model.RobotControlState;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.util.Duration;

public class MapPhysics {

    private final RobotModel model;
    private Timeline positionLoop;

    private final double MAX_PIXELS_PER_SECOND = 50;
    private final double MAX_RADIANS_PER_SECOND = 2 * Math.PI;

    public MapPhysics(RobotModel model) {
        this.model = model;
    }

    public void start() {
        // We update our model every 50 ms
        positionLoop = new Timeline(new KeyFrame(Duration.millis(50), e -> updatePhysics(0.05)));
        positionLoop.setCycleCount(Timeline.INDEFINITE);
        positionLoop.play();
    }

    public void stop() {
        if (positionLoop != null) positionLoop.stop();
    }

    private void updatePhysics(double changeInTime) {
        RobotMovementState action = model.getMovementState();
        RobotControlState control = model.getControlState();

        
        // Stopped so dont need to calculate new position
        if (action == RobotMovementState.STOPPED) return;

        int currentSpeed = model.getSpeed();
        
        // In current Bang Bang we turn at full speed 
        if (control == RobotControlState.LINE_FOLLOW_BANGBANG && (action == RobotMovementState.TURN_SPOT_LEFT || action == RobotMovementState.TURN_SPOT_RIGHT)) {
            currentSpeed = 255;
        }

        double linearVelocity = MAX_PIXELS_PER_SECOND * (currentSpeed / 255.0);
        double angularVelocity = model.getAngleMultiplier() * MAX_RADIANS_PER_SECOND * (currentSpeed / 255.0);

        double distance = linearVelocity * changeInTime;
        double angleDistance = angularVelocity * changeInTime;

        // Get current position
        double newX = model.getX();
        double newY = model.getY();
        double newAngle = model.getAngle();

        
        switch (action) {
            case FORWARD:
                newX += distance * Math.cos(newAngle);
                newY -= distance * Math.sin(newAngle);
                break;

            case BACKWARD:
                newX -= distance * Math.cos(newAngle);
                newY += distance * Math.sin(newAngle);
                break;

            case LEFT:
                newAngle += angleDistance;
                newX += distance * Math.cos(newAngle);
                newY -= distance * Math.sin(newAngle);
                break;

            case RIGHT:
                newAngle -= angleDistance;
                newX += distance * Math.cos(newAngle);
                newY -= distance * Math.sin(newAngle);
                break;

            case TURN_SPOT_LEFT:
                newAngle += angleDistance;
                break;

            case TURN_SPOT_RIGHT:
                newAngle -= angleDistance;
                break;

            case CW_LEFT:
                newX -= distance * Math.sin(newAngle);
                newY -= distance * Math.cos(newAngle);
                break;

            case CW_RIGHT:
                newX += distance * Math.sin(newAngle);
                newY += distance * Math.cos(newAngle);
                break;
                
            default: break;
        }

        // Update model with new position
        model.setX(newX);
        model.setY(newY);
        model.setAngle(newAngle);
        
    }
}