/*
y축 모터 제외하고 잘 움직인다. 왜인지는 아직 판단 못함
/*

#include <Servo.h>

// X축 모터 핀
const int XS = 40;
const int X1 = 36;
const int X2 = 38;

// Y축 모터 핀
const int YS = 46;
const int Y1 = 42;
const int Y2 = 44;

// Z축 모터 핀
const int ZS = 41;
const int Z1 = 37;
const int Z2 = 39;

// 그리퍼 서보모터 핀
const int GRIPPER_SERVO = 34;
Servo grip;

void setup() {
  Serial.begin(9600);

  // X축 모터 설정
  pinMode(XS, OUTPUT);
  pinMode(X1, OUTPUT);
  pinMode(X2, OUTPUT);

  // Y축 모터 설정
  pinMode(YS, OUTPUT);
  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);

  // Z축 모터 설정
  pinMode(ZS, OUTPUT);
  pinMode(Z1, OUTPUT);
  pinMode(Z2, OUTPUT);

  // 그리퍼 서보모터 설정
  grip.attach(GRIPPER_SERVO);

  Serial.println("모터 테스트 시작");
  Serial.println("x: X축 + 방향");
  Serial.println("X: X축 - 방향");
  Serial.println("y: Y축 + 방향");
  Serial.println("Y: Y축 - 방향");
  Serial.println("z: Z축 + 방향");
  Serial.println("Z: Z축 - 방향");
  Serial.println("g: 그리퍼 열기");
  Serial.println("G: 그리퍼 닫기");
  Serial.println("s: 모든 모터 정지");
}

void moveMotor(int MS, int M1, int M2, int direction) {
  if (direction > 0) {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
  } else if (direction < 0) {
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
  } else {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
  }
  analogWrite(MS, abs(direction) * 255);
}

void stopAllMotors() {
  moveMotor(XS, X1, X2, 0);
  moveMotor(YS, Y1, Y2, 0);
  moveMotor(ZS, Z1, Z2, 0);
}

void loop() {

  /*
  if (Serial.available()) {
    char cmd = Serial.read();

    switch(cmd) {
      case 'x':
        moveMotor(XS, X1, X2, 1);
        Serial.println("X축 + 방향 이동");
        break;

      case 'X':
        moveMotor(XS, X1, X2, -1);
        Serial.println("X축 - 방향 이동");
        break;

      case 'y':
        moveMotor(YS, Y1, Y2, 1);
        Serial.println("Y축 + 방향 이동");
        break;

      case 'Y':
        moveMotor(YS, Y1, Y2, -1);
        Serial.println("Y축 - 방향 이동");
        break;

      case 'z':
        moveMotor(ZS, Z1, Z2, 1);
        Serial.println("Z축 + 방향 이동");
        break;

      case 'Z':
        moveMotor(ZS, Z1, Z2, -1);
        Serial.println("Z축 - 방향 이동");
        break;

      case 'g':
        grip.write(180);
        Serial.println("그리퍼 열기");
        break;

      case 'G':
        grip.write(0);
        Serial.println("그리퍼 닫기");
        break;

      case 's':
        stopAllMotors();
        Serial.println("모든 모터 정지");
        break;
    }
  } */
}
