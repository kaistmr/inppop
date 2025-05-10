#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 12);
char data[10] = {0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
//               SOF  VI   DL   CMD  ACK  DH   DL   CHL  CHH  EOF
// CMD is command
// DH DL is different for command
// CHL CHH is checksum low / high

char decode[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char decoded[20]={};
char rxbuffer[11];;

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

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
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
    rxbuffer[rxpoint++] = mySerial.read();
    if(rxpoint == 11){
      rxpoint = 0;
      decodeRX();
    }
  }
}
