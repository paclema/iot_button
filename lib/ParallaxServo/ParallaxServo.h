// #ifndef ParallaxServo_h
#ifndef ESP32
#define ParallaxServo_h
#include <Arduino.h>
#include <Servo.h>
/*
ParallaxServo.h - Library to control the sensor head using a Parallax Feedback 360°
High-Speed Servo.

https://solarbotics.com/wp-content/uploads/900-00360-feedback-360-hs-servo-v1.2.pdf
*/

class ParallaxServo {
  private:
    Servo servo;                    // The Parallax Servo
    byte controlPin;                // The servo pin
    byte feedbackPin;               // The feedback pin (from servo)

    float DUTY_SCALE = 1000;
    unsigned long DUTY_CYCLE_MIN = 29;  // 2.9% * DUTY_SCALE
    unsigned long DUTY_CYCLE_MAX = 971; // 9.71% * DUTY_SCALE
    float UNITS_IN_FULL_CIRCLE = 360;   // Because 360 degrees are in a circle

    // Tune the vars below for controlling how fast the servo gets to the right place and stays there
    int ERROR_ANGLE_OFFSET_US = 50;
    float CONSTANT_KP = 0.8;

    // Max pulse 220, Min pulse 20 to each dirrection
    int MIN_PULSE_SPEED_OFFSET_US = -5;    // Going Counter-clockwise a bit - can be smaller ( < -40)
    int MAX_PULSE_SPEED_OFFSET_US = 5;     // Going Clockwise  a bit - can be bigger (> 40)
    int HOLD_STILL_PULSE_SPEED_US = 1500;   // HOLD_STILL is for keeping the servo in place (no movement, don't change)

    // Angles for different quadrants around the unit circle (for counting number of turns)
    int ANGLE_Q2_MIN = 90;
    int ANGLE_Q3_MAX = 270;

    // Holds the current state
    int targetAngle = 180;                  // The angle we want the servo to go to
    int currentAngle = 0;                   // The angle the servo is at
    int prevAngle = 0;                      // The last angle the servo had
    int errorAngle = 0;                     // How off we are from the target angle
    int turns = 0;                          // How many times we've gone around the circle

    bool debug = false;

  public:
    bool enabled;
    float angleAccuracy = 0;
    float servoSpeed = 0;

    ParallaxServo();
    ParallaxServo(byte pinControl, byte pinFeedback);

    void setup();
    bool handle();
    void rotate(float degree, int threshold);

    void enableServo(void){ this->servo.attach(this->controlPin); };
    void disableServo(void){ this->servo.detach(); };

    void setServoControl(byte servoPinNumber);
    void setServoFeedback(byte servoPinNumber);
    void setSpeedPulse(int speedPulse){
      this->MIN_PULSE_SPEED_OFFSET_US = -speedPulse;
      this->MAX_PULSE_SPEED_OFFSET_US = speedPulse;
    };

    float getAngle();
    float getAngleTarget();
    int getTurns();


    void setDebug(bool d){ this->debug = d; };
    bool isDebug(void){ return this->debug; };


};


#endif
