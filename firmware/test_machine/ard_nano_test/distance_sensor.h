#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
private:
  int sensorPin;
  int trigPin;
  const int DISTANCE_THRESHOLD = 10;  // 거리 변화 감지 임계값 (mm)

public:
  DistanceSensor(int sensorPin, int trigPin);
  void setup();
  int readDistance();
  bool isCoinInserted(int currentDistance, int lastDistance);
};

#endif
