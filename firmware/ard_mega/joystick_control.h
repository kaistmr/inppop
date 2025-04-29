#ifndef JOYSTICK_CONTROL_H
#define JOYSTICK_CONTROL_H

#include <Arduino.h>

class JoystickControl {
public:
  JoystickControl(int x_plus_pin, int x_minus_pin,
                 int y_plus_pin, int y_minus_pin,
                 int button_pin);
  void setup();
  void readInput(int& x_direction, int& y_direction, bool& button_pressed);
  void update();  // 인터럽트에서 호출됨

private:
  int xPlusPin;
  int xMinusPin;
  int yPlusPin;
  int yMinusPin;
  int buttonPin;

  const int DEBOUNCE_DELAY = 50;  // 버튼 디바운스 지연 시간
  unsigned long lastButtonPress = 0;  // 마지막 버튼 입력 시간
  bool lastButtonState = false;  // 이전 버튼 상태

  // 인터럽트에서 사용할 변수들
  volatile bool x_plus_triggered = false;
  volatile bool x_minus_triggered = false;
  volatile bool y_plus_triggered = false;
  volatile bool y_minus_triggered = false;
  volatile bool button_pressed = false;

  static void xPlusISR();
  static void xMinusISR();
  static void yPlusISR();
  static void yMinusISR();
  static void buttonISR();
  static JoystickControl* instance;
};

#endif
