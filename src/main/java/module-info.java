module com.project1 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.net.http;

    opens com.project1 to javafx.fxml;
    exports com.project1;
}
