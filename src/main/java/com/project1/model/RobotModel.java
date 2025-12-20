package com.project1.model;

import javafx.beans.property.*;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Point2D;

public class RobotModel {

    // We use these JavaFX property objects so we can bind our GUI to these values
    private final IntegerProperty speed = new SimpleIntegerProperty(80);
    private final DoubleProperty sensitivity = new SimpleDoubleProperty(5.0);
    private final DoubleProperty dampening = new SimpleDoubleProperty(1.0);
    private final IntegerProperty emergencyStopDistance = new SimpleIntegerProperty(20);
    private final BooleanProperty emergencyStopEnabled = new SimpleBooleanProperty(false);
    
    // State
    private final ObjectProperty<RobotMovementState> movementState = new SimpleObjectProperty<>(RobotMovementState.STOPPED);
    private final ObjectProperty<RobotControlState> controlState = new SimpleObjectProperty<>(RobotControlState.MANUAL);
    
    // Sensors
    private final StringProperty ultrasonic = new SimpleStringProperty("WAITING");
    private final StringProperty leftIR = new SimpleStringProperty("WAITING");
    private final StringProperty rightIR = new SimpleStringProperty("WAITING");
    
    // Map
    private final DoubleProperty x = new SimpleDoubleProperty(0);
    private final DoubleProperty y = new SimpleDoubleProperty(0);
    private final DoubleProperty angle = new SimpleDoubleProperty(Math.toRadians(90)); 
    private final DoubleProperty angleMultiplier = new SimpleDoubleProperty(1.5); 
    private final ObservableList<Point2D> positionHistory = FXCollections.observableArrayList();

    public RobotModel() {}

    public IntegerProperty speedProperty() { return speed; }
    public DoubleProperty sensitivityProperty() { return sensitivity; }
    public DoubleProperty dampeningProperty() { return dampening; }
    public IntegerProperty emergencyStopDistanceProperty() { return emergencyStopDistance; }
    public BooleanProperty emergencyStopEnabledProperty() { return emergencyStopEnabled; }
    
    public ObjectProperty<RobotMovementState> movementStateProperty() { return movementState; }
    public ObjectProperty<RobotControlState> controlStateProperty() { return controlState; }

    public DoubleProperty xProperty() { return x; }
    public DoubleProperty yProperty() { return y; }
    public DoubleProperty angleProperty() { return angle; }
    public DoubleProperty angleMultiplierProperty() { return angleMultiplier; }
    public ObservableList<Point2D> getPositionHistory() { return positionHistory; }

    public StringProperty ultrasonicProperty() { return ultrasonic; }
    public StringProperty leftIRProperty() { return leftIR; }
    public StringProperty rightIRProperty() { return rightIR; }
    

    public int getSpeed() { return speed.get(); }
    public void setSpeed(int val) { this.speed.set(val); }

    public double getSensitivity() { return sensitivity.get(); }
    public void setSensitivity(double val) { this.sensitivity.set(val); }

    public double getDampening() { return dampening.get(); }
    public void setDampening(double val) { this.dampening.set(val); }

    public int getEmergencyStopDistance() { return emergencyStopDistance.get(); }
    public void setEmergencyStopDistance(int val) { this.emergencyStopDistance.set(val); }

    public boolean isEmergencyStopEnabled() { return emergencyStopEnabled.get(); }
    public void setEmergencyStopEnabled(boolean val) { this.emergencyStopEnabled.set(val); }
    
    public RobotMovementState getMovementState() { return movementState.get(); }
    public void setMovementState(RobotMovementState val) { this.movementState.set(val); }
    
    public RobotControlState getControlState() { return controlState.get(); }
    public void setControlState(RobotControlState val) { this.controlState.set(val); }
    
    public double getX() { return x.get(); }
    public void setX(double val) { this.x.set(val); }
    
    public double getY() { return y.get(); }
    public void setY(double val) { this.y.set(val); }
    
    public double getAngle() { return angle.get(); }
    public void setAngle(double val) { this.angle.set(val); }
    
    public double getAngleMultiplier() { return angleMultiplier.get(); }
    public void setAngleMultiplier(double val) { this.angleMultiplier.set(val); }

    public void clearHistory() {
        positionHistory.clear();
    }

    public void setSensorData(String dist, String leftIR, String rightIR) {
        this.ultrasonic.set(dist);
        this.leftIR.set(leftIR);
        this.rightIR.set(rightIR);
    }
    
}