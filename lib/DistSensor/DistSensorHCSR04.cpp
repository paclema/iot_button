#include "DistSensorHCSR04.h"


DistSensorHCSR04::DistSensorHCSR04(void) {
  if (this->debug) Serial.println("\t\tCreating DistSensorHCSR04");
};


DistSensorHCSR04::~DistSensorHCSR04(void) {
  if (this->debug) Serial.println("\t\tDestroying DistSensorHCSR04");
};


void DistSensorHCSR04::setup(void){

  pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
  if (this->debug) Serial.print("\t --> HCSR04 initialized!");

}


bool DistSensorHCSR04::sensorRead(float *distance){

  unsigned long currentLoopMillis = millis();

	if ((currentLoopMillis-previousSensorReadMillis) >= this->timeBudget) {
		previousSensorReadMillis = currentLoopMillis;
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
		duration = pulseIn(echoPin, HIGH);
		//Calculate the distance (in cm) based on the speed of sound.
		distance[0] = 10*duration/58.2;

		if (this->debug){
			Serial.print(" duration: ");
			Serial.print(duration);
			Serial.print(" distance: ");
			Serial.println(distance[0]);
		}
		//Delay 50ms before next reading.
		// delay(50);
		return true;
	}
	return false;
}


String DistSensorHCSR04::getSensorRange(void){
  // return String(this->sensor.rangeStatusToString(this->sensor.ranging_data.range_status));
  return "Not implemented for DistSensorHCSR04";
}


void DistSensorHCSR04::printSensorStatus(void){
  Serial.println("Not implemented for DistSensorHCSR04");
}


void DistSensorHCSR04::parseWebConfig(JsonObjectConst configObject){

}
