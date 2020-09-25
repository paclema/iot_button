/*
  SensorMotor.h - Library for move the sensor head


*/
#ifndef SensorMotor_h
#define SensorMotor_h

#include <Arduino.h>
#include <Servo.h>



class SensorMotor
{
  private:

  Servo servo;
  float servoIncrement = 0;
  float servoPos = 0;
  float servoPosMin = 25;
  float servoPosMax = 160;
  int reverseIncrement = 1;
  float servoSpeed = 0;
  float angleAccuracy = 0;

  float servoAngle=0;

  int valPot = 0;

  long previousServoMillis = 0;
  long previousLoopMainMillis = 0;
  int time_spent_sum = 0;


  public:
  void sensorMotor(void);
  void sensorMotorSetup(float angle_accuracy, float servo_speed_ms){
    servoIncrement = angle_accuracy;
    angleAccuracy = angle_accuracy;
    servoSpeed = servo_speed_ms;
    servoPos = servoPosMin;
    servo.attach(0); // Attaching Servo to D3
    servo.write(servoPos);

  }


    int sensorMotorAngle(void){
      unsigned long currentLoopMillis = millis();

      int time_spent = currentLoopMillis-previousServoMillis;
      int loop_delay = currentLoopMillis-previousLoopMainMillis;

      valPot = analogRead(0);
      valPot = map(valPot, 0, 1023, 160, 3000);
      servoSpeed = valPot;

      // int time_between_servo_writes = servoSpeed * angleAccuracy/60;
      // int time_between_servo_writes = servoSpeed * angleAccuracy/60;
      float time_between_servo_writes = servoSpeed * angleAccuracy/60;

      if (time_spent >= time_between_servo_writes){
        previousServoMillis = currentLoopMillis;
        servoPos += servoIncrement;
        servo.write(servoPos);
        // servoAngle = servoPos;
        time_spent_sum += time_spent;

        if ((servoPos >= servoPosMax) || (servoPos <= servoPosMin)) // end of sweep
        {
          // reverse direction
          servoIncrement = -servoIncrement;
          reverseIncrement = -reverseIncrement;
          time_spent_sum = 0;
        }
        time_spent = 0;
      }

      // int angleIncrement = loop_delay*60/(servoSpeed);
      // servoAngle += (angleIncrement*reverseIncrement);
      // if (servoPos = 180)
      if (reverseIncrement > 0)
        servoAngle = servoPosMin + (time_spent_sum+time_spent)*60/(servoSpeed);
        // servoAngle = servoPosMin + (time_spent)*60/(servoSpeed);
      else
        servoAngle = servoPosMax - (time_spent_sum+time_spent)*60/(servoSpeed);
        // servoAngle = servoPosMax - (time_spent)*60/(servoSpeed);

      Serial.print("servoSpeed: ");
      Serial.print(servoSpeed);
      Serial.print(" angle_accuracy: ");
      Serial.print(angleAccuracy);
      Serial.print(" loop_delay: ");
      Serial.print(loop_delay);
      Serial.print(" servoAngle: ");
      Serial.print(servoAngle);

      Serial.print(" time_spent: ");
      Serial.print(time_spent);
      Serial.print(" time_spent_sum: ");
      Serial.print(time_spent_sum);
      Serial.print(" servoPos: ");
      Serial.println(servoPos);

      previousLoopMainMillis = currentLoopMillis;
      return servoAngle;

    }
};

#endif
