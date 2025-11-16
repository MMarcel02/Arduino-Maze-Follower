package com.project1;

import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.HashSet;
import java.util.Set;
import java.util.ArrayList;


import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.control.ToggleButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.animation.KeyFrame;
import javafx.geometry.Point2D;
import javafx.util.Duration;



public class GUIController {

    private ArduinoClient client = new ArduinoClient();

    private int speed = 80;

    private static final double MAX_PIXELS_PER_SECOND = 50;
    private static final double MAX_RADIANS_PER_SECOND = Math.PI/2.0;

    private String currentEndpoint = ArduinoEndpoints.STOP;
    private double robotX;
    private double robotY;
    private double robotAngle;
    
    private ArrayList<Point2D> positionHistory = new ArrayList<>();

    // A HashSet is basically just an ArrayList that cant have repeated elements, so e.g. "W, W, D" is not allowed
    private final Set<String> activeInputs = new HashSet<>();
    
    private GraphicsContext gc;
    private double centerX;
    private double centerY;
    
    @FXML
    private Canvas canvas;


    @FXML
    private ToggleButton emergencyStopToggle;

    @FXML
    private Label activeInputsLabel;

    @FXML
    private TextArea logArea;

    @FXML
    private Button clearMapButton;

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
    private Button extraSpace;

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
    private Label xLabel;

    @FXML
    private Label yLabel;

    @FXML
    private Label angleLabel;

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
    void clearMap(MouseEvent event) {
        positionHistory = new ArrayList<>();
                // Store center coordinates
        centerX = canvas.getWidth() / 2.0;
        centerY = canvas.getHeight() / 2.0;

        // Robot's logical position starts at the center
        robotX = centerX;
        robotY = centerY;
        robotAngle = Math.toRadians(90); // Start pointing UP
    }

    @FXML
    void dragRace(MouseEvent event) {

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
    void toggleEmergencyStop(ActionEvent event) {
        sendRequest(ArduinoEndpoints.TOGGLE_EMERGENCY_STOP);
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
        
        gc = canvas.getGraphicsContext2D();

        // Store center coordinates
        centerX = canvas.getWidth() / 2.0;
        centerY = canvas.getHeight() / 2.0;

        // Robot's logical position starts at the center
        robotX = centerX;
        robotY = centerY;
        robotAngle = Math.toRadians(90); // Start pointing UP


        Timeline robotPositionTimeline = new Timeline(new KeyFrame(Duration.millis(50), e -> {
            gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
            
            // 1. Update the robot's logical position
            updateRobotPosition(0.05);

            // 2. Save the default canvas state
            gc.save();

            // 3. Calculate the translation needed to keep the robot centered
            double offsetX = centerX - robotX;
            double offsetY = centerY - robotY;
            gc.translate(offsetX, offsetY);
            
            // 4. Draw the path. This is now drawn in the "moved" world.
            drawLastLine();
            
            // 5. Restore the canvas to its original state (no translation)
            gc.restore();
            
            // 6. Draw the robot at the fixed center, with rotation
            drawArrow();

            xLabel.setText("X: " + (int) -offsetX);
            yLabel.setText("Y: " + (int) offsetY);

            double angleInDegrees = Math.toDegrees(robotAngle);
            // Normalize angle to be in the range [0, 360)
            double displayAngle = ((angleInDegrees % 360) + 360) % 360; 
            angleLabel.setText(String.format("Angle: %.1f°", displayAngle));
        }));

        robotPositionTimeline.setCycleCount(Timeline.INDEFINITE);
        robotPositionTimeline.play();

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

    private void updateRobotPosition(double changeInTime) {
        double linearVelocity = MAX_PIXELS_PER_SECOND * ((double) speed / 255.0);
        double angularVelocity = MAX_RADIANS_PER_SECOND * ((double) speed / 255.0);

    
        switch(currentEndpoint){
            case ArduinoEndpoints.FORWARD:    
                robotX += linearVelocity * changeInTime * Math.cos(robotAngle);
                robotY -= linearVelocity * changeInTime * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.BACKWARD:
                robotX -= linearVelocity * changeInTime * Math.cos(robotAngle);
                robotY += linearVelocity * changeInTime * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.RIGHT:
                robotAngle -= angularVelocity * changeInTime;
                robotX += linearVelocity * changeInTime * Math.cos(robotAngle);
                robotY -= linearVelocity * changeInTime * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.LEFT:
                robotAngle += angularVelocity * changeInTime;
                robotX += linearVelocity * changeInTime * Math.cos(robotAngle);
                robotY -= linearVelocity * changeInTime * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.TURN_ON_SPOT_RIGHT:
                robotAngle -= angularVelocity * changeInTime;
                break;
            case ArduinoEndpoints.TURN_ON_SPOT_LEFT:
                robotAngle += angularVelocity * changeInTime;
                break;

            // case ArduinoEndpoints.CRAB_WALK_RIGHT:
            //     robotX += linearVelocity * changeInTime * Math.cos(robotAngle);
            //     positionHistory.add(new Point2D(robotX, robotY));
            //     break;
            // case ArduinoEndpoints.CRAB_WALK_LEFT:
            //     robotX -= linearVelocity * changeInTime * Math.cos(robotAngle);
            //     positionHistory.add(new Point2D(robotX, robotY));
            //     break;
            default:
                return;
            
        }

        
    }

    private void drawLastLine() {
        if (positionHistory.isEmpty()) return;

        // Draw the starting dot
        gc.setFill(javafx.scene.paint.Color.BLACK);
        Point2D startPos = positionHistory.get(0);
        gc.fillOval(startPos.getX() - 2, startPos.getY() - 2, 4, 4);

        if (positionHistory.size() < 2) return;

        gc.setStroke(Color.valueOf("#4F1C51"));
        gc.setLineWidth(2);

        // Loop from the second point
        for (int i = 1; i < positionHistory.size(); i++) {
            Point2D oldPos = positionHistory.get(i - 1);
            Point2D currentPos = positionHistory.get(i);

            gc.strokeLine(oldPos.getX(), oldPos.getY(), currentPos.getX(), currentPos.getY());
        }
        
    }

    // Mention used chatgpt for this or figure out a different way
    private void drawArrow() {
        // Size of the robot arrow
        double arrowSize = 12;
        double headSize = 8;

        // Define the arrow shape (a triangle pointing right, as 0 angle is to the right)
        double[] xPoints = { arrowSize / 2, -arrowSize / 2, -arrowSize / 2 };
        double[] yPoints = { 0,             -headSize / 2,  headSize / 2 };

        gc.save();
        
        // 1. Move to the center of the canvas
        gc.translate(centerX, centerY);
        
        // 2. Rotate the canvas. 
        //    Our robotAngle is CCW (0=Right, 90=Up).
        //    JavaFX rotate() is CW. So we use the negative angle.
        gc.rotate(-Math.toDegrees(robotAngle));
        
        // 3. Draw the arrow shape at (0,0) of the translated/rotated context
        // Also correct (using Color.valueOf):
        gc.setFill(Color.valueOf("#210F37"));
        gc.fillPolygon(xPoints, yPoints, 3);
        
        // 4. Restore the canvas to its original state
        gc.restore();
    }

    private void sendRequest(String endpoint) {
        logToTextArea("Sending request to " + endpoint + "...");
        currentEndpoint = endpoint;
        
        
        // We start a new thread so that our GUI doesnt freeze after we send a request
        new Thread(() -> {
            try {
                client.send(endpoint);
                Platform.runLater(() -> {
                    logToTextArea("Request to " + endpoint + " succeeded!");
                });
            } catch (Exception e) {
                Platform.runLater(() -> {
                    logToTextArea("Request to " + endpoint + " failed! " + e.getMessage());
                });
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

        boolean forward   = activeInputs.contains("W");
        boolean backward = activeInputs.contains("S");
        boolean left      = activeInputs.contains("A");
        boolean right     = activeInputs.contains("D");
        boolean shift     = activeInputs.contains("SHIFT");
        boolean shiftLeft     = activeInputs.contains("SHIFT_A");
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