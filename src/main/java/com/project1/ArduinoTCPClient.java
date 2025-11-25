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

    private Consumer<String> onDataReceived;

    public ArduinoTCPClient(Consumer<String> onDataReceived) {
        this.onDataReceived = onDataReceived;
    }

    public void connect() {
        // We run this in a new thread so we dont freeze our GUI while trying to connect
        new Thread(() -> {
            try {
                socket = new Socket(IP, PORT);
                connected = true;
                
                BufferedReader dataReceived = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String line;

                // Basically loops forever until connection breaks
                while (connected && (line = dataReceived.readLine()) != null) {
                    if (onDataReceived != null) {
                        onDataReceived.accept(line);
                    }
                }
            } catch (Exception e) {
                System.err.println("Failed to connect to TCP server: " + e.getMessage());
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
