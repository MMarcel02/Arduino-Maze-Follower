class ManualControlState : public RobotState {
  public:
    void enter(WifiClient& client) override {
      client.print("Entered manual control state");
    }
    
    virtual RobotState* handle(WifiClient& client) override {
      
      // The movement is handled on the server,
      // so we don't need to do anything here (for now).
      
      // no state changes, return THIS state
      return this;
    }
};
