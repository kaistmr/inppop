#ifndef JOYSTICK_CONTROL_H
#define JOYSTICK_CONTROL_H

#include <Arduino.h>

class JoystickControl {
public:
  JoystickControl(int x_pin, int y_pin, int button_pin);
  void setup();
  void readInput(int& x_direction, int& y_direction, bool& button_pressed);
  void update();  // 타이머 인터럽트에서 호출될 메서드

private:
  int xPin;
  int yPin;
  int buttonPin;
  const int DEADZONE = 100;  // 조이스틱 데드존
  const int DEBOUNCE_DELAY = 50;  // 버튼 디바운스 지연 시간
  unsigned long lastButtonPress = 0;  // 마지막 버튼 입력 시간
  bool lastButtonState = false;  // 이전 버튼 상태

  // 인터럽트에서 사용할 변수들
  volatile int x_direction = 0;
  volatile int y_direction = 0;
  volatile bool button_pressed = false;
  volatile unsigned long last_interrupt_time = 0;

  static const int INTERRUPT_INTERVAL = 10;  // 인터럽트 간격 (ms)
};

#endif
