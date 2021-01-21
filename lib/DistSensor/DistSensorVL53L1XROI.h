#ifndef DistSensorVL53L1XROI_H
#define DistSensorVL53L1XROI_H

#include <Arduino.h>
#include <DistSensor.h>

#include <Wire.h>
#include <tof_gestures.h>
#include <vl53l1_api.h>
#include <tof_gestures_DIRSWIPE_1.h>
// Add  lib_deps on platformio.ini

/*
People Counting Using a Single ST Time-of-Flight Sensor
Copyright 2019 Vladimir I. Yershov vlad.yer@gmail.com

This example illustrates direction recognition with the
single VL53L1X TOF sensor. Two Regions Of Interest(ROI)
are defined in SPAD sensor array:
  ROI1	  ROI2
0,15	12,15
  ####----####
  ####----####
  ####----####
  ...
  ####----####
  ####----####
	3,0		15,0
Data measurements distance0 and distance1 cyclically readed
from both ROIs.
Then STM32duino_Proximity_Gesture library used for event
interpretation.
NOTE:
tof_gestures_DIRSWIPE_1.cpp and tof_gestures_DIRSWIPE_1.h
need to be modified for  handMustCoverBoth parameter support

This code is for WEMOS D1 mini Lite, but with board and pins
adjustments will work with any Arduino
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
#define INT			16  // D0

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
#define MEASUREMENT_BUDGET_MS 0

// Interval between measurements, set through
// VL53L1_SetInterMeasurementPeriodMilliSeconds(). According to the API user
// manual (rev 2), "the minimum inter-measurement period must be longer than the
// timing budget + 4 ms." The STM32Cube example from ST uses 500 ms, but we
// reduce this to 55 ms to allow faster readings.
// #define INTER_MEASUREMENT_PERIOD_MS 55
#define INTER_MEASUREMENT_PERIOD_MS 15


class DistSensorVL53L1XROI: public DistSensor {

public:

  VL53L1_Dev_t		sensor;
  VL53L1_DEV			Dev = &sensor;

  String distance_mode;
  String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]
  int zones = 0;

  // Two ROI configurations
  VL53L1_UserRoi_t	roiConfig1 = { 12, 15, 15, 0 };
  VL53L1_UserRoi_t	roiConfig2 = { 0, 15, 3, 0 };

  // Four ROI configurations
  // VL53L1_UserRoi_t	roiZone1 = { 0, 15, 3, 0 };
  // VL53L1_UserRoi_t	roiZone2 = { 3, 15, 7, 0 };
  // VL53L1_UserRoi_t	roiZone3 = { 7, 15, 11, 0 };
  // VL53L1_UserRoi_t	roiZone4 = { 11, 15, 15, 0 };

  VL53L1_UserRoi_t	roiZone1 = { 0, 15, 15, 12 };
  VL53L1_UserRoi_t	roiZone2 = { 0, 11, 15, 8 };
  VL53L1_UserRoi_t	roiZone3 = { 0, 7, 15, 4 };
  VL53L1_UserRoi_t	roiZone4 = { 0, 3, 15, 0 };

  // Four ROI configurations on array:
  VL53L1_UserRoi_t	roiZone[4];

  int roiDistances[4] = { 0, 0, 0, 0 };

  Gesture_DIRSWIPE_1_Data_t gestureDirSwipeData;
  int status, i, distance[2] = { 0, 0 };
  int left = 0, right = 0, cnt = 0, oldcnt;
  volatile int LightON = 0, OLED_dimmed = 0, OLED_OFF_timeout = 10000;
  long timeMark = 0, DisplayUpdateTime = 0;

  int rangeThresholdCounter_mm = 600;

  // String distance_mode;
  // String distance_mode_options[JSON_MAX_SIZE_LIST]; //["Short", "Medium", "Long", "Unknown"]


  DistSensorVL53L1XROI(void);
  ~DistSensorVL53L1XROI();

  void setup(void);
  void disableSensor(void);
  bool sensorRead(float *distance);
  int sensorCountPersons(void);
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

  inline void dispUpdate() {	// 33mS
  	// OLED_dimmed = 0;
  	// display.clearDisplay();
  	// display.setCursor(0, 0);
  	// if (right)display.println("--->");
  	// else display.println("<---");
  	// display.print(cnt);
  	// if (LightON)	display.print(" * ");
  	// else display.print("   ");
  	// display.display();
  	// DisplayUpdateTime = millis();
  }

  void lighton(void) {
  	pinMode(light_on, OUTPUT);
  	digitalWrite(light_on, LOW);
  	delay(100);
  	pinMode(light_on, INPUT);
  }

  void lightoff(void) {
  	pinMode(light_off, OUTPUT);
  	digitalWrite(light_off, LOW);
  	delay(100);
  	pinMode(light_off, INPUT);
  }


};
#endif
