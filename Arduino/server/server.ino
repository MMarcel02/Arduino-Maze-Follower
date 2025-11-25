#include <WiFi101.h>

// Arduino ide automatically compiles all files in the same folder, so functions in
// movement.ino will be accessible

// Feather M0 WiFi (WINC1500) pins
const int WINC_CS  = 8, WINC_IRQ = 7, WINC_RST = 4, WINC_EN = 2;

const char ssid[] = "Team 36";
const char pass[] = "Team36Rules";     // >= 8 chars for WPA2

// Use this one for commands (e.g. /forward)
WiFiServer httpServer(80);
// Use this for data (e.g. Ultrasonic)
WiFiServer tcpServer(8080);

WiFiClient streamingClient;

// Timer variable so we can check if 100 ms has elapsed yet with SENSOR_INTERVAL
unsigned long lastSensorTime = 0;
// Send sensor data every 100 ms
const int SENSOR_INTERVAL = 100;

bool emergencyStop = true;
int emergencyStopDistance = 20;
bool followingLine = false;

double sensitivity = 5.0;
double dampening = 1.0;

// enum assigns numbers to these words
// less mistakes than using strings (typos) and easier and faster to compare numbers
enum RobotState {
  STOPPED, // Compiler assigns this = 0
  FORWARD, // Compiler assigns this = 1
  LEFT,
  RIGHT,
  OTHER
};

RobotState currentState = STOPPED;

// Helper to get string instead of 0 or 1
String boolToString(bool value) {
  if (value) {
    return "true";
  } else {
    return "false";
  }
}

// --------- Utility: IPAddress -> "A.B.C.D" -----
String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

void setup() {
  Serial.begin(115200);
  // DO NOT block on while(!Serial); we want it to run even without a PC attached
  setupUltraSonicSensor();
  setupIRSensors();

  Serial.println("IR Sensor Line Detection Initialized.");


  WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WINC1500 not detected"); while (1) {}
  }

  Serial.print("FW: "); Serial.println(WiFi.firmwareVersion());

  Serial.println("Starting AP…");
  int s = WiFi.beginAP(ssid, pass, 6);            // WPA2, ch 6
  if (s != WL_AP_LISTENING) {
    Serial.print("WPA2 AP failed ("); Serial.print(s); Serial.println("). Trying OPEN…");
    s = WiFi.beginAP(ssid, 6);                    // OPEN AP fallback
    if (s != WL_AP_LISTENING) { Serial.println("AP failed"); while (1) {} }
  }

  delay(8000); // let AP + DHCP come up

  Serial.print("AP IP: "); Serial.println(ipToString(WiFi.localIP())); // usually 192.168.1.1
  httpServer.begin();
  tcpServer.begin();


  // Initialize all four motors
  setupAllMotors();

  // Stop all motors initially
  stopAllMotors();
}

void serve(WiFiClient& c){
  c.setTimeout(1500);
  String rl=c.readStringUntil('\n');        // "GET /path?query HTTP/1.1"
  int sp1=rl.indexOf(' '), sp2=rl.indexOf(' ',sp1+1);
  String uri=(sp1>0&&sp2>sp1)?rl.substring(sp1+1,sp2):"/";
  int q=uri.indexOf('?'); String pth=(q>=0)?uri.substring(0,q):uri; String qry=(q>=0)?uri.substring(q+1):"";
  while(true){ String h=c.readStringUntil('\n'); if(h.length()==0||h=="\r") break; } // headers
  route(c,pth,qry);
}
void route(WiFiClient& c, const String& path, const String& q) {
    if (path == "/" || path == "") { handleRoot(c); return; }
    if (path == "/forward") { handleForward(c); return; }
    if (path == "/backward") { handleBackward(c); return; }
    if (path == "/turnOnSpotRight") { handleTurnOnSpotRight(c); return; }
    if (path == "/turnOnSpotLeft") { handleTurnOnSpotLeft(c); return; }
    if (path == "/left") { handleLeft(c); return; }
    if (path == "/right") { handleRight(c); return; }
    if (path == "/stop") { handleStop(c); return; }
    if (path == "/crabWalkLeft") { handleCrabWalkLeft(c); return; }
    if (path == "/crabWalkRight") { handleCrabWalkRight(c); return; }
    if (path == "/toggleEmergencyStop") { handleToggleEmergencyStop(c); return; }
    if (path == "/toggleLineFollowing") { handleToggleLineFollowing(c); return; }

    // when we change speed we pass down /setSpeed?s=(some value 0-255)
    if (path.startsWith("/setSpeed")) {
      int speedValue = parseIntEndpoint(q);
      handleSetSpeed(c, speedValue);
      return;
    }

    if (path.startsWith("/setEmergencyStopDistance")) {
      int distance = parseIntEndpoint(q);
      handleSetEmergencyStopDistance(c, distance);
      return;
    }

    if (path.startsWith("/setSensitivity")) {
      double sensitivity = parseDoubleEndpoint(q);
      handleSetSensitivity(c, sensitivity);
      return;
    }

    if (path.startsWith("/setDampening")) {
      double dampening = parseDoubleEndpoint(q);
      handleSetDampening(c, dampening);
      return;
    }
}

// Helper to get string after s= 
int parseIntEndpoint(String query) {
  int sIndex = query.indexOf("s=");
  return query.substring(sIndex + 2).toInt();
}

double parseDoubleEndpoint(String query) {
  double sIndex = query.indexOf("s=");
  return query.substring(sIndex + 2).toDouble();
}

// Helper to send minimal HTTP response
void sendHttpResponse(WiFiClient& client, const String& body) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Length: "); client.print(body.length()); client.print("\r\n\r\n");
    client.print(body);
    delay(1);
}

void handleRoot(WiFiClient& client) {
    sendHttpResponse(client, "Initial Page");
}

void handleForward(WiFiClient& client) {
    currentState = FORWARD;
    moveForward();
    sendHttpResponse(client, "Moved Forward");
}

void handleBackward(WiFiClient& client) {
    currentState = OTHER;
    moveBackward();
    sendHttpResponse(client, "Moved Backward");
}

void handleTurnOnSpotRight(WiFiClient& client) {
    currentState = OTHER;
    turnOnSpotRight();
    sendHttpResponse(client, "Turned Right on Spot");
}

void handleTurnOnSpotLeft(WiFiClient& client) {
    currentState = OTHER;
    turnOnSpotLeft();
    sendHttpResponse(client, "Turned Left on Spot");
}

void handleLeft(WiFiClient& client) {
    currentState = LEFT;
    moveLeft();
    sendHttpResponse(client, "Moved Left");
}

void handleRight(WiFiClient& client) {
    currentState = RIGHT;
    moveRight();
    sendHttpResponse(client, "Moved Right");
}

void handleStop(WiFiClient& client) {
    currentState = STOPPED;
    stopAllMotors();
    sendHttpResponse(client, "Stopped");
}

void handleCrabWalkLeft(WiFiClient& client) {
    currentState = OTHER;
    crabWalkLeft();
    sendHttpResponse(client, "Crab Walk Left");
}

void handleCrabWalkRight(WiFiClient& client) {
    currentState = OTHER;
    crabWalkRight();
    sendHttpResponse(client, "Crab Walk Right");
}

void handleSetSpeed(WiFiClient& client, int speed) {
    setSpeed(speed);
    sendHttpResponse(client, "Speed set to " + String(speed));
}

void handleSetEmergencyStopDistance(WiFiClient& client, int distance) {
  emergencyStopDistance = distance;
  sendHttpResponse(client, "Emergency Stop distance set to " + String(emergencyStopDistance));
}

void handleSetSensitivity (WiFiClient& client, double value) {
  sensitivity = value;
  sendHttpResponse(client, "Sensitivity set to " + String(sensitivity, 1));
}

void handleSetDampening(WiFiClient& client, double value) {
  dampening = value;
  sendHttpResponse(client, "Dampening set to " + String(dampening, 1));
}

void handleToggleEmergencyStop(WiFiClient& client) {
    emergencyStop = !emergencyStop;
    sendHttpResponse(client, ("Emergency stop set to " + boolToString(emergencyStop))); 
}   

void handleToggleLineFollowing(WiFiClient& client) {
    followingLine = !followingLine;
    if (!followingLine) {
      currentState = STOPPED;
      stopAllMotors();
      sendHttpResponse(client, ("Line following set to " + boolToString(followingLine))); 
    }
    sendHttpResponse(client, ("Line following set to " + boolToString(followingLine))); 
}   

void handleSensors(WiFiClient& client) {
  String body = buildSensorMessage();
  sendHttpResponse(client, body);
}


void loop() {
  // String test = buildSensorMessage();
  // We check for a http connection (one everytime we send a command e.g. /forward)
  WiFiClient httpClient = httpServer.available();
  if (httpClient) {
      httpClient.setTimeout(50); 
      serve(httpClient);
      httpClient.stop();
  }

  if (streamingClient && !streamingClient.connected()) {
      Serial.println("Client disconnected, freeing socket...");
      streamingClient.stop();
  }

  // Here we check if we have a new tcpServer to connect
  // If we do we overwrite the current streamingClient
  // This is so we can reconnect if we restart our program while connected, or the connection drops
  WiFiClient tcpClient = tcpServer.available();
  if (tcpClient) {
    if (streamingClient && streamingClient.connected()) {
      streamingClient.stop();
    }
    streamingClient = tcpClient;
    streamingClient.flush();
  }

  if (streamingClient && streamingClient.connected()) {
    // 32 bit, so would take about 50 days to run out of space
    unsigned long currentMillis = millis();
    // We check if 100ms has elapsed yet, if it has we send a packet with data
    if (currentMillis - lastSensorTime >= SENSOR_INTERVAL) {
      lastSensorTime = currentMillis;
      String sensorData = buildSensorMessage();
      streamingClient.println(sensorData);
    }
  }

  if (emergencyStop && (currentState == FORWARD || currentState == LEFT || currentState == RIGHT)) { 
    checkEmergencyStop();
  } else if (followingLine) {
    pdLineFollow();
  }
}
