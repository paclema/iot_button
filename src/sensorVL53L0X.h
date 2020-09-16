#include <Arduino.h>
#include "Adafruit_VL53L0X.h" // Add adafruit/Adafruit_VL53L0X @ ^1.1.0 lib_deps on platformio.ini
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// #include <VL53L0X.h>
// #include <Wire.h>
// VL53L0X sensor;


void sensorSetup(void){

	if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
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

int sensorLoop(void){

	int distance;
	VL53L0X_RangingMeasurementData_t measure;
	// Serial.print("Reading a measurement... ");
	lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

	if (measure.RangeStatus != 4) {  // phase failures have incorrect data
	  Serial.print("  - Distance (mm): "); Serial.println(measure.RangeMilliMeter);
	} else {
	  Serial.println(" out of range ");
	}


	// Serial.print("  - ZoneId: "); Serial.println(measure.ZoneId);
	// Serial.print("  - RangeFractionalPart: "); Serial.println(measure.RangeFractionalPart);
	// Serial.print("  - RangeStatus: "); Serial.println(measure.RangeStatus);
	// Serial.print("  - EffectiveSpadRtnCount: "); Serial.println(measure.EffectiveSpadRtnCount);

	// distance = measure.RangeMilliMeter/10;
	distance = measure.RangeMilliMeter;
	return distance;

}
