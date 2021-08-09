#include <Arduino.h>


// Main variables:
// #define DEBUG_ESP_CORE

// Enable wifi diagnostic using platformio build_glag: -D ENABLE_SERIAL_DEBUG:
#define ENABLE_SERIAL_DEBUG true


// Device configurations
unsigned long currentLoopMillis = 0;
unsigned long previousPublishMillis = 0;
unsigned long previousMainLoopMillis = 0;


// WebConfigServer Configuration
#include "WebConfigServer.h"
WebConfigServer config;   // <- global configuration object


#include <PubSubClient.h>
PubSubClient * mqttClient;


// PostBox:
int buttons[2] = {12, 13};    // Pins on which buttons are attached
int button = -1;              // Variable to store the button which triggered the bootup
int wake = 14;                // Pin which is used to Keep ESP awake until job is finished
int buttonState = 0;
int counter[2] = {0, 0};
volatile unsigned long lastTime =0;
bool wakeUpPublished = false;

int debounceMs = 200;         // To ignore button signals changes faster than this debounce ms

//Using tp4056 charger with battery fedback to ADC0 (max 1v!) and tp4056 chrg and stdby pins feedback:
#define USE_TP4056
#define NO_SLEEP_WHILE_CHARGING    // While charging the ESP won't go to sleep. Comment out this line if you want to avoid that


#ifdef USE_TP4056
  int chrg = 4;               // Pin for charge battery. LOW when usb is connected and baterry is being charged.
  int stdby = 5;              // Pin for stdby battery charge. LOW for battery charge termination.
#else
  ADC_MODE(ADC_VCC);          // Allows to monitor the internal VCC level; it varies with WiFi load
#endif

float readVoltage() {
  #ifdef USE_TP4056
    int sensorValue = analogRead(A0);
    float volts = sensorValue * (4.333 / 1023.0);
    Serial.printf("The internal VCC reads %1.3f volts. CHRG: %d - STDBY: %d\n", volts , !digitalRead(chrg), !digitalRead(stdby));
  #else
    float volts = ESP.getVcc();
    Serial.printf("The internal VCC reads %1.3f volts\n", volts / 1000);
  #endif
  return volts;
}

void publishWakeUp(String topic_end){

  String topic = config.getDeviceTopic() + topic_end;
  String msg_pub ="{\"wake_up_pin\": ";
  msg_pub += String(button);
  msg_pub = msg_pub + " ,\"vcc\": " + String(readVoltage());
  msg_pub = msg_pub + " ,\"rssi\": " + String(WiFi.RSSI());

  int sizeArray = sizeof buttons / sizeof *buttons;
  for(int i=0; i< sizeArray; i++) {
    msg_pub = msg_pub + " ,\"GPIO_" + buttons[i] + "_counter\": " + String(counter[i]);
    msg_pub = msg_pub + " ,\"GPIO_" + buttons[i] + "_state\": " + (digitalRead(buttons[i]) == HIGH ? "true" : "false");
  }

  #ifdef USE_TP4056
  msg_pub = msg_pub + " ,\"chrg\": " + !digitalRead(chrg);
  msg_pub = msg_pub + " ,\"stdby\": " + !digitalRead(stdby);
  #endif
  msg_pub +=" }";


  wakeUpPublished = mqttClient->publish(topic.c_str(), msg_pub.c_str());
}


ICACHE_RAM_ATTR void detectsChange(int pin) {
  if (millis() - lastTime < debounceMs) return; // ignore events faster than debounceMs
  lastTime = millis();

  int sizeArray = sizeof buttons / sizeof *buttons;
  for(int i=0; i< sizeArray; i++) {
    if (buttons[i] == pin){
      if (digitalRead(pin) == HIGH){
        counter[i]++;
      }
      Serial.printf(" -- GPIO %d count: %d\n", buttons[i], counter[i]);
      publishWakeUp("wakeup");
    return;
    }
  }

}

ICACHE_RAM_ATTR void pin12ISR() {detectsChange(12);}
ICACHE_RAM_ATTR void pin13ISR() {detectsChange(13);}


void setupPostBox(void){

  // ESP Awake Pin, the pin which keeps CH_PD HIGH, a requirement for normal functioning of ESP8266
  pinMode(wake, OUTPUT);
  digitalWrite(wake, HIGH);

  #ifdef USE_TP4056
  pinMode(chrg, INPUT);
  pinMode(stdby, INPUT);
  #endif

  //Check which button was pressed
  int sizeArray = sizeof buttons / sizeof *buttons;
  for(int i=0; i< sizeArray; i++) {
    pinMode(buttons[i], INPUT);
    if(digitalRead(buttons[i]) == HIGH) {
      button = buttons[i];
      counter[i]++;
      break;
    }
  }

  Serial.printf("\n -- Pin booter button: %d\n", button);

  attachInterrupt(digitalPinToInterrupt(12), pin12ISR , CHANGE);
  attachInterrupt(digitalPinToInterrupt(13), pin13ISR , CHANGE);

  // long bootDelay = millis() - connectionTime;
  long bootDelay = millis();
  Serial.printf("\tbootDelay: %ld - pin booter: %d\n", bootDelay, button);

  // Disconnect wifi if the reboot was not originated by GPIOs
  // but probably caused by updating firmware by UART
  if (button == -1) WiFi.disconnect();  
}

void turnESPOff (void){
    Serial.println("CH_PD disabled");
    delay(10);
    digitalWrite(wake, LOW); //Turns the ESP OFF
}



// Websocket functions to publish:
String getLoopTime(){ return String(currentLoopMillis - previousMainLoopMillis);}
String getRSSI(){ return String(WiFi.RSSI());}
String getHeapFree(){ return String((float)GET_FREE_HEAP/1000);}
String getVCC(){ return String((float)readVoltage());}




void setup() {
  Serial.begin(115200);
  
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.setDebugOutput(true);
  #endif

  setupPostBox();
  config.begin();
 
  config.addDashboardObject("heap_free", getHeapFree);
  config.addDashboardObject("loop", getLoopTime);
  config.addDashboardObject("RSSI", getRSSI);
  config.addDashboardObject("VCC", getVCC);

  config.setPreSleepRoutine(turnESPOff);

  mqttClient = config.getMQTTClient();

  Serial.println("###  Looping time\n");

}

void loop() {

  currentLoopMillis = millis();

  #if defined(USE_TP4056) && defined(NO_SLEEP_WHILE_CHARGING)
    if ( (!wakeUpPublished ) || !digitalRead(chrg) == true ) config.services.deep_sleep.enabled = false;    
    else config.services.deep_sleep.enabled = true;  
  #else
    if ( !wakeUpPublished ) config.services.deep_sleep.enabled = false;    
    else config.services.deep_sleep.enabled = true;  
  #endif

  config.loop();

  // Reconnection loop:
  // if (WiFi.status() != WL_CONNECTED) {
  //   config.begin();
  //   networkRestart();
  //   config.configureServer(&server);
  // }

  // Main Loop:
  if(mqttClient->connected() && (config.device.publish_time_ms != 0) &&
      (currentLoopMillis - previousPublishMillis > (unsigned)config.device.publish_time_ms)) {
    previousPublishMillis = currentLoopMillis;
    // Here starts the MQTT publish loop configured:


    if (config.services.deep_sleep.enabled) {
      String topic = config.getDeviceTopic() + "sleepCountdown";
      String msg_pub ="{\"time_to_sleep\": ";
      msg_pub += String((config.services.deep_sleep.sleep_delay*1000 + config.getDeviceSetupTime() - (currentLoopMillis))/1000);
      msg_pub +=" }";
      mqttClient->publish(topic.c_str(), msg_pub.c_str());
    }

    
    publishWakeUp("data");

  }

  previousMainLoopMillis = currentLoopMillis;
}
