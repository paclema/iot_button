#ifndef DistSensor_H
#define DistSensor_H

#define JSON_MAX_SIZE_LIST 6

#include <Arduino.h>
#include <Wire.h>

class DistSensor {

public:

  bool enabled;
  bool debug = false;
  float time_budget_ms;

  String name = "none";

  virtual ~DistSensor() {}
  virtual void setup(void) = 0;
  virtual bool sensorRead(float &distance) = 0;
  virtual String getSensorRange(void) = 0;
  virtual void printSensorStatus(void) = 0;
  virtual void free() = 0;

  void setName(String name){
    this->name = name;
  };

  String getName(void){
    return this->name;
  };

};
#endif
