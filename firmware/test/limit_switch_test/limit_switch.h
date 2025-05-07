#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include <Arduino.h>

class LimitSwitch {
public:
  LimitSwitch(int pin);
  void setup();
  bool isLimitTriggered();

private:
  int pin;
  bool triggered;
};

#endif
