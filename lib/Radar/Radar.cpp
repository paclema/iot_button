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
  this->motor.enabled = configObject["motor"]["enabled"] | false;
  this->motor.angleAccuracy = configObject["motor"]["angle_accuracy"];
  this->motor.servoSpeed = float(configObject["motor"]["servo_speed_ms/60"])/60;

  // this->hcsr04.enabled = configObject["HCSR04"]["enabled"] | true;
  // this->hcsr04.time_budget_ms = configObject["HCSR04"]["time_budget_ms"];

  this->sensorDistance.enabled = configObject["vl53l1x"]["enabled"] | true;
  this->sensorDistance.debug = configObject["vl53l1x"]["debug"] | false;
  this->sensorDistance.time_budget_ms = configObject["vl53l1x"]["time_budget_ms"];
  this->sensorDistance.distance_mode = configObject["vl53l1x"]["distance_mode"] | "Short" ;
  for (unsigned int i = 0; i < configObject["vl53l1x"]["distance_mode_options"].size(); i++) { //Iterate through results
    this->sensorDistance.distance_mode_options[i] = configObject["vl53l1x"]["distance_mode_options"][i].as<String>(); //Explicit cast
  }

  // this->roi.enabled = configObject["ROI"]["enabled"] | false;
  // this->roi.zones = configObject["ROI"]["zones"];

  // After new configurations, reconfigure the radar objects to apply new changes:
  Radar::enableRadarServices();


};


void Radar::enableRadarServices(void){
  Serial.println("--- Radar: ");

  // Motor:
  if (this->motor.enabled){
    Serial.println("   - Motor -> enabled");
  } else Serial.println("   - Motor -> disabled");
  this->motor.setup();

  // HCSR04 sensor:
  if (this->hcsr04.enabled){
    // sensorSetup();
    Serial.println("   - HCSR04 -> enabled");
  } else Serial.println("   - HCSR04 -> disabled");

  // VL53L1X sensor:
  if (this->sensorDistance.enabled){
    Serial.println("   - VL53L1X -> enabled");
    this->sensorDistance.setup();
  } else Serial.println("   - VL53L1X -> disabled");

  // ROI VL53L1X sensor:
  if (this->roi.enabled){
    Serial.println("   - ROI -> enabled");
  } else Serial.println("   - ROI -> disabled");


  Serial.println("");

};


void Radar::loop(void){
  if (this->motor.enabled) motor.moveServo();

};


void Radar::printStatus(void){
  // From sensorDistance vl53l1x:
  if (this->sensorDistance.debug){
    // Serial.print(this->sensorDistance.getSensorRange());
    this->sensorDistance.printSensorStatus();
  }
};


float Radar::getPosition(void){
  return this->motor.getFeedbackAngle();
};


bool Radar::getDistance(float &distance){
  return this->sensorDistance.sensorRead(distance);
};


bool Radar::getPoints(float *distances, float *angles){
  // Depending the sensorDistance type we can get several distances each time
  // Construct here the list to pass back to the main()
  float angle = this->motor.getFeedbackAngle();
  float distance;
  int sizeDistances = this->sensorDistance.sensorRead(distance);
  // float distancesNew[sizeDistances];
  // float anglesNew[sizeDistances];

  if (sizeDistances >= 0) return true;
  else return false;
};
