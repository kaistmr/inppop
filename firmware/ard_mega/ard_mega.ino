#include "motor_control.h"
#include "joystick_control.h"

// 모터 핀 정의
const MotorPins X_MOTOR_PINS = {40, 36, 38, 9, 11};
const MotorPins Y_MOTOR_PINS = {41, 37, 39, 13, 15};
const MotorPins Z_MOTOR_PINS = {46, 42, 44, -1, -1};  // Z축은 리밋 스위치 없음
const int GRIPPER_PIN = 34;

// 조이스틱 핀
const int JOYSTICK_X = A0;
const int JOYSTICK_Y = A1;
const int JOYSTICK_BUTTON = 23;

// 통신 핀
const int COMM_RECEIVE = 3;  // Nano로부터 신호 수신
const int COMM_SEND = 5;     // Nano로 신호 송신

// 게임 상태
enum GameState {
  WAITING,    // 대기 상태
  PLAYING,    // 게임 진행 중
  FINISHED    // 게임 종료
};

// 객체 생성
MotorControl motors(X_MOTOR_PINS, Y_MOTOR_PINS, Z_MOTOR_PINS, GRIPPER_PIN);
JoystickControl joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_BUTTON);

GameState currentState = WAITING;

void setup() {
  // 모터 및 조이스틱 초기화
  motors.setup();
  joystick.setup();

  // 통신 핀 설정
  pinMode(COMM_RECEIVE, INPUT);
  pinMode(COMM_SEND, OUTPUT);
  digitalWrite(COMM_SEND, LOW);

  // 시리얼 통신 시작
  Serial.begin(9600);
}

void loop() {
  // Nano로부터 게임 시작 신호 확인
  if (digitalRead(COMM_RECEIVE) == HIGH && currentState == WAITING) {
    currentState = PLAYING;
    digitalWrite(COMM_SEND, HIGH);
  }

  switch (currentState) {
    case WAITING:
      // 대기 상태에서는 모터 제어 없음
      break;

    case PLAYING:
      handleGamePlay();
      break;

    case FINISHED:
      digitalWrite(COMM_SEND, LOW);
      if (digitalRead(COMM_RECEIVE) == LOW) {
        currentState = WAITING;
      }
      break;
  }
}

void handleGamePlay() {
  int x_direction, y_direction;
  bool button_pressed;

  // 조이스틱 입력 읽기
  joystick.readInput(x_direction, y_direction, button_pressed);

  // 모터 제어
  motors.moveX(x_direction);
  motors.moveY(y_direction);

  // 버튼이 눌리면 그리퍼 시퀀스 실행
  if (button_pressed) {
    motors.executeGripperSequence();
    currentState = FINISHED;
  }
}
