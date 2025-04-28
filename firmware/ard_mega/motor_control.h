#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Servo.h>

// 모터 핀 정의
struct MotorPins {
  int signal;
  int in1;
  int in2;
  int limit_plus;
  int limit_minus;
};

// 모터 제어 클래스
class MotorControl {
public:
  MotorControl(const MotorPins& x_pins, const MotorPins& y_pins, const MotorPins& z_pins, int gripper_pin);
  void setup();
  void moveX(int direction);
  void moveY(int direction);
  void moveZ(int direction);
  void setGripper(int angle);
  void executeGripperSequence();
  void returnToHome();

private:
  MotorPins x_motor;
  MotorPins y_motor;
  MotorPins z_motor;
  Servo gripperServo;
  int gripperPin;

  void moveMotor(const MotorPins& motor, int direction);
  bool checkLimit(const MotorPins& motor, int direction);
};

#endif
