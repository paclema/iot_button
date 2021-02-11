#ifndef DistSensorVL53L1XROI_H
#define DistSensorVL53L1XROI_H

#include <Arduino.h>
#include <DistSensor.h>

#include <Wire.h>
#include <vl53l1_api.h>

/*
For Two Regions Of Interest(ROI)
are defined in SPAD sensor array:

// NARROW:
  ROI1	  ROI2
0,15	     12,15
  ####--------####
  ####--------####
  ####--------####
  ...
  ####--------####
  ####--------####
	   3,0		     15,0

// WIDE:
  ROI1	  ROI2
0,15	     12,15
  #######--#######
  #######--#######
  #######--#######
  ...
  #######--#######
  #######--#######
	   3,0		     15,0

*/

// Pinout with D1 mini:
//--------------------
//
// SDA -- D1
// SCL -- D2
// VIN -- 3.3V
// GND -- GND
#define  MAX_INIT_RETRIES    20
// #define SDA			2	// D4
#define SDA			4
#define SCL			5   // D1
#define XSHUT		14	// D5
// #define INT			16  // D0
#define INT			12  // D6

#define light_on	0	//D3
// #define light_off	4	//D2
#define light_off	2

#define dev1_sel	digitalWrite(XSHUT, HIGH);
#define dev1_desel	digitalWrite(XSHUT, LOW);

// By default, this example blocks while waiting for sensor data to be ready.
// Comment out this line to poll for data ready in a non-blocking way instead.
// #define USE_BLOCKING_LOOP

// For non bloquing use VL53L1_GetMeasurementDataReady() with measurement data ready pointer:
// #define USE_MEASUREMENT_DATA_READY_POINTER


// Timing budget set through VL53L1_SetMeasurementTimingBudgetMicroSeconds().
// #define MEASUREMENT_BUDGET_MS 50
#define MEASUREMENT_BUDGET_MS 33

// Interval between measurements, set through
// VL53L1_SetInterMeasurementPeriodMilliSeconds(). According to the API user
// manual (rev 2), "the minimum inter-measurement period must be longer than the
// timing budget + 4 ms." The STM32Cube example from ST uses 500 ms, but we
// reduce this to 55 ms to allow faster readings.
// #define INTER_MEASUREMENT_PERIOD_MS 55
#define INTER_MEASUREMENT_PERIOD_MS (MEASUREMENT_BUDGET_MS + 4)


class DistSensorVL53L1XROI: public DistSensor {

public:

  VL53L1_Dev_t		sensor;
  VL53L1_DEV			Dev = &sensor;

  String distance_mode;
  String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]
  int zones = 0;
  VL53L1_Error status = 0;
  int32_t CalDistanceMilliMeter = 200;

  // Two ROI configurations
  //NARROW:
  // VL53L1_UserRoi_t	roiConfig1 = { 12, 15, 15, 0 };
  // VL53L1_UserRoi_t	roiConfig2 = { 0, 15, 3, 0 };
  //WIDE:
  VL53L1_UserRoi_t	roiConfig1 = { 9, 15, 15, 0 };
  VL53L1_UserRoi_t	roiConfig2 = { 0, 15, 6, 0 };
  int distance[2] = { 0, 0 };

  // Four ROI configurations
  // VL53L1_UserRoi_t	roiZone1 = { 0, 15, 3, 0 };
  // VL53L1_UserRoi_t	roiZone2 = { 3, 15, 7, 0 };
  // VL53L1_UserRoi_t	roiZone3 = { 7, 15, 11, 0 };
  // VL53L1_UserRoi_t	roiZone4 = { 11, 15, 15, 0 };

  VL53L1_UserRoi_t	roiZone1 = { 0, 15, 15, 12 };
  VL53L1_UserRoi_t	roiZone2 = { 0, 11, 15, 8 };
  VL53L1_UserRoi_t	roiZone3 = { 0, 7, 15, 4 };
  VL53L1_UserRoi_t	roiZone4 = { 0, 3, 15, 0 };

  VL53L1_UserRoi_t	roiZone[4];

  int roiDistances[4] = { 0, 0, 0, 0 };


  // String distance_mode;
  // String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]


  DistSensorVL53L1XROI(void);
  ~DistSensorVL53L1XROI();

  void setup(void);
  int calibrate(int CalDistanceMilliMeter);
  void disableSensor(void);
  bool sensorRead(float *distance);
  bool sensorRead2Roi(void);
  String getSensorRange(void);
  void printSensorStatus(void);

  void free() { if (this->debug) Serial.println("\t\t\tfree DistSensorVL53L1XROI");  delete this; }

  void parseWebConfig(JsonObjectConst configObject);

  void checkDev(VL53L1_DEV Dev) {
    uint8_t byteData;
  	uint16_t wordData;
  	// VL53L1_RdWord(Dev, 0x010F, &wordData);
  	// Serial.printf("DevAddr: 0x%X VL53L1X: 0x%X\n\r", Dev->I2cDevAddr, wordData);

    VL53L1_RdByte(Dev, 0x010F, &byteData);
    Serial.print(F("VL53L1X Model_ID: "));
    Serial.println(byteData, HEX);
    VL53L1_RdByte(Dev, 0x0110, &byteData);
    Serial.print(F("VL53L1X Module_Type: "));
    Serial.println(byteData, HEX);
    VL53L1_RdWord(Dev, 0x010F, &wordData);
    Serial.print(F("VL53L1X: "));
    Serial.println(wordData, HEX);
  }

};
#endif
