package com.project1.view;

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

import javafx.fxml.FXML;
import javafx.geometry.Point2D;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.input.MouseEvent;
import javafx.application.Platform;
import javafx.beans.binding.Bindings;

public class DashboardView {

    private final double PIXELS_PER_CM = 1.0; // 1:1 scale for simplicity

    private final RobotModel robotModel = new RobotModel();
    private final InputHandler inputHandler = new InputHandler();
    private RobotController robotController;
    private MapView mapView;
    
    private ArduinoHTTPClient httpClient;
    private ArduinoTCPClient tcpClient;
    
    @FXML private Canvas canvas;
    @FXML private TextArea logArea;
    @FXML private Label activeInputsLabel, speedLabel, xLabel, yLabel, angleLabel, currentMovementStateLabel, totalDistanceLabel, currentControlStateLabel;
    @FXML private Label irDigitalLeft, irDigitalRight, ultraSonic;
    @FXML private Slider speedSlider, emergencyStopSlider;
    @FXML private Button upArrow, downArrow, leftArrow, rightArrow, crabWalkLeft, crabWalkRight, stopButton;

    // Speed
    @FXML void bigDecreaseSpeed() { robotController.setSpeed(robotModel.getSpeed() - 20); }
    @FXML void bigIncreaseSpeed() { robotController.setSpeed(robotModel.getSpeed() + 20); }
    @FXML void smallDecreaseSpeed() { robotController.setSpeed(robotModel.getSpeed() - 5); }
    @FXML void smallIncreaseSpeed() { robotController.setSpeed(robotModel.getSpeed() + 5); }
    @FXML void stopSpeed() { robotController.stop(); }

    // Map
    @FXML void clearMap() { robotController.resetOdometry(); }

    // Control State
    @FXML void handleBangLineFollow() { robotController.setControlState(RobotControlState.LINE_FOLLOW_BANGBANG); }
    @FXML void handleSolveMaze1() { robotController.setControlState(RobotControlState.SOLVE_MAZE_1); }
    @FXML void handleSolveMaze2() { robotController.setControlState(RobotControlState.SOLVE_MAZE_2); }
    @FXML void handleLostRobot() { robotController.setControlState(RobotControlState.LOST_ROBOT); }
    @FXML void handleEmergencyStop() { robotController.setControlState(RobotControlState.EMERGENCY_STOP); }
    @FXML void handleUTurn() { robotController.setControlState(RobotControlState.U_TURN); }
    @FXML void handleParkingInBox() { robotController.setControlState(RobotControlState.PARKING_IN_BOX); }

    // Only for arrow buttons, so that robot only drives when we're holding the button down
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
        httpClient = new ArduinoHTTPClient(this::logToTextArea);
        robotController = new RobotController(httpClient, this::logToTextArea, robotModel);

        mapView = new MapView(canvas, robotModel);
        mapView.startRendering();

        setupBindings();
        setupSliders();

        
        tcpClient = new ArduinoTCPClient(this::logToTextArea, data -> {
            Platform.runLater(() -> {
                updateModelWithTCPData(data, robotModel);
            });
        });
        
        tcpClient.connect();
    }

    private void setupBindings() {
        
        speedLabel.textProperty().bind(Bindings.concat("Speed: ", robotModel.speedProperty()));
        
        // Map
        xLabel.textProperty().bind(Bindings.format("X: %.1f cm", robotModel.xProperty()));
        yLabel.textProperty().bind(Bindings.format("Y: %.1f cm", robotModel.yProperty()));
        angleLabel.textProperty().bind(Bindings.format("Angle: %.1f deg", robotModel.angleProperty().multiply(180 / Math.PI)));
        totalDistanceLabel.textProperty().bind(Bindings.format("Dist: %.1f cm", robotModel.totalDistanceProperty()));

        // Sensors
        irDigitalLeft.textProperty().bind(Bindings.concat("IR-D Left: ", robotModel.leftIRDigitalProperty()));
        irDigitalRight.textProperty().bind(Bindings.concat("IR-D Right: ", robotModel.rightIRDigitalProperty()));

        ultraSonic.textProperty().bind(Bindings.concat("Ultrasonic: ", robotModel.ultrasonicProperty()));

        // States
        currentMovementStateLabel.textProperty().bind(Bindings.concat("M: ", robotModel.movementStateProperty()));
        currentControlStateLabel.textProperty().bind(Bindings.concat("C: ", robotModel.controlStateProperty()));
    }
    
    private void setupSliders() {
        // Listener fires when dragging stops, this sends HTTP req
        speedSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.setSpeed((int) speedSlider.getValue());
            }
        });

        emergencyStopSlider.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (!isChanging) {
                robotController.setEmergencyStopDistance((int) emergencyStopSlider.getValue());
            }
        });

        // Make sliders equal to default values
        speedSlider.setValue(robotModel.getSpeed());
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
        Platform.runLater(() -> {
            logArea.appendText("[" + timestamp + "] " + message + "\n");     
        });
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
            if (parts.length >= 7) {
                double newAngle = Double.parseDouble(parts[5]);
                double newTotalDistance = Double.parseDouble(parts[6]);

                double changeInDistance = newTotalDistance - robotModel.getTotalDistance();
                double changeInPixels = changeInDistance * PIXELS_PER_CM;

                double currentX = robotModel.getX();
                double currentY = robotModel.getY();

                double newX = currentX + (changeInPixels * Math.cos(newAngle));
                double newY = currentY - (changeInPixels * Math.sin(newAngle));

                robotModel.setX(newX);
                robotModel.setY(newY);

                robotModel.getPositionHistory().add(new Point2D(newX, newY));

                robotModel.setSensorData(
                    parseDistance(parts[0]), // Ultrasonic distance
                    parseToColour(parts[1]), //Left Digital ("WHITE" or "BLACK")
                    parseToColour(parts[2]), //Right Digital
                    newAngle, // Absolute angle in radiancs
                    newTotalDistance  // Total Distance in centimetres
                );
                robotModel.setMovementState(RobotMovementState.values()[Integer.parseInt(parts[3])]);
                robotModel.setControlState(RobotControlState.values()[Integer.parseInt(parts[4])]);

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

