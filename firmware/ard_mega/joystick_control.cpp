#include "joystick_control.h"

JoystickControl::JoystickControl(int x_pin, int y_pin, int button_pin)
  : xPin(x_pin), yPin(y_pin), buttonPin(button_pin) {}

void JoystickControl::setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  // 타이머 인터럽트 설정
  cli();  // 인터럽트 비활성화
  TCCR1A = 0;  // 타이머1 초기화
  TCCR1B = 0;
  TCNT1 = 0;

  // 16MHz / 64(prescaler) = 250kHz
  // 250kHz / 1000 = 250Hz (4ms)
  OCR1A = 249;  // 비교값 설정

  TCCR1B |= (1 << WGM12);  // CTC 모드
  TCCR1B |= (1 << CS11) | (1 << CS10);  // 64 prescaler
  TIMSK1 |= (1 << OCIE1A);  // 타이머 비교 인터럽트 활성화
  sei();  // 인터럽트 활성화
}

void JoystickControl::update() {
  // 조이스틱 입력 읽기
  int x_value = analogRead(xPin) - 512;  // 중앙값 512 기준으로 오프셋
  int y_value = analogRead(yPin) - 512;

  // 데드존 적용
  x_direction = (abs(x_value) > DEADZONE) ? (x_value > 0 ? 1 : -1) : 0;
  y_direction = (abs(y_value) > DEADZONE) ? (y_value > 0 ? 1 : -1) : 0;

  // 버튼 디바운스 처리
  bool currentButtonState = digitalRead(buttonPin) == LOW;  // 풀업 저항 사용
  unsigned long currentTime = millis();

  if (currentButtonState != lastButtonState) {
    lastButtonPress = currentTime;
  }

  if ((currentTime - lastButtonPress) > DEBOUNCE_DELAY) {
    button_pressed = currentButtonState;
  } else {
    button_pressed = lastButtonState;
  }

  lastButtonState = currentButtonState;
}

void JoystickControl::readInput(int& x_dir, int& y_dir, bool& btn_pressed) {
  // 인터럽트에서 업데이트된 값을 읽어옴
  x_dir = x_direction;
  y_dir = y_direction;
  btn_pressed = button_pressed;
}

// 타이머 인터럽트 핸들러
ISR(TIMER1_COMPA_vect) {
  static JoystickControl* instance = nullptr;
  if (instance) {
    instance->update();
  }
}
