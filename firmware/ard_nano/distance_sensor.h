#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
public:
  DistanceSensor(int sensor_pin, int trig_pin);
  void setup();
  int readDistance();
  bool isCoinInserted(int current_distance, int last_distance);

private:
  int sensorPin;
  int trigPin;
  const int COIN_THRESHOLD = 10;  // 동전 감지 임계값
};

#endif
