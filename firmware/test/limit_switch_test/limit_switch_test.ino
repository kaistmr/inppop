/*
x_plus 제외하고 잘 되는 거 확인 완료.
왜 안되는지는 확인 필요
/*

#include "limit_switch.h"

// 리밋 스위치 핀
const int LX_P = 9;
const int LX_M = 11;
const int LY_P = 13;
const int LY_M = 15;
const int LZ = 17;

// 객체 생성
LimitSwitch lx_p(LX_P);
LimitSwitch lx_m(LX_M);
LimitSwitch ly_p(LY_P);
LimitSwitch ly_m(LY_M);
LimitSwitch lz(LZ);

void setup() {
  Serial.begin(115200);
  Serial.println("리밋 스위치 테스트 시작");

  // 리밋 스위치 초기화
  lx_p.setup();
  lx_m.setup();
  ly_p.setup();
  ly_m.setup();
  lz.setup();
}

void loop() {
  // 각 리밋 스위치 상태 확인
  bool x_plus = lx_p.isLimitTriggered();
  bool x_minus = lx_m.isLimitTriggered();
  bool y_plus = ly_p.isLimitTriggered();
  bool y_minus = ly_m.isLimitTriggered();
  bool z = lz.isLimitTriggered();

  // 상태 출력
  Serial.println("\n=== 리밋 스위치 상태 ===");
  Serial.print("X+ : "); Serial.println(x_plus ? "ON" : "OFF");
  Serial.print("X- : "); Serial.println(x_minus ? "ON" : "OFF");
  Serial.print("Y+ : "); Serial.println(y_plus ? "ON" : "OFF");
  Serial.print("Y- : "); Serial.println(y_minus ? "ON" : "OFF");
  Serial.print("Z  : "); Serial.println(z ? "ON" : "OFF");
  Serial.println("=====================");

  delay(100);  // 100ms 간격으로 상태 업데이트
}
