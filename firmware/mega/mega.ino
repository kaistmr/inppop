#include <Servo.h>

const int X_MOTOR[3] = {41, 37, 39};
const int Y_MOTOR[3] = {40, 36, 38};
// const int Z_MOTOR[3] = {47, 43, 45};
const int Z_PIN = 47;
const int GRIP_PIN = 35;

// 조이스틱 핀
const int JX_P = 23;
const int JX_M = 25;
const int JY_P = 27;
const int JY_M = 29;
const int BTN = 31;

// 리밋 스위치 핀
const int LX_P = 21;
const int LX_M = 11;
const int LY_P = 15;
const int LY_M = 17;
const int LZ = 19;

// 통신 핀
const int COMM_OUT = 5;
const int COMM_IN = 2;

// 모터 제어
const int MOTOR_SPEED = 255;  // 모터 속도 (0-255)
const int GRIP_SPEED = 220;  // z축 모터 속도 (0-255)
const unsigned long MOTOR_TIMEOUT = 1000;  // 모터 타임아웃 (ms)
const int GRIP_OPEN = 1000; // 그리퍼 열림 값
const int GRIP_CLOSE = 2000; // 그리퍼 닫는 값
 const int GRIP_STOP = 1500; // 그리퍼 멈춤 
const int Z_DOWN = 1900;
const int Z_UP = 1000;
const int Z_STOP = 1450;
Servo grip;
Servo z_axis;

// flag 설정
int state = 0; //0: 대기, 1: 움직임
int gamecoin = 0;

void COIN_ISR()
{
  gamecoin++;
}

void MotorSetup(const int pins[3])
{
  pinMode(pins[0], OUTPUT);
  pinMode(pins[1], OUTPUT);
  pinMode(pins[2], OUTPUT);
  digitalWrite(pins[0], LOW);
}

void MotorMove(const int pins[3], int dir, int vel)
{
  if (dir > 0)
  {
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], LOW);
    analogWrite(pins[0], vel);
  } else if (dir < 0) {
    digitalWrite(pins[2], HIGH);
    digitalWrite(pins[1], LOW);
    analogWrite(pins[0], vel);
  } else {
    digitalWrite(pins[0], LOW);
  }
}

bool reachlimit(const int limit_pin)
{
  return !digitalRead(limit_pin);
}

void gohome(const int x_axis_motor[3], const int y_axis_motor[3], const int grip_pin, const int limit_x, const int limit_y)
{
  bool x_reach = false;
  bool y_reach = false;

  while(!x_reach || !y_reach)
  {
    x_reach = reachlimit(limit_x);
    y_reach = reachlimit(limit_y);
    !x_reach ? MotorMove(x_axis_motor, 1, MOTOR_SPEED) : MotorMove(x_axis_motor, 0, 0);
    !y_reach ? MotorMove(y_axis_motor, 1, MOTOR_SPEED) : MotorMove(y_axis_motor, 0, 0);
  }
  MotorMove(x_axis_motor, 0, 0);
  MotorMove(y_axis_motor, 0, 0);
  grip.writeMicroseconds(GRIP_OPEN); // open
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_STOP);
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_CLOSE); // close
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_STOP);
  delay(MOTOR_TIMEOUT);
}

void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(COMM_IN), COIN_ISR, RISING);

  MotorSetup(X_MOTOR);
  MotorSetup(Y_MOTOR);
  // MotorSetup(Z_MOTOR);
  grip.attach(GRIP_PIN);
  z_axis.attach(Z_PIN);

  pinMode(BTN, INPUT_PULLUP);
  pinMode(LX_P, INPUT_PULLUP);
  pinMode(LX_M, INPUT_PULLUP);
  pinMode(LY_P, INPUT_PULLUP);
  pinMode(LY_M, INPUT_PULLUP);
  pinMode(LZ, INPUT_PULLUP);
  pinMode(JX_P, INPUT_PULLUP);
  pinMode(JX_M, INPUT_PULLUP);
  pinMode(JY_P, INPUT_PULLUP);
  pinMode(JY_M, INPUT_PULLUP);

  pinMode(COMM_OUT, OUTPUT);
  pinMode(COMM_IN, INPUT);

  grip.writeMicroseconds(GRIP_OPEN); // open
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_STOP);
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_CLOSE); // close
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_STOP);
  delay(MOTOR_TIMEOUT);
  z_axis.writeMicroseconds(Z_STOP);
  delay(5000);
  gamecoin = 0;
}

void loop()
{
  Serial.print("gamecoin: ");
  Serial.print(gamecoin);
  Serial.print("  state: ");
  Serial.println(state);

  if(reachlimit(BTN))
  {
    MotorMove(X_MOTOR, 0, MOTOR_SPEED);
    MotorMove(Y_MOTOR, 0, MOTOR_SPEED);
  //  MotorMove(Z_MOTOR, -1, GRIP_SPEED);
    z_axis.writeMicroseconds(Z_STOP);
  //  MotorMove(Z_MOTOR, 0, 0);
    grip.writeMicroseconds(GRIP_OPEN);
    delay(MOTOR_TIMEOUT);
    grip.writeMicroseconds(GRIP_STOP);
    delay(MOTOR_TIMEOUT);
    z_axis.writeMicroseconds(Z_DOWN);
    delay(2000);
    z_axis.writeMicroseconds(Z_STOP);
    delay(500);
    grip.writeMicroseconds(GRIP_CLOSE);
    delay(MOTOR_TIMEOUT);
    grip.writeMicroseconds(GRIP_STOP);
    delay(MOTOR_TIMEOUT);
    z_axis.writeMicroseconds(Z_UP);
    delay(2000);
    /*
    while(!reachlimit(LZ))
    {
      MotorMove(Z_MOTOR, 1, GRIP_SPEED);
      delay(1000);
    } */
    z_axis.writeMicroseconds(Z_STOP);
    // MotorMove(Z_MOTOR, 0, 0);
    gohome(X_MOTOR, Y_MOTOR, GRIP_PIN, LX_M, LY_P);
    gamecoin--;
    digitalWrite(COMM_OUT, HIGH);
    delay(1000);
    digitalWrite(COMM_OUT, LOW);
  }
  if(!state)
  {
    gohome(X_MOTOR, Y_MOTOR, GRIP_PIN, LX_M, LY_P);
    MotorMove(X_MOTOR, 0, MOTOR_SPEED);
    MotorMove(Y_MOTOR, 0, MOTOR_SPEED);
    if(gamecoin > 0)
    {
      state = 1;
    }
  }
  if(state && (gamecoin > 0))
  {
    if(reachlimit(BTN)) return;

    int order_x = reachlimit(JX_P) - reachlimit(JX_M);
    int order_y = reachlimit(JY_P) - reachlimit(JY_M);

    if(reachlimit(LX_M) && reachlimit(JX_P)) order_x = 0;
    if(reachlimit(LX_P) && reachlimit(JX_M)) order_x = 0;
    if(reachlimit(LY_P) && reachlimit(JY_P)) order_y = 0;
    if(reachlimit(LY_M) && reachlimit(JY_M)) order_y = 0;

    MotorMove(X_MOTOR, order_x, MOTOR_SPEED);
    MotorMove(Y_MOTOR, order_y, MOTOR_SPEED);
  }
}
