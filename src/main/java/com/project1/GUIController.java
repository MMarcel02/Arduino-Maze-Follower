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

    // 2pi radians = 360 deg, so at max speed it rotates on the graph a full 360 degrees in 1 second
    // this needs to be calibrated in real world to match our robot
    private static final double MAX_RADIANS_PER_SECOND = 2*Math.PI;

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
        // We clear our history of points
        positionHistory = new ArrayList<>();

        // Reset coordinates to 0 offset
        robotX = canvas.getWidth() / 2.0;
        robotY = canvas.getHeight() / 2.0;

        robotAngle = Math.toRadians(90);
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
        
        // This is the object used to actually draw on the canvas
        gc = canvas.getGraphicsContext2D();

        // Store center coordinates of the canvas
        centerX  = canvas.getWidth() / 2.0;
        centerY = canvas.getHeight() / 2.0;

        // Robot starts at the centre and is pointing up (90 degrees)
        robotX = centerX;
        robotY = centerY;
        robotAngle = Math.toRadians(90);

        // Creates an animation that every 50 milliseconds does the following: 
        Timeline robotPositionTimeline = new Timeline(new KeyFrame(Duration.millis(50), e -> {

            // Clears the canvas
            gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
            
            // Use this to calculate new coordinates of the robot and add it to a list of points (pathHistory)
            updateRobotPosition(0.05);

            // Save the state of the canvas (how much its offset or rotated)
            gc.save();

            // Calculate how much the robot has moved from the centre of the canvas
            double offsetX = centerX - robotX;
            double offsetY = centerY - robotY;

            // This sets the new centre of the canvas to the following coordinates (essentially moving the camera)
            // so now when the path is drawn its relative to this new centre
            gc.translate(offsetX, offsetY);
            
            // Redraws all the lines going point by point in pathHistory 
            drawPath();
            
            // Restore canvas to how it was before the translation (so centre is actually the middle of canvas)
            gc.restore();
            
            // Draw arrow (always in the centre of the canvas just keep the rotation)
            // we draw it after we restore it so that is alwa
            drawArrow();

            // Updates labels below the canvas with position of robot relative to the centre of the canvas
            xLabel.setText("X: " + (int) -offsetX);
            yLabel.setText("Y: " + (int) offsetY);
            
            // Angle is stored in radians but we want to display it in degrees
            double angleInDegrees = Math.toDegrees(robotAngle);
            // We also make sure its between 0 and 360
            double displayAngle = ((angleInDegrees % 360) + 360) % 360; 
            // Angle given to 1 decimal point
            angleLabel.setText(String.format("Angle: %.1f°", displayAngle));
        }));
        
        // Right now the animation (that creates a new point and draws it every 50 milliseconds) goes on forever
        // This could be an issue causing our GUI to start lagging after a lot of time has elapsed, instead we could
        // limit it to only track the position of e.g. last 10 mins 
        robotPositionTimeline.setCycleCount(Timeline.INDEFINITE);

        // This actually starts the animation (that we defined earlier) for a set number of frames (that we defined earlier), in this case indefinite
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
        // Convert our speed into speed of drawing on canvas
        double linearVelocity = MAX_PIXELS_PER_SECOND * ((double) speed / 255.0);
        double angularVelocity = MAX_RADIANS_PER_SECOND * ((double) speed / 255.0);

        // How many pixels the robot moved
        double distanceTravelled = linearVelocity * changeInTime;

        // How many radians the robot rotated
        double angleTravelled = angularVelocity * changeInTime;

        // All the movement can be split to be some change in x and some change in y
        // This comes from the unit circle, where any point can be represented by (cos(angle), sin(angle))
        
        // cos(angle) tells us how horizontal it is, cos(0) = 1 (so pointing right), cos(90) = 0 (so pointing up)
        // sin(angle) tells us how vertical it is, sin(0) = 0 (pointing right so 0 vertical movement) sin(90) = 1 (pointing up so only vertical movement)

        // We can use this idea to tell us how much proportionally we should move horizontally and vertically

        switch(currentEndpoint){
            case ArduinoEndpoints.FORWARD:    
                robotX += distanceTravelled * Math.cos(robotAngle);

                // We subtract here because the y axis increases as it goes down in canvas (opposite of how you think in normal math)
                // This is because in canvas the point (0,0) is the top left corner not bottom left like in normal math
                robotY -= distanceTravelled * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.BACKWARD:
                robotX -= distanceTravelled * Math.cos(robotAngle);
                robotY += distanceTravelled * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.RIGHT:
                robotAngle -= angleTravelled;
                robotX += distanceTravelled * Math.cos(robotAngle);
                robotY -= distanceTravelled * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.LEFT:
                robotAngle += angleTravelled;
                robotX += distanceTravelled * Math.cos(robotAngle);
                robotY -= distanceTravelled * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.TURN_ON_SPOT_RIGHT:
                robotAngle -= angleTravelled;
                break;
            case ArduinoEndpoints.TURN_ON_SPOT_LEFT:
                robotAngle += angleTravelled;
                break;
            case ArduinoEndpoints.CRAB_WALK_RIGHT:
                // When crabwalking robot moves perpendicular to its angle, this swaps sin and cos for x and y as sin and cos are perpendicular to eachother 
                robotX += distanceTravelled * Math.sin(robotAngle);
                robotY += distanceTravelled * Math.cos(robotAngle); 
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            case ArduinoEndpoints.CRAB_WALK_LEFT:
                robotX -= distanceTravelled * Math.sin(robotAngle);
                robotY -= distanceTravelled * Math.cos(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;
            default:
                return;
        } 
    }

    private void drawPath() {
        if (positionHistory.isEmpty()) return;

        // Draw the starting dot
        Point2D startPos = positionHistory.get(0);
        gc.fillOval(startPos.getX() - 2, startPos.getY() - 2, 4, 4);

        gc.setStroke(Color.valueOf("#4F1C51"));
        gc.setLineWidth(2);

        // Loop from the second point drawing a line between each point
        for (int i = 1; i < positionHistory.size(); i++) {
            Point2D oldPos = positionHistory.get(i - 1);
            Point2D currentPos = positionHistory.get(i);

            gc.strokeLine(oldPos.getX(), oldPos.getY(), currentPos.getX(), currentPos.getY());
        }
        
    }

    private void drawArrow() {
        // Size of arrow
        double arrowLength = 6;
        double arrowBaseWidth = 4;

        // We define the (x,y) coordinates of each of the points of the arrow
        // Apparently default in graphics to be pointing right along x axis
        double[] xPoints = {arrowLength, -arrowLength, -arrowLength};
        double[] yPoints = { 0, -arrowBaseWidth, arrowBaseWidth};

        gc.save();
        
        // Moving the drawing context to the center of the canvas
        gc.translate(centerX, centerY);
        
        // Rotating the drawing context rather than rotating the arrow so we dont need to calculate all new x and y points
        // the angle is negative because gc uses clock-wise degrees from the x axis, so 0 is right, 90 is down
        // but our actual math has the degrees go counter-clock-wise, so 0 is right BUT 90 is up
        // to keep the math simpler we choose that and just correct it her
        gc.rotate(-Math.toDegrees(robotAngle));
    
        // Set color
        gc.setFill(Color.valueOf("#210F37"));

        // Draw the arrow on the translated and rotated canvas
        gc.fillPolygon(xPoints, yPoints, 3);
        
        // Bring canvas back to original state 
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