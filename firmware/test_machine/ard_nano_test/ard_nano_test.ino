#include "distance_sensor.h"
#include "mp3_player.h"

// 거리 센서 핀
const int DISTANCE_TRIG_PIN = 2;

// MP3 모듈 핀
const int MP3_RX = 6;
const int MP3_TX = 12;

// 통신 핀
const int COMM_SEND = 8;    // Mega로 신호 송신
const int COMM_RECEIVE = 7; // Mega로부터 신호 수신

// 객체 생성
DistanceSensor distanceSensor(DISTANCE_SENSOR_PIN, DISTANCE_TRIG_PIN);
MP3Player mp3Player(MP3_RX, MP3_TX);

// 테스트 상태
enum TestState {
  DISTANCE_TEST,    // 거리 센서 테스트
  MP3_TEST,         // MP3 플레이어 테스트
  COMM_TEST         // 통신 테스트
};

// MP3 테스트 상태
enum MP3TestState {
  WAITING_MUSIC,    // 대기 음악 재생
  GAME_MUSIC,       // 게임 음악 재생
  VOLUME_TEST,      // 볼륨 테스트
  RESET_TEST        // 리셋 테스트
};

// 전역 변수
TestState currentState = DISTANCE_TEST;
MP3TestState currentMP3State = WAITING_MUSIC;
unsigned long lastStateChange = 0;
unsigned long lastPrintTime = 0;
unsigned long lastCommTime = 0;
bool lastCommState = false;

// 상수
const unsigned long STATE_DURATION = 10000;    // 각 테스트 10초 유지
const unsigned long PRINT_INTERVAL = 500;      // 0.5초마다 출력
const unsigned long COMM_DEBOUNCE = 50;        // 디바운스 시간 (ms)

void setup() {
  Serial.begin(115200);
  Serial.println("아두이노 나노 통합 테스트 시작...");

  // 거리 센서 초기화
  distanceSensor.setup();
  pinMode(DISTANCE_TRIG_PIN, INPUT);

  // MP3 플레이어 초기화
  mp3Player.setup();

  // 통신 핀 설정
  pinMode(COMM_SEND, OUTPUT);
  pinMode(COMM_RECEIVE, INPUT);
  digitalWrite(COMM_SEND, LOW);

  Serial.println("초기화 완료!");
  Serial.println("테스트 시작...");
}

void loop() {
  // 10초마다 테스트 상태 변경
  if (millis() - lastStateChange >= STATE_DURATION) {
    switch (currentState) {
      case DISTANCE_TEST:
        Serial.println("\n=== MP3 플레이어 테스트 시작 ===");
        currentState = MP3_TEST;
        currentMP3State = WAITING_MUSIC;
        break;

      case MP3_TEST:
        Serial.println("\n=== 통신 테스트 시작 ===");
        currentState = COMM_TEST;
        break;

      case COMM_TEST:
        Serial.println("\n=== 거리 센서 테스트 시작 ===");
        currentState = DISTANCE_TEST;
        break;
    }
    lastStateChange = millis();
  }

  // 현재 테스트 상태에 따른 동작
  switch (currentState) {
    case DISTANCE_TEST:
      distanceSensorTest();
      break;

    case MP3_TEST:
      mp3PlayerTest();
      break;

    case COMM_TEST:
      communicationTest();
      break;
  }

  delay(10);  // 10ms 딜레이
}

// 거리 센서 테스트
void distanceSensorTest() {
  bool coinDetected = !digitalRead(DISTANCE_TRIG_PIN);

  // 동전 투입 감지
  if (coinDetected) {
    Serial.println("동전 투입 감지!");
  }

  // 0.5초마다 상태 출력
  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    Serial.print("동전 감지 상태: ");
    Serial.println(coinDetected ? "감지됨" : "감지되지 않음");
    lastPrintTime = millis();
  }
}

// MP3 플레이어 테스트
void mp3PlayerTest() {
  // 2.5초마다 MP3 테스트 상태 변경
  if (millis() - lastStateChange >= STATE_DURATION/4) {
    switch (currentMP3State) {
      case WAITING_MUSIC:
        Serial.println("게임 음악 재생 테스트...");
        mp3Player.playGameMusic();
        currentMP3State = GAME_MUSIC;
        break;

      case GAME_MUSIC:
        Serial.println("대기 음악 재생 테스트...");
        mp3Player.playWaitingMusic();
        currentMP3State = WAITING_MUSIC;
        break;
    }
    lastStateChange = millis();
  }
}

// 통신 테스트
void communicationTest() {
  bool currentCommState = digitalRead(COMM_RECEIVE);
  unsigned long currentTime = millis();

  // 상태 변화 감지
  if (currentCommState != lastCommState) {
    if ((currentTime - lastCommTime) > COMM_DEBOUNCE) {
      Serial.print("통신 상태 변화: ");
      Serial.println(currentCommState ? "HIGH" : "LOW");

      // 상태에 따른 응답
      if (currentCommState == HIGH) {
        digitalWrite(COMM_SEND, HIGH);
        Serial.println("응답 신호 송신: HIGH");
      } else {
        digitalWrite(COMM_SEND, LOW);
        Serial.println("응답 신호 송신: LOW");
      }

      lastCommTime = currentTime;
      lastCommState = currentCommState;
    }
  }
}
