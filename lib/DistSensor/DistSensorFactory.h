#ifndef DistSensorFactory_H
#define DistSensorFactory_H

#include <DistSensor.h>
#include <DistSensorHCSR04.h>
#include <DistSensorVL53L0X.h>
#include <DistSensorVL53L1X.h>
#include <DistSensorVL53L1XROI.h>

class DistSensorFactory {

public:

  static DistSensor* createDistSensor(String name, String type){

    DistSensor *sensor = NULL;
    const char * type_cchar = type.c_str();


    if (strcmp(type_cchar, "VL53L1X") == 0) {
      sensor = new DistSensorVL53L1X;
    } else if (strcmp(type_cchar, "VL53L1X_ROI") == 0){
      sensor = new DistSensorVL53L1XROI;
    } else if (strcmp(type_cchar, "VL53L0X") == 0){
      sensor = new DistSensorVL53L0X;
    } else if (strcmp(type_cchar, "HCSR04") == 0){
      sensor = new DistSensorHCSR04;
    } else {
      Serial.println("ERROR DistSensorFactory: Invalid distance sensor selected");
      return NULL;
    }


    if (sensor->isDebug()) Serial.println("DistSensorFactory creates DistSensor: " + type);
    // Preprocessing setups before returning the sensor object:
    sensor->setName(name);
    sensor->setType(type);
    // sensor->setup();

    return sensor;


  }


};
#endif
