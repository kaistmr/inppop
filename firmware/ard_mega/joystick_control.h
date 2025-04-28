#ifndef JOYSTICK_CONTROL_H
#define JOYSTICK_CONTROL_H

class JoystickControl {
public:
  JoystickControl(int x_pin, int y_pin, int button_pin);
  void setup();
  void readInput(int& x_direction, int& y_direction, bool& button_pressed);

private:
  int xPin;
  int yPin;
  int buttonPin;
  const int DEADZONE = 100;  // 조이스틱 데드존
};

#endif
