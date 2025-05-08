#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
private:
  int sensorPin;
  int trigPin;
  const int DISTANCE_THRESHOLD = 10;  // 거리 변화 감지 임계값 (mm)

public:
  DistanceSensor(int sensorPin, int trigPin) : sensorPin(sensorPin), trigPin(trigPin) {}

  void setup() {
    pinMode(sensorPin, INPUT);
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
  }

  int readDistance() {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 아날로그 값을 거리(mm)로 변환
    int rawValue = analogRead(sensorPin);
    int distance = map(rawValue, 0, 1023, 0, 300);  // 0-300mm 범위로 매핑
    return distance;
  }

  bool isCoinInserted(int currentDistance, int lastDistance) {
    // 거리가 급격히 감소했다가 다시 증가하는 경우 동전 투입으로 판단
    return (lastDistance - currentDistance > DISTANCE_THRESHOLD);
  }
};

#endif
