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
const int COMM_IN = 3;   // DigitalRead
const int COMM_OUT = 5;  // DigitalWrite

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
const unsigned long G_ACT = 1000;     // 1초
const int G_OPEN = 50;
const int G_CLOSE = 140;

// 모터 제어 상수
const int MOTOR_SPEED = 170;  // 모터 속도 (0-255)
const unsigned long MOTOR_TIMEOUT = 10000;  // 모터 타임아웃 (ms)

// 통신 관련 상수
const unsigned long COMM_DELAY = 3000;  // 통신 딜레이 (ms)
unsigned long lastCommTime = 0;         // 마지막 통신 시간

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
bool is_gripping = false;

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

  // 통신 핀 설정
  pinMode(COMM_IN, INPUT);
  pinMode(COMM_OUT, OUTPUT);
  digitalWrite(COMM_OUT, LOW);

  // 조이스틱 핀 설정
  pinMode(BTN, INPUT_PULLUP);
  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);

  // 그리퍼 초기화
  Grip.attach(GRIP_PIN);
  Grip.write(G_OPEN);

  Serial.println("초기화 완료!");
}

void loop() {
  // 통신 처리
  if (state == WAIT) {
    if (digitalRead(COMM_IN) == HIGH) {
      state = PLAY;
      digitalWrite(COMM_OUT, HIGH);
      Serial.println("게임 시작!");
    }
  }
  else if (state == DONE) {
    if (millis() - lastCommTime >= COMM_DELAY) {
      if (digitalRead(COMM_IN) == HIGH) {
        state = PLAY;
        digitalWrite(COMM_OUT, HIGH);
        Serial.println("게임 시작!");
      }
    }
  }

  // 버튼 상태 확인 (LOW가 눌림 상태)
  bool btn_state = !digitalRead(BTN);
  if (btn_state && !btn_pressed) {
    btn_pressed = true;
    L_M.stop();
    R_M.stop();
    Z_M.stop();
  }

  switch (state) {
    case WAIT:
      if (!is_start) {
        moveStart();
        Serial.println("초기 위치로 이동 완료");
      }
      break;

    case PLAY:
      gamePlay();
      if (btn_pressed) {
        gripMove();
        btn_pressed = false;
        state = DONE;
        digitalWrite(COMM_OUT, LOW);
        lastCommTime = millis();
        Serial.println("게임 종료!");
      }
      break;

    case DONE:
      moveStart();
      Serial.println("새 게임 준비 완료");
      state = WAIT;
      is_start = false;
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
      if (!digitalRead(LX_M)) {
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
      if (!digitalRead(LY_P)) {
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
  Serial.println("그리퍼 동작 시작");
  unsigned long startTime = millis();

  // Z축 하강
  Z_M.move(CW);
  delay(3000);
  Z_M.stop();

  // 그리퍼 동작 (열기-닫기)
  Grip.write(G_OPEN);
  delay(G_ACT);
  Grip.write(G_CLOSE);
  delay(G_ACT);

  // Z축 상승
  startTime = millis();
  while(!digitalRead(LZ)) {
    Z_M.move(CCW);
  }
  Z_M.stop();
  delay(10);

  // 끝점으로 이동
  moveEnd();

  // 그리퍼 동작 (열기-닫기)
  Grip.write(G_OPEN);
  delay(G_ACT);
  Grip.write(G_CLOSE);
  delay(G_ACT);
}
