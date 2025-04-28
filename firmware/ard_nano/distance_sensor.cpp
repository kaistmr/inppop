#include "distance_sensor.h"

DistanceSensor::DistanceSensor(int sensor_pin, int trig_pin)
  : sensorPin(sensor_pin), trigPin(trig_pin) {}

void DistanceSensor::setup() {
  pinMode(sensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

int DistanceSensor::readDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return analogRead(sensorPin);
}

bool DistanceSensor::isCoinInserted(int current_distance, int last_distance) {
  return (current_distance < last_distance - COIN_THRESHOLD);
}
