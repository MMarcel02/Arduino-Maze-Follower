package com.project1;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.function.Consumer;

public class ArduinoTCPClient {
    private static final String IP = "192.168.1.1";
    // We send tcp packets on 8080 and http on just 80
    private static final int PORT = 8080;
    
    private Socket socket;
    private boolean connected = false;

    // Lambda function, currently defined in GUIController initialize
    // Consumer accepts an input and runs something without producing output
    // This way we are decoupling our code in here from GUIController, so we can reuse this class
    // If we need another TCPClient with a different Consumer method
    private Consumer<String> onDataReceived;

    // We pass that function in and set it to Consumer
    public ArduinoTCPClient(Consumer<String> onDataReceived) {
        this.onDataReceived = onDataReceived;
    }

    public void connect() {
        // We run this in a new thread so we dont freeze our GUI while trying to connect
        new Thread(() -> {
            // Since in initalize we only run connect once, we need to keep our connection alive if we ever disconnect
            // Thats why everything is wrapped in a while(true), so it keeps retrying the connection
            while(true) {
                try {
                    
                    System.out.println("Attempting TCP connection...");
                    socket = new Socket(IP, PORT); // Socket is the endpoint on the Arduino we listen to
                    
                    // Only gets here if the new Socket(IP, PORT) didn't throw an Exception that would put is into the catch block
                    connected = true;
                    System.out.println("TCP Connected!");

                    // .getInputStream() just gets 1010101... from our Arduino
                    // InputStreamReader translates the 1010101 into chars like 'H' 'E' ..
                    // BufferedReader accumulates the chars into a String
                    BufferedReader dataReceived = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    String line;
    
                    // Basically loops forever until connection breaks
                    // readLine() method just gives you the String inside a BufferedReader until it sees a \n
                    // We do the null check to see if the connection has dropped
                    while (connected && (line = dataReceived.readLine()) != null) {
                        if (onDataReceived != null) {
                            onDataReceived.accept(line);
                        }
                    }

                    // Only get here if the while loop fails -> connection dropped
                    System.err.println("TCP server disconnected");
                } catch (Exception e) {
                    System.err.println("Failed to connect to TCP server: " + e.getMessage());
                } finally {
                    // Only get here if we fail to connect, so we reset our connection and clean up
                    disconnect();
                }

                // Only get here if fail to connect, so we wait a second before restarting our while loop that tries to connect
                // Usually it fails when we start our app too fast after starting the robot
                // So the robot hasnt had time to set up the TCP server yet
                try {
                    System.out.println("Retrying..");
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }).start();
    }

    public void disconnect() {
        connected = false;
        try {
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        } catch (Exception e) {
            System.err.println("Failed to close: " + e.getMessage());
        }
    }

}
