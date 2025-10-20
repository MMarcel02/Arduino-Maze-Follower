// package com.project1;
// import java.net.URI;
// import java.net.http.HttpClient;
// import java.net.http.HttpRequest;
// import java.net.http.HttpResponse;
// import java.time.Duration;


// import javafx.application.Application;
// import javafx.fxml.FXMLLoader;
// import javafx.scene.Scene;
// import javafx.stage.Stage;

// public class ArduinoClient extends Application {

    
//     @Override
//     public void start(Stage primaryStage) throws Exception {
//         // Load the FXML file
//         FXMLLoader loader = new FXMLLoader(getClass().getResource("/Main.fxml"));
//         Scene scene = new Scene(loader.load());

//         // Set stage properties
//         primaryStage.setTitle("Robot Controller");
//         primaryStage.setScene(scene);
//         primaryStage.show();
//     }
//     public static void main(String[] args) throws Exception {
//         launch(args);
        
//         // String url = "http://192.168.1.1/forward";

//         // HttpClient client = HttpClient.newBuilder()
//         //         .connectTimeout(Duration.ofSeconds(5))
//         //         .build();

//         // HttpRequest req = HttpRequest.newBuilder()
//         //         .uri(URI.create(url))
//         //         .timeout(Duration.ofSeconds(10))
//         //         .GET()
//         //         .build();

//         // HttpResponse<String> res = client.send(req, HttpResponse.BodyHandlers.ofString());
//         // System.out.println("Body: " + res.body());
//     }
// }
