#include <Servo.h>

// X축 모터 핀
const int X_MOTOR_SIGNAL = 40;
const int X_MOTOR_IN1 = 36;
const int X_MOTOR_IN2 = 38;

// Y축 모터 핀
const int Y_MOTOR_SIGNAL = 41;
const int Y_MOTOR_IN1 = 37;
const int Y_MOTOR_IN2 = 39;

// Z축 모터 핀
const int Z_MOTOR_SIGNAL = 46;
const int Z_MOTOR_IN1 = 42;
const int Z_MOTOR_IN2 = 44;

// 그리퍼 서보모터 핀
const int GRIPPER_SERVO = 34;
Servo gripperServo;

void setup() {
  Serial.begin(9600);

  // X축 모터 설정
  pinMode(X_MOTOR_SIGNAL, OUTPUT);
  pinMode(X_MOTOR_IN1, OUTPUT);
  pinMode(X_MOTOR_IN2, OUTPUT);

  // Y축 모터 설정
  pinMode(Y_MOTOR_SIGNAL, OUTPUT);
  pinMode(Y_MOTOR_IN1, OUTPUT);
  pinMode(Y_MOTOR_IN2, OUTPUT);

  // Z축 모터 설정
  pinMode(Z_MOTOR_SIGNAL, OUTPUT);
  pinMode(Z_MOTOR_IN1, OUTPUT);
  pinMode(Z_MOTOR_IN2, OUTPUT);

  // 그리퍼 서보모터 설정
  gripperServo.attach(GRIPPER_SERVO);

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

void moveMotor(int signal, int in1, int in2, int direction) {
  if (direction > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (direction < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(signal, abs(direction) * 255);
}

void stopAllMotors() {
  moveMotor(X_MOTOR_SIGNAL, X_MOTOR_IN1, X_MOTOR_IN2, 0);
  moveMotor(Y_MOTOR_SIGNAL, Y_MOTOR_IN1, Y_MOTOR_IN2, 0);
  moveMotor(Z_MOTOR_SIGNAL, Z_MOTOR_IN1, Z_MOTOR_IN2, 0);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch(cmd) {
      case 'x':
        moveMotor(X_MOTOR_SIGNAL, X_MOTOR_IN1, X_MOTOR_IN2, 1);
        Serial.println("X축 + 방향 이동");
        break;

      case 'X':
        moveMotor(X_MOTOR_SIGNAL, X_MOTOR_IN1, X_MOTOR_IN2, -1);
        Serial.println("X축 - 방향 이동");
        break;

      case 'y':
        moveMotor(Y_MOTOR_SIGNAL, Y_MOTOR_IN1, Y_MOTOR_IN2, 1);
        Serial.println("Y축 + 방향 이동");
        break;

      case 'Y':
        moveMotor(Y_MOTOR_SIGNAL, Y_MOTOR_IN1, Y_MOTOR_IN2, -1);
        Serial.println("Y축 - 방향 이동");
        break;

      case 'z':
        moveMotor(Z_MOTOR_SIGNAL, Z_MOTOR_IN1, Z_MOTOR_IN2, 1);
        Serial.println("Z축 + 방향 이동");
        break;

      case 'Z':
        moveMotor(Z_MOTOR_SIGNAL, Z_MOTOR_IN1, Z_MOTOR_IN2, -1);
        Serial.println("Z축 - 방향 이동");
        break;

      case 'g':
        gripperServo.write(180);
        Serial.println("그리퍼 열기");
        break;

      case 'G':
        gripperServo.write(0);
        Serial.println("그리퍼 닫기");
        break;

      case 's':
        stopAllMotors();
        Serial.println("모든 모터 정지");
        break;
    }
  }
}
