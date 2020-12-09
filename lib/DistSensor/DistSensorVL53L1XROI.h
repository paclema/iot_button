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
#define SCL			5	// D1
#define XSHUT		14	// D5
#define INT			D6

#define light_on	0	//D3
// #define light_off	4	//D2
#define light_off	2

#define dev1_sel	digitalWrite(XSHUT, HIGH);
#define dev1_desel	digitalWrite(XSHUT, LOW);


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
  bool sensorRead(float *distance);
  int sensorRead(void);
  String getSensorRange(void);
  void printSensorStatus(void);

  void free() { if (this->debug) Serial.println("\t\t\tfree DistSensorVL53L1XROI");  delete this; }

  void parseWebConfig(JsonObjectConst configObject);

  void checkDev(VL53L1_DEV Dev) {
  	uint16_t wordData;
  	VL53L1_RdWord(Dev, 0x010F, &wordData);
  	Serial.printf("DevAddr: 0x%X VL53L1X: 0x%X\n\r", Dev->I2cDevAddr, wordData);
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
