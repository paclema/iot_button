#include <Arduino.h>

#define DEBUG_ESP_CORE

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ESP8266FtpServer.h"


#include <WebConfigServer.h>
WebConfigServer config;   // <- global configuration object

ESP8266WebServer server(80);
FtpServer ftpSrv;

void networkRestart(void){
  if(config.status() == CONFIG_LOADED){
    // Config loaded correctly
    if (config.network.ssid_name!=NULL && config.network.ssid_password!=NULL){
        // Connect to Wi-Fi
        WiFi.mode(WIFI_STA);
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

void reconnect(void) {
  delay(1000);
  Serial.print("--- Free heap: "); Serial.println(ESP.getFreeHeap());

  config.begin();
  networkRestart();
  config.configureServer(&server);

  // Enable services:
  Serial.println("--- Services: ");
  if (config.services.ftp.enabled && config.services.ftp.user !=NULL && config.services.ftp.password !=NULL){
    ftpSrv.begin(config.services.ftp.user,config.services.ftp.password);
    Serial.println("   - FTP -> enabled");
  } else
    Serial.println("   - FTP -> disabled");

  if (config.services.OTA){

    Serial.println("   - OTA -> enabled");
  } else
    Serial.println("   - OTA -> disabled");

  if (config.services.sleep_mode){

    Serial.println("   - Sleep mode -> enabled");
  } else
    Serial.println("   - Sleep mode -> disabled");
}
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  reconnect();


}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    config.begin();
    networkRestart();
    config.configureServer(&server);
  }
  ftpSrv.handleFTP();
  server.handleClient();


}
