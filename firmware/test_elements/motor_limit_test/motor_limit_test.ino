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
const int LY_P = 13;
const int LY_M = 15;
const int LZ = 17;

#define CW 1
#define CCW 0

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

bool isGripperMoving = false;
unsigned long gripperStartTime = 0;
const unsigned long GRIPPER_DELAY = 5000;  // 5초

void setup() {
  Serial.begin(115200);
  Serial.println("모터 및 리밋 스위치 테스트 시작");

  // 모터 및 조이스틱 초기화
  L_M.setup();
  R_M.setup();
  Z_M.setup();
  Grip.attach(GRIP_PIN);

  lx_p.setup();
  lx_m.setup();
  ly_p.setup();
  ly_m.setup();
  lz.setup();

  // 핀 설정
  pinMode(BTN, INPUT_PULLUP);
  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);
}

void loop() {
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

  // 버튼 입력 처리
  if (digitalRead(BTN) == LOW && !isGripperMoving) {
    isGripperMoving = true;
    gripperStartTime = millis();
    Z_M.move(CW);  // Z축 올리기 시작
  }

  // 그리퍼 동작 처리
  if (isGripperMoving) {
    // Z축이 리밋에 도달했거나 타임아웃
    if (lz.isLimitTriggered() || (millis() - gripperStartTime > GRIPPER_DELAY)) {
      Z_M.stop();
      Grip.write(180);  // 그리퍼 열기
      delay(500);
      Grip.write(0);    // 그리퍼 닫기
      delay(500);

      // Z축 원위치
      while(!lz.isLimitTriggered()) {
        Z_M.move(CCW);
        delay(10);
      }
      Z_M.stop();
      isGripperMoving = false;
    }
  }

  // 디버그 정보 출력
  if (lx_p.isLimitTriggered() || lx_m.isLimitTriggered() ||
      ly_p.isLimitTriggered() || ly_m.isLimitTriggered() ||
      lz.isLimitTriggered()) {
    Serial.println("리밋 스위치 동작!");
  }
}
