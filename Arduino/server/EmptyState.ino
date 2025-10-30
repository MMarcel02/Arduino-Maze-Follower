class EmptyState : public RobotState {
  public:
    virtual RobotState* handle(WifiClient& client) override {
      // no state changes, return THIS state
      return this;
    }
};
