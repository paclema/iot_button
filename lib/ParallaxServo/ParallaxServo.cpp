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
    Serial.print("\tfeedbackPin: ");
    Serial.print(this->feedbackPin);
    Serial.print("\tangleOffset: ");
    Serial.println(this->angleOffset);
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

  // noInterrupts();  // Removing interrupts will cause other features such us webserver or mqtt client to fail
  unsigned long tHigh = pulseIn(pin, HIGH, this->pulseInTimeout);
  unsigned long tLow = pulseIn(pin, LOW, this->pulseInTimeout);
  // interrupts();
  unsigned long  tCycle = tHigh + tLow;

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

  // Save previous position
  this->prevAngle = currentAngle;

}


void ParallaxServo::rotate(float degree, int threshold) {
  this->targetAngle = degree;
  // this->ERROR_ANGLE_OFFSET_US = threshold;


}


bool ParallaxServo::handle() {
  // Run pulseWidth measuring to figure out the current angle of the servo
  byte pin = this->feedbackPin;
  // noInterrupts();  // Removing interrupts will cause other features such us webserver or mqtt client to fail
  unsigned long tHigh = pulseIn(pin, HIGH, this->pulseInTimeout);
  unsigned long tLow = pulseIn(pin, LOW, this->pulseInTimeout);
  // interrupts();
  unsigned long  tCycle = tHigh + tLow;
  // Check if our cycle time was appropriate
  if (!(tCycle > 1000 && tCycle < 1200) || tHigh == 0 || tLow == 0) {
    // Invalid cycle time, so try pulse measuring again
    // Serial.printf("Invalid cycle time in feedback pin %d: %lu ", pin, tCycle);
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

  // Handle longs delays while reading:
  int targetOver = 0;
  float freeOffset = 5;

    if ((this->targetAngle - freeOffset > this->currentAngle ) && (this->prevAngle > this->targetAngle) ){
        // Serial.printf("-- targetOver for servo pin %d 1\n", this->controlPin);
        targetOver = 1;
  } else if( (this->targetAngle + freeOffset < this->currentAngle ) && (this->prevAngle < this->targetAngle) ) {
        // Serial.printf("-- targetOver for servo pin %d 2\n", this->controlPin);
        targetOver = 2;
  } else {
      targetOver = 0;
  }

  // Save previous position
  this->prevAngle = currentAngle;
  this->errorAngle = targetAngle - currentAngle;


  // Simple P Controller to keep speed constant among the cicle
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
  outputSpeed += offset;


  // Prevent more than 1 loop in each direction:
  // Hardcode turn to come back to turn = 0 to avoid sensor wires to rotate around the servo horn
  if (this->turns >= 1 ){
    Serial.printf("Motor pin %d turn +1\n", this->controlPin);
    outputSpeed = -1 * abs(outputSpeed);
  } else if (this->turns < 0 ){
    Serial.printf("Motor pin %d turn -1\n", this->controlPin);
    outputSpeed = 1 * abs(outputSpeed);
  }


  // Prevent more than 1 loop in each direction if turns are not detected due lack of readings:
  /*
  if (targetOver != 0){
    // if(outputSpeed > 0) outputSpeed *= -1 ;
    // else outputSpeed *= -1 ;
    // outputSpeed = - outputSpeed ;

    if ( this->lastTargetOver != targetOver){
      if (targetOver == 1 ){
        Serial.printf("-- Reversed direction for servo pin %d with +1 loop\n", this->controlPin);
        outputSpeed = 1 * abs(outputSpeed);
      } else if (targetOver == 2 ){
        Serial.printf("-- Reversed direction for servo pin %d with -1 loop\n", this->controlPin);
        outputSpeed = -1 * abs(outputSpeed);
      }

    }
    this->lastTargetOver = targetOver;
  }
  */



  outputSpeed = HOLD_STILL_PULSE_SPEED_US + outputSpeed;
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
