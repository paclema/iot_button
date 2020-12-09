#ifndef Radar_H
#define Radar_H

#define JSON_MAX_SIZE_LIST 6

#include <LinkedList.h>
#include "IWebConfig.h"

#include "RadarMotor.h"
#include "DistSensor.h"
#include "DistSensorFactory.h"


class Radar: public IWebConfig {

private:

  bool debug = false;

public:

  RadarMotor motor;
  LinkedList<DistSensor*> distanceSensors = LinkedList<DistSensor*>();


  Radar(void);
  Radar(String name);

  void parseWebConfig(JsonObjectConst configObject);

  void enableRadarServices(void);
  void setupDistSensors(void);
  void loop(void);

  void printStatus(void);
  float getPosition(void);
  bool getDistance(float &distance);
  bool getPoints(float *distances, float *angles);

  int getDistanceSensorsId(String name);
  DistSensor* getDistanceSensor(String name);
  void removeDistanceSensor(String name);

  void setDebug(bool d){ this->debug = d; };
  bool isDebug(void){ return this->debug; };

};
#endif
