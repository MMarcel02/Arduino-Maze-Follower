package com.project1;

import java.net.http.HttpResponse;
import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.Set;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.control.ToggleButton;
import javafx.scene.input.MouseEvent;
import javafx.animation.Timeline;
import javafx.application.Platform;

public class GUIController {

    // Helping controllers
    private final RobotModel robotModel = new RobotModel();
    private final InputHandler inputHandler = new InputHandler();
    private RobotController robotController;
    private MapController mapController;
    
    // Networking
    private ArduinoHTTPClient httpClient = new ArduinoHTTPClient();
    private ArduinoTCPClient tcpClient;

    
    @FXML private Canvas canvas;
    @FXML private TextArea logArea;
    @FXML private Label activeInputsLabel, speedLabel, xLabel, yLabel, angleLabel;
    @FXML private Label irLeft, irRight, ultraSonic;
    @FXML private Slider speedSlider, canvasRotationSlider, emergencyStopSlider, sensitivitySlider, dampeningSlider;
    @FXML private Button clearMapButton, solveMaze, lostRobot, dragRace;
    @FXML private Button upArrow, downArrow, leftArrow, rightArrow, crabWalkLeft, crabWalkRight, stopButton;
    @FXML private Button bigDecrement, smallDecrement, bigIncrement, smallIncrement;
    @FXML private ToggleButton emergencyStopToggle, lineFollowToggle;
    @FXML private Button parkingInBox, reverseCorner, reverseStraight, threePointTurn, uTurn, extraSpace;


    @FXML
    void bigDecreaseSpeed(MouseEvent event) { 
        robotModel.updateSpeed(-20);
        updateSpeedLabel();    
    }

    @FXML
    void bigIncreaseSpeed(MouseEvent event) {
        robotModel.updateSpeed(20);
        updateSpeedLabel();
    }
    
    @FXML
    void smallDecreaseSpeed(MouseEvent event) {
        robotModel.updateSpeed(-5);
        updateSpeedLabel();
    }

    @FXML
    void smallIncreaseSpeed(MouseEvent event) {
        robotModel.updateSpeed(5);
        updateSpeedLabel();
    }

    @FXML
    void stopSpeed(MouseEvent event) {
        robotController.stop();
    }

    @FXML 
    void clearMap(MouseEvent event) {mapController.resetMap();}
    
    @FXML
    void toggleEmergencyStop(ActionEvent event) {robotController.toggleEmergencyStop();}

    @FXML
    void toggleLineFollowing(ActionEvent event) {robotController.toggleLineFollowing();}

    // For Phase 3
    @FXML
    void dragRace(MouseEvent event) {}

    @FXML
    void parkingInBox(MouseEvent event) {}

    @FXML
    void reverseCorner(MouseEvent event) {}

    @FXML
    void reverseStraight(MouseEvent event) {}

    @FXML
    void lostRobot(MouseEvent event) {}

    @FXML
    void solveMaze(MouseEvent event) {}

    @FXML
    void threePointTurn(MouseEvent event) {}

    @FXML
    void uTurn(MouseEvent event) {}


    @FXML
    void onButtonPressed(MouseEvent event) {
       Button source = (Button) event.getSource(); // Gives us the button that is being pressed
       String key = mapButtonToKey(source); // We look up which key that button corresponds to
        if (key != null && inputHandler.addKey(key)) { // We add that key to the list of inputs 
            updateMovement(); // We call the movement function with our new list
        }
    }

    @FXML
    void onButtonReleased(MouseEvent event) {
        Button source = (Button) event.getSource();
        String key = mapButtonToKey(source);
        if (key != null && inputHandler.removeKey(key)) {
            updateMovement();
        }
    }

    @FXML
    public void initialize() {
        robotController = new RobotController(httpClient, this::logToTextArea, robotModel);
        mapController = new MapController(canvas, robotModel, robotController);

        setupSliders();
        
        tcpClient = new ArduinoTCPClient(data -> {
            Platform.runLater(() -> {
                updateSensorLabels(data);
            });
        });
        tcpClient.connect();

        // Animation that updates the UI
        startLabelUpdater();
    }

    private void startLabelUpdater() {
        Timeline guiLoop = new javafx.animation.Timeline(
            new javafx.animation.KeyFrame(javafx.util.Duration.millis(50), e -> {

                double displayX = (mapController.getX() - canvas.getWidth()/2);
                double displayY = -(mapController.getY() - canvas.getHeight()/2);
                
                xLabel.setText("X: " + (int)displayX);
                yLabel.setText("Y: " + (int)displayY);
                angleLabel.setText(String.format("Angle: %.1f°", mapController.getAngleDegrees()));
            })
        );
        guiLoop.setCycleCount(javafx.animation.Timeline.INDEFINITE);
        guiLoop.play();
    }

    
    private void setupSliders() {

        // Listener fires when dragging starts or stops
        // Speed is only sent to the robot once drag is released, to avoid spamming requests  
        speedSlider.valueProperty().addListener((obs, oldVal, newVal) -> {                
            robotModel.setSpeed((int) speedSlider.getValue());
            updateSpeedLabel();    
            if (!speedSlider.isValueChanging()) {
                robotController.applySpeed();
            }
        });

        // Listener fires when dragging starts or stops
        speedSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.applySpeed();
            }
        });

        canvasRotationSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            mapController.setAngleMultiplier(canvasRotationSlider.getValue());  
        });

        emergencyStopSlider.valueProperty().addListener((obs, oldVal, newVal) -> {                
            if (!emergencyStopSlider.isValueChanging()) {
                robotModel.setEmergencyStopDistance((int) emergencyStopSlider.getValue());
                robotController.applyEmergencyStopDistUpdate();
            }
        });

        emergencyStopSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotModel.setEmergencyStopDistance((int) emergencyStopSlider.getValue());
                robotController.applyEmergencyStopDistUpdate();
            }
        });

        sensitivitySlider.valueProperty().addListener((obs, oldVal, newVal) -> {                
            robotModel.setSensitivity(Math.round(sensitivitySlider.getValue() * 10) / 10.0);
            if (!sensitivitySlider.isValueChanging()) {
                robotController.applySensitivity();
            }
        });

        sensitivitySlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.applySensitivity();
            }
        });

        dampeningSlider.valueProperty().addListener((obs, oldVal, newVal) -> {                
            robotModel.setDampening(Math.round(dampeningSlider.getValue() * 10) / 10.0);
            if (!dampeningSlider.isValueChanging()) {
                robotController.applyDampening();
            }
        });

        dampeningSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.applyDampening();
            }
        });
    }


    private String mapButtonToKey(Button button) {
    // Converts GUI button presses into the same virtual keys used for keyboard input
        if (button == upArrow) return "W";
        if (button == downArrow) return "S";
        if (button == leftArrow) return "A";
        if (button == rightArrow) return "D";
        if (button == crabWalkLeft) return "SHIFT_A";
        if (button == crabWalkRight) return "SHIFT_D";
        return null;
    }



    private void logToTextArea(String message) {
        String timestamp = LocalTime.now().truncatedTo(ChronoUnit.SECONDS).toString();
        logArea.appendText("[" + timestamp + "] " + message + "\n");     
    }
    
    public void setupInputHandlers(Scene scene){
        // Key events are attached at Scene level so 
        scene.setOnKeyPressed(event -> {
            String key = event.getCode().toString();
            if (inputHandler.addKey(key)) {
                updateMovement();
            }
        });

        scene.setOnKeyReleased(event -> {
            String key = event.getCode().toString();
            if (inputHandler.removeKey(key)) {
                updateMovement();
            }
        });
    }

    private void updateMovement() {
        updateActiveInputsLabel();
        robotController.handleMovement(inputHandler.getActiveInputs());
    }

    private void updateActiveInputsLabel() {
        Set<String> active = inputHandler.getActiveInputs();
        if (activeInputsLabel != null) {
            if (active.isEmpty()) {
                activeInputsLabel.setText("Active: None");
            } else {
                activeInputsLabel.setText("Active: " + String.join(", ", active));
            }
        }
    }

    public void shutdown() {
        if (tcpClient != null) {
            tcpClient.disconnect();
        }
    }

    private void updateSpeedLabel() {
        speedLabel.setText("Speed: " + robotModel.getSpeed());
        speedSlider.setValue(robotModel.getSpeed());
    }

    private void updateSensorLabels(String sensorData) {
        // Since TCP can be lossy we need to check if we actually got the data
        if (sensorData == null || sensorData.isEmpty()) {
            return;
        }
        
        String[] parts = sensorData.split(",");
        if (parts.length == 3) {

            ultraSonic.setText("Ultrasonic: " + parseDistance(parts[0]));
            irLeft.setText("IR Left: " + parseToColour(parts[1]));
            irRight.setText("IR Right: " + parseToColour(parts[2]));
        }
    }

    private String parseDistance(String dist) {
        if (dist.equals("0.00")) {
            return "Out Of Range";
        } else {
            return dist + " cm";
        }
    }

    private String parseToColour(String str) {
        if (str.equals("1")) {
            return "BLACK";
        } else {
            return "WHITE";
        }
    }
}

