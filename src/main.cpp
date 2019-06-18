#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#include <ArduinoJson.h>



#define CONFIG_FILE "/config.json"

struct Network {
  char ssid_name[64];
  char ssid_password[64];
  bool connection = false;
};

struct Mqtt {
  char server[64];
  int port;
};

struct Config {
  Network network;
  Mqtt mqtt;

};

Config config;   // <- global configuration object

ESP8266WebServer server(80);


//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + " B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + " KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + " MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
  }
}

// Loads the configuration from a file
void loadConfiguration(const char *filename, Config &config) {
  // Open file for reading
  File file = SPIFFS.open(filename, "r");

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<2048> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));


  // Network object:
  strlcpy(config.network.ssid_name, doc["network"]["ssid_name"] | "SSID_name", sizeof(config.network.ssid_name));
  strlcpy(config.network.ssid_password, doc["network"]["ssid_password"] | "SSID_password", sizeof(config.network.ssid_password));
  config.network.connection = false;

  // MQTT object:
  strlcpy(config.mqtt.server, doc["mqtt"]["server"] | "server_address", sizeof(config.mqtt.server));
  config.mqtt.port = doc["mqtt"]["port"] | 8888;


  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  // File file = SD.open(filename, FILE_WRITE);
  File file = SPIFFS.open(filename, "w");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<2048> doc;

  // Network object:
  doc["network"]["ssid_name"] = config.network.ssid_name;
  doc["network"]["ssid_password"] = config.network.ssid_password;
  doc["network"]["connection"] = config.network.connection;

  // Network object:
  doc["mqtt"]["server"] = config.mqtt.server;
  doc["mqtt"]["port"] = config.mqtt.port;



  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}


void updateGpio(){
  String gpio = server.arg("id");
  String val = server.arg("val");
  String success = "1";

  int pin = D5;
  if ( gpio == "D5" ) {
    pin = D5;
  } else if ( gpio == "D7" ) {
     pin = D7;
   } else if ( gpio == "D8" ) {
     pin = D8;
   } else {
     // Built-in nodemcu GPI16, pin 16 led
     // esp12 led is 2 or D4
      pin = LED_BUILTIN;

    }

  // Reverse current LED status:
  pinMode(pin, OUTPUT);
  // digitalWrite(pin, LOW);
  Serial.println(pin);
  Serial.print("Current status:");
  Serial.println(digitalRead(pin));
  digitalWrite(pin, !digitalRead(pin));


  // if ( val == "true" ) {
  //   digitalWrite(pin, HIGH);
  // } else if ( val == "false" ) {
  //   digitalWrite(pin, LOW);
  // } else {
  //   success = "true";
  //   Serial.println("Err parsing GPIO Value");
  // }

  String json = "{\"gpio\":\"" + String(gpio) + "\",";
  json += "\"val\":\"" + String(val) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";

  server.send(200, "application/json", json);
  Serial.println("GPIO updated!");
}


void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");

    loadConfiguration(CONFIG_FILE, config);
    printFile(CONFIG_FILE);

  }

  Serial.println((String)"Connected?: " + config.network.connection);


  // Connect to Wi-Fi
  WiFi.begin(config.network.ssid_name, config.network.ssid_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  config.network.connection=true;

  // Create configuration file
  //saveConfiguration(CONFIG_FILE, config);
  //printFile(CONFIG_FILE);



  //SERVER INIT
  //list directory
  // server.on("/", HTTP_GET, handleFileList);
  // server.serveStatic("/js", SPIFFS, "/js");
  // server.serveStatic("/css", SPIFFS, "/css");
  // server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/certs", SPIFFS, "/certs");
  server.serveStatic("/php", SPIFFS, "/php");
  // server.serveStatic("/save_config.php", SPIFFS, "/save_config.php");
  // server.serveStatic("/restore_backup.php", SPIFFS, "/restore_backup.php");
  server.serveStatic("/css", SPIFFS, "/css");
  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/config.json", SPIFFS, "/config.json");
  server.serveStatic("/", SPIFFS, "/index.html");
  server.on("/gpio", updateGpio);

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]() {
    // if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    // }
  });

  server.begin();
  Serial.println ( "HTTP server started" );



}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
