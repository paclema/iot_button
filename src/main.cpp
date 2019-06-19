#include <Arduino.h>

#define DEBUG_ESP_CORE

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include "ESP8266FtpServer.h"



#include <WebConfigServer.h>
WebConfigServer Webconfig;   // <- global configuration object

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.print("--- Free heap: "); Serial.println(ESP.getFreeHeap());

  Webconfig.begin();

  Serial.println((String)"Wifi ssid?: " + Webconfig.config.network.ssid_name);
  Serial.println((String)"Connected?: " + Webconfig.config.network.ssid_password);


  // Connect to Wi-Fi
  // WiFi.begin("paclema_fon", "paclematest");
  WiFi.begin(Webconfig.config.network.ssid_name, Webconfig.config.network.ssid_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print Local IP Address
  Serial.println(WiFi.localIP());

  Webconfig.configureServer(&server);

}

void loop() {
//  ftpSrv.handleFTP();
  server.handleClient();
}
