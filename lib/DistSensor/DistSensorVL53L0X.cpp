#include "DistSensorVL53L0X.h"


DistSensorVL53L0X::DistSensorVL53L0X(void) {
  if (this->debug) Serial.println("\t\tCreating DistSensorVL53L0X");
};


DistSensorVL53L0X::~DistSensorVL53L0X(void) {
  if (this->debug) Serial.println("\t\tDestroying DistSensorVL53L0X");
};


void DistSensorVL53L0X::setup(void){

  Serial.println("\tInitialising VL53L0X sensor");

  int initRetries = 0;
  if (!sensor.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (initRetries <= MAX_INIT_RETRIES) {
      delay(300);
      Serial.print(".");
      initRetries++;
    }
    Serial.println("");
  }

  if (initRetries <= MAX_INIT_RETRIES) {
    Serial.print("\t --> VL53L0X initialized!");

  } else {
    Serial.println("\t -->VL53L0X NOT initialized!");
    }

    // Wire.begin();
    // sensor.setTimeout(500);
    // if (!sensor.init())
    // {
    //   Serial.println("Failed to detect and initialize sensor!");
    //   while (1) {}
    // }
    //
    // Serial.println("###  Device sensors info:\n");
    // VL53L0X_Version_t* version;
    // Serial.print("   - Version: ");Serial.println(VL53L0X_GetVersion(version));
    // Serial.print("   - ROI zones: ");Serial.println(VL53L0X_GetVersion(version));

    // VL53L1_CalibrationData_t * calibrationData;
    // status = VL53L1_GetCalibrationData(Dev, &calibrationData);
    // Serial.print("VL53L1_GetCalibrationData returns ");
    // Serial.println(status);
    // Serial.print("Optical Center (x,y): ");
    // Serial.print(calibrationData.optical_centre.x_centre/16.0);  //8 bit fixed point
    // Serial.print(", ");
    // Serial.println(calibrationData.optical_centre.y_centre/16.0);
}


bool DistSensorVL53L0X::sensorRead(float &distance){

  VL53L0X_RangingMeasurementData_t measure;
	// Serial.print("Reading a measurement... ");
	sensor.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

	if (measure.RangeStatus != 4) {  // phase failures have incorrect data
	  Serial.print("  - Distance (mm): "); Serial.println(measure.RangeMilliMeter);
		// Serial.print("  - ZoneId: "); Serial.println(measure.ZoneId);
		// Serial.print("  - RangeFractionalPart: "); Serial.println(measure.RangeFractionalPart);
		// Serial.print("  - RangeStatus: "); Serial.println(measure.RangeStatus);
		// Serial.print("  - EffectiveSpadRtnCount: "); Serial.println(measure.EffectiveSpadRtnCount);

		// distance = measure.RangeMilliMeter/10;
		distance = measure.RangeMilliMeter;
		return true;
	} else {
	  Serial.println(" out of range ");
		return false;
	}
  return false;
}


String DistSensorVL53L0X::getSensorRange(void){
  // return String(this->sensor.rangeStatusToString(this->sensor.ranging_data.range_status));
  return "Not implemented for DistSensorVL53L0X";
}


void DistSensorVL53L0X::printSensorStatus(void){
  Serial.println("Not implemented for DistSensorVL53L0X");
}
