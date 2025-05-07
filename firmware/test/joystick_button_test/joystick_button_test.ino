/*
다 되는 것인거 확인 완료
조이스틱 방향은 재설정 필요
/*

// 조이스틱 핀 정의
const int JX_P = 23;  // 조이스틱 X+ (우)
const int JX_M = 25;  // 조이스틱 X- (좌)
const int JY_P = 27;  // 조이스틱 Y+ (상)
const int JY_M = 29;  // 조이스틱 Y- (하)
const int BTN = 31;   // 조이스틱 버튼

void setup() {
  Serial.begin(115200);
  Serial.println("조이스틱 테스트 시작");

  // 조이스틱 핀 설정
  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  // 조이스틱 값 읽기 (LOW가 눌림 상태)
  bool jx_p = !digitalRead(JX_P);
  bool jx_m = !digitalRead(JX_M);
  bool jy_p = !digitalRead(JY_P);
  bool jy_m = !digitalRead(JY_M);
  bool btn = !digitalRead(BTN);

  // 상태 출력
  Serial.println("\n=== 조이스틱 상태 ===");
  Serial.print("X+ (우): "); Serial.println(jx_p ? "ON" : "OFF");
  Serial.print("X- (좌): "); Serial.println(jx_m ? "ON" : "OFF");
  Serial.print("Y+ (상): "); Serial.println(jy_p ? "ON" : "OFF");
  Serial.print("Y- (하): "); Serial.println(jy_m ? "ON" : "OFF");
  Serial.print("버튼: "); Serial.println(btn ? "ON" : "OFF");
  Serial.println("====================");

  delay(100);  // 100ms 간격으로 상태 업데이트
}
