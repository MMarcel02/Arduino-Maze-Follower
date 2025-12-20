package com.project1.controller;

import javafx.application.Platform;

import java.net.http.HttpResponse;
import java.util.Set;
import java.util.function.Consumer;

import com.project1.model.RobotControlState;
import com.project1.model.RobotModel;
import com.project1.services.ArduinoEndpoints;
import com.project1.services.ArduinoHTTPClient;

public class RobotController {

    private final ArduinoHTTPClient client;
    private final Consumer<String> logger;
    private final RobotModel model;
    private String currentEndpoint = ArduinoEndpoints.STOP;

    public RobotController(ArduinoHTTPClient client, Consumer<String> logger, RobotModel model) {
        this.client = client;
        this.logger = logger;
        this.model = model;
    }

    public void setSpeed(int targetSpeed) {
        sendRequest(ArduinoEndpoints.getSpeedEndpoint(targetSpeed), res -> {
            model.setSpeed(targetSpeed);
            logger.accept("Speed set to: " + targetSpeed);
        });
    }

    public void setSensitivity(double targetVal) {
        sendRequest(ArduinoEndpoints.getSensitivityEndpoint(targetVal), res -> {
            model.setSensitivity(targetVal);
            logger.accept("Sensitivity set to: " + targetVal);
        });
    }

    public void setDampening(double targetVal) {
        sendRequest(ArduinoEndpoints.getDampeningEndpoint(targetVal), res -> {
            model.setDampening(targetVal);
            logger.accept("Dampening set to: " + targetVal);
        });
    }

    public void setEmergencyStopDistance(int targetDist) {
        sendRequest(ArduinoEndpoints.getEmergencyStopDistanceEndpoint(targetDist), res -> {
            model.setEmergencyStopDistance(targetDist);
            logger.accept("Emergency Stop Distance set to: " + targetDist);
        });
    }
    
    public void toggleEmergencyStop() {
        sendRequest(ArduinoEndpoints.TOGGLE_EMERGENCY_STOP, res -> {
            boolean newState = !model.isEmergencyStopEnabled();
            model.setEmergencyStopEnabled(newState);
            logger.accept("Emergency Stop Status: " + (newState ? "ENABLED" : "DISABLED"));
        });
    }

    public void setControlState(RobotControlState targetState) {
        String endpoint;
        switch (targetState) {
            case MANUAL:               endpoint = "/setMode?mode=manual"; break;
            case LINE_FOLLOW_BANGBANG: endpoint = "/setMode?mode=line_bang"; break;
            default: return;
        }

        sendRequest(endpoint, res -> {
            model.setControlState(targetState);
            logger.accept("Control Mode changed to: " + targetState);
        });
    }

    public void stop() {
        sendRequest(ArduinoEndpoints.STOP, null);
    }

    public void handleMovement(Set<String> activeInputs) {
        // We ignore keyboard input if not in manual mode
        if (model.getControlState() != RobotControlState.MANUAL) {
            return;
        }
        
        if (activeInputs.isEmpty()) {
            stop();
            return;
        }

        boolean forward   = activeInputs.contains("W");
        boolean backward  = activeInputs.contains("S");
        boolean left      = activeInputs.contains("A");
        boolean right     = activeInputs.contains("D");
        boolean shift     = activeInputs.contains("SHIFT");
        boolean shiftLeft = activeInputs.contains("SHIFT_A"); 
        boolean shiftRight= activeInputs.contains("SHIFT_D"); 

        String endpoint;

        if (forward && !left && !right) {
            endpoint = ArduinoEndpoints.FORWARD;
        } else if (forward && left) {
            endpoint = ArduinoEndpoints.LEFT;
        } else if (forward && right) {
            endpoint = ArduinoEndpoints.RIGHT;
        } else if (left && !right) {
            endpoint = ArduinoEndpoints.TURN_ON_SPOT_LEFT;
        } else if (right && !left) {
            endpoint = ArduinoEndpoints.TURN_ON_SPOT_RIGHT;
        } else if (backward) {
            endpoint = ArduinoEndpoints.BACKWARD;
        } else if ((shift && left && !right) || shiftLeft) { 
            endpoint = ArduinoEndpoints.CRAB_WALK_LEFT;
        } else if ((shift && right && !left) || shiftRight) {
            endpoint = ArduinoEndpoints.CRAB_WALK_RIGHT;
        } else {
            endpoint = ArduinoEndpoints.STOP;
        }

        sendRequest(endpoint, null);
    }

    public void sendRequest(String endpoint, Consumer<HttpResponse<String>> onSuccess) {
        
        currentEndpoint = endpoint;
        logger.accept("Sending: " + endpoint);

        new Thread(() -> {
            try {
                HttpResponse<String> response = client.send(endpoint);
                Platform.runLater(() -> {
                    if (response.statusCode() == 200) {
                        if (onSuccess != null) onSuccess.accept(response);
                    } else {
                        logger.accept("Command Failed: " + response.statusCode());
                    }
                });
            } catch (Exception e) {
                Platform.runLater(() -> logger.accept("Failed: " + e.getMessage()));
            }
        }).start();
    }

    public String getCurrentEndpoint() {
        return currentEndpoint;
    }
}