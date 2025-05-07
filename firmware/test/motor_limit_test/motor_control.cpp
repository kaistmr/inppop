#include "motor_control.h"

MotorControl::MotorControl(const int pins[3])
    : enablePin(pins[0]), dir1Pin(pins[1]), dir2Pin(pins[2]), isEnabled(false) {}

void MotorControl::setup() {
  pinMode(dir1Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin, LOW);  // 초기 상태는 비활성화
  isEnabled = true;
}

void MotorControl::move(int direction) {
  if (!isEnabled) return;

  if (direction > 0) {
    digitalWrite(dir1Pin, HIGH);
    digitalWrite(dir2Pin, LOW);
    analogWrite(enablePin, 255);
  }
  else if (direction < 0) {
    digitalWrite(dir2Pin, HIGH);
    digitalWrite(dir1Pin, LOW);
    analogWrite(enablePin, 255);
  }
  else {
    stop();
  }
}

void MotorControl::stop() {
  digitalWrite(enablePin, LOW);
}
