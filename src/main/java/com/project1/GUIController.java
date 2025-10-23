package com.project1;

import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.HashSet;
import java.util.Set;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.scene.input.MouseEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;


public class GUIController {

    private int speed = 80;
    private ArduinoClient client = new ArduinoClient();

    // A HashSet is basically just an ArrayList that cant have repeated elements, so e.g. "W, W, D" is not allowed
    private final Set<String> activeInputs = new HashSet<>();

    @FXML
    private Label activeInputsLabel;

    @FXML
    private TextArea logArea;

    @FXML
    private Button bigDecrement;

    @FXML
    private Button bigIncrement;

    @FXML
    private Button crabWalkLeft;

    @FXML
    private Button crabWalkRight;

    @FXML
    private Button downArrow;

    @FXML
    private Button dragRace;

    @FXML
    private Button emergencyStop;

    @FXML
    private Button leftArrow;

    @FXML
    private Button lostRobot;

    @FXML
    private Button parkingInBox;

    @FXML
    private Button reverseCorner;

    @FXML
    private Button reverseStraight;

    @FXML
    private Button rightArrow;

    @FXML
    private Button smallDecrement;

    @FXML
    private Button smallIncrement;

    @FXML
    private Button solveMaze;

    @FXML
    private Label speedLabel;

    @FXML
    private Slider speedSlider;

    @FXML
    private Button stopButton;

    @FXML
    private Button threePointTurn;

    @FXML
    private Button uTurn;

    @FXML
    private Button upArrow;

    @FXML
    void bigDecreaseSpeed(MouseEvent event) {
        updateSpeed(-20);
    }

    @FXML
    void bigIncreaseSpeed(MouseEvent event) {
        updateSpeed(20);
    }
    
    @FXML
    void smallDecreaseSpeed(MouseEvent event) {
        updateSpeed(-5);
    }

    @FXML
    void smallIncreaseSpeed(MouseEvent event) {
        updateSpeed(5);
    }


    @FXML
    void stopSpeed(MouseEvent event) {
        sendRequest(ArduinoEndpoints.STOP);
    }

    @FXML
    void dragRace(MouseEvent event) {

    }

    @FXML
    void emergencyStop(MouseEvent event) {

    }

    @FXML
    void parkingInBox(MouseEvent event) {

    }

    @FXML
    void reverseCorner(MouseEvent event) {

    }

    @FXML
    void reverseStraight(MouseEvent event) {

    }

    @FXML
    void lostRobot(MouseEvent event) {

    }

    @FXML
    void solveMaze(MouseEvent event) {

    }

    @FXML
    void threePointTurn(MouseEvent event) {

    }

    @FXML
    void uTurn(MouseEvent event) {

    }

    @FXML
    void onButtonPressed(MouseEvent event) {
        // Gives us the button that is being pressed 
        Button source = (Button) event.getSource();

        // We look up which key that button corresponds to
        String key = mapButtonToKey(source);

        // We add that key to the list of inputs 
        if (key != null && activeInputs.add(key)) {

            // We call the movement function with our new list
            handleMovement();
        }
    }

    @FXML
    void onButtonReleased(MouseEvent event) {
        // Gives us the button that is being released
        Button source = (Button) event.getSource();

        // Everything same as above but instead of adding we remove the button
        String key = mapButtonToKey(source);
        if (key != null && activeInputs.remove(key)) {
            handleMovement();
        }
    }


    private String mapButtonToKey(Button button) {
        if (button == upArrow) return "W";
        if (button == downArrow) return "S";
        if (button == leftArrow) return "A";
        if (button == rightArrow) return "D";
        if (button == crabWalkLeft) return "SHIFT_A";
        if (button == crabWalkRight) return "SHIFT_D";
        return null;
    }


    @FXML
    public void initialize() {
        speedSlider.setValue(speed);


        // Slider updates our speed value in the UI
        speedSlider.valueProperty().addListener((obs, oldVal, newVal) -> {                
            speed = (int) speedSlider.getValue();       
            speedLabel.setText("Speed: " + speed);
            if (!speedSlider.isValueChanging()) {
                String speedEndpoint = ArduinoEndpoints.getSpeedEndpoint(speed);
                sendRequest(speedEndpoint);
            }
        });

        // Speed is only sent to the robot once drag is released    
        speedSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                String speedEndpoint = ArduinoEndpoints.getSpeedEndpoint(speed);
                sendRequest(speedEndpoint); 
            }
        });

    }

    private void sendRequest(String endpoint) {
        logToTextArea("Sending request to " + endpoint + "...");

        // We start a new thread so that our GUI doesnt freeze after we send a request
        
        new Thread(() -> {
            try {
                client.send(endpoint);
                logToTextArea("Request to " + endpoint + " succeeded!");
            } catch (Exception e) {
                logToTextArea("Request to " + endpoint + " failed! " + e.getMessage());
            }
        }).start();
    }

    

    private void updateSpeed(int changeInSpeed) {
        speed += changeInSpeed;

        if (speed <= 0) speed = 0;
        if (speed >= 255) speed = 255;

        speedSlider.setValue(speed);

    }

    private void logToTextArea(String message) {
        String timestamp = LocalTime.now().truncatedTo(ChronoUnit.SECONDS).toString();
        logArea.appendText("[" + timestamp + "] " + message + "\n");    
    }

    public void setupInputHandlers(Scene scene){
        scene.setOnKeyPressed(event -> {
            String key = event.getCode().toString();
            if (activeInputs.add(key)) {
                handleMovement();
            }
        });

        scene.setOnKeyReleased(event -> {
            String key = event.getCode().toString();
            activeInputs.remove(key);
            handleMovement();
        });

    }

    private void handleMovement() {
        updateActiveInputsLabel(); 

        if (activeInputs.isEmpty()) {
            sendRequest(ArduinoEndpoints.STOP);
            return;
        }

        boolean forward  = activeInputs.contains("W");
        boolean backward = activeInputs.contains("S");
        boolean left     = activeInputs.contains("A");
        boolean right    = activeInputs.contains("D");
        boolean shift    = activeInputs.contains("SHIFT");
        boolean shiftLeft    = activeInputs.contains("SHIFT_A");
        boolean shiftRight    = activeInputs.contains("SHIFT_D");


        String commandToSend;

        if (forward && !left && !right) {
            commandToSend = ArduinoEndpoints.FORWARD;
        } else if (backward) {
            commandToSend = ArduinoEndpoints.BACKWARD;
        } else if (shift && left && !right || shiftLeft) {
            commandToSend = ArduinoEndpoints.CRAB_WALK_LEFT;
        } else if (shift && right && !left || shiftRight) {
            commandToSend = ArduinoEndpoints.CRAB_WALK_RIGHT;
        } else if (forward && left) {
            commandToSend = ArduinoEndpoints.LEFT;
        } else if (forward && right) {
            commandToSend = ArduinoEndpoints.RIGHT;
        } else if (left && !right) {
            commandToSend = ArduinoEndpoints.TURN_ON_SPOT_LEFT;
        } else if (right && !left) {
            commandToSend = ArduinoEndpoints.TURN_ON_SPOT_RIGHT;
        } else {
            commandToSend = ArduinoEndpoints.STOP;
        }

        sendRequest(commandToSend);
    }




    private void updateActiveInputsLabel() {
        if (activeInputsLabel != null) {
            if (activeInputs.isEmpty()) {
                activeInputsLabel.setText("Active: None");
            } else {
                activeInputsLabel.setText("Active: " + String.join(", ", activeInputs));
            }
        }
    }


}
