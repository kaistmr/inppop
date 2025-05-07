#include "motor_control.h"
#include "limit_switch.h"
#include <Servo.h>

// 모터 핀
const int L_MOTOR[3] = {40, 36, 38};
const int R_MOTOR[3] = {46, 42, 44};
const int Z_MOTOR[3] = {41, 37, 39};
const int GRIP_PIN = 34;

// 조이스틱 핀
const int JX_P = 23;
const int JX_M = 25;
const int JY_P = 27;
const int JY_M = 29;
const int BTN = 31;

// 통신 핀
const int COMM_IN = 3;  // Nano로부터 신호 수신
const int COMM_OUT = 5;  // Nano로 신호 송신

// 리밋 스위치 핀
const int LX_P = 9;
const int LX_M = 11;
const int LY_P = 13;
const int LY_M = 15;
const int LZ = 17;

#define CW 1
#define CCW 0

// 게임 상태
enum GameState {
  WAIT,    // 대기 상태
  PLAY,    // 게임 진행 중
  DONE     // 게임 종료
};

// 그리퍼 상수
const unsigned long G_DELAY = 5000;  // 5초
const unsigned long G_ACT = 100;     // 0.1초
const int G_OPEN = 180;
const int G_CLOSE = 0;

// 통신 관련 상수
const unsigned long COMM_DEBOUNCE = 50;  // 디바운스 시간 (ms)
unsigned long lastCommTime = 0;          // 마지막 통신 시간
bool lastCommState = false;              // 마지막 통신 상태

// 객체 생성
MotorControl L_M(L_MOTOR);
MotorControl R_M(R_MOTOR);
MotorControl Z_M(Z_MOTOR);
LimitSwitch lx_p(LX_P);
LimitSwitch lx_m(LX_M);
LimitSwitch ly_p(LY_P);
LimitSwitch ly_m(LY_M);
LimitSwitch lz(LZ);
Servo Grip;

GameState state = WAIT;
bool btn_pressed = false;
bool is_start = false;

void setup() {
  // 모터 및 조이스틱 초기화
  L_M.setup();
  R_M.setup();
  Z_M.setup();
  joystick.setup();
  lx_p.setup();
  lx_m.setup();
  ly_p.setup();
  ly_m.setup();
  lz.setup();

  // 통신 핀 설정
  pinMode(COMM_IN, INPUT);
  pinMode(COMM_OUT, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  digitalWrite(COMM_OUT, LOW);
  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BTN), btn_isr, FALLING);
}

void loop() {
  // Nano로부터 게임 시작 신호 확인 (디바운싱 적용)
  bool currentCommState = digitalRead(COMM_IN);
  unsigned long currentTime = millis();

  if (currentCommState != lastCommState) {
    lastCommTime = currentTime;
  }

  if ((currentTime - lastCommTime) > COMM_DEBOUNCE) {
    if (currentCommState == HIGH && state == WAIT) {
      state = PLAY;
      digitalWrite(COMM_OUT, HIGH);
    }
  }

  lastCommState = currentCommState;

  switch (state) {
    case WAIT:
      if (!is_start) moveStart();
      break;

    case PLAY:
      gamePlay();
      gripMove();
      moveEnd();
      btn_pressed = false;
      break;

    case DONE:
      moveStart();
      digitalWrite(COMM_OUT, HIGH);
      if (digitalRead(COMM_IN) == LOW) {
        state = WAIT;
      }
      break;
  }
}

void moveStart()
{
  while(!lx_m.isLimitTriggered())
  {
    L_M.move(CW);
    R_M.move(CW);
  }
  L_M.stop();
  R_M.stop();
  while(!ly_m.isLimitTriggered())
  {
    L_M.move(CW);
    R_M.move(CCW);
  }
  L_M.stop();
  R_M.stop();
  is_start = true;
}

void moveEnd()
{
  if (!btn_pressed) return;

  while(!lx_m.isLimitTriggered())
  {
    L_M.move(CW);
    R_M.move(CW);
  }
  L_M.stop();
  R_M.stop();
  while(!ly_p.isLimitTriggered())
  {
    L_M.move(CCW);
    R_M.move(CW);
  }
  L_M.stop();
  R_M.stop();
}

void gamePlay()
{
  int l_dir, r_dir;

  if (btn_pressed) return;

  // 조이스틱 입력 읽기
  int jx = digitalRead(JX_M)-digitalRead(JX_P);
  int jy = digitalRead(JY_M)-digitalRead(JY_P);

  // 리밋 스위치 체크
  if (lx_p.isLimitTriggered() && jx > 0) jx = 0;
  if (lx_m.isLimitTriggered() && jx < 0) jx = 0;
  if (ly_p.isLimitTriggered() && jy > 0) jy = 0;
  if (ly_m.isLimitTriggered() && jy < 0) jy = 0;

  l_dir = jx + jy;
  r_dir = jx - jy;

  // 모터 제어
  L_M.move(l_dir);
  R_M.move(r_dir);
}

void gripMove()
{
  if (!btn_pressed) return;

  unsigned long start = millis();
  Z_M.move(CW);

  // 타임아웃 체크를 포함한 Z축 이동
  while(!lz.isLimitTriggered() && (millis() - start < G_DELAY)) {
    delay(10);
  }
  Z_M.stop();

  // 그리퍼 동작
  Grip.write(G_OPEN);
  delay(G_ACT);
  Grip.write(G_CLOSE);
  delay(G_ACT);

  // Z축 원위치
  start = millis();
  while(!lz.isLimitTriggered() && (millis() - start < G_DELAY)) {
    Z_M.move(CCW);
    delay(10);
  }
  Z_M.stop();
}

void btn_isr()
{
  L_M.stop();
  R_M.stop();
  btn_pressed = true;
}

