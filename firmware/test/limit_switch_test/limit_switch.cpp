#include "limit_switch.h"

LimitSwitch::LimitSwitch(int pin)
  : pin(pin), triggered(false) {}

void LimitSwitch::setup() {
  pinMode(pin, INPUT_PULLUP);
}

bool LimitSwitch::isLimitTriggered() {
  triggered = !digitalRead(pin);
  return triggered;
}
