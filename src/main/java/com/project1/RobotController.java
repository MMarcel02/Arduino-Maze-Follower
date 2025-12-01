package com.project1;

import javafx.application.Platform;

import java.net.http.HttpResponse;
import java.util.Set;
import java.util.function.Consumer;

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

    public void applySpeed() {
        sendRequest(ArduinoEndpoints.getSpeedEndpoint(model.getSpeed()));
    }

    public void applySensitivity() {
        sendRequest(ArduinoEndpoints.getSensitivityEndpoint(model.getSensitivity()));
    }

    public void applyDampening() {
        sendRequest(ArduinoEndpoints.getDampeningEndpoint(model.getDampening()));
    }
    
    public void applyEmergencyStopDistUpdate() {
        sendRequest(ArduinoEndpoints.getEmergencyStopDistanceEndpoint(model.getEmergencyStopDistance()));
    }

    public void toggleEmergencyStop() {
        sendRequest(ArduinoEndpoints.TOGGLE_EMERGENCY_STOP);
    }
    
    public void toggleLineFollowing() {
        sendRequest(ArduinoEndpoints.TOGGLE_LINE_FOLLOWING);
    }

    public void stop() {
        sendRequest(ArduinoEndpoints.STOP);
    }

    public void handleMovement(Set<String> activeInputs) {
        
        if (activeInputs.isEmpty()) {
            sendRequest(ArduinoEndpoints.STOP);
            return;
        }

        boolean forward   = activeInputs.contains("W");
        boolean backward  = activeInputs.contains("S");
        boolean left      = activeInputs.contains("A");
        boolean right     = activeInputs.contains("D");
        boolean shift     = activeInputs.contains("SHIFT");
        boolean shiftLeft = activeInputs.contains("SHIFT_A"); 
        boolean shiftRight= activeInputs.contains("SHIFT_D"); 

        String commandToSend;

        if (forward && !left && !right) {
            commandToSend = ArduinoEndpoints.FORWARD;
        } else if (forward && left) {
            commandToSend = ArduinoEndpoints.LEFT;
        } else if (forward && right) {
            commandToSend = ArduinoEndpoints.RIGHT;
        } else if (left && !right) {
            commandToSend = ArduinoEndpoints.TURN_ON_SPOT_LEFT;
        } else if (right && !left) {
            commandToSend = ArduinoEndpoints.TURN_ON_SPOT_RIGHT;
        } else if (backward) {
            commandToSend = ArduinoEndpoints.BACKWARD;
        } else if ((shift && left && !right) || shiftLeft) { 
            commandToSend = ArduinoEndpoints.CRAB_WALK_LEFT;
        } else if ((shift && right && !left) || shiftRight) {
            commandToSend = ArduinoEndpoints.CRAB_WALK_RIGHT;
        } else {
            commandToSend = ArduinoEndpoints.STOP;
        }

        sendRequest(commandToSend);
    }

    public void sendRequest(String endpoint) {
        
        currentEndpoint = endpoint;
        logger.accept("Sending: " + endpoint);

        new Thread(() -> {
            try {
                HttpResponse<String> response = client.send(endpoint);
                Platform.runLater(() -> logger.accept("Response: " + response.body()));
            } catch (Exception e) {
                Platform.runLater(() -> logger.accept("Failed: " + e.getMessage()));
            }
        }).start();
    }

    public String getCurrentEndpoint() {
        return currentEndpoint;
    }
}