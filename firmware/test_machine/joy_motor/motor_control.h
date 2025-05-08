#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

class MotorControl {
public:
  MotorControl(const int pins[3]);  // [enable, dir1, dir2]

  void setup();
  void move(int direction);
  void stop();

private:
  int dir1Pin;
  int dir2Pin;
  int enablePin;
  bool isEnabled;
};

#endif
