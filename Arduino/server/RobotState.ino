class RobotState {
  public:
      virtual void enter() {}
      virtual void exit() {}
      
      /// The handle function is called each frame
      ///
      /// Must return a RobotState pointer to the next state,
      /// if you want the state to remain the same,
      /// return the reference to this (see EmptyState.ino for example).
      virtual RobotState* handle() = 0;
};
