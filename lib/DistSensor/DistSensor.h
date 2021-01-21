#ifndef DistSensor_H
#define DistSensor_H

#define JSON_MAX_SIZE_LIST 6
#include <ArduinoJson.h>

#include <Arduino.h>
#include <Wire.h>

class DistSensor {

protected:

  bool enabled;
  bool debug = false;
  float timeBudget;

  String name = "none";
  String type = "none";


public:

  virtual ~DistSensor() {}
  virtual void setup(void) = 0;
  virtual bool sensorRead(float *distance) = 0;
  virtual String getSensorRange(void) = 0;
  virtual void printSensorStatus(void) = 0;
  virtual void free() = 0;

  void setName(String name){ this->name = name; };
  void setType(String type){ this->type = type; };
  void setEnable(bool e){ this->enabled = e; };
  void setDebug(bool d){ this->debug = d; };
  void setTimeBudget(float t){ this->timeBudget = t; };

  virtual void parseWebConfig(JsonObjectConst configObject) = 0;

  String getName(void){ return this->name; };
  String getType(void){ return this->type; };
  bool isEnabled(void){ return this->enabled; };
  bool isDebug(void){ return this->debug; };

};
#endif
