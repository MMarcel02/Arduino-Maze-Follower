const int trigPin = 0;
const int echoPin = 1; 

// IR sensor declarations

// Define the processors pins connected to the IR sensors' Analog output
// Assuming the table refers to Microcontroller Analog pins A1 and A3.
const int LEFT_SENSOR_PIN = A1; 
const int RIGHT_SENSOR_PIN = A3; 

// Define a threshold value to distinguish between light (white) and dark (black) surfaces.
// This value MUST be calibrated for your specific sensors and environment.
// Lower value = Black (absorbs light), Higher value = White (reflects light).
// A typical threshold is around 400-600, but 500 is used as a starting guess.
const int BLACK_TAPE_THRESHOLD = 500; 

// End of IR sensor declarations

float duration, distance;  

void setup() {  
	pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
	Serial.begin(9600);  

	//ir sensor setup

	// Initialize serial communication to print sensor values
  Serial.begin(9600);
  Serial.println("IR Sensor Line Detection Initialized.");
}  


void loop() {  
	digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  

  duration = pulseIn(echoPin, HIGH);  

  distance = (duration*.0343)/2; 

  if (distance<=350) // Afer a distance of ~350-370 units it fails to recieve an echo
  {
  Serial.print("Distance: ");  
	Serial.println(distance);  
	delay(100); //delay between readings

	// IR Sensors

	  // 1. Read the raw analog values from both sensors (0 to 1023)
  int leftReading = analogRead(LEFT_SENSOR_PIN);
  int rightReading = analogRead(RIGHT_SENSOR_PIN);

  // 2. Determine the status (Below threshold means BLACK tape detected)
  // A reading *below* the threshold means less light is being reflected (black surface).
  bool leftIsOnBlack = (leftReading < BLACK_TAPE_THRESHOLD);
  bool rightIsOnBlack = (rightReading < BLACK_TAPE_THRESHOLD);

  // 3. Print the raw values and interpreted status

  Serial.print("Left Raw: ");
  Serial.print(leftReading);
  Serial.print(" | Status: ");
  // Print "BLACK" if the reading is below the threshold, otherwise "WHITE"
  Serial.print(leftIsOnBlack ? "BLACK" : "WHITE");
  
  Serial.print(" || Right Raw: ");
  Serial.print(rightReading);
  Serial.print(" | Status: ");
  Serial.print(rightIsOnBlack ? "BLACK" : "WHITE");
  
  // Print the overall line following condition
  if (leftIsOnBlack && rightIsOnBlack) {
    Serial.println(" | Action: Stop/Centered on thick line");
  } else if (leftIsOnBlack) {
    Serial.println(" | Action: Line on left (Turn Left)");
  } else if (rightIsOnBlack) {
    Serial.println(" | Action: Line on right (Turn Right)");
  } else {
    Serial.println(" | Action: Off line (Go Straight)");
  }
  
  delay(100); //delay between readings
  }
}  

