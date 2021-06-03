#include "DistSensorVL53L1X.h"
#include <VL53L1X.h>

DistSensorVL53L1X::DistSensorVL53L1X(void) {
  if (this->debug) Serial.println("\t\tCreating DistSensorVL53L1X");

};


DistSensorVL53L1X::~DistSensorVL53L1X(void) {
  sensor.stopContinuous();
  if (this->debug) Serial.println("\t\tDestroying DistSensorVL53L1X");
};


void DistSensorVL53L1X::disableSensor(void) {
  pinMode(XSHUTPin, OUTPUT);
  digitalWrite(XSHUTPin, LOW);
  if (this->debug) Serial.printf("\t%s disables %d\n", this->name.c_str(), XSHUTPin);
};


void DistSensorVL53L1X::setup(void){
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  pinMode(XSHUTPin, INPUT);
  Serial.println("\tInitialising VL53L1X sensor: " + this->name);

  int initRetries = 0;
  while (!sensor.init() && initRetries <= MAX_INIT_RETRIES) {
    delay(300);
    Serial.print(".");
    initRetries++;
  }

  if (initRetries <= MAX_INIT_RETRIES) {
    sensor.setAddress((uint8_t)this->address);
    sensor.setTimeout(500);

    // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
    // You can change these settings to adjust the performance of the sensor, but
    // the minimum timing budget is 20 ms for short distance mode and 33 ms for
    // medium and long distance modes. See the VL53L1X datasheet for more
    // information on range and timing limits.
    // sensor.setDistanceMode(VL53L1X::Long);
    if (this->distance_mode == "Short") sensor.setDistanceMode(VL53L1X::Short);
    else if (this->distance_mode == "Medium") sensor.setDistanceMode(VL53L1X::Medium);
    else if (this->distance_mode == "Long") sensor.setDistanceMode(VL53L1X::Long);
    else if (this->distance_mode == "Unknown") sensor.setDistanceMode(VL53L1X::Medium);
    // sensor.setMeasurementTimingBudget(50000);
    sensor.setMeasurementTimingBudget(this->timeBudget*1000);


    Serial.println("\tMode: " + this->distance_mode);
    Serial.println("\tTime budget: " + String(this->timeBudget));
    Serial.print("\tI2C address: "); Serial.println((uint8_t) this->sensor.getAddress());
    Serial.print("\tXSHUTPin: "); Serial.println(this->XSHUTPin);

    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    // sensor.startContinuous(50);
    sensor.startContinuous(this->timeBudget);
    Serial.println("\t -->" + this->name + " initialized!");

  } else {
    Serial.println("\t -->" + this->name + " NOT initialized!");
    }
}


bool DistSensorVL53L1X::sensorRead(float *distance){

  if (this->sensor.dataReady()){
    bool blocking = false;
    float dist = 0;
    dist = this->sensor.read(blocking);
    
    if (this->sensor.timeoutOccurred()) 
      return false;

    if (sensor.ranging_data.range_status == VL53L1X::RangeStatus::RangeValid) {
      distance[0] = dist;
    } else if(sensor.ranging_data.range_status == VL53L1X::RangeStatus::OutOfBoundsFail){
      // We report the max readable distance in this case:
      // Those distances are takeng as an average from pthe page 11the datasheet:
      // https://www.st.com/resource/en/datasheet/vl53l1x.pdf
      if (this->distance_mode == "Short") distance[0] = 1360;
      else if (this->distance_mode == "Medium") distance[0] = 2900;
      else if (this->distance_mode == "Long") distance[0] = 3600;
      else if (this->distance_mode == "Unknown") distance[0] = 2900;
    } else {
      if (this->debug){ 
        String error = "ERROR Distance from " + this->name + ": " + VL53L1X::rangeStatusToString(this->sensor.ranging_data.range_status);
        Serial.println(error);
      }
      return false;
    }

    return true;
  }
  return false;
}


String DistSensorVL53L1X::getSensorRange(void){
  return String(this->sensor.rangeStatusToString(this->sensor.ranging_data.range_status));
}


void DistSensorVL53L1X::printSensorStatus(void){
  Serial.print("range: ");
  Serial.print(this->sensor.ranging_data.range_mm);
  Serial.print("\tstatus: ");
  Serial.print(VL53L1X::rangeStatusToString(this->sensor.ranging_data.range_status));
  Serial.print("\tpeak signal: ");
  Serial.print(this->sensor.ranging_data.peak_signal_count_rate_MCPS);
  Serial.print("\tambient: ");
  Serial.print(this->sensor.ranging_data.ambient_count_rate_MCPS);

  Serial.println();
}


void DistSensorVL53L1X::parseWebConfig(JsonObjectConst configObject){

  this->distance_mode = configObject[this->name]["distance_mode"] | "Short" ;
  this->XSHUTPin = configObject[this->name]["XSHUT_pin"] ;
  this->address = configObject[this->name]["I2c_address"] ;
  // for (unsigned int i = 0; i < configObject["vl53l1x"]["distance_mode_options"].size(); i++) { //Iterate through results
  //   this->distance_mode_options[i] = configObject["vl53l1x"]["distance_mode_options"][i].as<String>(); //Explicit cast
  // }

}
