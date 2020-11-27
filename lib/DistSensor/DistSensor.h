#ifndef DistSensor_H
#define DistSensor_H

#define JSON_MAX_SIZE_LIST 6

#include <Arduino.h>
#include <Wire.h>

class DistSensor {

protected:

  bool enabled;
  bool debug = false;
  float timeBudget;

  String name = "none";


public:

  virtual ~DistSensor() {}
  virtual void setup(void) = 0;
  virtual bool sensorRead(float &distance) = 0;
  virtual String getSensorRange(void) = 0;
  virtual void printSensorStatus(void) = 0;
  virtual void free() = 0;

  void setName(String name){ this->name = name; };
  void setEnable(bool e){ this->enabled = e; };
  void setDebug(bool d){ this->debug = d; };
  void setTimeBudget(float t){ this->timeBudget = t; };

  String getName(void){ return this->name; };
  bool isEnabled(void){ return this->enabled; };
  bool isDebug(void){ return this->debug; };

};
#endif
