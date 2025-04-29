#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include <Arduino.h>

class LimitSwitch {
public:
  LimitSwitch(int x_limit_pin, int y_limit_pin);
  void setup();
  bool isXLimitTriggered();
  bool isYLimitTriggered();

private:
  int xLimitPin;
  int yLimitPin;
  volatile bool xLimitTriggered = false;
  volatile bool yLimitTriggered = false;

  static void xLimitISR();
  static void yLimitISR();
  static LimitSwitch* instance;
};

#endif
