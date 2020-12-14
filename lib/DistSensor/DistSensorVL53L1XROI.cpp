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

  // Creating 4 ROI definition
  for (uint8_t x = 0; x < 4; x++) {
    roiZone[i] = { 0, uint8_t(15-4*x), 15, uint8_t(15-3*x) };
  }


  pinMode(XSHUT, OUTPUT);
	delay(100);
	dev1_sel

  // This is the default 8-bit slave address (including R/W as the least
  // significant bit) as expected by the API. Note that the Arduino Wire library
  // uses a 7-bit address without the R/W bit instead (0x29 or 0b0101001).
	Dev->I2cDevAddr = 0x52;

  VL53L1_software_reset(Dev);

	Serial.printf("\n\rDevice data  \n");
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
	// status += VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_MEDIUM);
  // status += VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 10000);	// 73Hz
  // status += VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 15);
  status += VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, MEASUREMENT_BUDGET_MS * 1000);
  status += VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, INTER_MEASUREMENT_PERIOD_MS);

  status += VL53L1_StartMeasurement(Dev);
	if (status) {
		Serial.printf("StartMeasurement failed status: %d\n\r", status);
    Serial.println("\t -->VL53L1X NOT initialized!");
  } else {
    Serial.print("\t -->VL53L1X ROI initialized!");
  }


}


bool DistSensorVL53L1XROI::sensorRead(float *distance){
  // TODO
  #ifdef USE_BLOCKING_LOOP

    static VL53L1_RangingMeasurementData_t RangingData;

    // Roi zone 1:
    status = VL53L1_SetUserROI(Dev, &roiZone1);
    // while (digitalRead(INT));	// slightly faster
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
    VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
    if (status == 0) roiDistances[0] = RangingData.RangeMilliMeter;

    // Roi zone 2:
    status = VL53L1_SetUserROI(Dev, &roiZone2);
    // while (digitalRead(INT));	// slightly faster
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
    VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
    if (status == 0) roiDistances[1] = RangingData.RangeMilliMeter;

    // Roi zone 3:
    status = VL53L1_SetUserROI(Dev, &roiZone3);
    // while (digitalRead(INT));	// slightly faster
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
    VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
    if (status == 0) roiDistances[2] = RangingData.RangeMilliMeter;

    // Roi zone 4:
    status = VL53L1_SetUserROI(Dev, &roiZone4);
    // while (digitalRead(INT));	// slightly faster
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
    VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
    if (status == 0) roiDistances[3] = RangingData.RangeMilliMeter;


  #else


  static VL53L1_RangingMeasurementData_t RangingData;

  #ifdef USE_MEASUREMENT_DATA_READY_POINTER

    Serial.print("RangeStatus no bloquing: ");
    uint8_t isReady[4];
    for (int i = 0; i < 4; i++) {
      switch (i) {
        case 0:
          status = VL53L1_SetUserROI(Dev, &roiZone1);
          break;
        case 1:
          status = VL53L1_SetUserROI(Dev, &roiZone2);
          break;
        case 2:
          status = VL53L1_SetUserROI(Dev, &roiZone3);
          break;
        case 3:
          status = VL53L1_SetUserROI(Dev, &roiZone4);
          Serial.println(status);
          break;
      }
      status = VL53L1_GetMeasurementDataReady(Dev, &isReady[i]);
      Serial.print(RangingData.RangeStatus);
      Serial.print("-");
      if(!status && isReady[i]){
        status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
        roiDistances[i] = RangingData.RangeMilliMeter;
        // VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);
        VL53L1_ClearInterruptAndStartMeasurement(Dev);

      }
    }
    Serial.println("");
  #else
    Serial.print("RangeStatus: ");
    for (i = 0; i < 4; i++) {
      // switching ROI configs
      status = VL53L1_SetUserROI(Dev, &roiZone[i]);
    //		while (digitalRead(INT));	// slightly faster
      status = VL53L1_WaitMeasurementDataReady(Dev);
      if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
      VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
      if (status == 0){
        roiDistances[i] = RangingData.RangeMilliMeter;

      }
      Serial.print(RangingData.RangeStatus);
      Serial.print("-");
    }
    Serial.println("");
  #endif

  #endif


  Serial.print(" ROI: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(" - ");
    Serial.print(roiDistances[i]);
    distance[i]=roiDistances[i];
  }
  Serial.println();

  return true;
}


int DistSensorVL53L1XROI::sensorCountPersons(void){
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
