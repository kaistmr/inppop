#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 12); // RX, TX
char data[10] = {0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};

void buildChecksum() {
  uint16_t sum = 0;
  for (uint8_t i = 1; i <= 6; ++i)
    sum += (uint8_t)data[i];
  uint16_t chk = 0xFFFF - sum + 1;
  data[7] = chk >> 8;
  data[8] = chk & 0xFF;
}

void sendCommand() {
  for(int i=0; i<10; i++) {
    mySerial.print(data[i]);
  }
}

void playMusic(int folder, int track) {
  data[3] = 0x0F;      // play music command
  data[5] = folder;    // folder number
  data[6] = track;     // track number
  buildChecksum();
  sendCommand();
}

void setVolume(int volume) {
  data[3] = 0x06;      // volume command
  data[5] = 0x00;
  data[6] = volume;
  buildChecksum();
  sendCommand();
}

void reset() {
  data[3] = 0x0C;      // reset command
  data[5] = 0x00;
  data[6] = 0x00;
  buildChecksum();
  sendCommand();
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("DFPlayer Mini 테스트 시작");

  // 모듈 초기화
  reset();
  delay(1000);

  // 볼륨 설정
  setVolume(10);
  delay(1000);

  Serial.println("테스트 준비 완료");
  Serial.println("1: 폴더 1의 첫 번째 곡 재생");
  Serial.println("2: 폴더 2의 첫 번째 곡 재생");
  Serial.println("v: 볼륨 증가");
  Serial.println("V: 볼륨 감소");
  Serial.println("r: 리셋");
}

int volume = 10;

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch(cmd) {
      case '1':
        playMusic(1, 1);
        Serial.println("폴더 1의 첫 번째 곡 재생");
        break;

      case '2':
        playMusic(2, 1);
        Serial.println("폴더 2의 첫 번째 곡 재생");
        break;

      case 'v':
        if (volume < 30) {
          volume++;
          setVolume(volume);
          Serial.print("볼륨 증가: ");
          Serial.println(volume);
        }
        break;

      case 'V':
        if (volume > 0) {
          volume--;
          setVolume(volume);
          Serial.print("볼륨 감소: ");
          Serial.println(volume);
        }
        break;

      case 'r':
        reset();
        Serial.println("모듈 리셋");
        break;
    }
  }
}
