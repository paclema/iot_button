#include <Arduino.h>



// Main variables:
// #define DEBUG_ESP_CORE
#define ENABLE_SERIAL_DEBUG true
unsigned long connectionTime = millis();
unsigned long setupDeviceTime;

// WiFi
#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <ESPmDNS.h>
  WiFiMulti wifiMulti;

#elif defined(ESP8266)
  // Used for light sleep
  extern "C" {
    #include "user_interface.h"
  }

  #include <ESP8266WiFi.h>
  #include <ESP8266WiFiMulti.h>
  #include <ESP8266mDNS.h>
  ESP8266WiFiMulti wifiMulti;
#endif



// MQTT
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
WiFiClientSecure wifiClientSecure;    // To use with mqtt and certificates
WiFiClient wifiClient;                // To use with mqtt without certificates
PubSubClient mqttClient;
unsigned long previousMqttReconnectionMillis = millis();
unsigned int mqttReconnectionTime = 10000;
int mqttRetries = 0;
int mqttMaxRetries = 10;
String mqttQueueString = "{\"data\":[";


// WebConfigServer Configuration
#include "WebConfigServer.h"
WebConfigServer config;   // <- global configuration object

// To enable asyc webserver use platformio build_flags = -D USE_ASYNC_WEBSERVER
// Or define here and in WebCOnfigServer.h:
// #define USE_ASYNC_WEBSERVER

#ifdef ESP32
  #ifdef USE_ASYNC_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    AsyncWebServer server(80);
  #else
    #include <WebServer.h>
    WebServer server(80);
  #endif

#elif defined(ESP8266)
  #ifdef USE_ASYNC_WEBSERVER
    #include <ESPAsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    AsyncWebServer server(80);
  #else
    #include <ESP8266WebServer.h>
    ESP8266WebServer server(80);
  #endif
#endif



// FTP server
#include <ESP8266FtpServer.h>
#include <FS.h>
FtpServer ftpSrv;

// OTA Includes
#include "WrapperOTA.h"
// #include <display2.h>
WrapperOTA ota;

// Device configurations
unsigned long currentLoopMillis = 0;
unsigned long previousLoopMillis = 0;
unsigned long previousMQTTPublishMillis = 0;
unsigned long previousWSMillis = 0;
unsigned long previousMainLoopMillis = 0;

// Websocket server:
#include <WrapperWebSockets.h>
WrapperWebSockets ws;

// Radar:
#include "Radar.h"
// Radar *radar =  new radar("radar");
Radar radar;


// Websocket functions to publish:
String getLoopTime(){ return String(currentLoopMillis - previousMainLoopMillis);}
String getRSSI(){ return String(WiFi.RSSI());}
String getHeapFree(){ return String((float)GET_FREE_HEAP/1000);}
String mqttState(){ return String(mqttClient.state() );}
String mqttBufferSize(){ return String(mqttClient.getBufferSize() );}
String getPositionMotor1(){ return String(radar.getPosition(1) );}
String getTargetPositionMotor1(){ return String(radar.getTargetPosition(1) );}
String getPositionMotor2(){ return String(radar.getPosition(2) );}
String getTargetPositionMotor2(){ return String(radar.getTargetPosition(2) );}
String getMqttQueueString(){ return String((float)mqttQueueString.length()/1000 );}






// MAIN FUNCTIONS:
// --------------
//

void networkRestart(void){
  if(config.status() == CONFIG_LOADED){

    // WiFi setup:
    #ifdef ESP32
      WiFi.mode(WIFI_MODE_APSTA);
    #elif defined(ESP8266)
      // WiFi.mode(WIFI_STA);
      WiFi.mode(WIFI_AP_STA);
    #endif

    // Config acces point:
    if (config.network.ap_name!=NULL &&
        config.network.ap_password!=NULL){
          Serial.print("Setting soft-AP ... ");
          Serial.println(WiFi.softAP(config.network.ap_name.c_str(),
                      config.network.ap_password.c_str(),
                      config.network.ap_channel,
                      config.network.ap_ssid_hidden,
                      config.network.ap_max_connection) ? "Ready" : "Failed!");
          IPAddress myIP = WiFi.softAPIP();
          Serial.print(config.network.ap_name);Serial.print(" AP IP address: ");
          Serial.println(myIP);
    }

    // Client Wifi config:
    if (config.network.ssid_name!=NULL && config.network.ssid_password!=NULL){

      wifiMulti.addAP(config.network.ssid_name.c_str(),config.network.ssid_password.c_str());

      Serial.print("Connecting to ");Serial.println(config.network.ssid_name);
      while (wifiMulti.run() != WL_CONNECTED) {
        delay(200);
        Serial.print('.');
      }
      Serial.print("\n\nConnected to ");Serial.print(WiFi.SSID());
      Serial.print("\nIP address:\t");Serial.println(WiFi.localIP());
    }

    // Configure device hostname:
    if (config.network.hostname){
      if (MDNS.begin(config.network.hostname.c_str())) Serial.println("mDNS responder started");
      else Serial.println("Error setting up MDNS responder!");
    }

  }
}

void enableServices(void){
  Serial.println("\n--- Services: ");

  if (config.services.ota){
    // ota.init(&display);
    ota.init(&config);
    Serial.println("   - OTA -> enabled");
  } else Serial.println("   - OTA -> disabled");


  if (config.services.ftp.enabled && config.services.ftp.user !=NULL && config.services.ftp.password !=NULL){
    ftpSrv.begin(config.services.ftp.user,config.services.ftp.password);
    Serial.println("   - FTP -> enabled");
  } else Serial.println("   - FTP -> disabled");

  if (config.services.webSockets.enabled){
    ws.init();
    Serial.println("   - WebSockets -> enabled");
  } else Serial.println("   - WebSockets -> disabled");

  if (config.services.deep_sleep.enabled){
    // We will enable it on the loop function
    // Serial.println("   - Deep sleep -> configured");
    Serial.print("   - Deep sleep -> enabled for ");
    Serial.print(config.services.deep_sleep.sleep_time);
    Serial.print("s after waitting ");
    Serial.print(config.services.deep_sleep.sleep_delay);
    Serial.println("s. Choose sleep_time: 0 for infinite sleeping");
    Serial.println("     Do not forget to connect D0 to RST pin to auto-wake up! Or I will sleep forever");
  } else Serial.println("   - Deep sleep -> disabled");


  #ifdef ESP32
    // TODO: enable sleep modes for ESP32 here

  #elif defined(ESP8266)
    if (config.services.light_sleep.enabled){
      if (config.services.light_sleep.mode == "LIGHT_SLEEP_T")
        wifi_set_sleep_type(LIGHT_SLEEP_T);
      else if (config.services.light_sleep.mode == "NONE_SLEEP_T")
        wifi_set_sleep_type(NONE_SLEEP_T);
      else {
        Serial.println("   - Light sleep -> mode not available");
        return;
      }
      Serial.println("   - Light sleep -> enabled");
    } else Serial.println("   - Light sleep -> disabled");
  #endif

  Serial.println("");

}

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  // Serial.print("Message arrived [");
  // Serial.print(topic);
  // Serial.print("] ");

  char buff[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    buff[i] = (char)payload[i];
  }


  buff[length] = '\0';
  String message(buff);

  // Serial.print(message);
  // Serial.println();

/*
  if (strcmp(topic, "/lamp") == 0) {
    //Lamp color request:
    if (message.equals("red")){
      Serial.println("Turning lamp to red");
      //colorWipe(strip.Color(255, 0, 0), 10);
    }
    else if (strcmp(buff, "blue") == 0){
        Serial.println("Turning lamp to blue");
        //colorWipe(strip.Color(0, 0, 255), 10);
    } else if (message.equals("green")){
        Serial.println("Turning lamp to green");
        //colorWipe(strip.Color(0, 255, 0), 10);
    }
    //client.publish((char*)"/lamp",(char*)"color changed");
  }
*/

  // Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  ///

}

void initMQTT(void){

  if (config.mqtt.enable_certificates){
    mqttClient.setClient(wifiClientSecure);
    Serial.println("Configuring MQTT using certificates");
  } else {
    mqttClient.setClient(wifiClient);
    Serial.println("Configuring MQTT without certificates");
  }

  mqttClient.setServer(config.mqtt.server.c_str(), config.mqtt.port);
  mqttClient.setCallback(callbackMQTT);

  if (config.mqtt.enable_certificates){
    // Load certificate file:
    // But you must convert it to .der
    // openssl x509 -in ./certs/IoLed_controller/client.crt -out ./certs/IoLed_controller/cert.der -outform DER
    File cert = SPIFFS.open(config.mqtt.cert_file, "r"); //replace cert.crt with your uploaded file name
    if (!cert) Serial.println("Failed to open cert file ");
    else Serial.println("Success to open cert file");

    if (wifiClientSecure.loadCertificate(cert, cert.size())) Serial.println("cert loaded");
    else Serial.println("cert not loaded");
    cert.close();

    // Load private key:
    // But you must convert it to .der
    // openssl rsa -in ./certs/IoLed_controller/client.key -out ./certs/IoLed_controller/private.der -outform DER
    File private_key = SPIFFS.open(config.mqtt.key_file, "r");
    if (!private_key) Serial.println("Failed to open key file ");
    else Serial.println("Success to open key file");

    if (wifiClientSecure.loadPrivateKey(private_key, private_key.size())) Serial.println("key loaded");
    else Serial.println("key not loaded");
    private_key.close();

    // Load CA file:
    File ca = SPIFFS.open(config.mqtt.ca_file, "r");
    if (!ca) Serial.println("Failed to open CA file ");
    else Serial.println("Success to open CA file");

    if (wifiClientSecure.loadCACert(ca, ca.size())) Serial.println("CA loaded");
    else Serial.println("CA not loaded");
    ca.close();
  }

}

void reconnectMQTT() {
  // Loop until we're reconnected
  if (currentLoopMillis - previousMqttReconnectionMillis > mqttReconnectionTime){
    if (!mqttClient.connected() && (mqttRetries <= mqttMaxRetries) ) {
      bool mqttConnected = false;
      Serial.print("Attempting MQTT connection... ");
      String mqttWillTopic = "/" + config.mqtt.id_name + "/connected";
      uint8_t mqttWillQoS = 2;
      boolean mqttWillRetain = true;
      String mqttWillMessage = "false";
      if (config.mqtt.enable_user_and_pass)
        mqttConnected = mqttClient.connect(config.mqtt.id_name.c_str(),
                                            config.mqtt.user_name.c_str(),
                                            config.mqtt.user_password.c_str(),
                                            mqttWillTopic.c_str(),
                                            mqttWillQoS,
                                            mqttWillRetain,
                                            mqttWillMessage.c_str());
      else
        mqttConnected = mqttClient.connect(config.mqtt.id_name.c_str(),
                                            mqttWillTopic.c_str(),
                                            mqttWillQoS,
                                            mqttWillRetain,
                                            mqttWillMessage.c_str());

      if (mqttConnected) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        String base_topic_pub = "/" + config.mqtt.id_name + "/";
        String topic_connected_pub = base_topic_pub + "connected";
        String msg_connected ="true";
        mqttClient.publish(topic_connected_pub.c_str(), msg_connected.c_str(), true);
        // ... and resubscribe
        String base_topic_sub = base_topic_pub + "#";
        mqttClient.subscribe(base_topic_sub.c_str());

        long time_now = millis() - connectionTime;
        mqttRetries = 0;
        Serial.print("Time to connect MQTT client: ");
        Serial.print((float)time_now/1000);
        Serial.println("s");

      } else {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.print(" try again in ");
        Serial.print(mqttReconnectionTime/1000);
        Serial.print("s: ");
        Serial.print(mqttRetries);
        Serial.print("/");
        Serial.println(mqttMaxRetries);

      }
      previousMqttReconnectionMillis = millis();
      mqttRetries++;
    }
  }
}

void reconnect(void) {
  //delay(1000);
  Serial.print("--- Free heap: "); Serial.println(ESP.getFreeHeap());

  // Add radar object into WebConfigServer IWebCOnfig object list to parse its configurations from config.json
  config.addConfig(&radar, "radar");

  config.begin();
  networkRestart();
  config.configureServer(&server);

  // Enable services:
  enableServices();

  // Configure MQTT broker:
  if (config.mqtt.enabled) {
    initMQTT();
    if (config.mqtt.reconnect_mqtt)
      reconnectMQTT();
  }


  // Enable Radar services:
  radar.enableRadarServices();


}


void deepSleepHandler() {
  Serial.print("sleep_delay: "); Serial.print((config.services.deep_sleep.sleep_delay));
  Serial.print(" setupDeviceTime: "); Serial.print( (float)setupDeviceTime/1000);
  Serial.print(" currentLoopMillis: "); Serial.println((float)currentLoopMillis/1000);

  #ifdef ESP32


  #elif defined(ESP8266)
    if (currentLoopMillis > setupDeviceTime + (config.services.deep_sleep.sleep_delay*1000)){
      Serial.println("Deep sleeping...");
      if (config.services.deep_sleep.mode == "WAKE_RF_DEFAULT")
        // sleep_time is in secs, but the function gets microsecs
        ESP.deepSleep(config.services.deep_sleep.sleep_time * 1000000, WAKE_RF_DEFAULT);
      else if (config.services.deep_sleep.mode == "WAKE_RF_DISABLED")
        ESP.deepSleep(config.services.deep_sleep.sleep_time * 1000000, WAKE_RF_DISABLED);
      else if (config.services.deep_sleep.mode == "WAKE_RFCAL")
        ESP.deepSleep(config.services.deep_sleep.sleep_time * 1000000, WAKE_RFCAL);
      else if (config.services.deep_sleep.mode == "WAKE_NO_RFCAL")
        ESP.deepSleep(config.services.deep_sleep.sleep_time * 1000000, WAKE_NO_RFCAL);
      else {
        Serial.println("   - Deep sleep -> mode not available");
        return;
      }
    }
  #endif

}


void setup() {
  Serial.begin(115200);
  // Enable wifi diagnostic using platformio build_glag: -D ENABLE_SERIAL_DEBUG:
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.setDebugOutput(true);
  #endif

  reconnect();

  // Print some info:
  // uint32_t realSize = ESP.getFlashChipRealSize();
  // uint32_t ideSize = ESP.getFlashChipSize();
  // FlashMode_t ideMode = ESP.getFlashChipMode();

  // Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  // Serial.printf("Flash real size: %u bytes\n\n", realSize);

  // Serial.printf("Flash ide  size: %u bytes\n", ideSize);
  // Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  // Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  // if (ideSize != realSize) {
    // Serial.println("Flash Chip configuration wrong!\n");
  // } else {
    // Serial.println("Flash Chip configuration ok.\n");
  // }



  // Configure some Websockets object to publish to webapp dashboard:
  if (config.services.webSockets.enabled){
    ws.addObjectToPublish("heap_free", getHeapFree);
    ws.addObjectToPublish("loop", getLoopTime);
    ws.addObjectToPublish("RSSI", getRSSI);
    ws.addObjectToPublish("mqtt_state", mqttState);
    ws.addObjectToPublish("mqtt_buffer_size", mqttBufferSize);
    ws.addObjectToPublish("mqtt_queue_string_size", getMqttQueueString);
    ws.addObjectToPublish("radar_motor_1_position", getPositionMotor1);
    ws.addObjectToPublish("radar_motor_1_target_position", getTargetPositionMotor1);
    ws.addObjectToPublish("radar_motor_2_position", getPositionMotor2);
    ws.addObjectToPublish("radar_motor_2_target_position", getTargetPositionMotor2);


  }


  Serial.println("###  Looping time\n");

  setupDeviceTime = millis();
  previousMainLoopMillis = millis();
}

void loop() {
  currentLoopMillis = millis();
  //  ----------------------------------------------
  //
  // Config services Loop:
  //  ----------------------------------------------

  // Reconnection loop:
  // if (WiFi.status() != WL_CONNECTED) {
  //   config.begin();
  //   networkRestart();
  //   config.configureServer(&server);
  // }

  // Handle mqtt reconnection:
  if (config.mqtt.enabled) {
    if (config.mqtt.reconnect_mqtt && !mqttClient.connected())
      reconnectMQTT();
    mqttClient.loop();
  }

  // Handle WebConfigServer for not asyc webserver:
  #ifndef USE_ASYNC_WEBSERVER
    server.handleClient();
  #endif

  // Services loop:
  if (config.services.ota) ota.handle();
  if (config.services.ftp.enabled) ftpSrv.handleFTP();
  if (config.services.webSockets.enabled){
    ws.handle();
    if(currentLoopMillis - previousWSMillis > (unsigned)config.services.webSockets.publish_time_ms) {
      ws.publishClients();
      previousWSMillis = currentLoopMillis;
    }
  }
  if (config.services.deep_sleep.enabled){
    deepSleepHandler();
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
      if(mqttClient.connected() && (config.device.publish_time_ms != 0) &&
          (currentLoopMillis - previousMQTTPublishMillis > (unsigned)config.device.publish_time_ms)) {
        previousMQTTPublishMillis = currentLoopMillis;
        // Here starts the MQTT publish loop configured:

          mqttQueueString += "]}";

          String base_topic_pub = "/" + config.mqtt.id_name + "/";
          String topic_pub = base_topic_pub + "data";
          mqttClient.setBufferSize((uint16_t)(mqttQueueString.length() + 100));
          mqttClient.publish(topic_pub.c_str(), mqttQueueString.c_str(), mqttQueueString.length());
          // Serial.println("MQTT published: " + msg_pub + " -- loop: " + config.device.publish_time_ms);

          mqttQueueString = "{\"data\":[";
          previousMQTTPublishMillis = currentLoopMillis;
      } else {
        mqttQueueString += ",";
      }
    }
    previousLoopMillis = currentLoopMillis;
  }

previousMainLoopMillis = currentLoopMillis;
}
