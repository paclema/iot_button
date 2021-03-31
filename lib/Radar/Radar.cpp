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

  // Disable distance sensors:
  // if (this->radarInitialized) Radar::disableDistSensors();

  // Radar IWebConfig object:
  this->debug = configObject["debug"] | false;
  this->angleMin = configObject["angle_min"];
  this->angleMax = configObject["angle_max"];

  // Radar motor1:
  String nameMotor = "motor_1";
  this->motor1.enabled = configObject[nameMotor]["enabled"] | false;
  this->motor1.setServoControl(configObject[nameMotor]["control_pin"]);
  this->motor1.setServoFeedback(configObject[nameMotor]["feedback_pin"]);
  this->motor1.setSpeedPulse(configObject[nameMotor]["motor_speed_pulse"]);
  this->motor1.angleAccuracy = configObject[nameMotor]["angle_accuracy"];
  this->motor1.servoSpeed = float(configObject[nameMotor]["servo_speed_ms/60"])/60;
  this->motor1.setDebug(configObject[nameMotor]["debug"] | false);

  // Radar motor1:
  nameMotor = "motor_2";
  this->motor2.enabled = configObject[nameMotor]["enabled"] | false;
  this->motor2.setServoControl(configObject[nameMotor]["control_pin"]);
  this->motor2.setServoFeedback(configObject[nameMotor]["feedback_pin"]);
  this->motor2.setSpeedPulse(configObject[nameMotor]["motor_speed_pulse"]);
  this->motor2.angleAccuracy = configObject[nameMotor]["angle_accuracy"];
  this->motor2.servoSpeed = float(configObject[nameMotor]["servo_speed_ms/60"])/60;
  this->motor2.setDebug(configObject[nameMotor]["debug"] | false);


  // HCSR04 sensor:
  if (configObject["HCSR04"]["enabled"]){
    // if (this->debug) Serial.println("\tHCSR04 must be created");

    DistSensor *distanceSensor = getDistanceSensor("HCSR04", "HCSR04");
    distanceSensor->setEnable(configObject["HCSR04"]["enabled"] | false);
    distanceSensor->setTimeBudget(configObject["HCSR04"]["time_budget_ms"]);

  } else {
    if (this->debug) Serial.println("\tHCSR04 must be removed");
    removeDistanceSensor("HCSR04");
  }


  // VL53L1X sensor:
  String nameSensor = "vl53l1x_1";
  if (configObject[nameSensor]["enabled"]){
    // if (this->debug) Serial.println("\tvl53l1x must be created");

    // As VL53L1X with or whithout use different libraries, we have to remove
    // the VL53L1X_ROI object if it was created and create a VL53L1X one:
    removeDistanceSensor("VL53L1X_ROI");
    DistSensor *distanceSensor = getDistanceSensor(nameSensor, "VL53L1X");

    distanceSensor->setEnable(configObject[nameSensor]["enabled"] | false);
    distanceSensor->setDebug(configObject[nameSensor]["debug"] | false);
    distanceSensor->setTimeBudget(configObject[nameSensor]["time_budget_ms"]);

  } else {
    // if (this->debug) Serial.println("\tvl53l1x must be removed");
    removeDistanceSensor(nameSensor);

  }

  // VL53L1X second sensor:
  nameSensor = "vl53l1x_2";
  // Serial.print("\tCONFIGURING: ");
  // serializeJsonPretty(configObject[nameSensor], Serial);
  if (configObject[nameSensor]["enabled"]){
    // if (this->debug) Serial.println("\tvl53l1x must be created");

    // As VL53L1X with or whithout use different libraries, we have to remove
    // the VL53L1X_ROI object if it was created and create a VL53L1X one:
    // removeDistanceSensor("VL53L1X_ROI");
    DistSensor *distanceSensor = getDistanceSensor(nameSensor, "VL53L1X");

    distanceSensor->setEnable(configObject[nameSensor]["enabled"] | false);
    distanceSensor->setDebug(configObject[nameSensor]["debug"] | false);
    distanceSensor->setTimeBudget(configObject[nameSensor]["time_budget_ms"]);

  } else {
    // if (this->debug) Serial.println("\tvl53l1x must be removed");
    removeDistanceSensor(nameSensor);

  }


  // VL53L1X with ROI sensor:
  if (configObject["ROI"]["enabled"]){
    // if (this->debug) Serial.println("\tROI must be created");

    // As VL53L1X with or whithout use different libraries, we have to remove
    // the VL53L1X object if it was created and create a VL53L1X_ROI one:
    removeDistanceSensor("vl53l1x_1");
    DistSensor *distanceSensor = getDistanceSensor("VL53L1X_ROI", "VL53L1X_ROI");

    distanceSensor->setEnable(configObject["ROI"]["enabled"] | false);
    distanceSensor->setDebug(configObject["vl53l1x_1"]["debug"] | false);
    distanceSensor->setTimeBudget(configObject["vl53l1x_1"]["time_budget_ms"]);

  } else {
    // if (this->debug) Serial.println("\tROI must be removed");
    removeDistanceSensor("VL53L1X_ROI");

  }


  // For each DistSensor call parseWebConfig() to configure specific sensor configs:
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    sensorTemp->parseWebConfig(configObject);
  }


  // Setup again Distance sensors:
  // if (this->radarInitialized) Radar::setupDistSensors();


  // Distance sensors object:
  if (this->debug) {
    Serial.print("List distanceSensors Objects size: ");
    Serial.println(distanceSensors.size());
  }

};


void Radar::enableRadarServices(void){
  Serial.println("--- Radar: ");
  Serial.printf("   - Debug: %s\n", this->debug ? "true" : "false");
  Serial.printf("   - Angle min: %f\n", this->angleMin);
  Serial.printf("   - Angle max: %f\n", this->angleMax);

  // Motors:
  if (this->motor1.enabled){
    Serial.println("   - Motor 1 -> enabled");
  } else Serial.println("   - Motor 1 -> disabled");
  this->motor1.setup();
  // this->motor1.rotate((this->angleMax + this->angleMin)/2, 4);
  this->motor1.rotate(this->angleMin, 4);

  if (this->motor2.enabled){
    Serial.println("   - Motor 2 -> enabled");
  } else Serial.println("   - Motor 2 -> disabled");
  this->motor2.setup();
  // this->motor2.rotate((this->angleMax + this->angleMin)/2, 4);
  this->motor2.rotate((this->angleMin + 180), 4);

  // Distance sensors:
  Radar::disableDistSensors();
  Radar::setupDistSensors();

  this->radarInitialized = true;
};


void Radar::disableDistSensors(void){

  Serial.printf("Disabling %d sensors:\n", distanceSensors.size());
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    sensorTemp->disableSensor();
    if (this->debug) Serial.printf("\t-> %s disabled\n", sensorTemp->getName().c_str());
  }

};


void Radar::setupDistSensors(void){
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    String nameSensor = sensorTemp->getName();
    String typeSensor = sensorTemp->getType();
    Serial.print("   - ");
    Serial.print(nameSensor);
    Serial.print(" (");
    Serial.print(typeSensor);
    Serial.print(") -> ");
    if (sensorTemp->isEnabled()) Serial.print("enabled");
    else Serial.print("disabled");
    if (sensorTemp->isDebug()) Serial.print(" with debug");
    Serial.println();
    sensorTemp->setup();
  }

  Serial.println();

  if (debug){
    Serial.println ("I2C sensors scanner. Scanning ...");
    byte count = 0;
    for (byte i = 1; i < 120; i++) {
      Wire.beginTransmission (i);
      if (Wire.endTransmission () == 0) {
        Serial.print ("Found address: "); Serial.print (i, DEC); Serial.print (" (0x");
        Serial.print (i, HEX); Serial.println (")");
        count++;
        delay (1);
      }
    }
    Serial.println ("Done.");
    Serial.print ("Found "); Serial.print (count, DEC); Serial.println (" device(s).\n");
  }

};

void Radar::loop(void){

  // For RadarMotor:
  // if (this->motor.enabled){
  //   this->motor.enableServo();
  //   this->motor.moveServo();
  // }
  // else this->motor.disableServo();


  // For ParallaxServo:
  // MOTOR 1
  if (this->motor1.handle()){
    // if (debug){
    // Serial.print("Current angle: ");
    // Serial.print(this->motor1.getAngle());
    // Serial.print("/");
    // Serial.print(this->motor1.getAngleTarget());
    // Serial.print(" - ");
    // Serial.println(this->motor1.getTurns());
    // }

    if (this->motor1.enabled){
      this->motor1.enableServo();

      // Move here accordingly:
      if(this->motor1.getAngle() >= this->angleMax){
        this->motor1.rotate(this->angleMin, 4);
      }
      else if(this->motor1.getAngle() <= this->angleMin){
        this->motor1.rotate(this->angleMax, 4);
      }
    }
    else this->motor1.disableServo();
  } else {
    if (debug) Serial.println("Motor 1 not powered on or feedback not available!");
  }

  // MOTOR 2
  if (this->motor2.handle()){
    if (this->motor2.enabled){
      this->motor2.enableServo();

      // Move here accordingly:
      if(this->motor2.getAngle() >= (this->angleMax + 180)){
        this->motor2.rotate((this->angleMin + 180), 4);
      }
      else if(this->motor2.getAngle() <= (this->angleMin + 180)){
        this->motor2.rotate((this->angleMax + 180), 4);
      }
    }
    else this->motor2.disableServo();
  } else {
    if (debug) Serial.println("Motor 2 not powered on or feedback not available!");
  }


};


void Radar::printStatus(void){
  // From sensorDistance vl53l1x:
  // if (this->sensorDistance.debug){
  //   // Serial.print(this->sensorDistance.getSensorRange());
  //   this->sensorDistance.printSensorStatus();
  // }
};


float Radar::getPosition(int motorId){
  if (motorId == 1)
    return this->motor1.getAngle();
  else if (motorId == 2)
    return this->motor2.getAngle();
  else
    return 0;

};


float Radar::getTargetPosition(int motorId){
  if (motorId == 1)
    return this->motor1.getAngleTarget();
  else if (motorId == 2)
    return this->motor2.getAngleTarget();
  else
    return 0;
};


bool Radar::getDistance(float &distance){
  // return this->sensorDistance.sensorRead(distance);
  return false;
};


bool Radar::readPoints(void){
  // Depending the sensorDistance type we can get several distances each time
  // Construct here the list to pass back to the main()
  // float angle = this->motor.getAngle();
  // float distance;
  // int sizeDistances = this->sensorDistance.sensorRead(distance);
  // float distancesNew[sizeDistances];
  // float anglesNew[sizeDistances];

  // if (sizeDistances >= 0) return true;
  // else return false;

  int index = 0;
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    String nameSensor = sensorTemp->getName();

    float distance[4];
    if (sensorTemp->sensorRead(distance)){
      if (nameSensor == "VL53L1X_ROI") {
        // rPoints[index].fov_angle = sensorTemp->getFovAngle();
        int numPoints = 4;  //TODO: get the num depending on ROI zones
        float angle = this->motor1.getAngle();
        for(int j = 0; j < numPoints; j++){
          rPoints[index+j].angle = angle-(27./2) + (27./8) + (27./4)*(j);
          rPoints[index+j].distance = distance[j];
          rPoints[index+j].fov_angle = (27./4);
        }
        index = index + 4;
      } else if (nameSensor ==  "vl53l1x_1"){
        rPoints[index].angle = this->motor1.getAngle();
        rPoints[index].distance = distance[0];
        rPoints[index].fov_angle = 27;
        index++;
      } else if (nameSensor ==  "vl53l1x_2"){
        rPoints[index].angle = this->motor2.getAngle();
        rPoints[index].distance = distance[0];
        rPoints[index].fov_angle = 27;
        index++;
      } else if (nameSensor == "VL53L0X"){
        rPoints[index].angle = this->motor1.getAngle();
        rPoints[index].distance = distance[0];
        rPoints[index].fov_angle = 27;
        index++;
      } else if (nameSensor == "HCSR04"){
        rPoints[index].angle = this->motor1.getAngle();
        rPoints[index].distance = distance[0];
        rPoints[index].fov_angle = 27;
        index++;
      }
    }


  }

  this->rPointsSize = index;

  if (index == 0) return false;
  else return true;

}


String Radar::getJsonPoints(void){

  String data = "";

  for(int i = 0; i < this->rPointsSize; i++){
    if (i == 0) data += "{\"angle\":" + String(this->rPoints[i].angle);
    else data += ",{\"angle\":" + String(this->rPoints[i].angle);
    data += ",\"distance\":" + String(this->rPoints[i].distance);
    data += ",\"fov_angle\":" + String(this->rPoints[i].fov_angle);
    data += "}";
  }
  return data;
}


int Radar::getDistanceSensorsId(String name){
  DistSensor *sensorTemp ;
  for(int i = 0; i < distanceSensors.size(); i++){
    sensorTemp = distanceSensors.get(i);
    if (sensorTemp->getName() == name){
      if (this->debug) Serial.println("FOUND!: " + name + " of type " + sensorTemp->getType());
      return i;
    }
  }
  if (this->debug) Serial.println("NOT FOUND!: DistSensor " + name);
  // If name sensor is not in distanceSensors, return -1:
  return -1;
};


DistSensor* Radar::getDistanceSensor(String name, String type){
  DistSensor *distanceSensorTemp = NULL;

  // Check if the sensor is in distanceSensors list or create one:
  int sensorId = Radar::getDistanceSensorsId(name);
  if (sensorId == -1){
    // This sensor is not yet in distanceSensors list, create one:
    distanceSensorTemp = DistSensorFactory::createDistSensor(name, type);
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
    // distanceSensorTemp = this->distanceSensors.remove(sensorId);
    this->distanceSensors.remove(sensorId);
    delete distanceSensorTemp;
    distanceSensorTemp = NULL;
  }
};
