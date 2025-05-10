#include "mp3_player.h"
#include <Arduino.h>

MP3Player::MP3Player(int rx_pin, int tx_pin)
  : rxPin(rx_pin), txPin(tx_pin), mySoftwareSerial(rx_pin, tx_pin) {}

void MP3Player::setup() {
  mySoftwareSerial.begin(9600);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer Mini 초기화 실패!");
    while(true);
  }

  // 볼륨 설정
  myDFPlayer.volume(VOLUME);

  Serial.println("DFPlayer Mini 초기화 완료!");
}

void MP3Player::buildChecksum() {
  uint16_t sum = 0;
  for (int i = 1; i < 7; i++) {
    sum += (uint8_t)data[i];
  }
  uint16_t chk = -sum;
  data[7] = chk >> 8;
  data[8] = chk & 0xFF;
}

void MP3Player::sendCommand() {
  for (int i = 0; i < 10; i++) {
    mySoftwareSerial.write(data[i]);
  }
}

void MP3Player::playMusicNum(int folder, int musicNum) {
  data[3] = 0x0F;      // command for play music
  data[5] = folder;    // folder number
  data[6] = musicNum;  // music number
  buildChecksum();
  sendCommand();
}

void MP3Player::playWaitingMusic() {
  // 대기 상태 음악 재생 (폴더 1의 랜덤 재생)
  int randomTrack = random(1, 10);  // 1-9 사이의 랜덤 트랙
  playMusicNum(WAIT_FOLDER, randomTrack);
  myDFPlayer.enableLoop();  // 반복 재생
}

void MP3Player::playGameMusic() {
  // 게임 진행 음악 재생 (폴더 2의 순차 재생)
  myDFPlayer.playFolder(GAME_FOLDER, 1);
  myDFPlayer.enableLoop();  // 반복 재생
}

void MP3Player::setVolume(int volume) {
  data[3] = 0x06;    // command for volume set
  data[6] = volume;  // volume value
  buildChecksum();
  sendCommand();
}

void MP3Player::reset() {
  data[3] = 0x0C;    // command for reset
  buildChecksum();
  sendCommand();
}

void MP3Player::stop() {
  myDFPlayer.stop();
}
