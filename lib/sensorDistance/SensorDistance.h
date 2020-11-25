#ifndef SensorDistance_H
#define SensorDistance_H

#define JSON_MAX_SIZE_LIST 6

#include <Arduino.h>
#include <Wire.h>

// Choose one sensor library:
// #include "sensorVL53L0X.h"
// #include "sensorVL53L1X.h"
// #include "sensorHCSR04.h"

// #include "sensorVL53L1X_ROI.h"
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


class SensorDistance {

public:

  VL53L1X sensor;

  bool enabled;
  bool debug = false;
  float time_budget_ms;
  String distance_mode;
  String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]



  SensorDistance(void);

  void setup(void);

  bool sensorRead(float &distance);

  String getSensorRange(void);

  void printSensorStatus(void);

};
#endif
