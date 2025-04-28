// 거리 센서 핀
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

void setup() {
  Serial.begin(9600);

  // 거리 센서 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("거리 센서 테스트 시작");
}

float getDistance() {
  // 초음파 발사
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 초음파 수신 시간 측정
  long duration = pulseIn(ECHO_PIN, HIGH);

  // 거리 계산 (cm)
  float distance = duration * 0.034 / 2;

  return distance;
}

void loop() {
  float distance = getDistance();

  Serial.print("거리: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100);
}
