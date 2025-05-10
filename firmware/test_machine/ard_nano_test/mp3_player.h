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
  uint8_t data[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};  // 명령어 데이터 버퍼

  // DFPlayer Mini 명령어
  const uint8_t CMD_PLAY = 0x0F;    // 재생
  const uint8_t CMD_VOLUME = 0x06;  // 볼륨 설정
  const uint8_t CMD_RESET = 0x0C;   // 리셋

  // 음악 폴더
  const uint8_t WAIT_FOLDER = 1;    // 대기 상태 음악 폴더
  const uint8_t GAME_FOLDER = 2;    // 게임 진행 음악 폴더

  // 볼륨 설정
  const uint8_t VOLUME = 20;        // 볼륨 (0-30)

  void buildChecksum();
  void sendCommand();
  void playMusicNum(int folder, int musicNum);
  void setVolume(int volume);
  void reset();

public:
  MP3Player(int rxPin, int txPin);
  void setup();
  void playWaitingMusic();
  void playGameMusic();
  void stop();
};

#endif
