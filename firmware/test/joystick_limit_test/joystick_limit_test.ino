#include <joystick_control.h>
#include <limit_switch.h>

// 핀 정의
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_BUTTON_PIN 23

#define X_LIMIT_PIN 2
#define Y_LIMIT_PIN 3

// 객체 생성
JoystickControl joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, JOYSTICK_BUTTON_PIN);
LimitSwitch limitSwitch(X_LIMIT_PIN, Y_LIMIT_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Joystick and Limit Switch Test Started");

  joystick.setup();
  limitSwitch.setup();

  // 테스트 LED 설정
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int x_dir, y_dir;
  bool button_pressed;

  // 조이스틱 입력 읽기
  joystick.readInput(x_dir, y_dir, button_pressed);

  // 리미트 스위치 상태 확인
  bool x_limit = limitSwitch.isXLimitTriggered();
  bool y_limit = limitSwitch.isYLimitTriggered();

  // 시리얼 모니터 출력
  Serial.print("Joystick - X: ");
  Serial.print(x_dir);
  Serial.print(" Y: ");
  Serial.print(y_dir);
  Serial.print(" Button: ");
  Serial.print(button_pressed ? "Pressed" : "Released");

  Serial.print(" | Limits - X: ");
  Serial.print(x_limit ? "Triggered" : "Normal");
  Serial.print(" Y: ");
  Serial.println(y_limit ? "Triggered" : "Normal");

  // LED로 리미트 스위치 상태 표시
  digitalWrite(LED_BUILTIN, x_limit || y_limit);

  delay(100);  // 디버깅을 위한 짧은 지연
}
