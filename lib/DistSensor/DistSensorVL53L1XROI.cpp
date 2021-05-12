#include "DistSensorVL53L1XROI.h"


DistSensorVL53L1XROI::DistSensorVL53L1XROI(void) {
  if (this->debug) Serial.println("\t\tCreating DistSensorVL53L1XROI");
};


DistSensorVL53L1XROI::~DistSensorVL53L1XROI(void) {
  status = VL53L1_stop_range(Dev);
  if (this->debug){
  Serial.println("\t\tDestroying DistSensorVL53L1XROI");
  Serial.print("\t\tStopping current range: ");
  Serial.println(status);
  }
};


void DistSensorVL53L1XROI::disableSensor(void) {

};


void DistSensorVL53L1XROI::setup(void){

  if (Wire.setPins(SDA, SCL)) Serial.println("Pins configured for I2C");
  else Serial.println("Pins NOT configured for I2C");

  Wire.begin();
	Wire.setClock(400000); // use 400 kHz I2C

  // Serial.printf("\nI2C CLOCK: %zu", Wire.getClock());  
  Serial.println("\tInitialising VL53L1X ROI sensor");

  // Creating 4 ROI definition
  for (uint8_t x = 0; x < 4; x++) {
    roiZone[x] = { 0, uint8_t(15-4*x), 15, uint8_t(15-3*x) };
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

  Serial.printf("Configured roiZones[]: \n");
  for (int i = 0; i < this->zones; i++){
  // for (int i = 0; i < 4; i++){
    Serial.printf("\t-- roiZones[%d]:\n", i);
    Serial.printf("\t\t -TopLeftX: %d\n", roiZones[i].TopLeftX);
    Serial.printf("\t\t -TopLeftY: %d\n", roiZones[i].TopLeftY);
    Serial.printf("\t\t -BotRightX: %d\n", roiZones[i].BotRightX);
    Serial.printf("\t\t -BotRightY: %d\n", roiZones[i].BotRightY);
  }

	status += VL53L1_WaitDeviceBooted(Dev);
  // Serial.printf("VL53L1_WaitDeviceBooted status:%d\n", status);
	status += VL53L1_DataInit(Dev);
  // Serial.printf("VL53L1_DataInit status:%d\n", status);
	status += VL53L1_StaticInit(Dev);
  // Serial.printf("VL53L1_StaticInit status:%d\n", status);



  VL53L1_PresetModes currentPresetMode;
	status += VL53L1_GetPresetMode(Dev, &currentPresetMode);
  Serial.printf("Current preset mode: %d - status:%d\n", currentPresetMode, status);

	status += VL53L1_SetPresetMode(Dev, VL53L1_PRESETMODE_AUTONOMOUS);
  status += VL53L1_GetPresetMode(Dev, &currentPresetMode);
  Serial.printf("Performing set preset mode: %d - status:%d\n", currentPresetMode, status);


	status += VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_MEDIUM);
	// status += VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
	// status += VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_SHORT);



  // CALIBRATION:
  // status += DistSensorVL53L1XROI::calibrate(this->CalDistanceMilliMeter);


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


VL53L1_Error DistSensorVL53L1XROI::calibrate(int32_t CalDistanceMilliMeter){
  // Use and object at this distance:
  // int32_t CalDistanceMilliMeter = 200;
  int delayTime = 0;
  if ( this->debug) Serial.printf("CALIBRATION for test distance %dmm\n", CalDistanceMilliMeter);
  delay(delayTime);

  // SPAD
	status += VL53L1_PerformRefSpadManagement(Dev);
  if ( this->debug) Serial.printf("Performing VL53L1_PerformRefSpadManagement: %d\n", status);
  delay(delayTime);

  // Offset
	status += VL53L1_PerformOffsetSimpleCalibration(Dev, CalDistanceMilliMeter);
  if ( this->debug) Serial.printf("Performing VL53L1_PerformOffsetSimpleCalibration: %d\n", status);
  delay(delayTime);
	status += VL53L1_PerformOffsetCalibration(Dev, CalDistanceMilliMeter);
  if ( this->debug) Serial.printf("Performing VL53L1_PerformOffsetCalibration: %d\n", status);
  delay(delayTime);
  status += VL53L1_SetOffsetCalibrationMode(Dev, 2);
  if ( this->debug) Serial.printf("Performing VL53L1_SetOffsetCalibrationMode: %d\n", status);
  delay(delayTime);

  // Xtalk
  // uint8_t pXTalkCompensationEnable = 1;  //state 0=disabled or 1 = enabled
  uint8_t pXTalkCompensationEnable;         //state 0=disabled or 1 = enabled
	status += VL53L1_GetXTalkCompensationEnable(Dev, &pXTalkCompensationEnable);
  if ( this->debug) Serial.printf("Performing VL53L1_GetXTalkCompensationEnable: %d - enabled: %d\n", status, pXTalkCompensationEnable);
  delay(delayTime);
  uint8_t XTalkCompensationEnable = 1;      //state 0=disabled or 1 = enabled
  status += VL53L1_SetXTalkCompensationEnable(Dev, XTalkCompensationEnable);
  if ( this->debug) Serial.printf("Performing VL53L1_SetXTalkCompensationEnable: %d - enabled: %d\n", status, XTalkCompensationEnable);
  delay(delayTime);
  status += VL53L1_PerformSingleTargetXTalkCalibration(Dev, CalDistanceMilliMeter);
  if ( this->debug) Serial.printf("Performing VL53L1_PerformSingleTargetXTalkCalibration: %d\n", status);
  delay(delayTime);

  // Calibration
  VL53L1_CalibrationData_t calibrationData;
	status += VL53L1_GetCalibrationData(Dev, &calibrationData);
  if ( this->debug) Serial.printf("Performing VL53L1_GetCalibrationData: %d\n", status);
  delay(delayTime);
	status += VL53L1_SetCalibrationData(Dev, &calibrationData);
  if ( this->debug) Serial.printf("Performing VL53L1_SetCalibrationData: %d\n", status);
  delay(delayTime);


  if ( this->debug) Serial.printf("CALIBRATION DONE. Status: %d\n", status);
  return status;
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
    for (int i = 0; i < 4; i++) {
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


VL53L1_Error DistSensorVL53L1XROI::sensorRead2Roi(void){
  static VL53L1_RangingMeasurementData_t RangingData;
  int newDistance[2] = { 0, 0 };

  //MODE:  Waiting to get measurement:
  //---------------------------------
  
  for (unsigned int i = 0; i < this->zones; i++){
    status = VL53L1_SetUserROI(Dev, &roiZones[i]);
    // while (digitalRead(INT));	// slightly faster
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
    status += VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
    // status += VL53L1_ClearInterruptAndStartMeasurement(Dev);
    if (status == VL53L1_ERROR_NONE){
      newDistance[i] = RangingData.RangeMilliMeter;
      newDistanceStatus[i] = RangingData.RangeStatus;
      // newDistance[i] = (RangingData.RangeMilliMeter/10)*10;
    } 
  }

	// status = VL53L1_SetUserROI(Dev, &roiConfig1);
	// // while (digitalRead(INT));	// slightly faster
	// status = VL53L1_WaitMeasurementDataReady(Dev);
	// if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
	// VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
	// if (status == VL53L1_ERROR_NONE){
  //   newDistance[0] = RangingData.RangeMilliMeter;
  //   // newDistance[0] = (RangingData.RangeMilliMeter/10)*10;
  // } 

	// status = VL53L1_SetUserROI(Dev, &roiConfig2);
	// // while (digitalRead(INT));	// slightly faster
	// status = VL53L1_WaitMeasurementDataReady(Dev);
	// if (!status) status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);	//4mS
	// VL53L1_clear_interrupt_and_enable_next_range(Dev, VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT);	//2mS
	// if (status == VL53L1_ERROR_NONE){
  //   newDistance[1] = RangingData.RangeMilliMeter;
  //   // newDistance[1] = (RangingData.RangeMilliMeter/10)*10;
  // }

  // if (status == VL53L1_ERROR_NONE &&
  //     newDistanceStatus[0] != 7 &&
  //     newDistanceStatus[1] != 7
  // ){
  if (status == VL53L1_ERROR_NONE  ){
    this->distance[0] = newDistance[0];
    this->distance[1] = newDistance[1];
    // Serial.printf (" Distances: %d - %d -- Status: %d - %d\n", this->distance[0], this->distance[1], newDistanceStatus[0], newDistanceStatus[1]);
  } else {
    // Serial.println(" ++ ERROR reading ROI");
  }
  
  

  //MODE: Get Measurement if it´s ready:
  //-----------------------------------
  /*
  uint8_t isReady;

  status = VL53L1_SetUserROI(Dev, &roiConfig1);
  status = VL53L1_GetMeasurementDataReady(Dev, &isReady);
	if (!status){
    if(isReady){
      status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
      if (!status){ 
        // distance[0] = RangingData.RangeMilliMeter;
        distance[0] = (RangingData.RangeMilliMeter/10)*10;
      }
      // else Serial.print(F("\t\t\t error measurement. sensor 000000 \n"));
      VL53L1_ClearInterruptAndStartMeasurement(Dev);
    }
    // else Serial.printf("\t\t NOT READY. sensor 000000 --- STATUS: %d\n", status);

  }

	status = VL53L1_SetUserROI(Dev, &roiConfig2);
  status = VL53L1_GetMeasurementDataReady(Dev, &isReady);
	if (!status){
    if(isReady){
      status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
      if (!status) {
        // distance[1] = RangingData.RangeMilliMeter;
        distance[1] = (RangingData.RangeMilliMeter/10)*10;
      }
      // else Serial.print(F("\t\t\t error measurement. sensor 111111 \n"));
      VL53L1_ClearInterruptAndStartMeasurement(Dev);
    }
    // else Serial.printf("\t\t NOT READY. sensor 111111 --- STATUS: %d\n", status);
  }
  */
  



  // MODE: Get one measurement:
  //--------------------------
  /*
  uint8_t isReady;

  // non-blocking check for data ready
  status = VL53L1_GetMeasurementDataReady(Dev, &isReady);
  // Serial.printf("VL53L1_GetMeasurementDataReady Status: %d\n", status);
  if(!status) {
    if(isReady) {
      static VL53L1_RangingMeasurementData_t RangingData;
      status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
      if(!status) {
        distance[0] = RangingData.RangeMilliMeter;

        Serial.print(RangingData.RangeStatus);
        Serial.print(F(","));
        Serial.print(RangingData.RangeMilliMeter);
        Serial.print(F(","));
        Serial.print(RangingData.SignalRateRtnMegaCps/65536.0);
        Serial.print(F(","));
        Serial.println(RangingData.AmbientRateRtnMegaCps/65336.0);
      }
      VL53L1_ClearInterruptAndStartMeasurement(Dev);
      startMs = millis();
    }
    else if((uint16_t)(millis() - startMs) > VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS) {
      Serial.print(F("Timeout waiting for data ready.\n"));
      VL53L1_ClearInterruptAndStartMeasurement(Dev);
      startMs = millis();
    }
  } else {
    Serial.print(F("Error getting data ready: "));
    Serial.println(status);
  }
  */

  // Optional polling delay; should be smaller than INTER_MEASUREMENT_PERIOD_MS,
  // and MUST be smaller than VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS
  // delay(10);

	return status;
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

  // Parse each zone into roiZones[] configs depending on the number of zones selected:
  for (unsigned int i = 0; i < this->zones; i++){
    char zoneName[20];
    sprintf(zoneName, "zone_%d",i+1);
    if (this->debug) Serial.printf("Parsing zone object: %s\n", zoneName);
    this->roiZones[i].TopLeftX = configObject["ROI"][zoneName][0].as<uint8_t>();
    this->roiZones[i].TopLeftY = configObject["ROI"][zoneName][1].as<uint8_t>();
    this->roiZones[i].BotRightX = configObject["ROI"][zoneName][2].as<uint8_t>();
    this->roiZones[i].BotRightY = configObject["ROI"][zoneName][3].as<uint8_t>();
  }

  if (this->debug){
    Serial.printf("Configured roiZones[]: \n");
    for (int i = 0; i < this->zones; i++){
    // for (int i = 0; i < 4; i++){
      Serial.printf("\t-- roiZones[%d]:\n", i);
      Serial.printf("\t\t -TopLeftX: %d\n", roiZones[i].TopLeftX);
      Serial.printf("\t\t -TopLeftY: %d\n", roiZones[i].TopLeftY);
      Serial.printf("\t\t -BotRightX: %d\n", roiZones[i].BotRightX);
      Serial.printf("\t\t -BotRightY: %d\n", roiZones[i].BotRightY);
    }
  }
}
