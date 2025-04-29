#include <Arduino.h>
#include "joystick_control.h"
#include "motor_control.h"

// 조이스틱 핀 정의
const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;
const int JOYSTICK_BUTTON_PIN = 23;

// 모터 핀 정의
const int X_STEP_PIN = 2;
const int X_DIR_PIN = 3;
const int X_ENABLE_PIN = 4;
const int X_LIMIT_PLUS_PIN = 5;
const int X_LIMIT_MINUS_PIN = 6;

const int Y_STEP_PIN = 7;
const int Y_DIR_PIN = 8;
const int Y_ENABLE_PIN = 9;
const int Y_LIMIT_PLUS_PIN = 10;
const int Y_LIMIT_MINUS_PIN = 11;

// 객체 생성
JoystickControl joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, JOYSTICK_BUTTON_PIN);
MotorControl x_motor(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_LIMIT_PLUS_PIN, X_LIMIT_MINUS_PIN);
MotorControl y_motor(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_LIMIT_PLUS_PIN, Y_LIMIT_MINUS_PIN);

void setup() {
  Serial.begin(115200);

  // 초기화
  joystick.setup();
  x_motor.setup();
  y_motor.setup();

  // 모터 활성화
  x_motor.enable();
  y_motor.enable();
}

void loop() {
  // 조이스틱 값 읽기
  int x_value = joystick.readX();
  int y_value = joystick.readY();
  bool button_pressed = joystick.readButton();

  // X축 모터 제어
  if (abs(x_value) > joystick.DEADZONE) {
    x_motor.move(1, x_value > 0 ? HIGH : LOW);
  } else {
    x_motor.stop();
  }

  // Y축 모터 제어
  if (abs(y_value) > joystick.DEADZONE) {
    y_motor.move(1, y_value > 0 ? HIGH : LOW);
  } else {
    y_motor.stop();
  }

  // 버튼이 눌렸을 때
  if (button_pressed) {
    // 버튼 동작 구현
  }

  delay(10);  // 루프 지연
}
