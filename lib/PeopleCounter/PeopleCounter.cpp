#include "PeopleCounter.h"



PeopleCounter::PeopleCounter(void) {
  this->nameConfigObject = "PeopleCounter";
};


PeopleCounter::PeopleCounter(String name) {
  this->nameConfigObject = name;
};


void PeopleCounter::parseWebConfig(JsonObjectConst configObject){

  // JsonObject received:
  // serializeJsonPretty(configObject, Serial);

  // Disable distance sensors:
  // if (this->peopleCounterInitialized) PeopleCounter::disableDistSensors();


  // PeopleCounter IWebConfig object:
  this->debug = configObject["debug"] | false;
  this->rangeThresholdCounter_mm = configObject["person_threshold_mm"] | 800;


  // LDR sensor:
  this->LDREnabled = configObject["LDR"]["enabled"] | false;
  this->LDRPin = configObject["LDR"]["pin"];


  // Reed switch:
  this->reedSwitchEnabled = configObject["reed_swtich"]["enabled"] | false;
  this->reedSwitchPin = configObject["reed_swtich"]["pin"];


  // LED strip WS2812B:
  this->ledEnabled = configObject["LED_strip"]["enabled"] | false;
  this->ledPin = configObject["LED_strip"]["pin"];
  this->ledCount = configObject["LED_strip"]["count"];
  this->ledBrightness = configObject["LED_strip"]["brightness"];
  // this->ledType = configObject["LED_strip"]["type"];

  if (!this->ledEnabled) {
  // this->pixels.clear();
  this->pixels.setBrightness(0);
  this->pixels.show();
}

  // VL53L1X with ROI sensor:
  if (configObject["vl53l1x"]["enabled"]){
    sensor.setEnable(configObject["ROI"]["enabled"] | false);
    sensor.setDebug(configObject["vl53l1x"]["debug"] | false);
    sensor.setTimeBudget(configObject["vl53l1x"]["time_budget_ms"]);
  }

  sensor.parseWebConfig(configObject);


  // Setup again PeopleCounter:
  // if (this->peopleCounterInitialized) PeopleCounter::setupDistSensors();
};


void PeopleCounter::enablePeopleCounterServices(void){
  Serial.println("--- PeopleCounter: ");
  Serial.printf("   - Debug: %s\n", this->debug ? "true" : "false");


  // LDR sensor:
  Serial.printf("   - LDR sensor:\n");
  Serial.printf("       - Enabled: %s\n", this->LDREnabled ? "true" : "false");
  Serial.printf("       - Pin: %d\n", this->LDRPin);


  // Reed switch:
  PeopleCounter::setupReedSwitch();
  Serial.printf("   - Reed Switch:\n");
  Serial.printf("       - Enabled: %s\n", this->reedSwitchEnabled ? "true" : "false");
  Serial.printf("       - Pin: %d\n", this->reedSwitchPin);


  // LED strip WS2812B:
  PeopleCounter::setupLEDStrip();
  Serial.printf("   - LED strip:\n");
  Serial.printf("       - Enabled: %s\n", this->ledEnabled ? "true" : "false");
  Serial.printf("       - Pin: %d\n", this->ledPin);
  Serial.printf("       - Brightness: %d\n", this->ledBrightness);
  Serial.printf("       - Default color: %d\n", this->ledDefaultColor);


  // Distance sensor vl53l1x:
  sensor.setName("vl53l1x");
  sensor.setType("VL53L1X_ROI");
  sensor.setup();
  

  this->peopleCounterInitialized = true;
};


void PeopleCounter::disableDistSensors(void){

  // Serial.printf("Disabling %d sensors:\n", distanceSensors.size());
  // DistSensor *sensorTemp ;
  // for(int i = 0; i < distanceSensors.size(); i++){
  //   sensorTemp = distanceSensors.get(i);
  //   sensorTemp->disableSensor();
  //   if (this->debug) Serial.printf("\t-> %s disabled\n", sensorTemp->getName().c_str());
  // }

  sensor.disableSensor();


};


void PeopleCounter::setupDistSensors(void){

};

void PeopleCounter::setupLEDStrip(void){
  this->pixels.updateType(this->ledType);
  this->pixels.updateLength(this->ledCount);
  this->pixels.setPin(this->ledPin);

  this->pixels.begin();
  this->pixels.clear();
  PeopleCounter::setLEDStripColor(this->ledDefaultColor);

};

void PeopleCounter::setupReedSwitch(void){
  pinMode(this->reedSwitchPin, INPUT);
  this->reedSwitchState = digitalRead(this->reedSwitchPin);
  this->reedSwitchStateLast = this->reedSwitchState;
};

void PeopleCounter::setLEDStripColor(uint32_t c){
  // uint32_t c is a color of type pixels.Color(r,g,b) also can be 0xFF0000 for red and 0xFFFFFF for white
  if (this->ledEnabled){
    // this->pixels.clear();
    for(int i=0; i<this->ledCount; i++) { 
      this->pixels.setPixelColor(i,c);
      pixels.setBrightness(this->ledBrightness);
      this->pixels.show();
    }
  }
};

void PeopleCounter::setLEDStripColor(uint8_t r, uint8_t g, uint8_t b){
  if (this->ledEnabled) PeopleCounter::setLEDStripColor(this->pixels.Color(r,g,b));
};

void PeopleCounter::updateReedSwitch(void){
  reedSwitchState = digitalRead(reedSwitchPin);

  if (reedSwitchStateLast!=reedSwitchState){
    reedSwitchStateLast = reedSwitchState;
    PeopleCounter::notifyReedSwitch();
    PeopleCounter::notifyData();
  }
};

void PeopleCounter::loop(void){

  if (this->LDREnabled) this->LDRValue = analogRead(this->LDRPin);
  if (this->reedSwitchEnabled) PeopleCounter::updateReedSwitch();

  VL53L1_Error readStatus = sensor.sensorRead2Roi();

  // CHECK GESTURE USING ALGORITHM:
  if (readStatus == VL53L1_ERROR_NONE){
      if (sensor.distance[0] < rangeThresholdCounter_mm) statusFront = 2;
      else statusFront = 0;
      if (sensor.distance[1] < rangeThresholdCounter_mm) statusBack = 1;
      else statusBack = 0;

    // Process new gesture if no error occurs reading distances:
    PeopleCounter::processGesture();

    // Store distances:
    if (currentGesture == NO_PERSON_DETECTED) { 
      addZoneDistances(sensor.distance[0], sensor.distance[1]);
      }
    else if (currentGesture == PERSON_IN_RANGE_START &&
            lastGesture == NO_PERSON_DETECTED) {
      addZoneDistances(sensor.distance[0], sensor.distance[1]);
      PeopleCounter::notifyZoneDistances();
    }
    
  } else {
      currentGesture = ERROR_READING_SENSOR;
      PeopleCounter::notifyGesture(currentGesture);
  }
  lastGesture = currentGesture;
};


void PeopleCounter::processGesture(void){

  statusPersonNow = statusFront + statusBack;
  if (statusPersonLast != statusPersonNow){
    if (statusPersonIndex == 0){
      currentGesture = PERSON_IN_RANGE_START;
      PeopleCounter::notifyGesture(currentGesture);
    } else if (statusPersonIndex == 3){
      currentGesture = PERSON_IN_RANGE_END;
      PeopleCounter::notifyGesture(currentGesture);
    } else { 
      currentGesture = PERSON_IN_RANGE;
      PeopleCounter::notifyGesture(currentGesture);
    }

    statusPersonIndex++;
    statusPerson[statusPersonIndex] = statusPersonNow;
    statusPersonLast = statusPersonNow;

    // Notify statusPerson after new status added to the list:
    PeopleCounter::notifyStatusPerson();
    
    // If a person try to enter or leave several times in the same measurement like [0,1,3,1,3] or [0,2,3,2,3] or [0,1,3,2,3]
    // We should remove the status repeated at index 3 and 4: [0,1,3,-,-] or [0,2,3,-,-] to still keep track until the person leaves:
    if (statusPersonIndex == 4  && statusPerson[2] == 3 && statusPerson[2] == statusPerson[4]) {
      
      currentGesture = PERSON_IN_RANGE_IN_OUT;

      PeopleCounter::notifyGesture(currentGesture);
      statusPersonIndex = 2;
      statusPerson[3] = 0;
      statusPerson[4] = 0;
    }

  } else if (statusPersonNow == 0 && statusPersonLast == 0){
    statusPersonIndex = 0;
    currentGesture = NO_PERSON_DETECTED;
  }

  // Serial.printf("Current statusPersonNow: %d - statusPersonIndex: %d - cnt: %d\n", statusPersonNow, statusPersonIndex, cnt);
  
  // Decode person gesture [0,3,X...]:
  if ( statusPersonIndex == 1 && statusPersonNow == 3 ) {
      currentGesture = ERROR_PERSON_TOO_FAST;
      PeopleCounter::notifyGesture(currentGesture);
      // TODO: Check last distances and discover which zone went down first

      
  }

  // Decode person gesture if statusPerson counter reaches enough information:
  if ( statusPersonIndex == 4 || (statusPersonIndex == 2 && statusPersonNow == 0)) {

    // PeopleCounterGesture newGesture;
    int statusPersonTotal = 0;
    for (int i=0; i<=STATUS_PERSON_ARRAY_SIZE; i++){
      statusPersonTotal += statusPerson[i];
    }

    switch (statusPersonTotal){
    case 6:
      if (statusPerson[1] == 2){
        cnt++;
        currentGesture = PERSON_ENTERS;
      } else if (statusPerson[1] == 1) {
        cnt--;
        currentGesture = PERSON_LEAVES;
      } else {
        currentGesture = ERROR_DETECTING_PERSON;
      }
      break;
    case 2:
    case 7:
      currentGesture = PERSON_TRY_TO_ENTER;
      break;
    case 1:
    case 5:
      currentGesture = PERSON_TRY_TO_LEAVE;
      break;
    case 3:
      currentGesture = ERROR_PERSON_TOO_FAST;
      break;
    default:
      currentGesture = ERROR_PERSON_NOT_FULL_DETECTED;
      break;
    }
    Serial.println();

    PeopleCounter::notifyGesture(currentGesture);
    statusPersonIndex = 0;
    statusPersonNow = 0;
    statusPersonLast = 0;
    for (int i=0; i<=STATUS_PERSON_ARRAY_SIZE; i++){
      statusPerson[i] = 0;
    }
  }


  // if ( sensor.distance[0] < rangeThresholdCounter_mm || sensor.distance[1] < rangeThresholdCounter_mm) { timeMark = millis(); }
  // if (millis() - timeMark < 500) {

    // Serial.print("Distances: ");
    // Serial.print(sensor.distance[0]);
    // Serial.print(", ");
    // Serial.print(sensor.distance[1]);
    // Serial.print(": counter ---> ");
		// Serial.print((cnt));	
		// Serial.println();	
    // timeMark = millis(); 

  //   String msgStatusPerson = "[";
  //   for (int i=0; i<STATUS_PERSON_ARRAY_SIZE; i++){
  //     msgStatusPerson += String(statusPerson[i]);
  //     if (i != 4) msgStatusPerson += ",";
  //   }
  //   msgStatusPerson += "]";
  //   Serial.println(" - statusPerson[5]: " + msgStatusPerson + " - currentGesture: " + currentGesture);
  // }

};


void PeopleCounter::printStatus(void){
  // From sensorDistance vl53l1x:
  // if (this->sensorDistance.debug){
  //   // Serial.print(this->sensorDistance.getSensorRange());
  //   this->sensorDistance.printSensorStatus();
  // }
};


void PeopleCounter::notifyGesture(PeopleCounterGesture gesture){

  String msgGesture;
  String msgStatusPerson = "[";
  for (int i=0; i<=statusPersonIndex; i++){
    msgStatusPerson += String(statusPerson[i]);
    if (i != (statusPersonIndex)) msgStatusPerson += ",";
  }
  msgStatusPerson += "]";
  
  switch (gesture){
  case PERSON_ENTERS:
    msgGesture = "Person enters";
    PeopleCounter::setLEDStripColor(this->ledDefaultColor);
    break;
  case PERSON_LEAVES:
    msgGesture = "Person leaves";
    PeopleCounter::setLEDStripColor(this->ledDefaultColor);
    break;
  case PERSON_TRY_TO_ENTER:
    msgGesture = "Person try to enter";
    PeopleCounter::setLEDStripColor(200,200,255);
    break;
  case PERSON_TRY_TO_LEAVE:
    msgGesture = "Person try to leave";
    PeopleCounter::setLEDStripColor(200,200,255);
    break;
  case PERSON_IN_RANGE:
    msgGesture = "Person under ranging";
    PeopleCounter::setLEDStripColor(0,0,255);
    break;
  case PERSON_IN_RANGE_START:
    msgGesture = "Person under ranging starts";
    PeopleCounter::setLEDStripColor(255,255,0);
    break;
  case PERSON_IN_RANGE_END:
    msgGesture = "Person under ranging ends";
    PeopleCounter::setLEDStripColor(255,255,0);
    break;
  case PERSON_IN_RANGE_IN_OUT:
    msgGesture = "Person in and out under ranging";
    PeopleCounter::setLEDStripColor(255,255,0);
    break;
  case ERROR_PERSON_TOO_FAST:
    msgGesture = "Error person moved too fast: " + msgStatusPerson;
    PeopleCounter::setLEDStripColor(255,0,0);
    break;
  case ERROR_DETECTING_PERSON:
    msgGesture = "Error detecting person: " + msgStatusPerson;
    PeopleCounter::setLEDStripColor(255,0,0);
    break;
  case ERROR_PERSON_NOT_FULL_DETECTED:
    msgGesture = "Error person not full detected: " + msgStatusPerson;
    PeopleCounter::setLEDStripColor(255,0,0);
    break;
  case ERROR_READING_SENSOR:
    msgGesture = "Error reading distance sensor: " + msgStatusPerson;
    PeopleCounter::setLEDStripColor(255,0,0);
    break;
  default:
    msgGesture = "PeopleCounterGesture not implemented: " + msgStatusPerson;
    PeopleCounter::setLEDStripColor(255,0,0);
    break;
  }
  if (this->debug){
    Serial.print(msgGesture);
    Serial.printf(" - cnt: %d\n", cnt);
  }
  
  // Blink LED_BUILTIN once for 100ms:
  ledOn.once_ms(0, PeopleCounter::blink , true);
  ledOff.once_ms(100, PeopleCounter::blink, false);

  
  // Notify via MQTT:
  String topic_pub = this->mqttBaseTopic + "/data/PeopleCounterGesture";
  String msg_pub = String(gesture);
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

  topic_pub = this->mqttBaseTopic + "/data/PeopleCounterGesture/decoded";
  msg_pub = msgGesture;
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

  if (gesture == PERSON_ENTERS || gesture == PERSON_LEAVES) PeopleCounter::notifyData();
  
  // PeopleCounter::notifyStatusPerson();
}


void PeopleCounter::notifyZoneDistances(){

  // Notify via MQTT:
  String topic_pub = this->mqttBaseTopic + "/data/zoneDistances";
  String msg_pub = "{ \"zoneDistances_pre_gesture\": " + getLastZoneDistances() + " }";
  mqttClient->setBufferSize((uint16_t)(msg_pub.length() + 100));
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

  clearZoneDistances();
}


void PeopleCounter::notifyStatusPerson(){

  String msgStatusPerson = "[";
  for (int i=0; i<=statusPersonIndex; i++){
    msgStatusPerson += String(statusPerson[i]);
    if (i != (statusPersonIndex)) msgStatusPerson += ",";
  }
  msgStatusPerson += "]";
  
  if (this->debug) Serial.println(" - statusPerson[5]: " + msgStatusPerson);

  // Notify via MQTT:
  String topic_pub = this->mqttBaseTopic + "/data/statusPerson";
  String msg_pub = "{ \"statusPerson\": " + msgStatusPerson + " }";
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

}


void PeopleCounter::notifyData(){

  String topic_pub = this->mqttBaseTopic + "/data";
  String msg_pub = "{";
  msg_pub += "\"peopleCount\": " + String(this->cnt);
  // msg_pub += "\"statusPerson\": " + msgStatusPerson + ", ";
  if (this->LDREnabled) msg_pub += ", \"LDR\": " + String(this->LDRValue);
  if (this->reedSwitchEnabled) msg_pub += ", \"reedSwitch\": " + String(this->reedSwitchState);
  msg_pub += " }";

  mqttClient->setBufferSize((uint16_t)(msg_pub.length() + 100));
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

}


void PeopleCounter::notifyReedSwitch(){
  String topic_pub = this->mqttBaseTopic + "/data/reedSwitch";
  String msg_pub = "{ \"reedSwitch\": " + String(this->reedSwitchState) + " }";
  if (this->debug) Serial.printf("\t-> reedSwitch: %d\n", this->reedSwitchState);
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());
}


void PeopleCounter::clearZoneDistances(void){
  for (size_t i = 0; i < DIST_PRE_GESTURE_ARRAY_SIZE; i++) {
    zoneDistPreGesture[i].dist1 = 0;
    zoneDistPreGesture[i].dist2 = 0;
  }
};


void PeopleCounter::addZoneDistances(int dist1, int dist2){
  zoneDistPreGesture[wIndexPreGesture].dist1 = dist1;
  zoneDistPreGesture[wIndexPreGesture].dist2 = dist2;
  if ( wIndexPreGesture == DIST_PRE_GESTURE_ARRAY_SIZE-1) wIndexPreGesture = 0;
  else wIndexPreGesture++;
};


void PeopleCounter::printZoneDistances(void){
  Serial.printf("zoneDistPreGesture[%d]: [", DIST_PRE_GESTURE_ARRAY_SIZE);
  for (size_t i = 0; i < DIST_PRE_GESTURE_ARRAY_SIZE; i++) {
    if (i == DIST_PRE_GESTURE_ARRAY_SIZE-1) Serial.printf( " [%d,%d] ", zoneDistPreGesture[i].dist1, zoneDistPreGesture[i].dist2);
    else Serial.printf( " [%d,%d],", zoneDistPreGesture[i].dist1, zoneDistPreGesture[i].dist2);
  }
  Serial.printf("]  - wIndexPreGesture: %d\n", wIndexPreGesture);
}


String PeopleCounter::getLastZoneDistances(void){
  ZoneDistances objectTemp[DIST_PRE_GESTURE_ARRAY_SIZE] = {};
  int wIndexTemp = 0;
  String outString = "[";

  // From current write index to final point:
  for (int i = wIndexPreGesture; i < DIST_PRE_GESTURE_ARRAY_SIZE; i++) {
    outString += " [";
    outString += zoneDistPreGesture[i].dist1;
    outString += ",";
    outString += zoneDistPreGesture[i].dist2;
    outString += "],";
    objectTemp[wIndexTemp] = zoneDistPreGesture[i];
    wIndexTemp++;
  }

  // From 0 index to current write index point:
  for (int i = 0; i < wIndexPreGesture; i++) {
    if (i == (wIndexPreGesture-1)) {
      outString += " [";
      outString += zoneDistPreGesture[i].dist1;
      outString += ",";
      outString += zoneDistPreGesture[i].dist2;
      outString += "] ";
    } else {
      outString += " [";
      outString += zoneDistPreGesture[i].dist1;
      outString += ",";
      outString += zoneDistPreGesture[i].dist2;
      outString += "],";
    }
    objectTemp[wIndexTemp] = zoneDistPreGesture[i];
    wIndexTemp++;
  }
  outString += "]";

  // Re-order buffer to keep last value in the last index:
  memcpy(zoneDistPreGesture, objectTemp, sizeof(zoneDistPreGesture));
  wIndexPreGesture = 0;
  // printBuffer();
  if (this->debug) Serial.printf("zoneDistPreGesture[%d]: %s", DIST_PRE_GESTURE_ARRAY_SIZE, outString.c_str());

  return outString;
};