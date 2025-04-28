#include "motor_control.h"

MotorControl::MotorControl(const MotorPins& x_pins, const MotorPins& y_pins, const MotorPins& z_pins, int gripper_pin)
  : x_motor(x_pins), y_motor(y_pins), z_motor(z_pins), gripperPin(gripper_pin) {}

void MotorControl::setup() {
  // X축 모터 설정
  pinMode(x_motor.signal, OUTPUT);
  pinMode(x_motor.in1, OUTPUT);
  pinMode(x_motor.in2, OUTPUT);
  pinMode(x_motor.limit_plus, INPUT_PULLUP);
  pinMode(x_motor.limit_minus, INPUT_PULLUP);

  // Y축 모터 설정
  pinMode(y_motor.signal, OUTPUT);
  pinMode(y_motor.in1, OUTPUT);
  pinMode(y_motor.in2, OUTPUT);
  pinMode(y_motor.limit_plus, INPUT_PULLUP);
  pinMode(y_motor.limit_minus, INPUT_PULLUP);

  // Z축 모터 설정
  pinMode(z_motor.signal, OUTPUT);
  pinMode(z_motor.in1, OUTPUT);
  pinMode(z_motor.in2, OUTPUT);

  // 그리퍼 서보모터 설정
  gripperServo.attach(gripperPin);
}

void MotorControl::moveX(int direction) {
  moveMotor(x_motor, direction);
}

void MotorControl::moveY(int direction) {
  moveMotor(y_motor, direction);
}

void MotorControl::moveZ(int direction) {
  moveMotor(z_motor, direction);
}

void MotorControl::setGripper(int angle) {
  gripperServo.write(angle);
}

void MotorControl::moveMotor(const MotorPins& motor, int direction) {
  if (!checkLimit(motor, direction)) return;

  if (direction > 0) {
    digitalWrite(motor.in1, HIGH);
    digitalWrite(motor.in2, LOW);
  } else if (direction < 0) {
    digitalWrite(motor.in1, LOW);
    digitalWrite(motor.in2, HIGH);
  } else {
    digitalWrite(motor.in1, LOW);
    digitalWrite(motor.in2, LOW);
  }
  analogWrite(motor.signal, abs(direction) * 255);
}

bool MotorControl::checkLimit(const MotorPins& motor, int direction) {
  if (direction > 0 && digitalRead(motor.limit_plus) == LOW) return false;
  if (direction < 0 && digitalRead(motor.limit_minus) == LOW) return false;
  return true;
}

void MotorControl::executeGripperSequence() {
  // Z축 내리기
  moveZ(1);
  delay(2000);
  moveZ(0);

  // 그리퍼 작동
  setGripper(180);
  delay(1000);
  setGripper(0);
  delay(1000);

  // Z축 올리기
  moveZ(-1);
  delay(2000);
  moveZ(0);

  // 원점 이동
  returnToHome();

  // 그리퍼 펼치기
  setGripper(180);
}

void MotorControl::returnToHome() {
  // X축 원점 이동
  while (digitalRead(x_motor.limit_minus) == HIGH) {
    moveX(-1);
  }
  moveX(0);

  // Y축 원점 이동
  while (digitalRead(y_motor.limit_minus) == HIGH) {
    moveY(-1);
  }
  moveY(0);
}
