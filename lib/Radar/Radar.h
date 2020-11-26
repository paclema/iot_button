#ifndef Radar_H
#define Radar_H

#define JSON_MAX_SIZE_LIST 6

#include <LinkedList.h>
#include "IWebConfig.h"

#include "RadarMotor.h"
#include "DistSensor.h"
#include "DistSensorFactory.h"

#include "DistSensorVL53L1X.h"


class Radar: public IWebConfig {

public:

  RadarMotor motor;
  DistSensorVL53L1X sensorDistance;
  LinkedList<DistSensor*> distanceSensors = LinkedList<DistSensor*>();



  Radar(void);
  Radar(String name);

  void parseWebConfig(JsonObjectConst configObject);

  void enableRadarServices(void);
  void loop(void);

  void printStatus(void);
  float getPosition(void);
  bool getDistance(float &distance);
  bool getPoints(float *distances, float *angles);

  int getDistanceSensorsId(String name);
  DistSensor* getDistanceSensor(String name);
  void removeDistanceSensor(String name);


};
#endif
