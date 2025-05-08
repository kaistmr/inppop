/*
코드 구조
1. 2개의 상태가 존재: 동전 대기 / 게임 진행
2. 동전 대기:
- mp3 모듈에서 1번 폴더 속 노래를 랜덤하게 반복 재생
- distance 센서 인식 중
- 신호를 송신 받는 8번 핀이 LOW로 설정
3. 동전 대기 -> 게임 진행
- distance 센서 인식(D2) ; 인터럽트
- D8을 HIGH로 설정
- 만약 D7에 값이 HIGH가 들어오면 게임 진행 시퀀스로 돌입
4. 게임 진행:
- mp3 모듈에서 2번 폴더 속 노래를 랜덤하게 반복 재생
- 지속적으로 D7 핀에서 신호 수신(defalt: HIGH)
5. 게임 진행 -> 동전 대기
- D7 핀에 LOW 신호가 수신되면 D8 핀의 값도 LOW로 설정
- 다시 동전 대기 시퀀스 돌입
*/

#include "distance_sensor.h"
#include "mp3_player.h"

// 거리 센서 핀
const int DISTANCE_SENSOR_PIN = A7;
const int DISTANCE_TRIG_PIN = 2;

// MP3 모듈 핀
const int MP3_RX = 6;
const int MP3_TX = 12;

// 통신 핀
const int COMM_SEND = 8;    // Mega로 신호 송신
const int COMM_RECEIVE = 7; // Mega로부터 신호 수신

// 게임 상태
enum GameState {
  WAITING,    // 대기 상태
  PLAYING,    // 게임 진행 중
  FINISHED    // 게임 종료
};

// 거리 센서 상수
const int DISTANCE_THRESHOLD = 10;  // 거리 변화 감지 임계값 (mm)
const int COIN_DELAY = 1000;       // 동전 투입 후 대기 시간 (ms)

// 통신 관련 상수
const unsigned long COMM_DEBOUNCE = 50;  // 디바운스 시간 (ms)
unsigned long lastCommTime = 0;          // 마지막 통신 시간
bool lastCommState = false;              // 마지막 통신 상태

// 객체 생성
DistanceSensor distanceSensor(DISTANCE_SENSOR_PIN, DISTANCE_TRIG_PIN);
MP3Player mp3Player(MP3_RX, MP3_TX);

GameState currentState = WAITING;
int lastDistance = 0;
bool coinInserted = false;

void setup() {
  Serial.begin(115200);
  Serial.println("게임 시스템 초기화 중...");

  // 거리 센서 및 MP3 플레이어 초기화
  distanceSensor.setup();
  mp3Player.setup();

  // 통신 핀 설정
  pinMode(COMM_SEND, OUTPUT);
  pinMode(COMM_RECEIVE, INPUT);
  digitalWrite(COMM_SEND, LOW);

  // 대기 상태 음악 재생 시작
  mp3Player.playWaitingMusic();

  Serial.println("초기화 완료!");
  Serial.println("동전 투입 대기 중...");
}

void loop() {
  // 거리 센서 값 읽기
  int currentDistance = distanceSensor.readDistance();

  // 통신 상태 확인 (디바운싱 적용)
  bool currentCommState = digitalRead(COMM_RECEIVE);
  unsigned long currentTime = millis();

  if (currentCommState != lastCommState) {
    lastCommTime = currentTime;
  }

  switch (currentState) {
    case WAITING:
      // 동전 투입 감지
      if (!coinInserted && distanceSensor.isCoinInserted(currentDistance, lastDistance)) {
        coinInserted = true;
        Serial.println("동전 투입 감지!");
        delay(COIN_DELAY); // 동전이 완전히 들어갈 때까지 대기
        startGame();
      }
      lastDistance = currentDistance;
      break;

    case PLAYING:
      // Mega로부터 게임 종료 신호 확인 (디바운싱 적용)
      if ((currentTime - lastCommTime) > COMM_DEBOUNCE) {
        if (currentCommState == LOW) {
          currentState = FINISHED;
          digitalWrite(COMM_SEND, LOW);
          coinInserted = false;
          Serial.println("게임 종료 신호 수신!");
        }
      }
      break;

    case FINISHED:
      // Mega로부터 새 게임 시작 신호 확인
      if ((currentTime - lastCommTime) > COMM_DEBOUNCE) {
        if (currentCommState == LOW) {
          currentState = WAITING;
          mp3Player.playWaitingMusic();
          Serial.println("새 게임 준비 완료!");
        }
      }
      break;
  }

  lastCommState = currentCommState;
}

void startGame() {
  currentState = PLAYING;
  digitalWrite(COMM_SEND, HIGH);
  mp3Player.playGameMusic();
  Serial.println("게임 시작!");
}
