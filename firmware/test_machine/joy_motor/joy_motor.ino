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

// 리밋 스위치 핀
const int LX_P = 9;
const int LX_M = 11;
const int LY_P = 15;
const int LY_M = 17;
const int LZ = 19;

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
const int G_OPEN = 140;
const int G_CLOSE = 50;

// 모터 제어 상수
const int MOTOR_SPEED = 200;  // 모터 속도 (0-255)
const unsigned long MOTOR_TIMEOUT = 5000;  // 모터 타임아웃 (ms)

// 시리얼 통신 상수
const char CMD_START = 's';  // 게임 시작
const char CMD_STOP = 'e';   // 게임 종료
const char CMD_RESET = 'r';  // 리셋

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
  Serial.begin(115200);
  Serial.println("로봇 제어 시스템 초기화 중...");

  // 모터 및 조이스틱 초기화
  L_M.setup();
  R_M.setup();
  Z_M.setup();

  // 리밋 스위치 초기화
  lx_p.setup();
  lx_m.setup();
  ly_p.setup();
  ly_m.setup();
  lz.setup();

  // 조이스틱 핀 설정
  pinMode(BTN, INPUT_PULLUP);
  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);
}

void loop() {
  char cmd = 's';
  processCommand(cmd);
  // 버튼 상태 확인 (LOW가 눌림 상태)
  bool btn_state = !digitalRead(BTN);
  if (btn_state && !btn_pressed) {
    btn_pressed = true;
    L_M.stop();
    R_M.stop();
    Z_M.stop();
  }

  switch (state) {
    case PLAY:
      gamePlay();
      if (btn_pressed) {
        gripMove();
        moveEnd();
        btn_pressed = false;
        state = DONE;
        Serial.println("게임 종료!");
      }
      break;

    case DONE:
      moveStart();
      Serial.println("새 게임 준비 완료");
      state = PLAY;
      is_start = false;
      break;
  }
}

void processCommand(char cmd) {
  switch (cmd) {
    case CMD_START:
      if (state == WAIT) {
        state = PLAY;
        Serial.println("게임 시작!");
      }
      break;

    case CMD_STOP:
      if (state == PLAY) {
        state = DONE;
        Serial.println("게임 강제 종료!");
      }
      break;

    case CMD_RESET:
      state = WAIT;
      is_start = false;
      Serial.println("시스템 리셋!");
      break;
  }
}

void moveStart() {
  unsigned long startTime = millis();
  bool x_reached = false;
  bool y_reached = false;

  while((!x_reached || !y_reached) && (millis() - startTime < MOTOR_TIMEOUT)) {
    // X축 이동 (X+ 리밋 스위치로)
    if (!x_reached) {
      if (lx_p.isLimitTriggered()) {
        x_reached = true;
        L_M.stop();
        R_M.stop();
      } else {
        L_M.move(CCW);
        R_M.move(CCW);
      }
    }

    // Y축 이동 (Y+ 리밋 스위치로)
    if (!y_reached) {
      if (ly_p.isLimitTriggered()) {
        y_reached = true;
        L_M.stop();
        R_M.stop();
      } else {
        L_M.move(CCW);
        R_M.move(CW);
      }
    }

    delay(10);
  }

  L_M.stop();
  R_M.stop();
  is_start = true;
}

void moveEnd() {
  unsigned long startTime = millis();
  bool x_reached = false;
  bool y_reached = false;

  while((!x_reached || !y_reached) && (millis() - startTime < MOTOR_TIMEOUT)) {
    // X축 이동 (X- 리밋 스위치로)
    if (!x_reached) {
      if (lx_m.isLimitTriggered()) {
        x_reached = true;
        L_M.stop();
        R_M.stop();
      } else {
        L_M.move(CW);
        R_M.move(CW);
      }
    }

    // Y축 이동 (Y+ 리밋 스위치로)
    if (!y_reached) {
      if (ly_p.isLimitTriggered()) {
        y_reached = true;
        L_M.stop();
        R_M.stop();
      } else {
        L_M.move(CCW);
        R_M.move(CW);
      }
    }

    delay(10);
  }

  L_M.stop();
  R_M.stop();
}

void gamePlay() {
  if (btn_pressed) return;

  // 조이스틱 입력 읽기
  int jx = digitalRead(JX_M)-digitalRead(JX_P);
  int jy = digitalRead(JY_M)-digitalRead(JY_P);

  // 리밋 스위치 체크
  if (lx_p.isLimitTriggered() && jx > 0) jx = 0;
  if (lx_m.isLimitTriggered() && jx < 0) jx = 0;
  if (ly_p.isLimitTriggered() && jy > 0) jy = 0;
  if (ly_m.isLimitTriggered() && jy < 0) jy = 0;

  // 모터 제어
  int l_dir = jx + jy;
  int r_dir = jx - jy;

  L_M.move(l_dir);
  R_M.move(r_dir);
}

void gripMove() {
  Serial.println("gripp!!");
  unsigned long startTime = millis();

  // Z축 하강
  Z_M.move(CW);
  while(!lz.isLimitTriggered() && (millis() - startTime < G_DELAY)) {
    delay(10);
  }
  Z_M.stop();

  // 그리퍼 동작
  Grip.write(G_OPEN);
  delay(G_ACT);
  Grip.write(G_CLOSE);
  delay(G_ACT);

  // Z축 상승
  startTime = millis();
  Z_M.move(CCW);
  while(!lz.isLimitTriggered() && (millis() - startTime < G_DELAY)) {
    delay(10);
  }
  Z_M.stop();
}
