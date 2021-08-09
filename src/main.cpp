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


// Radar:
#include "Radar.h"
// Radar *radar =  new radar("radar");
Radar radar;



#include <PubSubClient.h>
PubSubClient * mqttClient;
String mqttQueueString = "{\"data\":[";


// Websocket functions to publish:
String getLoopTime(){ return String(currentLoopMillis - previousMainLoopMillis);}
String getRSSI(){ return String(WiFi.RSSI());}
String getHeapFree(){ return String((float)GET_FREE_HEAP/1000);}
String mqttState(){ return String(mqttClient->state() );}
String mqttBufferSize(){ return String(mqttClient->getBufferSize() );}
String getPositionMotor1(){ return String(radar.getPosition(1) );}
String getTargetPositionMotor1(){ return String(radar.getTargetPosition(1) );}
String getPositionMotor2(){ return String(radar.getPosition(2) );}
String getTargetPositionMotor2(){ return String(radar.getTargetPosition(2) );}
String getMqttQueueString(){ return String((float)mqttQueueString.length()/1000 );}



void setup() {
  Serial.begin(115200);
  
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.setDebugOutput(true);
  #endif

  config.addConfig(radar, "radar");

  config.begin();

  // Enable Radar services:
  radar.enableRadarServices();
 
  config.addDashboardObject("heap_free", getHeapFree);
  config.addDashboardObject("loop", getLoopTime);
  config.addDashboardObject("RSSI", getRSSI);
  config.addDashboardObject("mqtt_state", mqttState);
  config.addDashboardObject("mqtt_buffer_size", mqttBufferSize);
  config.addDashboardObject("mqtt_queue_string_size", getMqttQueueString);
  config.addDashboardObject("radar_motor_1_position", getPositionMotor1);
  config.addDashboardObject("radar_motor_1_target_position", getTargetPositionMotor1);
  config.addDashboardObject("radar_motor_2_position", getPositionMotor2);
  config.addDashboardObject("radar_motor_2_target_position", getTargetPositionMotor2);
    
  mqttClient = config.getMQTTClient();


  Serial.println("###  Looping time\n");

  // previousMainLoopMillis = millis();
}

void loop() {
  currentLoopMillis = millis();
  //  ----------------------------------------------
  //
  // Config services Loop:
  //  ----------------------------------------------

  config.loop();

  // Reconnection loop:
  // if (WiFi.status() != WL_CONNECTED) {
  //   config.begin();
  //   networkRestart();
  //   config.configureServer(&server);
  // }

  // Reset mqtt buffer if mqtt not connected to avoid memory leaking
  if (mqttClient->state() != 0){
    mqttQueueString = "{\"data\":[";
  }

  // Radar services loop:
  radar.loop();


  // Main Loop:
  if((config.device.loop_time_ms != 0 ) &&
      (currentLoopMillis - previousLoopMillis > (unsigned)config.device.loop_time_ms)) {
    // Here starts the device loop configured:


    // float radarAngle = 0;
    // float radarDistance = 0;
    // radarAngle = radar.getPosition();
    radar.printStatus();


    // If radar measure is not available, mqtt is not sent:
    // if (radar.getDistance(radarDistance)){
    if (radar.readPoints()){


      // String msg_pub ="{\"angle\":" + String(radarAngle) + ", \"distance\" :"+ String(radarDistance) +"}";
      String msg_pub = radar.getJsonPoints();
      mqttQueueString += msg_pub;

      // Publish mqtt sensor feedback:
      if(mqttClient->connected() && (config.device.publish_time_ms != 0) &&
          (currentLoopMillis - previousPublishMillis > (unsigned)config.device.publish_time_ms)) {
        previousPublishMillis = currentLoopMillis;
        // Here starts the MQTT publish loop configured:

          mqttQueueString += "]}";

          String topic = config.getDeviceTopic() + "data";
          mqttClient->setBufferSize((uint16_t)(mqttQueueString.length() + 100));
          mqttClient->publish(topic.c_str(), mqttQueueString.c_str(), mqttQueueString.length());
          // Serial.println("MQTT published: " + msg_pub + " -- loop: " + config.device.publish_time_ms);

          mqttQueueString = "{\"data\":[";
          previousPublishMillis = currentLoopMillis;
      } else {
        mqttQueueString += ",";
      }
    }
    previousLoopMillis = currentLoopMillis;
  }

previousMainLoopMillis = currentLoopMillis;
}
