#include <Servo.h>

const int GRIP_PIN = 35;
const unsigned long MOTOR_TIMEOUT = 1000;  // 모터 타임아웃 (ms)
const int GRIP_OPEN = 2000; // 그리퍼 열림 값
const int GRIP_CLOSE = 1000; // 그리퍼 닫는 값
const int GRIP_STOP = 1500; // 그리퍼 닫는 값

Servo grip;

void setup() {
  // put your setup code here, to run once:
  grip.attach(GRIP_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  grip.writeMicroseconds(GRIP_OPEN); // open
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_STOP);
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_CLOSE); // close
  delay(MOTOR_TIMEOUT);
  grip.writeMicroseconds(GRIP_STOP);
  delay(MOTOR_TIMEOUT);

}
