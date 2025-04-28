#include "mp3_player.h"

MP3Player::MP3Player(int rx_pin, int tx_pin)
  : mySerial(rx_pin, tx_pin) {}

void MP3Player::setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  // 모듈 초기화
  reset();
  delay(1000);

  // 볼륨 설정
  setVolume(10);
  delay(1000);
}

void MP3Player::buildChecksum() {
  uint16_t sum = 0;
  for (uint8_t i = 1; i <= 6; ++i)
    sum += (uint8_t)data[i];
  uint16_t chk = 0xFFFF - sum + 1;
  data[7] = chk >> 8;
  data[8] = chk & 0xFF;
}

void MP3Player::sendCommand() {
  for(int i=0; i<10; i++) {
    mySerial.print(data[i]);
  }
}

void MP3Player::playMusicNum(int folder, int musicNum) {
  data[3] = 0x0F;      // command for play music
  data[5] = folder;    // folder number
  data[6] = musicNum;  // music number in folder
  buildChecksum();
  sendCommand();
}

void MP3Player::playWaitingMusic() {
  int randomTrack = random(1, 10);  // 폴더 1에 10개의 곡이 있다고 가정
  playMusicNum(WAITING_FOLDER, randomTrack);
}

void MP3Player::playGameMusic() {
  playMusicNum(GAME_FOLDER, 1);  // 게임 시작 음악
}

void MP3Player::setVolume(int volume) {
  data[3] = 0x06;    // command for volume set
  data[5] = 0x00;    // DH
  data[6] = volume;  // DL, volume value
  buildChecksum();
  sendCommand();
}

void MP3Player::reset() {
  data[3] = 0x0C;    // command for reset
  data[5] = 0x00;    // DH
  data[6] = 0x00;    // DL
  buildChecksum();
  sendCommand();
}
