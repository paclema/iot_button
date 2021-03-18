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

void PeopleCounter::loop(void){

  bool readStatus = sensor.sensorRead2Roi();

  // CHECK GESTURE USING ALGORITHM:
  if (sensor.distance[0] < rangeThresholdCounter_mm) statusFront = 2;
  else statusFront = 0;
  if (sensor.distance[1] < rangeThresholdCounter_mm) statusBack = 1;
  else statusBack = 0;

  statusPersonNow = statusFront + statusBack;
  if (statusPersonLast != statusPersonNow){
    statusPersonIndex++;
    statusPerson[statusPersonIndex] = statusPersonNow;
    statusPersonLast = statusPersonNow;
  } else if (statusPersonNow == 0 && statusPersonLast == 0){
    statusPersonIndex = 0;
  }

  // Serial.printf("Current statusPersonNow: %d - statusPersonIndex: %d - cnt: %d\n", statusPersonNow, statusPersonIndex, cnt);
  
  // Decode person gesture if statusPerson counter reaches enough information:
  if ( statusPersonIndex == 4 || (statusPersonIndex == 2 && statusPersonNow == 0)) {

    PeopleCounterGesture newGesture;
    int statusPersonTotal = 0;
    for (int i=0; i<STATUS_PERSON_ARRAY_SIZE; i++){
      statusPersonTotal += statusPerson[i];
    }

    switch (statusPersonTotal){
    case 6:
      if (statusPerson[1] == 2){
        cnt++;
        newGesture = PERSON_ENTERS;
      } else if (statusPerson[1] == 1) {
        cnt--;
        newGesture = PERSON_LEAVES;
      } else {
        newGesture = ERROR_DETECTING_PERSON;
      }
      break;
    case 2:
    case 7:
      newGesture = PERSON_TRY_TO_ENTER;
      break;
    case 1:
    case 5:
      newGesture = PERSON_TRY_TO_LEAVE;
      break;
    case 3:
      newGesture = ERROR_PERSON_TOO_FAST;
      break;
    default:
      newGesture = PERSON_NOT_FULL_DETECTED;
      break;
    }
    Serial.println();

    currentGesture = newGesture;
    PeopleCounter::notifyChange(newGesture);
    statusPersonIndex = 0;
    statusPersonNow = 0;
    statusPersonLast = 0;
    for (int i=0; i<STATUS_PERSON_ARRAY_SIZE; i++){
      statusPerson[i] = 0;
    }
  } else {
    currentGesture = NO_PERSON_DETECTED;
  }


  if ( sensor.distance[0] < rangeThresholdCounter_mm || sensor.distance[1] < rangeThresholdCounter_mm) { timeMark = millis(); }
  if (millis() - timeMark < 500) {

    // Serial.print("Distances: ");
    // Serial.print(sensor.distance[0]);
    // Serial.print(", ");
    // Serial.print(sensor.distance[1]);
    // Serial.print(": counter ---> ");
		// Serial.print((cnt));	
		// Serial.println();	
    // timeMark = millis(); 

    String msgStatusPerson = "[";
    for (int i=0; i<STATUS_PERSON_ARRAY_SIZE; i++){
      msgStatusPerson += String(statusPerson[i]);
      if (i != 4) msgStatusPerson += ",";
    }
    msgStatusPerson += "]";
    Serial.println(" - statusPerson[5]: " + msgStatusPerson + " - currentGesture: " + currentGesture);

    String topic_pub = "/iot-door/data/statusPerson";
    String msg_pub = "{ \"statusPerson\": " + msgStatusPerson + " }";
    mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());
    }




};


void PeopleCounter::printStatus(void){
  // From sensorDistance vl53l1x:
  // if (this->sensorDistance.debug){
  //   // Serial.print(this->sensorDistance.getSensorRange());
  //   this->sensorDistance.printSensorStatus();
  // }
};


void PeopleCounter::notifyChange(PeopleCounterGesture gesture){

  String msgGesture;
  String msgStatusPerson = "[";
  for (int i=0; i<STATUS_PERSON_ARRAY_SIZE; i++){
    msgStatusPerson += String(statusPerson[i]);
    if (i != 4) msgStatusPerson += ",";
  }
  msgStatusPerson += "]";
  
  switch (gesture){
  case PERSON_ENTERS:
    msgGesture = "Person enters";
    break;
  case PERSON_LEAVES:
    msgGesture = "Person leaves";
    break;
  case PERSON_TRY_TO_ENTER:
    msgGesture = "Person try to enter";
    break;
  case PERSON_TRY_TO_LEAVE:
    msgGesture = "Person try to leave";
    break;
  case ERROR_PERSON_TOO_FAST:
    msgGesture = "Error person moved to fast: " + msgStatusPerson;
    break;
  case ERROR_DETECTING_PERSON:
    msgGesture = "Error detecting person: " + msgStatusPerson;
    break;
  default:
    msgGesture = "PeopleCounterGesture not implemented: " + msgStatusPerson;
    break;
  }
  Serial.print(msgGesture);
  Serial.printf(" - cnt: %d", cnt);

  Serial.println(" - statusPerson[5]: " + msgStatusPerson + " - currentGesture: " + currentGesture);


  // Blink LED_BUILTIN once for 100ms:
  ledOn.once_ms(0, PeopleCounter::blink , true);
  ledOff.once_ms(100, PeopleCounter::blink, false);

  
  // Notify via MQTT:
  String topic_pub = "/iot-door/data/PeopleCounterGesture";
  String msg_pub = String(gesture);
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

  topic_pub = "/iot-door/data/PeopleCounterGesture/decoded";
  msg_pub = msgGesture;
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

  topic_pub = "/iot-door/data";
  msg_pub = "{ \"people_count\": " + String(this->cnt) + " }";
  mqttClient->publish(topic_pub.c_str(), msg_pub.c_str(), msg_pub.length());

}



