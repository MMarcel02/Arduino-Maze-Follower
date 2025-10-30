class ManualControlState : public RobotState {
  public:
    void enter(WifiClient& client) override {
      client.print("Entered manual control state");
    }
    
    virtual RobotState* handle(WifiClient& client) override {
      
      // The movement is handled on the server,
      // so we don't need to do anything here (for now).
      
      // as an example for a transition,
      // change to the empty state, if the sensors
      // read a distance of less than 20cm:
      if (getDistanceCM() < 20) 
        return &EMPTY_STATE;
      
      // no state changes, return THIS state
      return this;
    }
};
