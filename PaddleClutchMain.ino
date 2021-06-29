#include <Joystick.h>
Joystick_ Joystick;

#include <EEPROM.h>

#include "PaddleClutch.h"

PaddleClutch PaddleClutchManager;

int leftPaddlePin = A0;
int rightPaddlePin = A1;
int bitePointPin = A2;

uint16_t leftPaddleValue;
uint16_t rightPaddleValue;
uint16_t clutchOutput;

void setup()
{
  Serial.begin(115200);

  Joystick.begin();
  Joystick.setThrottleRange(0, 65535);

  updateCalibration();
}

void updateCalibration()
{
  PaddleClutchManager.calibVals.lpMin = 0;      //left paddle minimum
  PaddleClutchManager.calibVals.lpMax = 1023;   //left paddle maximum

  PaddleClutchManager.calibVals.rpMin = 0;      //right paddle minimum
  PaddleClutchManager.calibVals.rpMax = 1023;   //right paddle maximum

  PaddleClutchManager.calibVals.btptMin = 0;    //bitepoint minimum (for potentiometer bitepoint)
  PaddleClutchManager.calibVals.btptMax = 1023; //bitepoint maximum (for potentiometer bitepoint)
}

void loop()
{
  leftPaddleValue = analogRead(leftPaddlePin);
  rightPaddleValue = analogRead(rightPaddlePin);

  PaddleClutchManager.updateBitePoint(analogRead(A2));

  clutchOutput = PaddleClutchManager.getClutchOutput(leftPaddleValue, rightPaddleValue);

  Joystick.setThrottle(clutchOutput);

  String serialoutput = "Left Paddle: " + String(leftPaddleValue) + ", Right Paddle: " + String(rightPaddleValue) + ", Clutch Output: " + String(clutchOutput);
  Serial.println(serialoutput);
}
