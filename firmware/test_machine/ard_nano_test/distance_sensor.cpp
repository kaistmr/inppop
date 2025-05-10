#include "distance_sensor.h"
#include <Arduino.h>

DistanceSensor::DistanceSensor(int sensor_pin, int trig_pin)
  : sensorPin(sensor_pin), trigPin(trig_pin) {}

void DistanceSensor::setup() {
  pinMode(sensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
}

int DistanceSensor::readDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 아날로그 값을 거리(mm)로 변환
  int rawValue = analogRead(sensorPin);
  int distance = map(rawValue, 0, 1023, 0, 300);  // 0-300mm 범위로 매핑
  return distance;
}

bool DistanceSensor::isCoinInserted(int current_distance, int last_distance) {
  // 거리가 급격히 감소했다가 다시 증가하는 경우 동전 투입으로 판단
  return (last_distance - current_distance > DISTANCE_THRESHOLD);
}
