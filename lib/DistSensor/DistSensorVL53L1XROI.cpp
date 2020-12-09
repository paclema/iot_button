#include "DistSensorVL53L1XROI.h"


DistSensorVL53L1XROI::DistSensorVL53L1XROI(void) {
  if (this->debug) Serial.println("\t\tCreating DistSensorVL53L1XROI");
};


DistSensorVL53L1XROI::~DistSensorVL53L1XROI(void) {
  if (this->debug) Serial.println("\t\tDestroying DistSensorVL53L1XROI");
};


void DistSensorVL53L1XROI::setup(void){

  Wire.begin(SDA, SCL);
	Wire.setClock(400000); // use 400 kHz I2C


  Serial.println("\tInitialising VL53L1X ROI sensor");

  pinMode(XSHUT, OUTPUT);
	delay(100);
	dev1_sel
	Dev->I2cDevAddr = 0x52;

	Serial.printf("\n\rDevice data  ");
	checkDev(Dev);
	delay(1000);

	// with modified DIRDWIPE lib
	tof_gestures_initDIRSWIPE_1(rangeThresholdCounter_mm, 0, 1000, false, &gestureDirSwipeData);
	// with NOT modified DIRDWIPE lib
	//	tof_gestures_initDIRSWIPE_1(800, 0, 1000, &gestureDirSwipeData);

	status += VL53L1_WaitDeviceBooted(Dev);
	status += VL53L1_DataInit(Dev);
	status += VL53L1_StaticInit(Dev);
	status += VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_SHORT);
	status += VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 10000);	// 73Hz
	status += VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 15);
	if (status) {
		Serial.printf("StartMeasurement failed status: %d\n\r", status);
    Serial.println("\t -->VL53L1X NOT initialized!");
  } else {
    Serial.print("\t -->VL53L1X ROI initialized!");
  }

	VL53L1_StartMeasurement(Dev);

}


bool DistSensorVL53L1XROI::sensorRead(float *distance){
  // TODO
  return false;
}


int DistSensorVL53L1XROI::sensorRead(void){
  static VL53L1_RangingMeasurementData_t RangingData;
	int gesture_code;


	status = VL53L1_SetUserROI(Dev, &roiConfig1);

	while (digitalRead(INT));	// slightly faster
//	status = VL53L1_WaitMeasurementDataReady(Dev);
	if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
	VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
	if (status == 0) distance[0] = RangingData.RangeMilliMeter;

	status = VL53L1_SetUserROI(Dev, &roiConfig2);

	while (digitalRead(INT));	// slightly faster
//	status = VL53L1_WaitMeasurementDataReady(Dev);
	if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
	VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
	if (status == 0) distance[1] = RangingData.RangeMilliMeter;

	gesture_code = tof_gestures_detectDIRSWIPE_1(distance[0], distance[1], &gestureDirSwipeData);	//0mS

	switch (gesture_code)
	{
	case GESTURES_SWIPE_LEFT_RIGHT:
		cnt++;
		right = 1;
		break;
	case GESTURES_SWIPE_RIGHT_LEFT:
		cnt--;
		left = 1;
		break;
	default:
		break;
	}

	if (distance[0] < rangeThresholdCounter_mm || distance[1] < rangeThresholdCounter_mm) { timeMark = millis(); }

	if (cnt != oldcnt) {
		oldcnt = cnt;
		if (cnt < 0) {
			LightON = 1;
			lighton();
		}
		else {
			LightON = 0;
			lightoff();
		}
		dispUpdate();
		left = 0; right = 0;
	}

	if (millis() - timeMark < 500) {
		// suitable to use with SerialPlot:  https://bitbucket.org/hyOzd/serialplot
		// Serial.printf("%d,%d,%d\n\r", distance[0], distance[1], cnt * 500);
    // Serial.print("Distances: " + distance[0] + ", " + distance[1] + ":---> " + (cnt * 500));
    Serial.print("Distances: ");
    Serial.print(distance[0]);
    Serial.print(", ");
    Serial.print(distance[1]);
    Serial.print(": counter ---> ");
		Serial.println((cnt));	}

		// sensorDistance_1 = distance[0];
		// sensorDistance_2 = distance[1];

	return cnt;
}


String DistSensorVL53L1XROI::getSensorRange(void){
  // return String(this->sensor.rangeStatusToString(this->sensor.ranging_data.range_status));
  return "Not implemented for DistSensorVL53L1XROI";
}


void DistSensorVL53L1XROI::printSensorStatus(void){
  Serial.println("Not implemented for DistSensorVL53L1XROI");
}


void DistSensorVL53L1XROI::parseWebConfig(JsonObjectConst configObject){

  this->distance_mode = configObject["vl53l1x"]["distance_mode"] | "Short" ;
  // for (unsigned int i = 0; i < configObject["vl53l1x"]["distance_mode_options"].size(); i++) { //Iterate through results
  //   this->distance_mode_options[i] = configObject["vl53l1x"]["distance_mode_options"][i].as<String>(); //Explicit cast
  // }
  this->zones = configObject["ROI"]["zones"];

}
