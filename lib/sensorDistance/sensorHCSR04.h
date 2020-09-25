#include <Arduino.h>



#define echoPin D8 // Echo Pin
#define trigPin D7 // Trigger Pin

long duration; // Duration used to calculate distance
long previousSensorReadMillis = 0;
int sensorReadTimingBudget = 50;


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

		Serial.print(" duration: ");
		Serial.print(duration);
		Serial.print(" distance: ");
		Serial.println(distance);
		//Delay 50ms before next reading.
		// delay(50);
		return true;
	}
	return false;
}
