#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>
// Add pololu/VL53L1X @ ^1.0.1 lib_deps on platformio.ini

VL53L1X sensor;


void sensorSetup(void){

	Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

	sensor.setTimeout(500);
	if (!sensor.init())
	{
		Serial.println("Failed to detect and initialize sensor!");
		while (1);
	}

	// Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(50);

}

int sensorLoop(void){

	int distance = sensor.read();
	// Serial.print(distance);
	if (sensor.timeoutOccurred()){
		// Serial.print(" TIMEOUT");
		return 0;
	};

	// Serial.println();
	return distance;

}