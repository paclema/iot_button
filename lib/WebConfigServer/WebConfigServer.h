#ifndef WebConfigServer_H
#define WebConfigServer_H

#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <FS.h>

#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#include <ArduinoJson.h>
#define JSON_CONFIG_BUFF_SIZZE 1150

#define CONFIG_FILE "/config.json"

#define CONFIG_LOADED "loaded"
#define CONFIG_NOT_LOADED "not_loaded"


class WebConfigServer {

public:

  // const char* status = new char(32);
  String config_status;

  struct Network {
    char ssid_name[64];
    char ssid_password[64];
    bool connection = false;
  } network;

  struct Mqtt {
    char server[64];
    int port;
  } mqtt;

  struct FTP {
    bool enabled;
    char user[64];
    char password[64];
  };

  struct Services {
    FTP ftp;
    bool OTA;
    bool sleep_mode;
  } services;



  WebConfigServer(void);

  void configureServer(ESP8266WebServer *server);
  void handle(void);
  bool begin(void);

  String status(void) { return config_status;};

private:

  String formatBytes(size_t bytes);

  void saveWebConfigurationFile(const char *filename, const JsonDocument& doc);
  void parseConfig(const JsonDocument& doc);

  void loadConfigurationFile(const char *filename);
  void saveConfigurationFile(const char *filename);
  void printFile(String filename);
  void restoreBackupFile(String filenamechar);
  // void updateGpio(void);
  void updateGpio(ESP8266WebServer *server);


};
#endif
