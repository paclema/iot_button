#include <Arduino.h>


// Pinout with D1 mini:
//--------------------
//
// Echo -- D8
// Trigger -- D7
// VCC -- 5V
// GND -- GND


#define echoPin D8
#define trigPin D7

long duration; // Duration used to calculate distance
long previousSensorReadMillis = 0;
int sensorReadTimingBudget = 50;

boolean debug = false;


void sensorSetup(void){

	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);

}

bool sensorRead(float &distance){

	unsigned long currentLoopMillis = millis();

	if ((currentLoopMillis-previousSensorReadMillis) >= sensorReadTimingBudget) {
		previousSensorReadMillis = currentLoopMillis;
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
		duration = pulseIn(echoPin, HIGH);
		//Calculate the distance (in cm) based on the speed of sound.
		distance = 10*duration/58.2;
		
		if (debug){
			Serial.print(" duration: ");
			Serial.print(duration);
			Serial.print(" distance: ");
			Serial.println(distance);
		}
		//Delay 50ms before next reading.
		// delay(50);
		return true;
	}
	return false;
}
