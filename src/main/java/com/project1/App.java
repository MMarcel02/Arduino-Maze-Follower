package com.project1;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.stage.Stage;

import java.io.IOException;

/**
 * JavaFX App
 */
public class App extends Application {

    private static Scene scene;
    @Override
    public void start(Stage stage) throws IOException {
        // So we load our layout of our GUI (kind of like HTML in web dev)
        FXMLLoader fxmlLoader = new FXMLLoader(App.class.getResource("GUI.fxml"));
        // We set our layout to be 900 * 600 pixels
        scene = new Scene(fxmlLoader.load(), 900, 600);
        // We load the style sheet for it (just like css in web dev)
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());

        // A stage is like a window frame, a scene is like the glass inside, our application only has one scene
        stage.setScene(scene);

        // We create the controller so that we can read key inputs
        GUIController controller = fxmlLoader.getController();
        controller.setupInputHandlers(scene);

        // This is just so that when we use the arrow keys it doesnt move focus accross the UI
        scene.getRoot().setFocusTraversable(true);
        scene.getRoot().requestFocus(); 

        // When launched it should be centered on the screen
        stage.centerOnScreen();
        stage.show();
    }
    
    
    public static void main(String[] args) {
        launch();
    }

}