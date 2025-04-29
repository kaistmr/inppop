#include "joystick_control.h"

JoystickControl* JoystickControl::instance = nullptr;

JoystickControl::JoystickControl(int x_plus_pin, int x_minus_pin,
                               int y_plus_pin, int y_minus_pin,
                               int button_pin)
    : xPlusPin(x_plus_pin), xMinusPin(x_minus_pin),
      yPlusPin(y_plus_pin), yMinusPin(y_minus_pin),
      buttonPin(button_pin) {
  instance = this;
}

void JoystickControl::setup() {
  pinMode(xPlusPin, INPUT_PULLUP);
  pinMode(xMinusPin, INPUT_PULLUP);
  pinMode(yPlusPin, INPUT_PULLUP);
  pinMode(yMinusPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(xPlusPin), xPlusISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(xMinusPin), xMinusISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(yPlusPin), yPlusISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(yMinusPin), yMinusISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
}

void JoystickControl::readInput(int& x_direction, int& y_direction, bool& button_pressed) {
  noInterrupts();
  x_direction = 0;
  if (x_plus_triggered) {
    x_direction = 1;
    x_plus_triggered = false;
  }
  if (x_minus_triggered) {
    x_direction = -1;
    x_minus_triggered = false;
  }

  y_direction = 0;
  if (y_plus_triggered) {
    y_direction = 1;
    y_plus_triggered = false;
  }
  if (y_minus_triggered) {
    y_direction = -1;
    y_minus_triggered = false;
  }

  button_pressed = this->button_pressed;
  this->button_pressed = false;
  interrupts();
}

void JoystickControl::update() {
  // 인터럽트에서 호출되는 메서드
  // 현재는 추가적인 처리가 필요하지 않음
}

void JoystickControl::xPlusISR() {
  if (instance) {
    instance->x_plus_triggered = true;
  }
}

void JoystickControl::xMinusISR() {
  if (instance) {
    instance->x_minus_triggered = true;
  }
}

void JoystickControl::yPlusISR() {
  if (instance) {
    instance->y_plus_triggered = true;
  }
}

void JoystickControl::yMinusISR() {
  if (instance) {
    instance->y_minus_triggered = true;
  }
}

void JoystickControl::buttonISR() {
  if (instance) {
    unsigned long current_time = millis();
    if (current_time - instance->lastButtonPress > instance->DEBOUNCE_DELAY) {
      instance->button_pressed = true;
      instance->lastButtonPress = current_time;
    }
  }
}
