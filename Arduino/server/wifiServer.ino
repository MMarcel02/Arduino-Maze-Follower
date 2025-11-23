/* Pins that describe the WIFI module. Pins used from the Arduino Wi-Fi library 
*/
#include <Arduino.h>
#include <WiFi101.h>

const int WINC_CS  = 8, WINC_IRQ = 7, WINC_RST = 4, WINC_EN = 2;

WiFiServer server(80);
// Constant variables for the name and the pass of our Wifi access point
const char ssid[] = "FeatherAP";
const char pass[] = "test1234";

// Function to setup which pins the WIFI101 library will use
void setupWifiPins() {
    WiFi.setPins(WINC_CS,WINC_IRQ,WINC_RST,WINC_EN);
}

/* Helper function that converts IP to readable format, it takes as argument an object of IPAddress type that is provided in wifilib
The IPAddress object contains the IP like an array so we use ip[i] to access all four numbers and then return it as one string*/
String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

/*Function to check if the library can find the Wifi Module
if it cant then it ends an infite loop to freeze the program*/
void wifiSafetyCheck() {
    if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WINC1500 not detected"); while (1) {} // if no module is found freeze
  }
}

/* Function to start the wifi ap */
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
    // This tells the wifiserver object to begin listening it is the object we declared using WiFiServer server(80)
    server.begin();
    Serial.print("Server succesfully setup!");
    
}

/*GET /forward?10 HTTP/1.1
Host: 192.168.1.1
... (rest of headers)*/

// Function that gets the texts sent to the robot by our browser
// serve function accepts as arg a WiFiClient object from the wifilib it represent a specific
// device connected to our server and get the path and data
void serve(WiFiClient& c) {
  c.setTimeout(1500); // method of the WiFiClient object makes sure robot doesn't freeze if no data is sent for more than 1.5 second
  String rl = c.readStringUntil('\n'); // stop reading after new line character to keep only the get command, not the metadata
  // Robot only cares about the /command part of our httlp get request
  int sp1 = rl.indexOf(' '); //get the index of the first " " you encounter in the string of request
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

  void route(WiFiClient& c,const String& path, const String& q){
    //gets the WiFiClient object and the path + query
    // it needs the WiFi Client so that it knows on which device it is connected
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

      // Helper to send minimal HTTPS
  void sendHttpResponse(WiFiClient& client, const String& body) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Length: "); client.print(body.length()); client.print("\r\n\r\n");
    client.print(body);
    delay(1);
  }

  // Handlers

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
    // create a boolean flag for object detection on
    // here make sure it tells us if it sets the emergency stop to true or false
    sendHttpResponse(client, "Emergency stop set to " ); 
} 

void acceptClients(){
  WiFiClient client = server.available(); // returns a WiFiClient object if someone is accesing our server
  if (!client) return; // if no one is accessing exit function
  client.setTimeout(2000);
  serve(client);
  client.stop();
  // if (objectdetection true) 
  //float dist = getDistanceCM();
 // if (dist<20){stopAllMotors();}
}

void EmergencyStop(){
  int dist =0;
  // use a function to set dist as the ultrasound distance

  if (dist <20) {stopAllMotors;}
}






