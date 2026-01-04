package com.project1.view;

import java.net.http.HttpResponse;
import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.Set;

import com.project1.controller.InputHandler;
import com.project1.controller.RobotController;
import com.project1.model.RobotControlState;
import com.project1.model.RobotModel;
import com.project1.model.RobotMovementState;
import com.project1.services.ArduinoHTTPClient;
import com.project1.services.ArduinoTCPClient;
import com.project1.services.MapPhysics;

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
import javafx.application.Platform;
import javafx.beans.binding.Bindings;

public class DashboardView {

    // Helping controllers
    private final RobotModel robotModel = new RobotModel();
    private final InputHandler inputHandler = new InputHandler();
    private RobotController robotController;
    private MapPhysics mapPhysics;
    private MapView mapView;
    
    // Networking
    private ArduinoHTTPClient httpClient = new ArduinoHTTPClient();
    private ArduinoTCPClient tcpClient;
    
    @FXML private Canvas canvas;
    @FXML private TextArea logArea;
    @FXML private Label activeInputsLabel, speedLabel, xLabel, yLabel, angleLabel, currentMovementStateLabel, currentControlStateLabel;
    @FXML private Label irLeft, irRight, ultraSonic;
    @FXML private Slider speedSlider, canvasRotationSlider, emergencyStopSlider, sensitivitySlider, dampeningSlider;
    @FXML private Button clearMapButton;
    @FXML private Button upArrow, downArrow, leftArrow, rightArrow, crabWalkLeft, crabWalkRight, stopButton;
    @FXML private Button bigDecrement, smallDecrement, bigIncrement, smallIncrement;
    @FXML private ToggleButton emergencyStopToggle;
    @FXML private Button bangLineFollow, pdLineFollow, solveMaze1, solveMaze2, lostRobot, parkingInBox, reverseCorner, reverseStraight, threePointTurn, uTurn, extraSpace;


    @FXML void bigDecreaseSpeed(MouseEvent e) { robotController.setSpeed(robotModel.getSpeed() - 20); }
    @FXML void bigIncreaseSpeed(MouseEvent e) { robotController.setSpeed(robotModel.getSpeed() + 20); }
    @FXML void smallDecreaseSpeed(MouseEvent e) { robotController.setSpeed(robotModel.getSpeed() - 5); }
    @FXML void smallIncreaseSpeed(MouseEvent e) { robotController.setSpeed(robotModel.getSpeed() + 5); }
    @FXML void stopSpeed(MouseEvent e) { robotController.stop(); }

    @FXML void clearMap(MouseEvent e) { mapView.clear(); }
    @FXML void toggleEmergencyStop(ActionEvent event) { robotController.toggleEmergencyStop(); }

    @FXML
    public void handleBangLineFollow(MouseEvent event) {
        robotController.setControlState(RobotControlState.LINE_FOLLOW_BANGBANG);
    }

    @FXML
    public void handlePDLineFollow(MouseEvent event) {
        robotController.setControlState(RobotControlState.LINE_FOLLOW_PD);
    }

    @FXML
    public void handleSolveMaze1(MouseEvent event) {
        robotController.setControlState(RobotControlState.SOLVE_MAZE_1);
    }

    @FXML
    public void handleSolveMaze2(MouseEvent event) {
        robotController.setControlState(RobotControlState.SOLVE_MAZE_2);
    }

    @FXML
    public void handleLostRobot(MouseEvent event) {
        robotController.setControlState(RobotControlState.LOST_ROBOT);
    }

    @FXML
    public void handleReverseStraight(MouseEvent event) {
        robotController.setControlState(RobotControlState.REVERSE_STRAIGHT);
    }

    @FXML
    public void handleReverseCorner(MouseEvent event) {
        robotController.setControlState(RobotControlState.REVERSE_CORNER);
    }

    @FXML
    public void handleThreePointTurn(MouseEvent event) {
        robotController.setControlState(RobotControlState.THREE_POINT_TURN);
    }

    @FXML
    public void handleUTurn(MouseEvent event) {
        robotController.setControlState(RobotControlState.U_TURN);
    }

    @FXML
    public void handleParkingInBox(MouseEvent event) {
        robotController.setControlState(RobotControlState.PARKING_IN_BOX);
    }


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
        mapPhysics = new MapPhysics(robotModel);
        mapView = new MapView(canvas, robotModel);

        setupBindings();
        setupSliders();
        mapPhysics.start();

        
        tcpClient = new ArduinoTCPClient(data -> {
            Platform.runLater(() -> {
                updateModelWithTCPData(data, robotModel);
            });
        });
        
        tcpClient.connect();
    }

    private void setupBindings() {
        
        speedLabel.textProperty().bind(Bindings.concat("Speed: ", robotModel.speedProperty()));
        
        // Map
        xLabel.textProperty().bind(Bindings.format("X: %.1f", robotModel.xProperty()));
        yLabel.textProperty().bind(Bindings.format("Y: %.1f", robotModel.yProperty()));
        angleLabel.textProperty().bind(Bindings.format("Angle: %.2f rad", robotModel.angleProperty()));

        // Sensors
        irLeft.textProperty().bind(Bindings.concat("IR Left: ", robotModel.leftIRProperty()));
        irRight.textProperty().bind(Bindings.concat("IR Right: ", robotModel.rightIRProperty()));
        ultraSonic.textProperty().bind(Bindings.concat("Ultrasonic: ", robotModel.ultrasonicProperty()));

        // States
        emergencyStopToggle.selectedProperty().bindBidirectional(robotModel.emergencyStopEnabledProperty());
        currentMovementStateLabel.textProperty().bind(Bindings.concat("Movement State: ", robotModel.movementStateProperty()));
        currentControlStateLabel.textProperty().bind(Bindings.concat("Control Mode: ", robotModel.controlStateProperty()));
    }
    
    private void setupSliders() {

        // Listener fires when dragging stops, this sends HTTP req
        speedSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.setSpeed((int) speedSlider.getValue());
            }
        });

        // Map
        canvasRotationSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            robotModel.setAngleMultiplier(newVal.doubleValue());  
        });

        emergencyStopSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.setEmergencyStopDistance((int) emergencyStopSlider.getValue());
            }
        });

        sensitivitySlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.setSensitivity(sensitivitySlider.getValue());
            }
        });

        dampeningSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.setDampening(dampeningSlider.getValue());
            }
        });


        // Make sliders equal to default values
        speedSlider.setValue(robotModel.getSpeed());
        canvasRotationSlider.setValue(robotModel.getAngleMultiplier());
        sensitivitySlider.setValue(robotModel.getSensitivity());
        dampeningSlider.setValue(robotModel.getDampening());
        emergencyStopSlider.setValue(robotModel.getEmergencyStopDistance());
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
        if (tcpClient == null) return;
        tcpClient.disconnect();
    }

    private void updateModelWithTCPData(String tcpData, RobotModel robotModel) {
        if (tcpData == null || tcpData.isEmpty()) return;

        try {
            String[] parts = tcpData.split(",");
            if (parts.length >= 4) {
                robotModel.setSensorData(
                    parseDistance(parts[0]), 
                    parseToColour(parts[1]), 
                    parseToColour(parts[2])
                );
                robotModel.setMovementState(RobotMovementState.values()[Integer.parseInt(parts[3])]);
            }
        } catch (Exception e) {
            logToTextArea("Error parsing TCP: " + tcpData);
        }
    }

    private String parseDistance(String distance) {
        return distance.equals("0.00") ? "Out Of Range" : distance + " cm";
    }

    private String parseToColour(String colour) {
        return colour.equals("1") ? "BLACK" : "WHITE";
    }
}

