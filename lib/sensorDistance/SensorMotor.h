/*
  SensorMotor.h - Library for move the sensor head


*/
#ifndef SensorMotor_h
#define SensorMotor_h

#include <Arduino.h>
#include <Servo.h>

#define  START_ANGLE          25.0
#define  END_ANGLE            160.0
#define ANGLE_RANGE           (END_ANGLE - START_ANGLE)
#define TIME_TO_TRAVEL_RANGE  605 // Time on ms prooved that the servo takes to move its full range
#define ANGULAR_SPEED         (ANGLE_RANGE / TIME_TO_TRAVEL_RANGE)
#define MIN_MS_PER_GRAD       (TIME_TO_TRAVEL_RANGE / ANGLE_RANGE)

class SensorMotor
{
  private:

  Servo servo;
  float servoIncrement = 0;
  float servoPos = START_ANGLE;
  int reverseIncrement = 1;
  float servoSpeed = 0;
  float angleAccuracy = 0;

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
  void sensorMotor(void);
  void sensorMotorSetup(float angle_accuracy, float servo_speed_ms){
    servoIncrement = angle_accuracy;
    angleAccuracy = angle_accuracy;
    servoSpeed = servo_speed_ms/60;
    servoPos = START_ANGLE;

    if (servoSpeed <= MIN_MS_PER_GRAD){
      servoSpeed = MIN_MS_PER_GRAD;
    }
    time_between_servo_writes = angleAccuracy * servoSpeed;

    Serial.print("MIN_MS_PER_GRAD:");
    Serial.print(MIN_MS_PER_GRAD);
    Serial.print(" servoSpeed:");
    Serial.print(servoSpeed);
    Serial.print(" angleAccuracy:");
    Serial.print(angleAccuracy);
    Serial.print(" angleAccuracy/(ANGULAR_SPEED):");
    Serial.print(angleAccuracy/(ANGULAR_SPEED));
    Serial.print(" time_between_servo_writes:");
    Serial.println(time_between_servo_writes);

    servo.attach(0); // Attaching Servo to D3
    servo.write(servoPos);

    setRange();

    // while (1) {
    //   testMovement();
    // }

  }

    void testMovement(void){
      servo.write(START_ANGLE);
      delay(3000);
      int increment = 1;
      for (int i=START_ANGLE; i<END_ANGLE; i = i+increment){
        servo.write(i);
        delay(10);
        feedBack = getFeedback();
        mappedPulse = map(i,START_ANGLE,END_ANGLE,lowEnd,highEnd);
        offset = mappedPulse - feedBack;
        printData(i);
      }
      for (int i=END_ANGLE; i>START_ANGLE; i = i-increment){
          servo.write(i);
          delay(10);
          feedBack = getFeedback();
          mappedPulse = map(i,START_ANGLE,END_ANGLE,lowEnd,highEnd);
          offset = mappedPulse - feedBack;
          printData(i);
      }
    }

    void printData(int i){
      Serial.print("Angle: ");
      Serial.print(i);
      Serial.print("  =  ");
      Serial.print(feedBack);
      Serial.print(" ");
      Serial.print(offset);
      Serial.print(" ");
      Serial.print(mappedPulse);
      Serial.print(" --> ");
      angleFeedback = map((feedBack),lowEnd,highEnd,START_ANGLE,END_ANGLE);
      Serial.print(angleFeedback);
      Serial.print(" DEVIATION: ");
      Serial.println(i - angleFeedback);

    }


    void setRange(void ){
      servo.write(START_ANGLE);
      delay(TIME_TO_TRAVEL_RANGE*4);
      lowEnd = getFeedback();
      servo.write(END_ANGLE);
      delay(TIME_TO_TRAVEL_RANGE*4);
      highEnd = getFeedback();

      Serial.print("START_ANGLE= ");
      Serial.print(lowEnd);
      Serial.print(" ");
      Serial.print("END_ANGLE= ");
      Serial.println(highEnd);

    }

    float getFeedback(void){
      float mean = 0;
      float result;
      int test;
      boolean done;
      if (debug)
        Serial.print("     Readings --> ");
      // Get several servo potentiometer feedback measurements:
      for (int j=0; j<feedbackReadings; j++){
        reading[j] = analogRead(0);
        if (debug){
          Serial.print(reading[j]);
          Serial.print(", ");
        }
        // delay(3);
      }

      // Sort the readings low to high in array:
      done = false;
      while(done != true){
        done = true;
        for (int j=0; j<feedbackReadings; j++){
          if (reading[j] > reading[j + 1]){
            test = reading[j + 1];
            reading [j+1] = reading[j] ;
            reading[j] = test;
            done = false;
          }
        }
      }

      // Discard the discardReadings highest and discardReadings lowest readings:
      for (int k=discardReadings; k<(feedbackReadings-discardReadings); k++){
        mean += reading[k];
      }
      // Average of readings:
      result = mean/8;
      if (debug){
        Serial.print(" --> ");
        Serial.println(result);
      }

      return(result);
    }

    float getFeedbackAngle(void){
      return map((getFeedback()),lowEnd,highEnd,START_ANGLE,END_ANGLE);
    }


    void moveServo(void){
      unsigned long currentLoopMillis = millis();
      timeSinceLastMove = currentLoopMillis - previousServoMillis;


      if (timeSinceLastMove >= time_between_servo_writes){
        previousServoMillis = currentLoopMillis;


        if (((servoPos + servoIncrement) >= END_ANGLE) || ((servoPos + servoIncrement) <= START_ANGLE)){
          // reverse direction
          servoIncrement = -servoIncrement;
          if((servoPos + servoIncrement) >= END_ANGLE)  servoPos = END_ANGLE;
          if((servoPos + servoIncrement) <= START_ANGLE)  servoPos = START_ANGLE;

        }
        else {
          servoPos = servoPos + servoIncrement;
        }

        servo.write(servoPos);
      }


    };

    float sensorMotorAngle(void){
      unsigned long currentLoopMillis = millis();

      // moveServo();

      float angle = getFeedbackAngle();

      if (debug){
        Serial.print("timeSinceLastMove: ");
        Serial.print(timeSinceLastMove);
        Serial.print("\t");
        Serial.print(" servoIncrement: ");
        Serial.print(servoIncrement);
        Serial.print("\t");
        Serial.print("servoPos - angle: ");
        Serial.print(servoPos);
        Serial.print(" - ");
        Serial.println(angle);
      }


      // return servoAngle;
      return angle;



    };

};

#endif
