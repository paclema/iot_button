#ifndef Radar_H
#define Radar_H

#define JSON_MAX_SIZE_LIST 6


#include "IWebConfig.h"

#include "RadarMotor.h"
#include "SensorDistance.h"
// #include "sensorVL53L1X.h"


class Radar: public IWebConfig {

public:

  RadarMotor motor;
  SensorDistance sensorDistance;



  struct HCSR04 {
    bool enabled;
    float time_budget_ms;
  } hcsr04;


  struct ROI {
    bool enabled;
    int zones;
  } roi;




  Radar(void);
  Radar(String name);

  void parseWebConfig(JsonObjectConst configObject);

  void enableRadarServices(void);
  void loop(void);

  void printStatus(void);
  float getPosition(void);
  bool getDistance(float &distance);


};
#endif
