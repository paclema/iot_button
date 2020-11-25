#ifndef RadarMotor_h
#define RadarMotor_h
/*
RadarMotor.h - Library for move the sensor head


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
  boolean debug = false;

  public:
  bool enabled;
  float angleAccuracy = 0;
  float servoSpeed = 0;

  RadarMotor(void);

  void setup();

  void testMovement(void);
  void printData(int i);
  void setRange(void);

  float getFeedback(void);
  float getFeedbackAngle(void);

  void moveServo(void);

};

#endif
