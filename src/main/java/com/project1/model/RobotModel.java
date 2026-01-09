package com.project1.model;

import javafx.beans.property.*;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Point2D;

// In MVC the model stores all the data in one place, is is updated by the Controller and it tells the View what to display
public class RobotModel {
    public RobotModel() {};

    // We use these JavaFX property objects so we can bind our GUI to these values
    private final IntegerProperty speed = new SimpleIntegerProperty(80);
    private final DoubleProperty sensitivity = new SimpleDoubleProperty(5.0);
    private final DoubleProperty dampening = new SimpleDoubleProperty(1.0);
    private final IntegerProperty emergencyStopDistance = new SimpleIntegerProperty(20);
    private final BooleanProperty emergencyStopEnabled = new SimpleBooleanProperty(false);
    
    // State
    private final ObjectProperty<RobotMovementState> movementState = new SimpleObjectProperty<>(RobotMovementState.LEFT);
    private final ObjectProperty<RobotControlState> controlState = new SimpleObjectProperty<>(RobotControlState.MANUAL);
    
    // Sensors
    private final StringProperty ultrasonic = new SimpleStringProperty("WAITING");
    private final StringProperty irLeftDigital = new SimpleStringProperty("WAITING");
    private final StringProperty irRightDigital = new SimpleStringProperty("WAITING");

    private final IntegerProperty irAnalogLeftRaw = new SimpleIntegerProperty(0);
    private final IntegerProperty irAnalogRightRaw = new SimpleIntegerProperty(0);

    private final IntegerProperty leftIRThreshold = new SimpleIntegerProperty(37);
    private final IntegerProperty rightIRThreshold = new SimpleIntegerProperty(37);

    // Map
    private final DoubleProperty x = new SimpleDoubleProperty(0);
    private final DoubleProperty y = new SimpleDoubleProperty(0);
    private final DoubleProperty angle = new SimpleDoubleProperty(Math.toRadians(90)); 
    private final DoubleProperty angleMultiplier = new SimpleDoubleProperty(1.5); 
    private final ObservableList<Point2D> positionHistory = FXCollections.observableArrayList();

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
    public StringProperty leftIRDigitalProperty() { return irLeftDigital; }
    public StringProperty rightIRDigitalProperty() { return irRightDigital; }

    public IntegerProperty leftIRAnalogRawProperty() { return irAnalogLeftRaw; }
    public IntegerProperty rightIRAnalogRawProperty() { return irAnalogRightRaw; }

    public IntegerProperty leftIRThresholdProperty() { return leftIRThreshold; }
    public IntegerProperty rightIRThresholdProperty() { return rightIRThreshold; }

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

    public int getLeftIRThreshold() { return leftIRThreshold.get(); }
    public void setLeftIRThreshold(int val) { this.leftIRThreshold.set(val); }

    public int getRightIRThreshold() { return rightIRThreshold.get(); }
    public void setRightIRThreshold(int val) { this.rightIRThreshold.set(val); }
    
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

    public void setSensorData(String dist, String irLeftDigital, String irRightDigital, int irLeftRaw, int irRightRaw) {
        this.ultrasonic.set(dist);
        this.irLeftDigital.set(irLeftDigital);
        this.irRightDigital.set(irRightDigital);
        this.irAnalogLeftRaw.set(irLeftRaw);
        this.irAnalogRightRaw.set(irRightRaw);
    }
    
}