#ifndef Radar_H
#define Radar_H

#define JSON_MAX_SIZE_LIST 6

// #include <LinkedList.h>
// Using AsyncWebServer LinkedList lib can not be used because there is a class
// using the same name. For that reason, for now we use SimpleList until we fix
// this using namespace for example.
#include <SimpleList.h>
#include "IWebConfig.h"

#include "RadarMotor.h"
#include "DistSensor.h"
#include "DistSensorFactory.h"
// ParallaxServo class can work with ESP8266 or ESP32:
#include "ParallaxServo.h"

#ifdef ESP32
// To use paclema/ESP32Servo360:
#include "ParallaxServoESP32.h"
// #elif defined(ESP8266)
#endif


class Radar: public IWebConfig {

private:
  float angleMin = 0;
  float angleMax = 0;

  bool debug = false;
  bool radarInitialized  = false;

  struct RPoint {
    float angle;
    float distance;
    float fov_angle;
  };

public:

  // Radar Servo motor:
  #ifdef ESP32
    // RadarMotor motor;            // Using Hacked Servo
    // ParallaxServoESP32 motor;    // Using Parallax Servo with ESP32Servo360
    ParallaxServo motor;            // Using Parallax Servo

  #elif defined(ESP8266)
    // RadarMotor motor;               // Using Hacked Servo
    ParallaxServo motor;            // Using Parallax Servo
  #endif

  // LinkedList<DistSensor*> distanceSensors = LinkedList<DistSensor*>();
  SimpleList<DistSensor*> distanceSensors = SimpleList<DistSensor*>();

  RPoint rPoints[20];
  int rPointsSize = 0;

  Radar(void);
  Radar(String name);

  void parseWebConfig(JsonObjectConst configObject);

  void enableRadarServices(void);
  void disableDistSensors(void);
  void setupDistSensors(void);
  void loop(void);

  void printStatus(void);
  float getPosition(void);
  float getTargetPosition(void);
  bool getDistance(float &distance);
  bool readPoints(void);
  String getJsonPoints(void);

  int getDistanceSensorsId(String name);
  DistSensor* getDistanceSensor(String name, String type);
  void removeDistanceSensor(String name);

  void setDebug(bool d){ this->debug = d; };
  bool isDebug(void){ return this->debug; };

};
#endif
