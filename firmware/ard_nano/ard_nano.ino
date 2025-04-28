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

// 객체 생성
DistanceSensor distanceSensor(DISTANCE_SENSOR_PIN, DISTANCE_TRIG_PIN);
MP3Player mp3Player(MP3_RX, MP3_TX);

GameState currentState = WAITING;
int lastDistance = 0;
bool coinInserted = false;

void setup() {
  // 거리 센서 및 MP3 플레이어 초기화
  distanceSensor.setup();
  mp3Player.setup();

  // 통신 핀 설정
  pinMode(COMM_SEND, OUTPUT);
  pinMode(COMM_RECEIVE, INPUT);
  digitalWrite(COMM_SEND, LOW);

  // 대기 상태 음악 재생 시작
  mp3Player.playWaitingMusic();
}

void loop() {
  // 거리 센서 값 읽기
  int currentDistance = distanceSensor.readDistance();

  switch (currentState) {
    case WAITING:
      // 동전 투입 감지
      if (!coinInserted && distanceSensor.isCoinInserted(currentDistance, lastDistance)) {
        coinInserted = true;
        delay(1000); // 동전이 완전히 들어갈 때까지 대기
        startGame();
      }
      lastDistance = currentDistance;
      break;

    case PLAYING:
      // Mega로부터 게임 종료 신호 확인
      if (digitalRead(COMM_RECEIVE) == LOW) {
        currentState = FINISHED;
        digitalWrite(COMM_SEND, LOW);
        coinInserted = false;
      }
      break;

    case FINISHED:
      if (digitalRead(COMM_RECEIVE) == LOW) {
        currentState = WAITING;
        mp3Player.playWaitingMusic();
      }
      break;
  }
}

void startGame() {
  currentState = PLAYING;
  digitalWrite(COMM_SEND, HIGH);
  mp3Player.playGameMusic();
}
