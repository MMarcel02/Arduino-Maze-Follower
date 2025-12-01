package com.project1;

// This is the alternative GUI we created using Swing Library, it can be ran standalone from this file 

import java.io.*;
import javax.swing.*;

import java.awt.event.KeyEvent;
import java.awt.event.ActionEvent;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class SwingGui {

    private static String lastAction= "";
    public static ArduinoClient arduClient = new ArduinoClient();


    public static void main(String[] args){
        createGUI();
    }
    
    // Helper method that gets called every time on button listeners to send the HTTPS request
    public static void sendCommand(JButton button, String endpoint) {
    button.doClick();
    lastAction=endpoint;
    System.out.println(lastAction);
    try {
        HttpResponse<String> httpResponse = arduClient.send(endpoint);
        String responseBody = httpResponse.body();
        System.out.println(endpoint + " Succeed");
        System.out.println(responseBody);
    } catch (Exception e) {System.out.println("HTTP, Request failed try again");}
    }


    public static JFrame createGUI(){
    // Create an instance of JFrame
    JFrame frame = new JFrame("GUI Example");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frame.setSize(1000,1000);
    frame.setLocationRelativeTo(null);

    //Label for the top
    JLabel movementLabel = new JLabel("Movement Buttons",JLabel.CENTER);
    movementLabel.setFont(new Font ("Arial",Font.BOLD,16));
    
    // Instructions
    String[] columnNames = {"Key / Button", "Action"};
    Object[][] data = {
    {"↑ Arrow", "Forward"},
    {"↓ Arrow", "Backward"},
    {"← Arrow", "Turn Left"},
    {"→ Arrow", "Turn Right"},
    {"A", "Drift Left"},
    {"D", "Drift Right"},
    {"C", "Crab Walk Left"},
    {"V", "Crab Walk Right"},
    {"ESC", "KillSwitch"}
    };

    JTable keyTable = new JTable(data, columnNames);
    keyTable.setEnabled(false); // prevent editing
    keyTable.setFont(new Font("Arial", Font.PLAIN, 14));
    keyTable.setRowHeight(25);


    
    // Create a panel of movement buttons
    JButton buttonUp = new JButton("Forward");
    JButton buttonDown = new JButton("Backward");
    JButton buttonLeft = new JButton("Left");
    JButton buttonRight = new JButton("Right");
    JButton crabWalkLeft= new JButton("CrabWalkLeft");
    JButton crabWalkRight= new JButton("CrabWalkRight");
    JButton buttonSpotLeft = new JButton("SpotLeft");
    JButton buttonSpotRight = new JButton("SpotRight");
    JPanel buttonPanel = new JPanel();
    buttonPanel.setLayout(new GridLayout(2,4,5,5));
    buttonPanel.add(buttonSpotLeft);
    buttonPanel.add(buttonUp);
    buttonPanel.add(buttonSpotRight);
    buttonPanel.add(buttonLeft);
    buttonPanel.add(buttonDown);
    buttonPanel.add(buttonRight);
    

    //crabwalking panel
    JLabel crabWalkLabel = new JLabel("Special Movements !",JLabel.CENTER);
    crabWalkLabel.setFont(new Font ("Arial",Font.BOLD,16));
    JPanel crabWalkPanel = new JPanel();
    crabWalkPanel.setLayout(new GridLayout(1,2,5,5));
    crabWalkPanel.add(crabWalkLeft);
    crabWalkPanel.add(crabWalkRight);
  
    // Killswith button

    JButton killswitch = new JButton("KillSwitch");
    killswitch.setPreferredSize(new Dimension(killswitch.getPreferredSize().width, 40));
    
    //Label for speed panel
    JLabel speedLabel = new JLabel("SpeedButtons",JLabel.CENTER);
    speedLabel.setFont(new Font ("Arial",Font.BOLD,16));
    
    //Panel for speed buttons
    JButton moreSpeed = new JButton("+");
    JButton moreSpeedExtra = new JButton("++");
    JButton lessSpeed = new JButton("-");
    JButton lessSpeedExtra = new JButton("--");
    JPanel speedPanel = new JPanel();
    speedPanel.setLayout(new GridLayout(1,4,5,5));
    speedPanel.add(lessSpeed);
    speedPanel.add(lessSpeedExtra);
    speedPanel.add(moreSpeed);
    speedPanel.add(moreSpeedExtra);
    
    //Add a slider
    JSlider slider = new JSlider(0, 255, 80); // min, max, initial value

    
    //Label to display the slider value
    JLabel sliderValueLabel = new JLabel("80", JLabel.CENTER);
    sliderValueLabel.setFont(new Font("Arial", Font.BOLD, 16));
    
    slider.addChangeListener(e -> {
    int value = slider.getValue();
    sliderValueLabel.setText(String.valueOf(value));
     if (!slider.getValueIsAdjusting()) {
        try {
            String endpoint = ArduinoEndpoints.getSpeedEndpoint(value);
            arduClient.send(endpoint);
            System.out.println("Speed set to " + value + " via " + endpoint);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    });
    
    // Add ActionListeners to speed buttons that will change the value of the slider
    moreSpeed.addActionListener(e -> {
    int newValue = Math.min(slider.getValue() + 1, slider.getMaximum());
    slider.setValue(newValue);
    });
    
    moreSpeedExtra.addActionListener(e -> {
    int newValue = Math.min(slider.getValue() + 10, slider.getMaximum());
    slider.setValue(newValue);
    });
    
    lessSpeed.addActionListener(e -> {
    int newValue = Math.max(slider.getValue() - 1, slider.getMinimum());
    slider.setValue(newValue);
    });
    
    lessSpeedExtra.addActionListener(e -> {
    int newValue = Math.max(slider.getValue() - 10, slider.getMinimum());
    slider.setValue(newValue);
    });
    
    
    // Add the panel and the label to the main frame
    frame.setLayout(new GridBagLayout());
    GridBagConstraints gbc = new GridBagConstraints();
    
    // Add the label at the top
    gbc.gridx = 0;
    gbc.gridy = 0;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor= GridBagConstraints.NORTH;
    gbc.weightx = 1.0;
    gbc.weighty = 0.0;
    frame.add(movementLabel, gbc);
    
    // Add the button panel below the label
    gbc.gridx = 0;
    gbc.gridy = 1;
    gbc.fill = GridBagConstraints.NONE;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(buttonPanel, gbc);
    
    // Add the crabwalkPanel
    gbc.gridx = 0;
    gbc.gridy = 2;
    gbc.fill = GridBagConstraints.NONE;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(crabWalkPanel, gbc);

    // Add the killswitch below the button panel
    gbc.gridx=0;
    gbc.gridy=3;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty=0.0;
    frame.add(killswitch,gbc);
    
    // Speed label
    gbc.gridx = 0;
    gbc.gridy = 4;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor= GridBagConstraints.NORTH;
    gbc.weightx = 1.0;
    gbc.weighty = 0.0;
    frame.add(speedLabel, gbc);
    // Adding speed panel 
    gbc.gridx = 0;
    gbc.gridy = 5;
    gbc.fill = GridBagConstraints.NONE;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(speedPanel, gbc);

        // Filler that takes up the rest of the space so that the panels dont overfit
    gbc.gridx = 0;
    gbc.gridy = 5;
    gbc.weighty = 1.0;
    gbc.fill = GridBagConstraints.BOTH;
    frame.add(Box.createGlue(), gbc);
    
    // Adding the slider
    gbc.gridx = 0;
    gbc.gridy = 8;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(slider, gbc);
    
    // Adding the slider value label
    gbc.gridx = 0;
    gbc.gridy = 7;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(sliderValueLabel, gbc);
    
    // Filler that takes up the rest of the space so that the panels dont overfit
    gbc.gridx = 0;
    gbc.gridy = 8;
    gbc.weighty = 1.0;
    gbc.fill = GridBagConstraints.BOTH;
    frame.add(Box.createGlue(), gbc);
    
    // Put the table in a scroll pane
    JScrollPane scrollPane = new JScrollPane(keyTable);

    // Add it to the frame
    gbc.gridx = 0;
    gbc.gridy = 9;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(scrollPane, gbc);

    // Key bindings
    InputMap inputMap = frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
    ActionMap actionMap = frame.getRootPane().getActionMap();

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0),"pressForward");
    actionMap.put("pressForward", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(buttonUp, ArduinoEndpoints.FORWARD);
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0), "pressBackward");
    actionMap.put("pressBackward", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(buttonDown, ArduinoEndpoints.BACKWARD);
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, 0), "pressSpotLeft");
    actionMap.put("pressSpotLeft", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
           sendCommand(buttonSpotLeft, ArduinoEndpoints.TURN_ON_SPOT_LEFT);
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, 0), "pressSpotRight");
    actionMap.put("pressSpotRight", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
           sendCommand(buttonSpotRight, ArduinoEndpoints.TURN_ON_SPOT_RIGHT);
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_C, 0), "pressCrabLeft");
    actionMap.put("pressCrabLeft", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(crabWalkLeft, ArduinoEndpoints.CRAB_WALK_LEFT);
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_V, 0), "pressCrabRight");
    actionMap.put("pressCrabRight", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(crabWalkRight, ArduinoEndpoints.CRAB_WALK_RIGHT);
        }  
    });

      inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_A, 0), "pressLeft");
    actionMap.put("pressLeft", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(buttonLeft, ArduinoEndpoints.LEFT);
        }  
    });


      inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_D, 0), "pressRight");
    actionMap.put("pressRight", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(buttonRight, ArduinoEndpoints.RIGHT);
        }  
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), "KillSwitch");
    actionMap.put("KillSwitch", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            sendCommand(killswitch, ArduinoEndpoints.STOP);
        }
    });
  frame.setVisible(true);
    return frame;
    }
}
