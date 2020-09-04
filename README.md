# iot_button

### Features:

* Store configs as json and certs on SPI Flash File System (SPIFFS).
* ftp server to easily modify files stored on SPIFFS.
* Boootstrap Web server.
* MQTT connection.
* OTA updates.

### Requirements:

* nodemcuv2 or esp12
* push button on RST --> this will triger the pushh action after restart the board
* push button on D5 --> to erase wifi data and restart in AP mode
* CHPD (or EN) to VCC

##### Add a new configuration object:
1. Update the new object in the configuration file _/data/config.json_
2. Add the struct object in the configuration class _/lib/WebConfigServer/WebConfigServer.h_
3. Update the parse function to link the json object with the class struct in the configuration class _/lib/WebConfigServer/WebConfigServer.cpp_

##### Upload _/data_ folder to ESP SPIFFS File System:

Using platformio run the next command: `pio run -t uploadfs`
