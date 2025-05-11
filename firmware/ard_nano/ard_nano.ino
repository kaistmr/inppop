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
const int COMM_RECEIVE = 3; // Mega로부터 신호 수신

bool coinInserted = false;

int gamestate = 0;

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

void playmusicnum(int filenum, int musicnum){
  Serial.print("play music : ");
  data[3] = 0x0F;      //command for play music [DL, 0~255] from file [DH, 0~10]
  data[5] = filenum;      //DH, file select
  data[6] = musicnum;  //DL, music select in file
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

void coin_ISR(){
  if(!gamestate)
  {
    playmusicnum(1, random(1,133));
  }
  gamestate++;
  digitalWrite(COMM_SEND, HIGH);
  delayMicroseconds(1000);
  digitalWrite(COMM_SEND, LOW);
}

void coinused_ISR()
{
  gamestate--;
  if(!gamestate)
  {
    playmusicnum(2, random(1,83));
  }
}

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  mySerial.begin(9600);
  Serial.println("System Rebooting...");

  attachInterrupt(digitalPinToInterrupt(DISTANCE_SENSOR_PIN), coin_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(COMM_RECEIVE), coinused_ISR, RISING);

  pinMode(COMM_SEND, OUTPUT);
  pinMode(COMM_RECEIVE, INPUT);
  digitalWrite(COMM_SEND, LOW);

  // 동전 감지 핀 설정
  pinMode(DISTANCE_SENSOR_PIN, INPUT);

  reset();
  delay(1000);
  volumeset(1);
}

int rxpoint = 0;

void loop() {
  if(gamestate)
  {
    if(mySerial.available())
    {
      rxbuffer[rxpoint++] = mySerial.read();
      if(rxpoint == 11)
      {
        rxpoint = 1;
        decodeRX();
        delay(500);
        playmusicnum(1, random(1,133));
      }
    }
  }
  else
  {
    if(mySerial.available())
    {
      rxbuffer[rxpoint++] = mySerial.read();
      if(rxpoint == 11)
      {
        rxpoint = 1;
        decodeRX();
        delay(500);
        playmusicnum(2, random(1,83));
      }
    }
  }
  Serial.print(digitalRead(COMM_SEND));
  Serial.print(digitalRead(COMM_RECEIVE));
  Serial.print(gamestate);
  Serial.println(digitalRead(DISTANCE_SENSOR_PIN));
}
