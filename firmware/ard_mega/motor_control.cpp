#include "motor_control.h"

MotorControl::MotorControl(int step_pin, int dir_pin, int enable_pin,
                         int limit_plus_pin, int limit_minus_pin)
    : stepPin(step_pin), dirPin(dir_pin), enablePin(enable_pin),
      limitPlusPin(limit_plus_pin), limitMinusPin(limit_minus_pin),
      isEnabled(false) {}

void MotorControl::setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(limitPlusPin, INPUT_PULLUP);
  pinMode(limitMinusPin, INPUT_PULLUP);

  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(enablePin, HIGH);  // 초기 상태는 비활성화
}

void MotorControl::move(int steps, int direction) {
  if (!isEnabled) return;

  digitalWrite(dirPin, direction);

  for (int i = 0; i < steps; i++) {
    if (checkLimit()) {
      stop();
      return;
    }

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }
}

void MotorControl::stop() {
  digitalWrite(stepPin, LOW);
}

bool MotorControl::checkLimit() {
  return digitalRead(limitPlusPin) == LOW || digitalRead(limitMinusPin) == LOW;
}

void MotorControl::enable() {
  digitalWrite(enablePin, LOW);
  isEnabled = true;
}

void MotorControl::disable() {
  digitalWrite(enablePin, HIGH);
  isEnabled = false;
}
