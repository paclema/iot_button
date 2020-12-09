#ifndef DistSensorVL53L0X_H
#define DistSensorVL53L0X_H

#define JSON_MAX_SIZE_LIST 6

#include <Arduino.h>
#include <DistSensor.h>

#include "Adafruit_VL53L0X.h" // Add adafruit/Adafruit_VL53L0X @ ^1.1.0 lib_deps on platformio.ini

// #include <VL53L0X.h>
// #include <Wire.h>
// VL53L0X sensor;

// Pinout with D1 mini:
//--------------------
//
// SDA -- D1
// SCL -- D2
// VIN -- 3.3V
// GND -- GND
#define  MAX_INIT_RETRIES    20


class DistSensorVL53L0X: public DistSensor {

public:

  Adafruit_VL53L0X sensor = Adafruit_VL53L0X();

  String distance_mode;
  String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]


  DistSensorVL53L0X(void);
  ~DistSensorVL53L0X();


  void setup(void);
  bool sensorRead(float *distance);
  String getSensorRange(void);
  void printSensorStatus(void);

  void free() { if (this->debug) Serial.println("\t\t\tfree DistSensorVL53L0X"); delete this; }

  void parseWebConfig(JsonObjectConst configObject);


};
#endif
