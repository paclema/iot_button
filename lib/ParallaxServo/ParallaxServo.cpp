#include <Arduino.h>
#include "ParallaxServo.h"


ParallaxServo::ParallaxServo() {

}


ParallaxServo::~ParallaxServo()
{

  ParallaxServo::disableServo();
}


ParallaxServo::ParallaxServo(byte pinControl, byte pinFeedback) {
  setServoControl(pinControl);
  setServoFeedback(pinFeedback);
}

void ParallaxServo::setup() {
  if (debug){
    Serial.print("\tcontrolPin: ");
    Serial.print(this->controlPin);
    Serial.print("\t");
    Serial.print(" feedbackPin: ");
    Serial.println(this->feedbackPin);
  }
  setServoControl(this->controlPin);
  setServoFeedback(this->feedbackPin);

  ParallaxServo::enableServo();
}


void ParallaxServo::setServoControl(byte pin) {

  #ifdef ESP32
    this->controlPin = GPIO_ID_PIN(pin);
  #elif defined(ESP8266)
    this->controlPin = pin;
  #endif

  // ParallaxServo::enableServo();
  // this->servo.attach(this->controlPin);
}


void ParallaxServo::setServoFeedback(byte pin) {

  this->feedbackPin = pin;
  // Servo feedback pin attach
  pinMode(this->feedbackPin, INPUT);
}


void ParallaxServo::rotate(float degree, int threshold) {
  this->targetAngle = degree;
  // this->ERROR_ANGLE_OFFSET_US = threshold;


}


bool ParallaxServo::handle() {
  // Run pulseWidth measuring to figure out the current angle of the servo
  byte pin = this->feedbackPin;
  unsigned long tHigh = pulseIn(pin, HIGH, this->pulseInTimeout);
  unsigned long tLow = pulseIn(pin, LOW, this->pulseInTimeout);
  unsigned long  tCycle = tHigh + tLow;
  // Check if our cycle time was appropriate
  if (!(tCycle > 1000 && tCycle < 1200) || tHigh == 0 || tLow == 0) {
    // Invalid cycle time, so try pulse measuring again
    // Serial.println("Invalid cycle time");
    return false;
  }
  // Calculate the duty cycle of the pulse
  float dutyCycle = (DUTY_SCALE) * ((float) tHigh / tCycle);
  float maxUnitsForCircle = UNITS_IN_FULL_CIRCLE - 1.0;

  // Calculate exact angle of servo
  this->currentAngle = maxUnitsForCircle - ((dutyCycle - DUTY_CYCLE_MIN) * UNITS_IN_FULL_CIRCLE) / ((DUTY_CYCLE_MAX - DUTY_CYCLE_MIN) + 1);

  // Clip current angle if we're somehow above or below range
  if (currentAngle < 0) {
    this->currentAngle = 0;
  } else if (currentAngle > maxUnitsForCircle) {
    this->currentAngle = maxUnitsForCircle;
  }

  // Handle quadrant wrap q1 -> q4 and q4 -> q1, to count turns
  if ((currentAngle < ANGLE_Q2_MIN) && (prevAngle > ANGLE_Q3_MAX)) {
    this->turns += 1;
  } else if ((prevAngle < ANGLE_Q2_MIN) && (currentAngle > ANGLE_Q3_MAX)) {
    this->turns -= 1;
  }

  // Save previous position
  this->prevAngle = currentAngle;
  this->errorAngle = targetAngle - currentAngle;


  // Simple P Controller
  int outputSpeed = errorAngle * CONSTANT_KP;

  if (outputSpeed > MAX_PULSE_SPEED_OFFSET_US) {
    outputSpeed = MAX_PULSE_SPEED_OFFSET_US;
  } else if (outputSpeed < MIN_PULSE_SPEED_OFFSET_US) {
    outputSpeed = MIN_PULSE_SPEED_OFFSET_US;
  }

  int offset = 0;
  if (errorAngle > 0) {
    offset = ERROR_ANGLE_OFFSET_US;
  } else if (errorAngle < 0) {
    offset = -1 * ERROR_ANGLE_OFFSET_US;
  }
  outputSpeed = HOLD_STILL_PULSE_SPEED_US + outputSpeed + offset;
  servo.writeMicroseconds(outputSpeed);

  return true;

}


float ParallaxServo::getAngle(){
  return this->currentAngle;
}


float ParallaxServo::getAngleTarget(){
  return this->targetAngle;
}


int ParallaxServo::getTurns(){
  return this->turns;
}


void ParallaxServo::enableServo(void){
  if(!this->servoAttached){
    this->servo.attach(this->controlPin);
    this->servoAttached = true;
  }
}


void ParallaxServo::disableServo(void){
  if(this->servoAttached){
    this->servo.detach();
    this->servoAttached = false;
  }
}
