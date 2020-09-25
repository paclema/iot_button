#include <Arduino.h>

#define DEBUG_ESP_CORE

// Used for light sleep
extern "C" {
  #include "user_interface.h"
}

#include <ESP8266WiFi.h>


// MQTT
#include <PubSubClient.h>
WiFiClientSecure wifiClientSecure;    // To use with mqtt and certificates
WiFiClient wifiClient;                // To use with mqtt without certificates
PubSubClient mqttClient;
long connection_time = millis();

// Configuration
#include "WebConfigServer.h"
#include <ESP8266WebServer.h>
WebConfigServer config;   // <- global configuration object
ESP8266WebServer server(80);

// FTP server
#include <ESP8266FtpServer.h>
#include <FS.h>
FtpServer ftpSrv;

// OTA Includes
#include "WrapperOTA.h"
// #include <display2.h>
WrapperOTA ota;

// Device configurations
long previousLoopMillis = 0;
long previousLoopMainMillis = 0;
long previousMQTTPublishMillis = 0;

// Distance sensor:
#include "SensorMotor.h"
SensorMotor sensorHead;

// Choose one sensor library:
// #include "sensorVL53L0X.h"
#include "sensorVL53L1X.h"

// #include "sensorVL53L1X_ROI.h"


int sensorAngle=0;
float sensorDistance=0;
float sensorDistance_1=0;
float sensorDistance_2=0;



void networkRestart(void){
  if(config.status() == CONFIG_LOADED){
    // Config loaded correctly
    if (config.network.ssid_name!=NULL && config.network.ssid_password!=NULL){
        // Connect to Wi-Fi
        // WiFi.mode(WIFI_STA);
        WiFi.mode(WIFI_AP_STA);

        // Access point config:
        WiFi.softAP(config.network.ap_name,config.network.ap_password, false);
        IPAddress myIP = WiFi.softAPIP();
        Serial.print(config.network.ap_name);Serial.print(" AP IP address: ");
        Serial.println(myIP);

        // Wifi client config:
        WiFi.begin(config.network.ssid_name, config.network.ssid_password);
        Serial.print("Connecting to ");Serial.print(config.network.ssid_name);
        while (WiFi.status() != WL_CONNECTED) {
          delay(300);
          Serial.print(".");
        }
        Serial.println("");
    }
  }

  // Print Local IP Address
  Serial.println(WiFi.localIP());

}


void enableServices(void){
  Serial.println("--- Services: ");

  if (config.services.ota){
    // ota.init(&display);
    ota.init(&config);
    Serial.println("   - OTA -> enabled");
  } else Serial.println("   - OTA -> disabled");


  if (config.services.ftp.enabled && config.services.ftp.user !=NULL && config.services.ftp.password !=NULL){
    ftpSrv.begin(config.services.ftp.user,config.services.ftp.password);
    Serial.println("   - FTP -> enabled");
  } else Serial.println("   - FTP -> disabled");

  if (config.services.deep_sleep.enabled){
    // We will enable it on the loop function
    // Serial.println("   - Deep sleep -> configured");
    Serial.print("   - Deep sleep -> enabled for ");
    Serial.print(config.services.deep_sleep.sleep_time);
    Serial.print("secs after waitting ");
    Serial.print(config.services.deep_sleep.sleep_delay);
    Serial.println("secs. Choose sleep_time: 0 for infinite sleeping");
    Serial.println("     Do not forget to connect D0 to RST pin to auto-wake up! Or I will sleep forever");
  } else Serial.println("   - Deep sleep -> disabled");

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

  Serial.println("");

}


void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  // Serial.print("Message arrived [");
  // Serial.print(topic);
  // Serial.print("] ");

  char buff[length + 1];
  for (int i = 0; i < length; i++) {
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

    if (wifiClientSecure.loadCertificate(cert)) Serial.println("cert loaded");
    else Serial.println("cert not loaded");
    cert.close();

    // Load private key:
    // But you must convert it to .der
    // openssl rsa -in ./certs/IoLed_controller/client.key -out ./certs/IoLed_controller/private.der -outform DER
    File private_key = SPIFFS.open(config.mqtt.key_file, "r");
    if (!private_key) Serial.println("Failed to open key file ");
    else Serial.println("Success to open key file");

    if (wifiClientSecure.loadPrivateKey(private_key)) Serial.println("key loaded");
    else Serial.println("key not loaded");
    private_key.close();

    // Load CA file:
    File ca = SPIFFS.open(config.mqtt.ca_file, "r");
    if (!ca) Serial.println("Failed to open CA file ");
    else Serial.println("Success to open CA file");

    if (wifiClientSecure.loadCACert(ca)) Serial.println("CA loaded");
    else Serial.println("CA not loaded");
    ca.close();
  }

}

void reconnectMQTT() {
  // Loop until we're reconnected
  int retries = 0;
  int max_retries = 5;
  bool mqtt_connected = false;
  while (!mqttClient.connected() && (retries <= max_retries) ) {
    Serial.print("Attempting MQTT connection...");
    if (config.mqtt.enable_user_and_pass)
      mqtt_connected = mqttClient.connect(config.mqtt.id_name.c_str(),
                                          config.mqtt.user_name.c_str(),
                                          config.mqtt.user_password.c_str());
    else
        mqtt_connected = mqttClient.connect(config.mqtt.id_name.c_str());

    if (mqtt_connected) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      String base_topic_pub = "/" + config.mqtt.id_name + "/";
      String topic_connected_pub = base_topic_pub + "connected";
      // String msg_connected = config.mqtt.id_name + " connected";
      String msg_connected ="true";
      mqttClient.publish(topic_connected_pub.c_str(), msg_connected.c_str());
      // ... and resubscribe
      String base_topic_sub = base_topic_pub + "#";
      mqttClient.subscribe(base_topic_sub.c_str());

      long time_now = millis() - connection_time;
      Serial.print("Time to setup and be connected: ");
      Serial.print(time_now/1000);
      Serial.println("s");

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.print(" try again in 5 seconds: ");
      Serial.print(retries);
      Serial.print("/");
      Serial.println(max_retries);

      // Wait 5 seconds before retrying
      delay(5000);

    }
    retries++;
  }
}


void reconnect(void) {
  //delay(1000);
  Serial.print("--- Free heap: "); Serial.println(ESP.getFreeHeap());

  config.begin();
  networkRestart();
  config.configureServer(&server);

  // Enable services:
  enableServices();

  // Configure MQTT broker:
  initMQTT();
  if (config.mqtt.reconnect_mqtt)
    reconnectMQTT();

}


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  reconnect();

  // Print some info:
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size: %u bytes\n\n", realSize);

  Serial.printf("Flash ide  size: %u bytes\n", ideSize);
  Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  if (ideSize != realSize) {
    Serial.println("Flash Chip configuration wrong!\n");
  } else {
    Serial.println("Flash Chip configuration ok.\n");
  }


  // Device configs:
  sensorSetup();
  sensorHead.sensorMotorSetup(config.device.angle_accuracy, config.device.servo_speed_ms);

  Serial.println("###  Looping time\n");

  delay(1000);
  previousLoopMainMillis = millis();

}

void loop() {

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
  if (config.mqtt.reconnect_mqtt && !mqttClient.connected())
    reconnectMQTT();

  server.handleClient();
  mqttClient.loop();

  // Services loop:
  if (config.services.ota) ota.handle();
  if (config.services.ftp.enabled) ftpSrv.handleFTP();

  if (config.services.deep_sleep.enabled){
    // long time_now = millis();
    if (millis() > connection_time + (config.services.deep_sleep.sleep_delay*1000)){
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
  }



  //  ----------------------------------------------
  //
  // Main Loop:
  //  ----------------------------------------------
  unsigned long currentLoopMillis = millis();

  int loop_delay = currentLoopMillis - previousLoopMainMillis;
  // Serial.print("Loop time: ");
  // Serial.println(loop_delay);


  // int time_spent = (currentLoopMillis-previousServoMillis);
  // int time_between_servo_writes = config.device.servo_speed_ms * config.device.angle_accuracy/60;
  //
  // if (time_spent >= time_between_servo_writes){
  //   previousServoMillis = currentLoopMillis;
  //   servoPos += servoIncrement;
  //   servo.write(servoPos);
  //   sensorAngle = servoPos;
  //
  //   if ((servoPos >= 180) || (servoPos <= 0)) // end of sweep
  //   {
  //     // reverse direction
  //     servoIncrement = -servoIncrement;
  //     reverseIncrement = -reverseIncrement;
  //
  //   }
  // }
  //
  //
  // // int angleIncrement = (config.device.servo_speed_ms*config.device.angle_accuracy*config.device.angle_accuracy/60)/time_spent;
  // int angleIncrement = loop_delay*60/(config.device.servo_speed_ms);
  // sensorAngle += (angleIncrement*reverseIncrement);

  sensorAngle = sensorHead.sensorMotorAngle();

  // Serial.print("servo_speed_ms: ");
  // Serial.print(config.device.servo_speed_ms);
  // Serial.print(" angle_accuracy: ");
  // Serial.print(config.device.angle_accuracy);
  // Serial.print(" loop_delay: ");
  // Serial.print(loop_delay);
  // Serial.print(" time_spent: ");
  // Serial.print(time_spent);
  // Serial.print(" sensorAngle: ");
  // Serial.print(sensorAngle);
  // Serial.print(" angleIncrement: ");
  // Serial.print(angleIncrement);
  // Serial.print(" servoPos: ");
  // Serial.println(servoPos);

  // Sensor reading:
  if((config.device.loop_sensor_time_ms != 0 ) && (currentLoopMillis - previousLoopMillis > config.device.loop_sensor_time_ms)) {
    previousLoopMillis = currentLoopMillis;

    // If measure is not available, mqtt is not sent:
    if (sensorRead(sensorDistance)){

      // Publish mqtt sensor feedback:
      if((config.device.publish_time_ms != 0 ) && (currentLoopMillis - previousMQTTPublishMillis > config.device.publish_time_ms)) {
        previousMQTTPublishMillis = currentLoopMillis;
        // Here starts the MQTT publish loop configured:

        String base_topic_pub = "/" + config.mqtt.id_name + "/";
        String topic_pub = base_topic_pub + "data";

        String msg_pub ="{'angle':" + String(sensorAngle) + ", 'distance' :"+ String(sensorDistance) +"}";
        if (sensorDistance != 0 ){
          // String msg_pub ="{'persons':" + String(sensorDistance) + ", 'distance_1' :" + String(sensorDistance_1) + ", 'distance_2' :"+ String(sensorDistance_2)+" }";
          mqttClient.publish(topic_pub.c_str(), msg_pub.c_str());
          Serial.println("MQTT published: " + msg_pub + " -- loop: " + config.device.publish_time_ms);
        }
      }

    }


  }


  previousLoopMainMillis = currentLoopMillis;
}
