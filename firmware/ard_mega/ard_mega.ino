// core xy에서 x축 y축으로 변경
// 하나하나 체크하면서 이동관련해서 코드 짜야 함.
// 기존의 코드를 이용해서 코드 참고해서 짜도 좋을듯?

#include "motor_control.h"
#include "limit_switch.h"
#include <Servo.h>

// 모터 핀
const int L_MOTOR[3] = {40, 36, 38};
const int R_MOTOR[3] = {41, 37, 39};
const int Z_MOTOR[3] = {47, 43, 45};
const int GRIP_PIN = 35;

// 조이스틱 핀
const int JX_P = 25;
const int JX_M = 23;
const int JY_P = 29;
const int JY_M = 27;
const int BTN = 31;

// 리밋 스위치 핀
const int LX_P = 21;
const int LX_M = 11;
const int LY_P = 17;
const int LY_M = 15;
const int LZ = 19;

const int COMM_OUT = 5;
const int COMM_IN = 2;

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
const unsigned long G_ACT = 2000;     // 1.5초
const int G_OPEN = 100;
const int G_CLOSE = 80;

// 모터 제어 상수
const int MOTOR_SPEED = 200;  // 모터 속도 (0-255)
const int GRIP_SPEED = 100;  // z축 모터 속도 (0-255)
const unsigned long MOTOR_TIMEOUT = 5000;  // 모터 타임아웃 (ms)

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
int gamecoin = 0;

void COIN_ISR() {
  gamecoin++;
  digitalWrite(COMM_OUT, HIGH);
}

void moveHome() {
  bool x_reached = false;
  bool y_reached = false;

  while(!x_reached || !y_reached)
  {
    // X축 이동 (X- 리밋 스위치로)
    if (!x_reached) {
      if (lx_m.isLimitTriggered()) {
        x_reached = true;
        L_M.stop();
        R_M.stop();
      } else {
        L_M.move(-1, MOTOR_SPEED);
        R_M.move(-1, MOTOR_SPEED);
      }
    }

    // Y축 이동 (Y+ 리밋 스위치로)
    if (!y_reached) {
      if (ly_m.isLimitTriggered()) {
        y_reached = true;
        L_M.stop();
        R_M.stop();
      } else {
        L_M.move(-1, MOTOR_SPEED);
        R_M.move(1, MOTOR_SPEED);
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

  L_M.move(l_dir, MOTOR_SPEED);
  R_M.move(r_dir, MOTOR_SPEED);
}

void gripMove() {
  // Z축 하강
  analogWrite(47, 255);
  digitalWrite(43, HIGH);
  digitalWrite(45, LOW);
  delay(1000);
  analogWrite(47, 0);
  digitalWrite(43, HIGH);
  digitalWrite(45, LOW);
  delay(1000);

  // 그리퍼 동작
  Grip.write(G_OPEN);
  delay(G_ACT);
  Grip.write(G_CLOSE);
  delay(G_ACT);

  // Z축 상승
  while(!lz.isLimitTriggered())
  {
    analogWrite(47, 255);
    digitalWrite(43, LOW);
    digitalWrite(45, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("로봇 제어 시스템 초기화 중...");

  attachInterrupt(digitalPinToInterrupt(COMM_IN), COIN_ISR, RISING);

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

  // 그리퍼 초기화
  Grip.attach(GRIP_PIN);
  Grip.write(G_OPEN);
  delay(1000);

  Serial.println("초기화 완료!");
  gamecoin = 0;
  delay(1000);
}

void loop() {
  Serial.print("gamecoin: ");
  Serial.println(gamecoin);

  // 버튼 상태 확인 (LOW가 눌림 상태)
  bool btn_state = !digitalRead(BTN);
  if (btn_state && !btn_pressed) {
    btn_pressed = true;
    L_M.stop();
    R_M.stop();
    Z_M.stop();
  }

  if(gamecoin > 0) {
    state = PLAY;
  }

  switch (state) {
    case WAIT:
      if (!is_start) {
        moveHome();
        Serial.println("초기 위치로 이동 완료");
      }
      break;

    case PLAY:
      gamePlay();
      if (btn_pressed) {
        gripMove();
        moveHome();
        btn_pressed = false;
        state = DONE;
        Serial.println("게임 종료!");
      }
      break;

    case DONE:
      moveHome();
      Serial.println("새 게임 준비 완료");
      state = WAIT;
      is_start = false;
      gamecoin--;
      digitalWrite(COMM_OUT, LOW);
      break;
  }
}
