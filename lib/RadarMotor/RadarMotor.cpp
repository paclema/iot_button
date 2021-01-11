#include "RadarMotor.h"

  RadarMotor::RadarMotor(void){

  };


  void RadarMotor::setup(){
    servoIncrement = angleAccuracy;
    servoPos = START_ANGLE;

    if (servoSpeed <= MIN_MS_PER_GRAD){
      servoSpeed = MIN_MS_PER_GRAD;
    }
    time_between_servo_writes = angleAccuracy * servoSpeed;

    Serial.print("\tMIN_MS_PER_GRAD:");
    Serial.print(MIN_MS_PER_GRAD);
    Serial.print(" servoSpeed:");
    Serial.print(servoSpeed);
    Serial.print(" angleAccuracy:");
    Serial.print(angleAccuracy);
    Serial.print(" angleAccuracy/(ANGULAR_SPEED):");
    Serial.print(angleAccuracy/(ANGULAR_SPEED));
    Serial.print(" time_between_servo_writes:");
    Serial.println(time_between_servo_writes);

    servo.attach(this->controlPin); // Attaching Servo to D3
    servo.write(servoPos);

    setRange();

    // while (1) {
    //   testMovement();
    // }

  }

    void RadarMotor::testMovement(void){
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

    void RadarMotor::printData(int i){
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


    void RadarMotor::setRange(void ){
      servo.write(START_ANGLE);
      delay(TIME_TO_TRAVEL_RANGE*4);
      lowEnd = getFeedback();
      servo.write(END_ANGLE);
      delay(TIME_TO_TRAVEL_RANGE*4);
      highEnd = getFeedback();

      if (debug){
        Serial.print("START_ANGLE= ");
        Serial.print(lowEnd);
        Serial.print(" ");
        Serial.print("END_ANGLE= ");
        Serial.println(highEnd);
      }
    }

    float RadarMotor::getFeedback(void){
      float mean = 0;
      float result;
      int test;
      boolean done;
      if (debug)
        Serial.print("     Readings --> ");
      // Get several servo potentiometer feedback measurements:
      for (int j=0; j<feedbackReadings; j++){
        reading[j] = analogRead(this->feedbackPin);
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

    float RadarMotor::getAngle(void){

      float angle = map((getFeedback()),lowEnd,highEnd,START_ANGLE,END_ANGLE);
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

      return angle;
    }


    void RadarMotor::moveServo(void){
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


void RadarMotor::setServoControl(byte pin) {

  this->controlPin = pin;
  // Servo control pin attach
  this->servo.attach(this->controlPin);
}


void RadarMotor::setServoFeedback(byte pin) {

  this->feedbackPin = pin;
  // Servo feedback pin attach
  pinMode(this->feedbackPin, INPUT);
}
