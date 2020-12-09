#include "Radar.h"


Radar::Radar(void) {
  this->nameConfigObject = "radar";
};


Radar::Radar(String name) {
  this->nameConfigObject = name;
};


void Radar::parseWebConfig(JsonObjectConst configObject){

  // JsonObject received:
  // serializeJsonPretty(configObject, Serial);


  // Radar IWebConfig object:
  this->debug = configObject["debug"] | false;

  // Radar motor:
  this->motor.enabled = configObject["motor"]["enabled"] | false;
  this->motor.angleAccuracy = configObject["motor"]["angle_accuracy"];
  this->motor.servoSpeed = float(configObject["motor"]["servo_speed_ms/60"])/60;
  this->motor.setDebug(configObject["motor"]["debug"] | false);


  // HCSR04 sensor:
  if (configObject["HCSR04"]["enabled"]){
    if (this->debug) Serial.println("\tHCSR04 must be created");

    DistSensor *distanceSensor = getDistanceSensor("HCSR04");
    distanceSensor->setEnable(configObject["HCSR04"]["enabled"] | false);
    distanceSensor->setTimeBudget(configObject["HCSR04"]["time_budget_ms"]);

  } else {
    if (this->debug) Serial.println("\tHCSR04 must be removed");
    removeDistanceSensor("HCSR04");
  }


  // VL53L1X sensor:
  if (configObject["vl53l1x"]["enabled"]){
    if (this->debug) Serial.println("\tvl53l1x must be created");

    // As VL53L1X with or whithout use different libraries, we have to remove
    // the VL53L1X_ROI object if it was created and create a VL53L1X one:
    removeDistanceSensor("VL53L1X_ROI");
    DistSensor *distanceSensor = getDistanceSensor("VL53L1X");

    distanceSensor->setEnable(configObject["vl53l1x"]["enabled"] | false);
    distanceSensor->setDebug(configObject["vl53l1x"]["debug"] | false);
    distanceSensor->setTimeBudget(configObject["vl53l1x"]["time_budget_ms"]);
    // distanceSensor->distance_mode = configObject["vl53l1x"]["distance_mode"] | "Short" ;
    // for (unsigned int i = 0; i < configObject["vl53l1x"]["distance_mode_options"].size(); i++) { //Iterate through results
    //   distanceSensor->distance_mode_options[i] = configObject["vl53l1x"]["distance_mode_options"][i].as<String>(); //Explicit cast
    // }

  } else {
    if (this->debug) Serial.println("\tvl53l1x must be removed");
    removeDistanceSensor("VL53L1X");

  }


  // VL53L1X with ROI sensor:
  if (configObject["ROI"]["enabled"]){
    if (this->debug) Serial.println("\tROI must be created");

    // As VL53L1X with or whithout use different libraries, we have to remove
    // the VL53L1X object if it was created and create a VL53L1X_ROI one:
    removeDistanceSensor("VL53L1X");
    DistSensor *distanceSensor = getDistanceSensor("VL53L1X_ROI");

    distanceSensor->setEnable(configObject["ROI"]["enabled"] | false);
    distanceSensor->setDebug(configObject["vl53l1x"]["debug"] | false);
    distanceSensor->setTimeBudget(configObject["vl53l1x"]["time_budget_ms"]);
    // distanceSensor->zones = configObject["ROI"]["zones"];

  } else {
    if (this->debug) Serial.println("\tROI must be removed");
    removeDistanceSensor("VL53L1X_ROI");

  }

  // Setup again Distance sensors:
  Radar::setupDistSensors();


  // Distance sensors object:
  if (this->debug) {
    Serial.print("List distanceSensors Objects size: ");
    Serial.println(distanceSensors.size());
  }

};


void Radar::enableRadarServices(void){
  Serial.println("--- Radar: ");

  // Motor:
  if (this->motor.enabled){
    Serial.println("   - Motor -> enabled");
  } else Serial.println("   - Motor -> disabled");
  this->motor.setup();

  // Distance sensors:
  Radar::setupDistSensors();

};


void Radar::setupDistSensors(void){
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    String nameSensor = sensorTemp->getName();
    Serial.print("   - ");
    Serial.print(nameSensor);
    Serial.print(" -> ");
    if (sensorTemp->isEnabled()) Serial.print("enabled");
    else Serial.print("disabled");
    if (sensorTemp->isDebug()) Serial.print(" with debug");
    Serial.println();
    sensorTemp->setup();
  }

  Serial.println();

};

void Radar::loop(void){
  if (this->motor.enabled) motor.moveServo();
};


void Radar::printStatus(void){
  // From sensorDistance vl53l1x:
  // if (this->sensorDistance.debug){
  //   // Serial.print(this->sensorDistance.getSensorRange());
  //   this->sensorDistance.printSensorStatus();
  // }
};


float Radar::getPosition(void){
  return this->motor.getFeedbackAngle();
};


bool Radar::getDistance(float &distance){
  // return this->sensorDistance.sensorRead(distance);
  return false;
};


bool Radar::getPoints(float *distances, float *angles){
  // Depending the sensorDistance type we can get several distances each time
  // Construct here the list to pass back to the main()
  // float angle = this->motor.getFeedbackAngle();
  // float distance;
  // int sizeDistances = this->sensorDistance.sensorRead(distance);
  // float distancesNew[sizeDistances];
  // float anglesNew[sizeDistances];

  // if (sizeDistances >= 0) return true;
  // else return false;
  return false;
};


int Radar::getDistanceSensorsId(String name){
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    if (sensorTemp->getName() == name){
      // Serial.println("FOUND!: " + name);
      return i;
    }
  }
  if (this->debug) Serial.println("NOT FOUND!: DistSensor " + name);
  // If name sensor is not in distanceSensors, return -1:
  return -1;
};


DistSensor* Radar::getDistanceSensor(String name){
  DistSensor *distanceSensorTemp = NULL;

  // Check if the sensor is in distanceSensors list or create one:
  int sensorId = Radar::getDistanceSensorsId(name);
  if (sensorId == -1){
    // This sensor is not yet in distanceSensors list, create one:
    distanceSensorTemp = DistSensorFactory::createDistSensor(name);
    this->distanceSensors.add(distanceSensorTemp);
  } else {
      distanceSensorTemp = this->distanceSensors.get(sensorId);
  }
  return distanceSensorTemp;
};


void Radar::removeDistanceSensor(String name){
  DistSensor *distanceSensorTemp = NULL;

  // Check if the sensor is in distanceSensors list to remove it:
  int sensorId = Radar::getDistanceSensorsId(name);
  if (sensorId != -1){
    // This sensor is in distanceSensors list
    distanceSensorTemp = this->distanceSensors.remove(sensorId);
    delete distanceSensorTemp;
    distanceSensorTemp = NULL;
  }
};
