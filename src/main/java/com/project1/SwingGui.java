package com.project1;

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
    
    public static JFrame createGUI(){
    // Create an instance of JFrame
    JFrame frame = new JFrame("GUI Example");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frame.setSize(1000,1000);
    frame.setLocationRelativeTo(null);




    //Label for the top
    JLabel movementLabel = new JLabel("Movement Buttons",JLabel.CENTER);
    movementLabel.setFont(new Font ("Arial",Font.BOLD,16));
    
    // Create a panel of movement buttons
    JButton buttonUp = new JButton("Forward");
    JButton buttonDown = new JButton("Backward");
    JButton buttonLeft = new JButton("Left");
    JButton buttonRight = new JButton("Right");
    JButton crabWalkLeft= new JButton("CrabWalkLeft");
    JButton crabWalkRight= new JButton("CrabWalkRight");
    JPanel buttonPanel = new JPanel();
    buttonPanel.setLayout(new GridLayout(2,3,5,5));
    buttonPanel.add(crabWalkLeft);
    buttonPanel.add(buttonUp);
    buttonPanel.add(crabWalkRight);
    buttonPanel.add(buttonLeft);
    buttonPanel.add(buttonDown);
    buttonPanel.add(buttonRight);
    // Killswith button

    JButton killswitch = new JButton("KillSwitch");
    killswitch.setPreferredSize(new Dimension(killswitch.getPreferredSize().width, 40));
    
    //Label for speed panel
    JLabel speedLabel = new JLabel("SpeedButtons",JLabel.CENTER);
    speedLabel.setFont(new Font ("Arial",Font.BOLD,16));
    
    // Create a panel for speed buttons
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
    
    // Add a slider
    JSlider slider = new JSlider(0, 255, 80); // min, max, initial value
    slider.setMajorTickSpacing(50);
    slider.setMinorTickSpacing(10);
    slider.setPaintTicks(true);
    slider.setPaintLabels(true);
    
    // Label to display the slider value
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
    
    // Add ActionListeners to speed buttons
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
    gbc.fill = GridBagConstraints.NONE; // Do not stretch the panel
    gbc.anchor = GridBagConstraints.NORTH; // Center the panel
    gbc.weighty = 0.0;
    frame.add(buttonPanel, gbc);
    
    // Add the killswitch below the button panel
    gbc.gridx=0;
    gbc.gridy=2;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty=0.0;
    frame.add(killswitch,gbc);
    
    // Speed label
    gbc.gridx = 0;
    gbc.gridy = 3;
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor= GridBagConstraints.NORTH;
    gbc.weightx = 1.0;
    gbc.weighty = 0.0;
    frame.add(speedLabel, gbc);
    
    gbc.gridx = 0;
    gbc.gridy = 4;
    gbc.fill = GridBagConstraints.NONE; // Do not stretch the panel
    gbc.anchor = GridBagConstraints.NORTH; // Center the panel
    gbc.weighty = 0.0;
    frame.add(speedPanel, gbc);
    
    // Add the slider
    gbc.gridx = 0;
    gbc.gridy = 5; // Place after speedPanel
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(slider, gbc);
    
    // Add the slider value label
    gbc.gridx = 0;
    gbc.gridy = 6; // Place after the slider
    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.anchor = GridBagConstraints.NORTH;
    gbc.weighty = 0.0;
    frame.add(sliderValueLabel, gbc);
    
    
    // Filler
    gbc.gridx = 0;
    gbc.gridy = 7;
    gbc.weighty = 1.0;  // Take up all extra vertical space
    gbc.fill = GridBagConstraints.BOTH;
    frame.add(Box.createGlue(), gbc);
    

    // Key bindings
    InputMap inputMap = frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
    ActionMap actionMap = frame.getRootPane().getActionMap();

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0),"pressForward");
    actionMap.put("pressForward", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            buttonUp.doClick();
            lastAction = ArduinoEndpoints.FORWARD;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.FORWARD);
                System.out.println(ArduinoEndpoints.FORWARD + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
            }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0), "pressBackward");
    actionMap.put("pressBackward", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            buttonDown.doClick();
            lastAction = ArduinoEndpoints.BACKWARD;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.BACKWARD);
                System.out.println(ArduinoEndpoints.BACKWARD + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, 0), "pressLeft");
    actionMap.put("pressLeft", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            buttonLeft.doClick();
            lastAction = ArduinoEndpoints.LEFT;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.LEFT);
                System.out.println(ArduinoEndpoints.LEFT + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }
        
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, 0), "pressRight");
    actionMap.put("pressRight", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            buttonRight.doClick();
            lastAction = ArduinoEndpoints.RIGHT;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.RIGHT);
                System.out.println(ArduinoEndpoints.RIGHT + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_C, 0), "pressCrabLeft");
    actionMap.put("pressCrabLeft", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            crabWalkLeft.doClick();
            lastAction = ArduinoEndpoints.CRAB_WALK_LEFT;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.CRAB_WALK_LEFT);
                System.out.println(ArduinoEndpoints.CRAB_WALK_LEFT + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
        
        }
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_V, 0), "pressCrabRight");
    actionMap.put("pressCrabRight", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            crabWalkRight.doClick();
            lastAction = ArduinoEndpoints.CRAB_WALK_RIGHT;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.CRAB_WALK_RIGHT);
                System.out.println(ArduinoEndpoints.CRAB_WALK_RIGHT + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }  
    });

    inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), "KillSwitch");
    actionMap.put("KillSwitch", new AbstractAction() {
        @Override
        public void actionPerformed(ActionEvent event) {
            killswitch.doClick();
            lastAction = ArduinoEndpoints.STOP;
            System.out.println(lastAction);
            try {
                arduClient.send(ArduinoEndpoints.STOP);
                System.out.println(ArduinoEndpoints.STOP + " Succeed");
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }
    });

  frame.setVisible(true);
    return frame;
    }

}
