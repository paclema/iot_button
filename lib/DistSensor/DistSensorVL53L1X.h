#ifndef DistSensorVL53L1X_H
#define DistSensorVL53L1X_H

#include <Arduino.h>
#include <DistSensor.h>

#include <Wire.h>
#include <VL53L1X.h>

// Add pololu/VL53L1X @ ^1.0.1 lib_deps on platformio.ini
// Lib documentation: https://github.com/pololu/vl53l1x-arduino


// Pinout with D1 mini:
//--------------------
//
// SDA -- D1
// SCL -- D2
// VIN -- 3.3V
// GND -- GND
#define  MAX_INIT_RETRIES    20


class DistSensorVL53L1X: public DistSensor {

public:

  VL53L1X sensor;

  String distance_mode;
  String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]


  DistSensorVL53L1X(void);
  ~DistSensorVL53L1X();


  void setup(void);
  bool sensorRead(float &distance);
  String getSensorRange(void);
  void printSensorStatus(void);

  void free() { if (this->debug) Serial.println("\t\t\tfree DistSensorVL53L1X"); delete this; }

  void parseWebConfig(JsonObjectConst configObject);


};
#endif
