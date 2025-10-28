
// Arduino ide automatically compiles all files in the same folder, so functions in
// movement.ino will be accessible

// Feather M0 WiFi (WINC1500) pins
const int WINC_CS  = 8, WINC_IRQ = 7, WINC_RST = 4, WINC_EN = 2;

const char ssid[] = "FeatherAP";
const char pass[] = "test1234";     // >= 8 chars for WPA2
WiFiServer server(80);

// --------- Utility: IPAddress -> "A.B.C.D" -----
String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

void setup() {
  Serial.begin(115200);
  // DO NOT block on while(!Serial); we want it to run even without a PC attached

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
  server.begin();

    // Initialize all four motors
  setupAllMotors();
  //Add your code to control the other motors.

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

    // when we change speed we pass down /setSpeed?s=(some value 0-255)
    if (path.startsWith("/setSpeed")) {
        //we want to extract the number after s= since thats our speed value
        int sIndex = q.indexOf("s=");
        if (sIndex >= 0) {
            //here we extract it and change it from a String to and int
            int speedValue = q.substring(sIndex + 2).toInt();
            handleSetSpeed(c, speedValue);
        }
        return;
    }
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

void handleToggleEmergencyStop(WiFiClient& client) {
    // here call your function that changes the emergency stop
    // e.g. setEmergencyStop(!emergencyStop)

    // here make sure it tells us if it sets the emergency stop to true or false
    sendHttpResponse(client, "Emergency stop set to " ); 
}   


void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  client.setTimeout(2000); // 2s read timeout
  serve(client);
  client.stop();
}
