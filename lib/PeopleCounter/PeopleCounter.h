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


#define STATUS_PERSON_ARRAY_SIZE 5

typedef int8_t PeopleCounterGesture;
#define NO_PERSON_DETECTED              ((PeopleCounterGesture)   0)
#define PERSON_ENTERS                   ((PeopleCounterGesture)   1)
#define PERSON_LEAVES                   ((PeopleCounterGesture)   2)
#define PERSON_TRY_TO_ENTER             ((PeopleCounterGesture)   3)
#define PERSON_TRY_TO_LEAVE             ((PeopleCounterGesture)   4)

#define ERROR_PERSON_TOO_FAST           ((PeopleCounterGesture)  -1)
#define ERROR_DETECTING_PERSON          ((PeopleCounterGesture)  -2)
#define PERSON_NOT_FULL_DETECTED        ((PeopleCounterGesture)  -3)



class PeopleCounter: public IWebConfig {

private:

  bool debug = false;
  bool peopleCounterInitialized  = false;
  
  int statusPerson[STATUS_PERSON_ARRAY_SIZE];
  int statusPersonNow = 0, statusPersonLast = 0, statusPersonIndex = 0;
  int statusFront = 0;
  int statusBack = 0;

  int  cnt = 0;
  long timeMark = 0;

  int rangeThresholdCounter_mm = 1200;

  PeopleCounterGesture currentGesture = NO_PERSON_DETECTED;
  PubSubClient *mqttClient;
  Ticker ledOn, ledOff;

public:

  DistSensorVL53L1XROI sensor;

  PeopleCounter(void);
  PeopleCounter(String name);

  void parseWebConfig(JsonObjectConst configObject);

  void enablePeopleCounterServices(void);
  void disableDistSensors(void);
  void setupDistSensors(void);
  void loop(void);

  void notifyChange(PeopleCounterGesture);
  void setMQTTClient(PubSubClient *client){ mqttClient = client; }

  void printStatus(void);

  String getPeopleCount(void){ return String(cnt); };
  String getDistZone(int i){ return String(sensor.distance[i]); };
  String getStatusFront(){ return String(statusFront); };
  String getStatusBack(){ return String(statusBack); };
  String getStatusPersonNow(){ return String(statusPersonNow); };
  String getCurrentGesture(){ return String(currentGesture); };


  void setDebug(bool d){ this->debug = d; };
  bool isDebug(void){ return this->debug; };


  static void blink(bool status){
    pinMode(LED_BUILTIN, OUTPUT);
    // Serial.printf("Blinking to %d", status);
    digitalWrite(LED_BUILTIN, !status); 
  };

};
#endif
