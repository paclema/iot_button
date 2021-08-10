#include <Arduino.h>


// Main variables:
// #define DEBUG_ESP_CORE

// Enable wifi diagnostic using platformio build_glag: -D ENABLE_SERIAL_DEBUG:
#define ENABLE_SERIAL_DEBUG true


// Device configurations
unsigned long currentLoopMillis = 0;
unsigned long previousLoopMillis = 0;
unsigned long previousPublishMillis = 0;
unsigned long previousMainLoopMillis = 0;


// WebConfigServer Configuration
#include "WebConfigServer.h"
WebConfigServer config;   // <- global configuration object

// PeopleCounter:
#include "PeopleCounter.h"
// PeopleCounter *PeopleCounter =  new PeopleCounter("PeopleCounter");
PeopleCounter peopleCounter;


#include <PubSubClient.h>
PubSubClient * mqttClient;


// Websocket functions to publish:
String getLoopTime(){ return String(currentLoopMillis - previousMainLoopMillis);}
String getRSSI(){ return String(WiFi.RSSI());}
String getHeapFree(){ return String((float)GET_FREE_HEAP/1000);}
String mqttState(){ return String(mqttClient->state() );}
String mqttBufferSize(){ return String(mqttClient->getBufferSize() );}
String getPeopleCount(){ return String(peopleCounter.getPeopleCount());}
String getZone1(){ return String(peopleCounter.getDistZone(0));}
String getZone2(){ return String(peopleCounter.getDistZone(1));}
String getStatusFront(){ return String(peopleCounter.getStatusFront());}
String getStatusBack(){ return String(peopleCounter.getStatusBack());}
String getStatusPersonNow(){ return String(peopleCounter.getStatusPersonNow());}
String getCurrentGesture(){ return String(peopleCounter.getCurrentGesture());}
String getLDR(){ return String(peopleCounter.getLDR());}
String getReedSwitch(){ return String(peopleCounter.getReedSwitch());}


void setup() {
  Serial.begin(115200);
  
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.setDebugOutput(true);
  #endif

  // Add PeopleCounter object into WebConfigServer IWebCOnfig object list to parse its configurations from config.json
  config.addConfig(peopleCounter, "PeopleCounter");

  config.begin();

  mqttClient = config.getMQTTClient();

  // Enable PeopleCounter services:
  peopleCounter.setMQTTClient(mqttClient);
  peopleCounter.setMQTTBaseTopic(config.getDeviceTopic());
  peopleCounter.enablePeopleCounterServices();
 
  config.addDashboardObject("heap_free", getHeapFree);
  config.addDashboardObject("loop", getLoopTime);
  config.addDashboardObject("RSSI", getRSSI);
  config.addDashboardObject("mqtt_state", mqttState);
  config.addDashboardObject("mqtt_buffer_size", mqttBufferSize);
  config.addDashboardObject("dist_zone_1", getZone1);
  config.addDashboardObject("dist_zone_2", getZone2);
  config.addDashboardObject("status_front", getStatusFront);
  config.addDashboardObject("status_back", getStatusBack);
  config.addDashboardObject("status_person_now", getStatusPersonNow);
  config.addDashboardObject("current_gesture", getCurrentGesture);
  config.addDashboardObject("people_count", getPeopleCount);
  config.addDashboardObject("LDR", getLDR);
  config.addDashboardObject("reedSwitch", getReedSwitch);
    

  Serial.println("\n\n###  Looping time");

  // previousMainLoopMillis = millis();
}

void loop() {
  currentLoopMillis = millis();
  //  ----------------------------------------------
  //
  // Config services Loop:
  //  ----------------------------------------------

  config.loop();

  // peopleCounter.setSendTimestamp(config.getTimeSet());

  // Reconnection loop:
  // if (WiFi.status() != WL_CONNECTED) {
  //   config.begin();
  //   networkRestart();
  //   config.configureServer(&server);
  // }


  // Main Loop:
  if((config.device.loop_time_ms != 0 ) &&
      (currentLoopMillis - previousLoopMillis > (unsigned)config.device.loop_time_ms)) {
    // Here starts the device loop configured:


    
    // PeopleCounter services loop:
    peopleCounter.loop();
    peopleCounter.printStatus();


    // Publish mqtt sensor feedback:
    if(mqttClient->connected() && (config.device.publish_time_ms != 0) &&
      (currentLoopMillis - previousPublishMillis > (unsigned)config.device.publish_time_ms)) {
      
      // Here starts the MQTT publish loop configured:
      peopleCounter.notifyData();
      previousPublishMillis = currentLoopMillis;
    }
    
    previousLoopMillis = currentLoopMillis;
  }

previousMainLoopMillis = currentLoopMillis;
}
