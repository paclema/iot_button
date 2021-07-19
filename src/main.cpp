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

  #if !IP_NAPT
    // #error "IP_NAPT is not available with this configuration."
  #else
    #include "esp_wifi.h"
    #include "lwip/lwip_napt.h"
  #endif

  uint8_t AP_clients = 0;
  uint8_t AP_clients_last = AP_clients;

  #define PROTO_TCP 6
  #define PROTO_UDP 17

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

// NTP
#include <time.h>                         // time() ctime()
#include <sys/time.h>                     // struct timeval

#ifdef ESP32
  #define NTP_MIN_VALID_EPOCH 1620413406  // For example: May 7th, 2021
#elif defined(ESP8266)
  #include <coredecls.h>                  // settimeofday_cb()
  timeval cbtime;			// time set in callback
  bool cbtime_set = false;

  // Callback function to know when the time is synch with ntp server:
  void time_is_set(void) {
    gettimeofday(&cbtime, NULL);
    cbtime_set = true;
    Serial.println("------------------ settimeofday() was called ------------------");
  }
#endif

extern "C" int clock_gettime(clockid_t unused, struct timespec *tp);


// Device configurations
unsigned long currentLoopMillis = 0;
unsigned long previousLoopMillis = 0;
unsigned long previousMQTTPublishMillis = 0;
unsigned long previousWSMillis = 0;
unsigned long previousHandleAPMillis = 0;
unsigned long previousMainLoopMillis = 0;

// Websocket server:
#include <WrapperWebSockets.h>
WrapperWebSockets ws;

// Websocket functions to publish:
String getLoopTime(){ return String(currentLoopMillis - previousMainLoopMillis);}
String getRSSI(){ return String(WiFi.RSSI());}
String getHeapFree(){ return String((float)GET_FREE_HEAP/1000);}


// Co2 sensor
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD30
SCD30 airSensor;

// GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 14, TXPin = 12; // RX to D6 and TX to D5
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);




// MAIN FUNCTIONS:
// --------------
//

#if ESP32 && IP_NAPT
esp_err_t enableNAT(void){

  // Give DNS servers to AP side:

  esp_err_t err;
  tcpip_adapter_dns_info_t ip_dns_main;
  tcpip_adapter_dns_info_t ip_dns_backup;


  err = tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP); if (err != ESP_OK) return err;

  err = tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &ip_dns_main); if (err != ESP_OK) return err;
  err = tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_BACKUP, &ip_dns_backup); if (err != ESP_OK) return err;

  err = tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_AP, ESP_NETIF_DNS_MAIN, &ip_dns_main); if (err != ESP_OK) return err;
  // Serial.printf("\ntcpip_adapter_set_dns_info ESP_NETIF_DNS_MAIN: err %s . ip_dns:" IPSTR, esp_err_to_name(err), IP2STR(&ip_dns_main.ip.u_addr.ip4)) ;

  dhcps_offer_t opt_val = OFFER_DNS; // supply a dns server via dhcps
  tcpip_adapter_dhcps_option(ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, &opt_val, 1);

  err = tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP); if (err != ESP_OK) return err;


  // Enable NAT:
  ip_napt_enable(WiFi.softAPIP(), 1);

  // Example port mapping to stations:
  // Mapping Webserver: (of an sta connected to this ap)
  IPAddress ap_ip = WiFi.localIP();
  ip_portmap_add(PROTO_TCP,ap_ip, 8080,IPAddress(192, 168, 4, 2), 80 );
  ip_portmap_add(PROTO_UDP,ap_ip, 8080,IPAddress(192, 168, 4, 2), 80 );
  // Mapping WebSockets:
  ip_portmap_add(PROTO_TCP,ap_ip, 94,IPAddress(192, 168, 4, 2), 94 );
  ip_portmap_add(PROTO_UDP,ap_ip, 94,IPAddress(192, 168, 4, 2), 94 );

  return err;
}
#endif

void networkRestart(void){
  if(config.status() == CONFIG_LOADED){

    // WiFi setup:
    // WiFi.disconnect(true);        // close old connections
    #ifdef ESP32
      WiFi.setHostname(config.network.hostname.c_str());
      WiFi.mode(WIFI_MODE_APSTA);
    #elif defined(ESP8266)
      WiFi.hostname(config.network.hostname);
      // WiFi.mode(WIFI_STA);
      WiFi.mode(WIFI_AP_STA);
    #endif



    // Client Wifi config:
    if (config.network.ssid_name!=NULL && config.network.ssid_password!=NULL){

      // wifiMulti.addAP(config.network.ssid_name.c_str(),config.network.ssid_password.c_str());    // Using wifiMulti
      WiFi.begin(config.network.ssid_name.c_str(),config.network.ssid_password.c_str());      // Connecting just to one ap

      Serial.printf("Connecting to %s...\n",config.network.ssid_name.c_str());
      int retries = 0;
      // while ((wifiMulti.run() != WL_CONNECTED)) {   // Using wifiMulti
      while (WiFi.status() != WL_CONNECTED) {    // Connecting just to one ap
        Serial.print('.');
        delay(500);
        retries++;
        if (config.network.connection_retries != 0 && (retries >= config.network.connection_retries)) break;
      }
      if ((config.network.connection_retries != 0 && (retries >= config.network.connection_retries)) || config.network.connection_retries == 0) {
        Serial.print("\n\nConnected to ");Serial.print(WiFi.SSID());
        Serial.print("\nIP address:\t");Serial.println(WiFi.localIP());
      } else {Serial.print("\n\nNot Connected to ");Serial.print(config.network.ssid_name);Serial.println(" max retries reached.");}

    }


    // Config access point:
    bool APEnabled = false;
    uint8_t channelSTA;
    #ifdef ESP32
      wifi_ap_record_t staConfig;
      esp_wifi_sta_get_ap_info(&staConfig);
      channelSTA = staConfig.primary;
    #elif defined(ESP8266)
      channelSTA = wifi_get_channel();
    #endif


    if (config.network.ap_name!=NULL && config.network.ap_password!=NULL){
        Serial.print("Setting soft-AP... ");
        
        if (config.network.enable_NAT){
          APEnabled = WiFi.softAP(config.network.ap_name.c_str(),
            config.network.ap_password.c_str());
        } else {
          APEnabled = WiFi.softAP(config.network.ap_name.c_str(),
            config.network.ap_password.c_str(),
            // If STA connected, use the same channel instead configured one:
            (WiFi.isConnected() && channelSTA) ? channelSTA : config.network.ap_channel,  
            config.network.ap_ssid_hidden,
            config.network.ap_max_connection);
        }
        Serial.println(APEnabled ? "Ready" : "Failed!");
        IPAddress myIP = WiFi.softAPIP();
        Serial.print(config.network.ap_name);Serial.print(" AP IP address: ");
        Serial.println(myIP);

    }

    // Enable NAPT:
    #if ESP32 && IP_NAPT
      if (config.network.enable_NAT){
        if (WiFi.isConnected() && APEnabled) {
          esp_err_t err = enableNAT();
          if (err == ESP_OK) Serial.println("NAT configured and enabled");
          else Serial.printf("Error configuring NAT: %s\n", esp_err_to_name(err));
        } else Serial.printf("Error configuring NAT: STA or AP not working\n");
      }
    #endif

    // Configure device hostname:
    if (config.network.hostname){
      if (MDNS.begin(config.network.hostname.c_str())) Serial.println("mDNS responder started");
      else Serial.println("Error setting up MDNS responder!");
    }

  }
}

#if ESP32 && IP_NAPT
void handleAPStations(void){
  AP_clients = WiFi.softAPgetStationNum();

  if (AP_clients_last != AP_clients){
    Serial.printf("Stations connected to AP: %d\n", AP_clients);
    AP_clients_last = AP_clients;

    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;
  
    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

    // delay(500);   // To give time to AP to provide IP to the new station
    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
  
    for (int i = 0; i < adapter_sta_list.num; i++) {
      tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
      Serial.printf("\t - Station %d MAC: ", i);
      for(int i = 0; i< 6; i++){
        Serial.printf("%02X", station.mac[i]);  
        if(i<5)Serial.print(":");
      }
      Serial.printf("  IP: " IPSTR, IP2STR(&station.ip));
      Serial.println();
    }
  }
}
#endif

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
    if (config.services.webSockets.port) ws.setPort(config.services.webSockets.port);
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

  if (config.services.ntp.enabled){
    configTime(config.services.ntp.gmtOffset_sec, config.services.ntp.daylightOffset_sec, config.services.ntp.ntpServer.c_str());

    #ifdef ESP8266
      settimeofday_cb(time_is_set);
    #elif defined(ESP32)
      Serial.print ("\t\tSynching Time over NTP ");
      while((time(nullptr)) < NTP_MIN_VALID_EPOCH) {
        // warning : no time out. May loop here forever
        delay(20);
        Serial.print(".");
      }
      Serial.println("");
    #endif
    
    Serial.print("   - NTP -> enabled\n");
    Serial.printf("          - Server: %s\n", config.services.ntp.ntpServer.c_str());
    Serial.printf("          - GMT offset: %d\n", config.services.ntp.gmtOffset_sec);
    Serial.printf("          - Day light offset: %d\n", config.services.ntp.daylightOffset_sec);
  } else Serial.println("   - NTP -> disabled");



  Serial.println("");

}

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char buff[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    buff[i] = (char)payload[i];
  }


  buff[length] = '\0';
  String message(buff);

  Serial.print(message);
  Serial.println();

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

  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
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

    // if (wifiClientSecure.loadCertificate(cert, cert.size())) Serial.println("cert loaded");
    // else Serial.println("cert not loaded");
    // cert.close();

    // Load private key:
    // But you must convert it to .der
    // openssl rsa -in ./certs/IoLed_controller/client.key -out ./certs/IoLed_controller/private.der -outform DER
    File private_key = SPIFFS.open(config.mqtt.key_file, "r");
    if (!private_key) Serial.println("Failed to open key file ");
    else Serial.println("Success to open key file");

    // if (wifiClientSecure.loadPrivateKey(private_key, private_key.size())) Serial.println("key loaded");
    // else Serial.println("key not loaded");
    // private_key.close();

    // Load CA file:
    File ca = SPIFFS.open(config.mqtt.ca_file, "r");
    if (!ca) Serial.println("Failed to open CA file ");
    else Serial.println("Success to open CA file");

    // if (wifiClientSecure.loadCACert(ca, ca.size())) Serial.println("CA loaded");
    // else Serial.println("CA not loaded");
    // ca.close();
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

  config.begin();

  // Configure NTP if enabled before wifi restart:
  if (config.services.ntp.enabled){
    #ifdef ESP8266
      settimeofday_cb(time_is_set);
    #endif
    configTime(config.services.ntp.gmtOffset_sec, config.services.ntp.daylightOffset_sec, config.services.ntp.ntpServer.c_str());
  }

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


}


void deepSleepHandler() {
  if (currentLoopMillis - previousLoopMillis > (unsigned)config.device.loop_time_ms){
    Serial.print("sleep_delay: "); Serial.print((config.services.deep_sleep.sleep_delay));
    Serial.print(" setupDeviceTime: "); Serial.print( (float)setupDeviceTime/1000);
    Serial.print(" currentLoopMillis: "); Serial.println((float)currentLoopMillis/1000);
  }

  #ifdef ESP32


  #elif defined(ESP8266)
    if (currentLoopMillis > setupDeviceTime + (config.services.deep_sleep.sleep_delay*1000)){
      if(config.services.deep_sleep.sleep_time == 0) Serial.printf("Deep sleep activated forever or until RST keeps HIGH...\n");
      else Serial.printf("Deep sleep activated for %f seconds...\n", config.services.deep_sleep.sleep_time);
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
        delay(100);
        return;
      }
    }
  #endif

}

void initSCD30(void){
  Wire.begin();

  //Start sensor using the Wire port and enable the auto-calibration (ASC)
  if (airSensor.begin(Wire, true) == false)
  {
      Serial.println("Air sensor not detected. Please check wiring. Freezing...");
      while (1)
          ;
  }

  airSensor.setMeasurementInterval(2); //Change number of seconds between measurements: 2 to 1800 (30 minutes)


  Serial.print("Auto calibration set to ");
  if (airSensor.getAutoSelfCalibration() == true)
      Serial.println("true");
  else
      Serial.println("false");

  //The SCD30 has data ready every two seconds

    //Read altitude compensation value
  unsigned int altitude = airSensor.getAltitudeCompensation();
  Serial.print("Current altitude: ");
  Serial.print(altitude);
  Serial.println("m");

  //My desk is ~116m above sealevel
  airSensor.setAltitudeCompensation(116); //Set altitude of the sensor in m, stored in non-volatile memory of SCD30

  //Pressure in Boulder, CO is 24.65inHg or 834.74mBar
  // airSensor.setAmbientPressure(835); //Current ambient pressure in mBar: 700 to 1200, will overwrite altitude compensation

  //Read temperature offset
  float offset = airSensor.getTemperatureOffset();
  Serial.print("Current temp offset: ");
  Serial.print(offset, 2);
  Serial.println("C");

  //airSensor.setTemperatureOffset(5); //Optionally we can set temperature offset to 5°C, stored in non-volatile memory of SCD30

}

void initGPS(void){
  Serial.print(F("TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();

  ss.begin(GPSBaud);
}

void setup() {
  Serial.begin(115200);
  // Enable wifi diagnostic using platformio build_glag: -D ENABLE_SERIAL_DEBUG:
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.setDebugOutput(true);
  #endif


  // SCD30 and GPS setup:
  initGPS();
  initSCD30();


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
  }

  Serial.println("###  Looping time\n");

  setupDeviceTime = millis();
}

void loop() {

  currentLoopMillis = millis();

  // Reconnection loop:
  // if (WiFi.status() != WL_CONNECTED) {
  //   config.begin();
  //   networkRestart();
  //   config.configureServer(&server);
  // }

  // Handle stations connection to the AP:
  #if ESP32 && IP_NAPT
    if(currentLoopMillis - previousHandleAPMillis > (unsigned)3000) {
      handleAPStations();
      previousHandleAPMillis = currentLoopMillis;
    }
  #endif

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


  // Check GPS location:
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (airSensor.dataAvailable()) {
  Serial.print("co2(ppm):");
  Serial.print(airSensor.getCO2());

  Serial.print(" temp(C):");
  Serial.print(airSensor.getTemperature(), 1);

  Serial.print(" humidity(%):");
  Serial.print(airSensor.getHumidity(), 1);

  Serial.println();
  
  Serial.printf("Lat: %f - Long: %f - Date: %d - Time: %d - Spped: %f km/h\n", gps.location.lat(), gps.location.lng(), gps.date.value(), gps.time.value(), gps.speed.kmph());

  // if(mqttClient.connected() && (config.device.publish_time_ms != 0) &&
  //   (currentLoopMillis - previousMQTTPublishMillis > (unsigned)config.device.publish_time_ms)) {
  //   previousMQTTPublishMillis = currentLoopMillis;
  if(mqttClient.connected()) {
    // Here starts the MQTT publish loop configured:

    String base_topic_pub = "/" + config.mqtt.id_name + "/";
    String topic_pub = base_topic_pub + "data";
    
    String msg_pub;
    StaticJsonDocument<256> doc;

    doc["CO2"] = airSensor.getCO2();
    doc["temp"] = airSensor.getTemperature();
    doc["humidity"] = airSensor.getHumidity();
    doc["lat"] = gps.location.lat();
    doc["lng"] = gps.location.lng();
    doc["date"] = gps.date.value();
    doc["time"] = gps.time.value();
    doc["speed"] = gps.speed.kmph();

    serializeJson(doc, msg_pub);    
    mqttClient.publish(topic_pub.c_str(), msg_pub.c_str());

  }

}


  // Main Loop:
  if((config.device.loop_time_ms != 0 ) &&
      (currentLoopMillis - previousLoopMillis > (unsigned)config.device.loop_time_ms)) {
    previousLoopMillis = currentLoopMillis;
    // Here starts the device loop configured:

    // NTP time example:
    // time_t now = time(nullptr);
    // timeval tv;
    // gettimeofday(&tv, nullptr);
    // timespec tp;
    // clock_gettime(0, &tp);

    // // time from boot
    // Serial.print("Time from boot: ");
    // Serial.print((uint32_t)tp.tv_sec);
    // Serial.print("s / ");
    // Serial.print((uint32_t)tp.tv_nsec);
    // Serial.println("ns");
    // // EPOCH+tz+dst
    // Serial.print("gtod: ");
    // Serial.print((uint32_t)tv.tv_sec);
    // Serial.print("s / ");
    // Serial.print((uint32_t)tv.tv_usec);
    // Serial.println("us");

    // Serial.print("timestamp:");
    // Serial.print((uint32_t)now);
    // Serial.print(" - ");
    // Serial.println(ctime(&now));


  }
  if(mqttClient.connected() && (config.device.publish_time_ms != 0) &&
      (currentLoopMillis - previousMQTTPublishMillis > (unsigned)config.device.publish_time_ms)) {
    previousMQTTPublishMillis = currentLoopMillis;
    // Here starts the MQTT publish loop configured:
    /*
    String base_topic_pub = "/" + config.mqtt.id_name + "/";
    String topic_pub = base_topic_pub + "data";
    // String msg_pub ="{\"angle\":35, \"distance\": 124}";
    String msg_pub ="{\"connected\":true}";
    mqttClient.publish(topic_pub.c_str(), msg_pub.c_str());
    Serial.println("MQTT published: " + msg_pub + " -- loop: " + config.device.publish_time_ms);
    */
  }




previousMainLoopMillis = currentLoopMillis;
}
