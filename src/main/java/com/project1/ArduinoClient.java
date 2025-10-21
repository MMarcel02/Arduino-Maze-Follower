package com.project1;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.Duration;

public class ArduinoClient {
  public static final String IP = "http://192.168.1.1";
  
  private HttpClient _client;
  
  public ArduinoClient() {
    _client = HttpClient.newBuilder()
      .connectTimeout(Duration.ofSeconds(5))
      .build();
  }

  ///
  ///   This function sends a GET request to the arduino server at the given endpoint.
  ///   
  ///   e.g. Send("/forward") -> Sends the request, should move the robot forward.
  ///   
  ///   -----------------------------------------------------------------------------
  /// 
  ///   Use the ArduinoEndpoints class for the endpoints, don't hardcode them.
  /// 
  ///   e.g. Send(ArduinoEndpoints.FORWARD)
  /// 
  public HttpResponse<String> Send(String endpoint) throws Exception {
    String url = IP + endpoint;
    
    HttpRequest req = HttpRequest.newBuilder()
      .uri(URI.create(url))
      .timeout(Duration.ofSeconds(10))
      .GET()
      .build();

    // TODO: Asynchronous queue for calls.
      
    HttpResponse<String> res = _client.send(req, HttpResponse.BodyHandlers.ofString());
    System.out.println("Body: " + res.body());
    
    return res;
  }
}
