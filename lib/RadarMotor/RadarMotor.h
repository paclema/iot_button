#ifndef RadarMotor_h
#define RadarMotor_h
/*
RadarMotor.h - Library to control and move the sensor head using a positional
servo motor hacked to get the feedback position reading the internal potentiometer
as analog input.
You must get out the internal potentimeter signal (usually among the 3 pins it
is the middle one) soldering a wire and connect it to an analog input.

There is only one analog input A0 for ESP8266
Servo PWM control signal attached on GPIO0 also known as D3 in Wemos D1 mini.

If you are using ESP32 you can choose among several GPIOs with ADC capabilities.

*/


#include <Arduino.h>

#include <Servo.h>


#define  START_ANGLE          25.0
#define  END_ANGLE            160.0
#define ANGLE_RANGE           (END_ANGLE - START_ANGLE)
#define TIME_TO_TRAVEL_RANGE  605 // Time on ms prooved that the servo takes to move its full range
#define ANGULAR_SPEED         (ANGLE_RANGE / TIME_TO_TRAVEL_RANGE)
#define MIN_MS_PER_GRAD       (TIME_TO_TRAVEL_RANGE / ANGLE_RANGE)

class RadarMotor{

  private:

  Servo servo;

  #ifdef ESP32
    byte controlPin = GPIO_ID_PIN(14);
    byte feedbackPin = GPIO_ID_PIN(34);  // GPIO 34 (Analog ADC1_CH6)
  #elif defined(ESP8266)
    byte controlPin = D3;
    byte feedbackPin = A0;
  #endif

  float servoIncrement = 0;
  float servoPos = START_ANGLE;
  int reverseIncrement = 1;

  long previousServoMillis = 0;
  int timeSinceLastMove = 0;

  // float time_between_servo_writes = angleAccuracy*(ANGULAR_SPEED);
  float time_between_servo_writes = 0;

  // Testing servo pot:
  int lowEnd;                         // servo feedback at 0 degrees
  int highEnd;                        // servo feedback at 180 degrees
  float feedBack;                     // used to hold servo feedback value
  static const int feedbackReadings = 20; // Readings for each feedback
  const int  discardReadings = 6;    // Discarted readings
  int reading[feedbackReadings];     // Array of readings for the same feedback
  float offset = 0;
  float angleFeedback;                // Feedback potentiometer pulse to angle
  float mappedPulse;                  // Expected potentiometer feedback
  bool debug = false;

  public:
  bool enabled;
  float angleAccuracy = 0;
  float servoSpeed = 0;

  RadarMotor(void);

  void setup();
  bool handle(){
    moveServo();
    return true;
  };
  void rotate(float degree, int threshold){
    if (this->debug) Serial.println(" rotate() not implemented");
  };

  void testMovement(void);
  void printData(int i);
  void setRange(void);

  float getFeedback(void);
  float getAngle(void);

  void moveServo(void);
  void enableServo(void){ this->servo.attach(this->controlPin); };
  void disableServo(void){ this->servo.detach(); };

  void setServoControl(byte servoPinNumber);
  void setServoFeedback(byte servoPinNumber);
  void setSpeedPulse(int speedPulse){
    this->servoSpeed = speedPulse;
  };

  float getAngleTarget(){ return servoPos + servoIncrement; };
  int getTurns(){ return 0; };

  void setDebug(bool d){ this->debug = d; };
  bool isDebug(void){ return this->debug; };

};

#endif
