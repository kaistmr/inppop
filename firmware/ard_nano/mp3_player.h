#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <SoftwareSerial.h>

class MP3Player {
public:
  MP3Player(int rx_pin, int tx_pin);
  void setup();
  void playWaitingMusic();
  void playGameMusic();
  void setVolume(int volume);
  void reset();

private:
  SoftwareSerial mySerial;
  char data[10] = {0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
  const int WAITING_FOLDER = 1;
  const int GAME_FOLDER = 2;

  void buildChecksum();
  void sendCommand();
  void playMusicNum(int folder, int musicNum);
};

#endif
