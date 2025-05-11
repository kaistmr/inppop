/*
// nano와의 통신핀 설정
const int NANO_RECEIVE = 3;
const int NANO_SEND = 5;

// flag 설정
int coin=0;
int stop_motor_z;
int gohome;
int grip;

// interrupt 관련 함수
void gamestart_ISP()
{
  coin++;
}

void gripper_ISP()
{
  // 모터 전부 멈추는 코드
  grip = 1;
}

// joystick 및 limitswitch 가져오기
// 1이면 움직이는 상태
// 0이면 움직이면 안되는 상태

bool xp = ((!(PINA & B00000010) >> 1) & (PINH & B01000000) >> 6);
bool xm = ((!(PINA & B00001000) >> 3) & (PINB & B00100000) >> 5);
bool yp = ((!(PINA & B00100000) >> 5) & (PINJ & B00000001));
bool ym = ((!(PINA & B10000000) >> 7) & (PINH & B00000001));
bool zl = (PIND & B00000100) >> 2;


void setup()
{
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(3), gamestart_ISP, RISING);
  attachInterrupt(digitalPinToInterrupt(2), gripper_ISP, FALLING);
  pinMode(3, INPUT);
  pinMode(9, INPUT_PULLUP); //limit_xp
  pinMode(11, INPUT_PULLUP); //limit_xm
  pinMode(15, INPUT_PULLUP); //limit_yp
  pinMode(17, INPUT_PULLUP); //limit_ym
  pinMode(19, INPUT_PULLUP); //limit_z
  pinMode(23, INPUT_PULLUP); //joy_xp
  pinMode(25, INPUT_PULLUP); //joy_xm
  pinMode(27, INPUT_PULLUP); //joy_yp
  pinMode(29, INPUT_PULLUP); //joy_ym
  pinMode(31, INPUT_PULLUP); //push = D2

  pinMode(NANO_RECEIVE, INPUT);
  pinMode(NANO_SEND, OUTPUT);
  digitalWrite(NANO_SEND, LOW);
}

void loop()
{
  while(coin)
  {
    if(grip)
    {
      // 그리퍼 열기
      // z축 내리기
      // 그리퍼 닫기
      // z축 리밋 if 문 돌리면서 올리기
      gohome = 1;
      coin--;
    }
    if(gohome)
    {
      // 좌측 상단으로 움직이기
      // 그리퍼 열기
      // 그리퍼 닫기
      gohome = 0;
    }
    Serial.print(coin);
    Serial.print(", ");
    Serial.print(grip);
    Serial.print(", ");
    Serial.print(NANO_RECEIVE);
    Serial.print(", ");
    Serial.print(NANO_SEND);
    Serial.print(", ");
    Serial.print(xp);
    Serial.print(", ");
    Serial.print(xm);
    Serial.print(", ");
    Serial.print(yp);
    Serial.print(", ");
    Serial.print(ym);
    Serial.print(", ");
    Serial.println(zl);
  }
}
*/
