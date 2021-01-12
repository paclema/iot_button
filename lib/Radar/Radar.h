#ifndef Radar_H
#define Radar_H

#define JSON_MAX_SIZE_LIST 6

#include <LinkedList.h>
#include "IWebConfig.h"

#include "RadarMotor.h"
#include "ParallaxServo.h"
#include "DistSensor.h"
#include "DistSensorFactory.h"


class Radar: public IWebConfig {

private:
  float angleMin = 0;
  float angleMax = 0;

  bool debug = false;

  struct RPoint {
    float angle;
    float distance;
    float fov_angle;
  };

public:

  // RadarMotor motor; // Using Hacked Servo
  ParallaxServo motor; // Using Hacked Servo

  LinkedList<DistSensor*> distanceSensors = LinkedList<DistSensor*>();

  RPoint rPoints[20];
  int rPointsSize = 0;

  Radar(void);
  Radar(String name);

  void parseWebConfig(JsonObjectConst configObject);

  void enableRadarServices(void);
  void setupDistSensors(void);
  void loop(void);

  void printStatus(void);
  float getPosition(void);
  float getTargetPosition(void);
  bool getDistance(float &distance);
  bool readPoints(void);
  String getJsonPoints(void);

  int getDistanceSensorsId(String name);
  DistSensor* getDistanceSensor(String name);
  void removeDistanceSensor(String name);

  void setDebug(bool d){ this->debug = d; };
  bool isDebug(void){ return this->debug; };

};
#endif
