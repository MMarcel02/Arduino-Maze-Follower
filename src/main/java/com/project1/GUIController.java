package com.project1;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.scene.input.MouseEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;

public class GUIController {

    private int speed;

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
    private Font x1;

    @FXML
    private Color x2;

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
    void crabWalkLeft(MouseEvent event) {

    }

    @FXML
    void crabWalkRight(MouseEvent event) {

    }

    @FXML
    void moveBackwards(MouseEvent event) {

    }

    @FXML
    void moveForward(MouseEvent event) {

    }

    @FXML
    void rotateLeft(MouseEvent event) {

    }

    @FXML
    void rotateRight(MouseEvent event) {

    }

    @FXML
    void stopSpeed(MouseEvent event) {

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
        //Slider updates our speed value
        speedSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            speed = newVal.intValue();
            speedLabel.setText("Speed: " + speed);
        });
    }

    private void updateSpeed(int changeInSpeed) {
        if ((speed + changeInSpeed) <= 0) {
            speed = 0;
        } else if ((speed + changeInSpeed) >= 255) {
            speed = 255;
        } else {
            speed += changeInSpeed;
        }
        speedSlider.setValue(speed);
    }

}
