class EmptyState : public RobotState {
  public:
    virtual RobotState* handle() override {
      // no state changes, return THIS state
      return this;
    }
};
