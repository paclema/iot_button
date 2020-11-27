#include "DistSensorVL53L1X.h"


DistSensorVL53L1X::DistSensorVL53L1X(void) {
  if (this->debug) Serial.println("\t\tCreating DistSensorVL53L1X");

};


DistSensorVL53L1X::~DistSensorVL53L1X(void) {
  if (this->debug) Serial.println("\t\tDestroying DistSensorVL53L1X");
};


void DistSensorVL53L1X::setup(void){

  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  Serial.println("\tInitialising VL53L1X sensor");

  int initRetries = 0;
  while (!sensor.init() && initRetries <= MAX_INIT_RETRIES) {
    delay(300);
    Serial.print(".");
    initRetries++;
  }
  Serial.println("");

  if (initRetries <= MAX_INIT_RETRIES) {
    Serial.print("\t --> VL53L1X initialized!");
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
    sensor.setMeasurementTimingBudget(this->time_budget_ms*1000);

    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    // sensor.startContinuous(50);
    sensor.startContinuous(this->time_budget_ms);

    Serial.print(" Mode: " + this->distance_mode);
    Serial.println("; Time budget: " + String(this->time_budget_ms));


  } else {
    Serial.println("\t -->VL53L1X NOT initialized!");
    }
}


bool DistSensorVL53L1X::sensorRead(float &distance){

  if (this->sensor.dataReady()){
    bool blocking = false;
    distance = this->sensor.read(blocking);
    // Serial.print(distance);
    if (this->sensor.timeoutOccurred()){
      // Serial.print(" TIMEOUT");
      return false;
    };
    // Serial.println();
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
