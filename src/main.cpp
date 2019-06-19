#include <Arduino.h>

#define DEBUG_ESP_CORE

#include <ESP8266WiFi.h>
//#include "ESP8266FtpServer.h"



#include <WebConfigServer.h>
WebConfigServer Webconfig;   // <- global configuration object


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.print("--- Free heap: "); Serial.println(ESP.getFreeHeap());

  Webconfig.begin();

  delay(5000);


  // >>>stack>>>
  // 3fffff20:  00000000 3ffefe54 3ffee688 402055ec
  // 3fffff30:  40104d30 0021d3b0 3ffee74c 3ffee6f4
  // 3fffff40:  3ffee654 3ffee768 3ffee588 40202a93
  // 3fffff50:  3ffee654 3ffee6f4 40206e24 3fffefa0
  // 3fffff60:  402075da 000003e8 000003e8 402075cf
  // 3fffff70:  3ffee654 3ffee588 3ffee688 40201136
  // 3fffff80:  402121f8 f201a8c0 ffefeffe 00000000
  // 3fffff90:  00000000 ffefeffe feefeffe feefeffe
  // 3fffffa0:  3fffdad0 00000000 3ffee6c4 40206ecc
  // 3fffffb0:  feefeffe feefeffe 3ffe8550 4010051d
  // <<<stack<<<


  Serial.println((String)"Wifi ssid?: " + Webconfig.config.network.ssid_name);
  Serial.println((String)"Connected?: " + Webconfig.config.network.ssid_password);


  // Connect to Wi-Fi
  // WiFi.begin("paclema_fon", "paclematest");
  WiFi.begin(Webconfig.config.network.ssid_name, Webconfig.config.network.ssid_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  delay(5000);
  // Print Local IP Address
  Serial.println(WiFi.localIP());

  delay(5000);
  Webconfig.serverBegin();

}

void loop() {
//  ftpSrv.handleFTP();
  Webconfig.handle();
}
