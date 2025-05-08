#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class MP3Player {
private:
  SoftwareSerial mySoftwareSerial;
  DFRobotDFPlayerMini myDFPlayer;
  int rxPin;
  int txPin;

  // DFPlayer Mini 명령어
  const uint8_t CMD_PLAY = 0x0F;    // 재생
  const uint8_t CMD_VOLUME = 0x06;  // 볼륨 설정
  const uint8_t CMD_RESET = 0x0C;   // 리셋

  // 음악 폴더
  const uint8_t WAIT_FOLDER = 1;    // 대기 상태 음악 폴더
  const uint8_t GAME_FOLDER = 2;    // 게임 진행 음악 폴더

  // 볼륨 설정
  const uint8_t VOLUME = 20;        // 볼륨 (0-30)

public:
  MP3Player(int rxPin, int txPin) : rxPin(rxPin), txPin(txPin), mySoftwareSerial(rxPin, txPin) {}

  void setup() {
    mySoftwareSerial.begin(9600);

    if (!myDFPlayer.begin(mySoftwareSerial)) {
      Serial.println("DFPlayer Mini 초기화 실패!");
      while(true);
    }

    // 볼륨 설정
    myDFPlayer.volume(VOLUME);

    Serial.println("DFPlayer Mini 초기화 완료!");
  }

  void playWaitingMusic() {
    // 대기 상태 음악 재생 (폴더 1의 랜덤 재생)
    myDFPlayer.playFolder(WAIT_FOLDER, 1);
    myDFPlayer.enableLoop();  // 반복 재생
  }

  void playGameMusic() {
    // 게임 진행 음악 재생 (폴더 2의 순차 재생)
    myDFPlayer.playFolder(GAME_FOLDER, 1);
    myDFPlayer.enableLoop();  // 반복 재생
  }

  void stop() {
    myDFPlayer.stop();
  }
};

#endif
