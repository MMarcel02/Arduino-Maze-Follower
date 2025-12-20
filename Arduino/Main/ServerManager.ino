
WiFiClient streamingClient;

// Use this one for commands (e.g. /forward)
WiFiServer httpServer(80);
// Use this for data (e.g. Ultrasonic)
WiFiServer tcpServer(8080);

// Interval (in ms) which determines how often we send sensor data
const int SENSOR_SEND_INTERVAL = 50;
// Timer variable which we cross check with SENSOR_SEND_INTERVAL
unsigned long lastSensorSendTime = 0;

void setupWifiPins() {
  WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);
}

// Check to see if theres a hardware issue with the wifi chip
void wifiSafetyCheck() {
  // WL_NO_SHIELD is a specific error meaning that it cant find the chip
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");

    // Creates infinte loop that stops the robot from doing anything else
    while (true); 
  }
}

// Helper to get string instead of 0 or 1
String boolToString(bool value) {
  if (value) {
    return "true";
  } else {
    return "false";
  }
}

/* Helper function that converts IP to readable format, it takes as argument an object of IPAddress type that is provided in wifilib
The IPAddress object contains the IP like an array so we use ip[i] to access all four numbers and then return it as one string*/
String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

/* Function to start the wifi ap */
// Serial used for debugging over USB
void startWifiAp(){
    Serial.print("The Wifi Module's firmware is: "); 
    Serial.println(WiFi.firmwareVersion());
    Serial.println("Starting AP…");
    // Initialize the AP using Wifi.beginAP, it returns ints but they have been mapped to strings
    int s = WiFi.beginAP(ssid, pass, 6);  // Channel 6
    if (s != WL_AP_LISTENING) {
    Serial.print("WPA2 AP failed ("); 
    Serial.print(s); Serial.println("). Trying OPEN…");
    s = WiFi.beginAP(ssid, 6); // OPEN AP fallback
    if (s != WL_AP_LISTENING) { Serial.println("AP failed"); while (1) {} } // freeze if fail
    }
    delay(8000); // let AP + DHCP come up
    Serial.print("AP IP: "); Serial.println(ipToString(WiFi.localIP())); // usually 192.168.1.1
    // This tells the wifiserver objects to begin listening, http on port 80 and tcp on port 8080
    httpServer.begin();
    tcpServer.begin();
    Serial.print("Server succesfully setup!");
}

// Function that gets the http requests sent to the robot
// serve function accepts as arg a WiFiClient object from the wifilib it represent a specific device connected to our server 
void serve(WiFiClient& c) {
  String rl = c.readStringUntil('\n'); // stop reading after new line character to keep only the get command, not the metadata
  // Robot only cares about the /command part of our http get request
  int sp1 = rl.indexOf(' '); // get the index of the first " " you encounter in the string of request
  int sp2 = rl.indexOf(' ',sp1+1); // get the index of " " after the first one
  // Why do we do this ?
  // The HTTP request is GET /forward HTTP/1.1 we want to keep only the /forward of this string
  // The following is using ternary operator a way of expressing an if else statement in c
  String uri = (sp1>0 && sp2>sp1)?rl.substring(sp1+1,sp2):"/"; //complex c statement if then else return "/"
  // Finding the question mark
  int q = uri.indexOf('?');
  // Separating the command from the data
  String pth = (q>=0)? uri.substring(0,q):uri; // get just the path or if only paths exists index would be -1 so again only the path
  // Getting the query data
  String qry = (q>=0)?uri.substring(q+1):"";
  while(true) {
    String h=c.readStringUntil('\n');
    if (h.length() == 0 || h == "\r") break;}
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
    moveForward();
    sendHttpResponse(client, "Moved Forward");
}

void handleBackward(WiFiClient& client) {
    moveBackward();
    sendHttpResponse(client, "Moved Backward");
}

void handleTurnOnSpotRight(WiFiClient& client) {
    turnOnSpotRight();
    sendHttpResponse(client, "Turned Right on Spot");
}

void handleTurnOnSpotLeft(WiFiClient& client) {
    turnOnSpotLeft();
    sendHttpResponse(client, "Turned Left on Spot");
}

void handleLeft(WiFiClient& client) {
    moveLeft();
    sendHttpResponse(client, "Moved Left");
}

void handleRight(WiFiClient& client) {
    moveRight();
    sendHttpResponse(client, "Moved Right");
}

void handleStop(WiFiClient& client) {
    stopAllMotors();
    sendHttpResponse(client, "Stopped");
}

void handleCrabWalkLeft(WiFiClient& client) {
    crabWalkLeft();
    sendHttpResponse(client, "Crab Walk Left");
}

void handleCrabWalkRight(WiFiClient& client) {
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
      stopAllMotors();
      sendHttpResponse(client, ("Line following set to " + boolToString(followingLine))); 
    }
    sendHttpResponse(client, ("Line following set to " + boolToString(followingLine))); 
}   

// We check for a http connection (one everytime we send a command e.g. /forward)
void handleHTTPCommands() {
  WiFiClient httpClient = httpServer.available();  
  if (httpClient) {
    httpClient.setTimeout(100); 
    serve(httpClient);
    httpClient.stop();
  }
}

void handleTCPData() {
  // If we're not connected to the old streamingClient we make sure we disconnect
  if (streamingClient && !streamingClient.connected()) {
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
    if (currentMillis - lastSensorSendTime >= SENSOR_SEND_INTERVAL) {

  
      // Creates String with data separated by commas
      String tcpPacket = buildSensorMessage() + "," + String(currentState);
      
      // Sends the data all at once as a tcp packet
      streamingClient.println(tcpPacket);
      lastSensorSendTime = currentMillis;
    }
  }
}

void manageRobotMovementState() {
  // Should only stop IF we're trying to move generally forward, otherwise it will block when we try to reverse or rotate
  if (emergencyStop) { 
    checkEmergencyStop();
  } 
  
  if (followingLine) {
    bangLineFollow();
  }
}
