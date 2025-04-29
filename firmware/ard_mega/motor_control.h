#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

class MotorControl {
public:
  MotorControl(int step_pin, int dir_pin, int enable_pin,
               int limit_plus_pin, int limit_minus_pin);

  void setup();
  void move(int steps, int direction);
  void stop();
  bool checkLimit();
  void enable();
  void disable();

private:
  int stepPin;
  int dirPin;
  int enablePin;
  int limitPlusPin;
  int limitMinusPin;
  bool isEnabled;

  static const int STEP_DELAY = 1000;  // 마이크로초 단위
};

#endif
