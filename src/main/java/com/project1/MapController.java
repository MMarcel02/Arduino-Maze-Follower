package com.project1;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.geometry.Point2D;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
import javafx.util.Duration;
import java.util.ArrayList;


public class MapController {

    private final Canvas canvas;
    private final GraphicsContext gc;
    private final RobotModel model;

    private final double MAX_PIXELS_PER_SECOND = 50;
    private final double MAX_RADIANS_PER_SECOND = 2 * Math.PI;
    
    private double angleMultiplier = 0.37;
    private double robotX;
    private double robotY;
    private double robotAngle;
    private double centreX;
    private double centreY;
    private ArrayList<Point2D> positionHistory = new ArrayList<>();
    private Timeline animationLoop;

    public MapController(Canvas canvas, RobotModel model) {
        this.canvas = canvas;
        this.model = model;
        this.centreX = canvas.getWidth() / 2.0;
        this.centreY = canvas.getHeight() / 2.0;
        this.gc = canvas.getGraphicsContext2D(); // This is the object used to actually draw on the canvas
        
        resetMap();
        startAnimation();
    }

    public void setAngleMultiplier(double value) {
        this.angleMultiplier = value;
    }

    public void resetMap() {
        positionHistory.clear();
        robotX = centreX;
        robotY = centreY;
        robotAngle = Math.toRadians(90); 
    }

    private void startAnimation() {
        // Creates an animation that every 50 milliseconds runs the frame method
        animationLoop = new Timeline(new KeyFrame(Duration.millis(50), e -> frame(0.05)));
        animationLoop.setCycleCount(Timeline.INDEFINITE);
        animationLoop.play();
    }

    public void stop() {
        if (animationLoop != null) animationLoop.stop();
    }

    private void frame(double changeInTime) {
        gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight()); // Clear canvas screen

        updateRobotPosition(changeInTime); // Use this to calculate new coordinates of the robot and add it to a list of points (pathHistory)

        gc.save(); // Save the state of the canvas (how much its offset or rotated)

        // Calculate how much the robot has moved from the centre of the canvas
        double offsetX = centreX - robotX;
        double offsetY = centreY - robotY;

        // This sets the new centre of the canvas to the following coordinates (essentially moving the camera)
        // so now when the path is drawn its relative to this new centre
        gc.translate(offsetX, offsetY);

        // Redraws all the lines going point by point in pathHistory 
        drawPath();
        
        // Restore canvas to how it was before the translation (so centre is actually the middle of canvas)
        gc.restore();
        
        drawArrow();
    }

    private void updateRobotPosition(double changeInTime) {
        RobotState action = model.getState();
        boolean isFollowingLine = model.getFollowingLineStatus();
        int speed;
        
        // Im bang-bang when we turn we use maximum speed, otherwise we use the current motorspeed
        if (isFollowingLine && (action == RobotState.TURN_SPOT_LEFT || action == RobotState.TURN_SPOT_RIGHT)) {
            speed = 255;
        } else {
            speed = model.getSpeed();
        }

        double linearVelocity = MAX_PIXELS_PER_SECOND * (speed / 255.0);
        double angularVelocity = angleMultiplier * MAX_RADIANS_PER_SECOND * (speed / 255.0);

        double dist = linearVelocity * changeInTime;
        double angle = angularVelocity * changeInTime;

        
        // All the movement can be split to be some change in x and some change in y
        // This comes from the unit circle, where any point can be represented by (cos(angle), sin(angle))
        
        // cos(angle) tells us how horizontal it is, cos(0) = 1 (so pointing right), cos(90) = 0 (so pointing up)
        // sin(angle) tells us how vertical it is, sin(0) = 0 (pointing right so 0 vertical movement) sin(90) = 1 (pointing up so only vertical movement)

        // We can use this idea to tell us how much proportionally we should move horizontally and vertically

        switch (action) {
            case FORWARD:
                robotX += dist * Math.cos(robotAngle);
                robotY -= dist * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;

            case BACKWARD:
                robotX -= dist * Math.cos(robotAngle);
                robotY += dist * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;

            case LEFT:
                robotAngle += angle;
                robotX += dist * Math.cos(robotAngle);
                robotY -= dist * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;

            case RIGHT:
                robotAngle -= angle;
                robotX += dist * Math.cos(robotAngle);
                robotY -= dist * Math.sin(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;

            case TURN_SPOT_LEFT:
                robotAngle += angle;
                break;

            case TURN_SPOT_RIGHT:
                robotAngle -= angle;
                break;

            case CW_LEFT:
                robotX -= dist * Math.sin(robotAngle);
                robotY -= dist * Math.cos(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;

            case CW_RIGHT:
                robotX += dist * Math.sin(robotAngle);
                robotY += dist * Math.cos(robotAngle);
                positionHistory.add(new Point2D(robotX, robotY));
                break;

            case STOPPED: break;
            default:
                break;
        }

    }

    private void drawPath() {
        if (positionHistory.isEmpty()) return;

        gc.setStroke(Color.valueOf("#4F1C51"));
        gc.setLineWidth(2);

        // Only draw if we have at least 2 points
        for (int i = 1; i < positionHistory.size(); i++) {
            Point2D p1 = positionHistory.get(i - 1);
            Point2D p2 = positionHistory.get(i);
            gc.strokeLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
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
        
        // Moving the drawing context to the centre of the canvas
        gc.translate(centreX, centreY);
        
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
    
    public double getX() { return robotX; }

    public double getY() { return robotY; }
    
    public double getAngleDegrees() { 
        double deg = Math.toDegrees(robotAngle);
        return ((deg % 360) + 360) % 360; 
    }
}