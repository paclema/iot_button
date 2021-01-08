#ifndef WebConfigServer_H
#define WebConfigServer_H

#include <Arduino.h>

#include <LinkedList.h>
#include "IWebConfig.h"

#ifdef ESP32
  #include <WebServer.h>
  #include <SPIFFS.h>

#elif defined(ESP8266)
  #include <ESP8266WebServer.h>
  #include <FS.h>
#endif

#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#include <ArduinoJson.h>

// Allocate a temporary JsonDocument
// Don't forget to change the capacity to match your requirements.
// Use arduinojson.org/assistant to compute the capacity.
#define CONFIG_FILE "/config/config.json"
#define CONFIG_JSON_SIZE 4000
#define MQTT_TOPIC_MAX_SIZE_LIST 10
#define JSON_MAX_SIZE_LIST 6

#define CONFIG_LOADED "loaded"
#define CONFIG_NOT_LOADED "not_loaded"



class WebConfigServer {

public:

  // const char* status = new char(32);
  String config_status;

  struct Network {
    String ap_name;
    String ap_password;
    int ap_channel;
    bool ap_ssid_hidden;
    int ap_max_connection;
    String ssid_name;
    String ssid_password;
    String ip_address;
    String subnet;
    String dns_server;
    String hostname;
  } network;

  struct Mqtt {
    bool enabled;
    String server;
    int port;
    String id_name;
    bool reconnect_mqtt;
    bool enable_user_and_pass;
    String user_name;
    String user_password;
    bool enable_certificates;
    String ca_file;
    String cert_file;
    String key_file;
    String pub_topic[MQTT_TOPIC_MAX_SIZE_LIST];
    String sub_topic[MQTT_TOPIC_MAX_SIZE_LIST];
  } mqtt;

  struct FTP {
    bool enabled;
    String user;
    String password;
  };

  struct WebSockets {
    bool enabled;
    int publish_time_ms;
  };

  struct DeepSleep {
    bool enabled;
    String mode;
    String mode_options[JSON_MAX_SIZE_LIST];
    float sleep_time;
    float sleep_delay;
  };

  struct LightSleep {
    bool enabled;
    String mode;
    String mode_options[JSON_MAX_SIZE_LIST];
  };

  struct Services {
    bool ota;
    FTP ftp;
    WebSockets webSockets;
    DeepSleep deep_sleep;
    LightSleep light_sleep;
  } services;

  struct Device {
    bool track_restart_counter;
    int loop_time_ms;
    int publish_time_ms;
  } device;

  struct Info {
    int restart_counter;
    String fw_version;
    String repo;
  } info;



  WebConfigServer(void);

  #ifdef ESP32
    void configureServer(WebServer *server);
  #elif defined(ESP8266)
    void configureServer(ESP8266WebServer *server);
  #endif

  void handle(void);
  bool begin(void);

  String status(void) { return config_status;};

  void addConfig(IWebConfig* config, String nameObject);


private:

  LinkedList<IWebConfig*> configs = LinkedList<IWebConfig*>();

  void parseIWebConfig(const JsonDocument& doc);

  String formatBytes(size_t bytes);

  void saveWebConfigurationFile(const char *filename, const JsonDocument& doc);
  void parseConfig(const JsonDocument& doc);

  void loadConfigurationFile(const char *filename);
  void saveConfigurationFile(const char *filename);
  void printFile(String filename);
  void restoreBackupFile(String filenamechar);

  #ifdef ESP32
    void updateGpio(WebServer *server);
    bool handleFileRead(WebServer *server, String path);
  #elif defined(ESP8266)
    void updateGpio(ESP8266WebServer *server);
    bool handleFileRead(ESP8266WebServer *server, String path);
  #endif

  String getContentType(String filename);


};
#endif
