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

#include <SoftwareSerial.h>

const int DISTANCE_SENSOR_PIN = 2;

SoftwareSerial mySerial(6, 12);
char data[10] = {0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
//               SOF  VI   DL   CMD  ACK  DH   DL   CHL  CHH  EOF
// CMD is command
// DH DL is different for command
// CHL CHH is checksum low / high

char decode[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char decoded[20]={};
char rxbuffer[11];;

// 통신 핀
const int COMM_SEND = 8;    // Mega로 신호 송신
const int COMM_RECEIVE = 7; // Mega로부터 신호 수신

// 게임 상태
enum GameState {
  WAITING,    // 대기 상태
  PLAYING,    // 게임 진행 중
  FINISHED    // 게임 종료
};

GameState currentState = WAITING;

bool coinInserted = false;

void decodefunction(){
  for(int i=0;i<20;i++){
    if(i%2 == 0){
      decoded[i] = decode[(data[(int)(i/2)] & 0xF0) >> 4];
      Serial.print(" ");
    }else{
      decoded[i] = decode[(data[(int)(i/2)] & 0x0F)];
    }
    Serial.print(decoded[i]);
  }
  Serial.println("");
}

void decodeRX(){
  Serial.print("from module : ");
  for(int i=2;i<22;i++){
    if(i%2 == 0){
      decoded[i] = decode[(rxbuffer[(int)(i/2)] & 0xF0) >> 4];
      Serial.print(" ");
    }else{
      decoded[i] = decode[(rxbuffer[(int)(i/2)] & 0x0F)];
    }
    Serial.print(decoded[i]);
  }
  Serial.println("");
}

void buildChecksum(){
  uint16_t sum = 0;

  for (uint8_t i = 1; i <= 6; ++i)
    sum += (uint8_t)data[i];

  uint16_t chk = 0xFFFF - sum + 1;

  data[7] = chk >> 8;
  data[8] = chk & 0xFF;
}

void playmusicnum(int musicnum){
  Serial.print("play music : ");
  data[3] = 0x0F;      //command for play music [DL, 0~255] from file [DH, 0~10]
  data[5] = 0x01;      //DH, file select
  data[6] = musicnum;  //DL, music select in file
  buildChecksum();
  decodefunction();
  for(int i=0;i<10;i++){
    mySerial.print(data[i]);
  }
}

void folderset(){
  Serial.print("folder set : ");
  data[3] = 0x0F;      //command for folder repeat
  data[5] = 0x01;
  data[6] = 0x01;
  buildChecksum();
  decodefunction();
  for(int i=0;i<10;i++){
    mySerial.print(data[i]);
  }
}

void folderrepeat(){
  Serial.print("folder repeat : ");
  data[3] = 0x08;
  data[5] = 0x00;
  data[6] = 0x01;
  buildChecksum();
  decodefunction();
  for(int i=0;i<10;i++){
    mySerial.print(data[i]);
  }
}

void playback(){
  Serial.print("playback : ");
  data[3] = 0x0D;
  data[5] = 0x00;
  data[6] = 0x00;
  buildChecksum();
  decodefunction();
  for(int i=0;i<10;i++){
    mySerial.print(data[i]);
  }
}

void reset(){
  Serial.print("module reset : ");
  data[3] = 0x0C;
  data[5] = 0x00;
  data[6] = 0x00;
  buildChecksum();
  decodefunction();
  for(int i=0;i<10;i++){
    mySerial.print(data[i]);
  }
}

void volumeset(int volume){
  Serial.print("volume set : ");
  data[3] = 0x06;    //command for folder repeat
  data[5] = 0x00;    //DH, file select
  data[6] = volume;  //DL, music select in file
  buildChecksum();
  decodefunction();
  for(int i=0;i<10;i++){
    mySerial.print(data[i]);
  }
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("System Rebooting...");

  pinMode(COMM_SEND, OUTPUT);
  pinMode(COMM_RECEIVE, INPUT);
  digitalWrite(COMM_SEND, LOW);

  // 동전 감지 핀 설정
  pinMode(DISTANCE_SENSOR_PIN, INPUT);

  reset();
  delay(1000);
  volumeset(10);
  delay(1000);
  //folderset();
  //delay(1000);
  playmusicnum(1);
  delay(1000);
  //folderrepeat();
  //delay(1000);
  //playback();
}

int rxpoint = 0;

void loop() {
  if(mySerial.available()){
    Serial.print("Distance:");
    Serial.println(!digitalRead(DISTANCE_SENSOR_PIN));
    /*
    Serial.print("COMR:");
    Serial.println(digitalRead(COMM_RECEIVE));
    digitalWrite(COMM_SEND, HIGH);
    Serial.print("COMS:");
    Serial.println(digitalRead(COMM_SEND));
    digitalWrite(COMM_SEND, LOW);
    Serial.print("COMS:");
    Serial.println(digitalRead(COMM_SEND));
    */
    rxbuffer[rxpoint++] = mySerial.read();
    if(rxpoint == 11){
      rxpoint = 0;
      decodeRX();
    }
  }
}
