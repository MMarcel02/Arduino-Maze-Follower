package com.project1;

import java.time.LocalTime;
import java.time.temporal.ChronoUnit;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.scene.input.MouseEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.paint.Color;    
import javafx.scene.text.Font;

public class GUIController {

    private int speed = 80;
    private ArduinoClient client = new ArduinoClient();

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
    void moveBackwards(MouseEvent event) {
        sendRequest(ArduinoEndpoints.BACKWARD);
    }

    @FXML
    void moveForward(MouseEvent event) {
        sendRequest(ArduinoEndpoints.FORWARD);
    }

    @FXML
    void rotateLeft(MouseEvent event) {
        sendRequest(ArduinoEndpoints.TURN_ON_SPOT_LEFT);
    }

    @FXML
    void rotateRight(MouseEvent event) {
        sendRequest(ArduinoEndpoints.TURN_ON_SPOT_RIGHT);
    }

    @FXML
    void crabWalkLeft(MouseEvent event) {
        sendRequest(ArduinoEndpoints.CRAB_WALK_LEFT);
    }

    @FXML
    void crabWalkRight(MouseEvent event) {
        sendRequest(ArduinoEndpoints.CRAB_WALK_RIGHT);
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
    public void initialize() {
        speedSlider.setValue(speed);

        // //Slider updates our speed value
        speedSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            // logToTextArea("Speed set to: " + speed);
                
            speed = (int) speedSlider.getValue();       
            speedLabel.setText("Speed: " + speed);
        });
            
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
        speedLabel.setText("Speed: " + speed);
        
        String speedEndpoint = ArduinoEndpoints.getSpeedEndpoint(speed);
        sendRequest(speedEndpoint);

    }

    private void logToTextArea(String message) {
        String timestamp = LocalTime.now().truncatedTo(ChronoUnit.SECONDS).toString();
        logArea.appendText("[" + timestamp + "] " + message + "\n");    
    }

}
