#include "joystick_control.h"

JoystickControl::JoystickControl(int x_pin, int y_pin, int button_pin)
  : xPin(x_pin), yPin(y_pin), buttonPin(button_pin) {}

void JoystickControl::setup() {
  pinMode(buttonPin, INPUT_PULLUP);
}

void JoystickControl::readInput(int& x_direction, int& y_direction, bool& button_pressed) {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  // X축 방향 결정
  if (xValue > 512 + DEADZONE) {
    x_direction = 1;
  } else if (xValue < 512 - DEADZONE) {
    x_direction = -1;
  } else {
    x_direction = 0;
  }

  // Y축 방향 결정
  if (yValue > 512 + DEADZONE) {
    y_direction = 1;
  } else if (yValue < 512 - DEADZONE) {
    y_direction = -1;
  } else {
    y_direction = 0;
  }

  // 버튼 상태 확인
  button_pressed = (digitalRead(buttonPin) == LOW);
}
