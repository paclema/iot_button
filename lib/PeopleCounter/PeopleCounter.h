#ifndef PeopleCounter_H
#define PeopleCounter_H

#include <PubSubClient.h>
#include "Ticker.h"

#define JSON_MAX_SIZE_LIST 6

// #include <LinkedList.h>
// Using AsyncWebServer LinkedList lib can not be used because there is a class
// using the same name. For that reason, for now we use SimpleList until we fix
// this using namespace for example.
// #include <SimpleList.h>
#include "IWebConfig.h"
#include "DistSensorVL53L1XROI.h"

#include <Adafruit_NeoPixel.h>


#define STATUS_PERSON_ARRAY_SIZE 5

typedef int8_t PeopleCounterGesture;
#define NO_PERSON_DETECTED                ((PeopleCounterGesture)   0)
#define PERSON_ENTERS                     ((PeopleCounterGesture)   1)
#define PERSON_LEAVES                     ((PeopleCounterGesture)   2)
#define PERSON_TRY_TO_ENTER               ((PeopleCounterGesture)   3)
#define PERSON_TRY_TO_LEAVE               ((PeopleCounterGesture)   4)
#define PERSON_IN_RANGE                   ((PeopleCounterGesture)   5)
#define PERSON_IN_RANGE_START             ((PeopleCounterGesture)   6)
#define PERSON_IN_RANGE_END               ((PeopleCounterGesture)   7)
#define PERSON_IN_RANGE_IN_OUT            ((PeopleCounterGesture)   8)

#define ERROR_PERSON_TOO_FAST             ((PeopleCounterGesture)  -1)
#define ERROR_DETECTING_PERSON            ((PeopleCounterGesture)  -2)
#define ERROR_PERSON_NOT_FULL_DETECTED    ((PeopleCounterGesture)  -3)
#define ERROR_READING_SENSOR              ((PeopleCounterGesture)  -4)


#define DIST_PRE_GESTURE_ARRAY_SIZE 20
struct ZoneDistances {
  int dist1;
  int dist2;
  int loopDelay;
};

class PeopleCounter: public IWebConfig {

private:

  bool debug = false;
  bool peopleCounterInitialized  = false;

  ZoneDistances zoneDistPreGesture[DIST_PRE_GESTURE_ARRAY_SIZE] = {};
  int wIndexPreGesture = 0;

  int statusPerson[STATUS_PERSON_ARRAY_SIZE];
  int statusPersonNow = 0, statusPersonLast = 0, statusPersonIndex = 0;
  int statusFront = 0;
  int statusBack = 0;

  int  cnt = 0;
  long timeMark = 0;

  int rangeThresholdCounter_mm = 1200;

  PeopleCounterGesture currentGesture = NO_PERSON_DETECTED;
  PeopleCounterGesture lastGesture = currentGesture;
  PubSubClient *mqttClient;
  Ticker ledOn, ledOff;

  unsigned long currentMillis = millis();
  unsigned long lastMillis = currentMillis;


  String mqttBaseTopic = "/";


  // LDR sensor:
  bool LDREnabled = false;
  uint16_t LDRPin = A0;
  int LDRValue = 0;


  // LED strip WS2812B:
  Adafruit_NeoPixel pixels;
  bool ledEnabled = false;
  uint16_t ledCount = 1;
  uint16_t ledPin = 13;  // Pin D7 or GPIO13
  uint8_t ledBrightness = 255;  // 0-255
  neoPixelType ledType = NEO_GRB + NEO_KHZ800;
  uint32_t ledDefaultColor = pixels.Color(255,255,255);


  // Reed switch:
  bool reedSwitchEnabled = false;
  uint16_t reedSwitchPin = 12;  // Pin D6 or GPIO12
  bool reedSwitchState = false;
  bool reedSwitchStateLast = false;



public:

  DistSensorVL53L1XROI sensor;

  PeopleCounter(void);
  PeopleCounter(String name);

  void parseWebConfig(JsonObjectConst configObject);
  
  void enablePeopleCounterServices(void);
  void disableDistSensors(void);

  void setupDistSensors(void);
  void setupLEDStrip(void);
  void setupReedSwitch(void);

  void setLEDStripColor(uint32_t c);
  void setLEDStripColor(uint8_t r, uint8_t g, uint8_t b);
  void updateReedSwitch(void);

  void loop(void);

  void processGesture(void);

  void notifyGesture(PeopleCounterGesture);
  void notifyZoneDistances(void);
  void notifyStatusPerson(void);
  void notifyData(void);
  void notifyReedSwitch(void);
  
  void setMQTTClient(PubSubClient *client){ this->mqttClient = client; }
  void setMQTTBaseTopic(String topic){ this->mqttBaseTopic = topic; }

  void printStatus(void);

  String getPeopleCount(void){ return String(cnt); };
  String getDistZone(int i){ return String(sensor.distance[i]); };
  String getStatusFront(){ return String(statusFront); };
  String getStatusBack(){ return String(statusBack); };
  String getStatusPersonNow(){ return String(statusPersonNow); };
  String getCurrentGesture(){ return String(currentGesture); };
  String getLDR(void){ return String(LDRValue); };
  String getReedSwitch(void){ return String(reedSwitchState); };

  void setDebug(bool d){ this->debug = d; };
  bool isDebug(void){ return this->debug; };


  static void blink(bool status){
    pinMode(LED_BUILTIN, OUTPUT);
    // Serial.printf("Blinking to %d", status);
    digitalWrite(LED_BUILTIN, !status); 
  };


  void clearZoneDistances(void);
  void addZoneDistances(int distance1, int distance2);
  void printZoneDistances(void);
  String getLastZoneDistances(void);

};
#endif
