// X축 리밋 스위치 핀
const int X_LIMIT_PLUS = 9;
const int X_LIMIT_MINUS = 11;

// Y축 리밋 스위치 핀
const int Y_LIMIT_PLUS = 13;
const int Y_LIMIT_MINUS = 15;

void setup() {
  Serial.begin(9600);

  // X축 리밋 스위치 설정
  pinMode(X_LIMIT_PLUS, INPUT_PULLUP);
  pinMode(X_LIMIT_MINUS, INPUT_PULLUP);

  // Y축 리밋 스위치 설정
  pinMode(Y_LIMIT_PLUS, INPUT_PULLUP);
  pinMode(Y_LIMIT_MINUS, INPUT_PULLUP);

  Serial.println("리밋 스위치 테스트 시작");
  Serial.println("리밋 스위치 상태가 0이면 눌린 상태입니다.");
}

void loop() {
  // 리밋 스위치 상태 출력
  Serial.print("X+:"); Serial.print(digitalRead(X_LIMIT_PLUS));
  Serial.print(" X-:"); Serial.print(digitalRead(X_LIMIT_MINUS));
  Serial.print(" Y+:"); Serial.print(digitalRead(Y_LIMIT_PLUS));
  Serial.print(" Y-:"); Serial.println(digitalRead(Y_LIMIT_MINUS));

  delay(100);
}
