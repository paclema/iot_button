#ifdef ESP32
#include <Arduino.h>
#include "ParallaxServoESP32.h"


ParallaxServoESP32::ParallaxServoESP32() {

}


ParallaxServoESP32::~ParallaxServoESP32() {
  ParallaxServoESP32::disableServo();
}


ParallaxServoESP32::ParallaxServoESP32(byte pinControl, byte pinFeedback) {
  setServoControl(pinControl);
  setServoFeedback(pinFeedback);
}

void ParallaxServoESP32::setup() {
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

  ParallaxServoESP32::enableServo();

  // servo.adjustSignal(34, 1045);
  // servo.calibrate();
  servo.setSpeed(140);
  Serial.println("Servo calibrated! Moving to 90deg");
  servo.rotateTo(90);
  delay(3000);
  // servo.wait();
  Serial.println("Servo init!");
  delay(1000);


}


void ParallaxServoESP32::setServoControl(byte pin) {

  this->controlPin = GPIO_ID_PIN(pin);
  // Servo control pin attach

  // this->servo.attach(this->controlPin);
  // servo.attach(this->controlPin, this->feedbackPin);
  ParallaxServoESP32::enableServo();
}


void ParallaxServoESP32::setServoFeedback(byte pin) {

  this->feedbackPin = pin;
  // Servo feedback pin attach
  // pinMode(this->feedbackPin, INPUT);
  // servo.attach(this->controlPin, this->feedbackPin);
  ParallaxServoESP32::enableServo();
}


void ParallaxServoESP32::rotate(float degree, int threshold) {
  this->targetAngle = degree;
  // this->ERROR_ANGLE_OFFSET_US = threshold;
  // servo.rotate(degree);
  // servo.rotateTo(this->targetAngle);
  servo.easeRotateTo(this->targetAngle);



}


bool ParallaxServoESP32::handle() {
  // // Run pulseWidth measuring to figure out the current angle of the servo
  // byte pin = this->feedbackPin;
  // unsigned long tHigh = pulseIn(pin, HIGH, this->pulseInTimeout);
  // unsigned long tLow = pulseIn(pin, LOW, this->pulseInTimeout);
  // unsigned long  tCycle = tHigh + tLow;
  // // Check if our cycle time was appropriate
  // if (!(tCycle > 1000 && tCycle < 1200)) {
  //   // Invalid cycle time, so try pulse measuring again
  //   // Serial.println("Invalid cycle time");
  //   return false;
  // }
  // // Calculate the duty cycle of the pulse
  // float dutyCycle = (DUTY_SCALE) * ((float) tHigh / tCycle);
  // float maxUnitsForCircle = UNITS_IN_FULL_CIRCLE - 1.0;
  //
  // // Calculate exact angle of servo
  // this->currentAngle = maxUnitsForCircle - ((dutyCycle - DUTY_CYCLE_MIN) * UNITS_IN_FULL_CIRCLE) / ((DUTY_CYCLE_MAX - DUTY_CYCLE_MIN) + 1);
  //
  // // Clip current angle if we're somehow above or below range
  // if (currentAngle < 0) {
  //   this->currentAngle = 0;
  // } else if (currentAngle > maxUnitsForCircle) {
  //   this->currentAngle = maxUnitsForCircle;
  // }
  //
  // // Handle quadrant wrap q1 -> q4 and q4 -> q1, to count turns
  // if ((currentAngle < ANGLE_Q2_MIN) && (prevAngle > ANGLE_Q3_MAX)) {
  //   this->turns += 1;
  // } else if ((prevAngle < ANGLE_Q2_MIN) && (currentAngle > ANGLE_Q3_MAX)) {
  //   this->turns -= 1;
  // }
  //
  // // Save previous position
  // this->prevAngle = currentAngle;
  // this->errorAngle = targetAngle - currentAngle;
  //
  //
  // // Simple P Controller
  // int outputSpeed = errorAngle * CONSTANT_KP;
  //
  // if (outputSpeed > MAX_PULSE_SPEED_OFFSET_US) {
  //   outputSpeed = MAX_PULSE_SPEED_OFFSET_US;
  // } else if (outputSpeed < MIN_PULSE_SPEED_OFFSET_US) {
  //   outputSpeed = MIN_PULSE_SPEED_OFFSET_US;
  // }
  //
  // int offset = 0;
  // if (errorAngle > 0) {
  //   offset = ERROR_ANGLE_OFFSET_US;
  // } else if (errorAngle < 0) {
  //   offset = -1 * ERROR_ANGLE_OFFSET_US;
  // }
  // outputSpeed = HOLD_STILL_PULSE_SPEED_US + outputSpeed + offset;
  // servo.writeMicroseconds(outputSpeed);
  //
  return true;



}


float ParallaxServoESP32::getAngle(){
  this->currentAngle = servo.getAngle();
  return this->currentAngle;
}


float ParallaxServoESP32::getAngleTarget(){
  return this->targetAngle;
}


int ParallaxServoESP32::getTurns(){
  this->turns = servo.getTurns();
  return this->turns;
}


void ParallaxServoESP32::enableServo(void){
  if(!this->servoAttached){
    servo.attach(this->controlPin, this->feedbackPin);
    this->servoAttached = true;
  }
}


void ParallaxServoESP32::disableServo(void){
  if(this->servoAttached){
    this->servo.detach();
    this->servoAttached = false;
  }
}

#endif
