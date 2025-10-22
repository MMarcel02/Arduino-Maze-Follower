package com.project1;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.Duration;

public class ArduinoClient {
  public static final String IP = "http://192.168.1.1";
  private HttpClient _client;
  
  // This is our constructor for the ArduinoClient method. That means when someone makes a new ArduinoClient object,
  // these will be the default 
  public ArduinoClient() {

    // we will start creating our HTTP request one part at a time (thats why we use the builder)
    _client = HttpClient.newBuilder()

      // for now we just make the HTTP request have a default timeout of 5 seconds, the rest we keep as default from the builder
      .connectTimeout(Duration.ofSeconds(5))

      // creates a brand new HttpClient object with that 5 second default 
      .build();
  }

  ///   This function sends a GET request to the arduino server at the given endpoint.
  ///   
  ///   e.g. Send("/forward") -> Sends the request, should move the robot forward.

  public HttpResponse<String> send(String endpoint) throws Exception {
    String url = IP + endpoint;
    HttpRequest req = HttpRequest.newBuilder()
    .uri(URI.create(url))
    .timeout(Duration.ofSeconds(10))
    .GET()
    .build();
    
    // TODO: Asynchronous queue for calls.
    // System.out.println(req.toString());
      
    HttpResponse<String> res = _client.send(req, HttpResponse.BodyHandlers.ofString());
    System.out.println("Body: " + res.body());
    
    return res;
  }
}
