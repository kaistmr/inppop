#include "limit_switch.h"

LimitSwitch* LimitSwitch::instance = nullptr;

LimitSwitch::LimitSwitch(int x_limit_pin, int y_limit_pin)
  : xLimitPin(x_limit_pin), yLimitPin(y_limit_pin) {
  instance = this;
}

void LimitSwitch::setup() {
  pinMode(xLimitPin, INPUT_PULLUP);
  pinMode(yLimitPin, INPUT_PULLUP);

  // 인터럽트 설정
  attachInterrupt(digitalPinToInterrupt(xLimitPin), xLimitISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(yLimitPin), yLimitISR, FALLING);
}

bool LimitSwitch::isXLimitTriggered() {
  return xLimitTriggered;
}

bool LimitSwitch::isYLimitTriggered() {
  return yLimitTriggered;
}

void LimitSwitch::xLimitISR() {
  if (instance) {
    instance->xLimitTriggered = true;
  }
}

void LimitSwitch::yLimitISR() {
  if (instance) {
    instance->yLimitTriggered = true;
  }
}
