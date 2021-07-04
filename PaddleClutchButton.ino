#include <Joystick.h>
Joystick_ Joystick;

#include "PaddleClutch.h"

PaddleClutch PaddleClutchManager;

int leftPaddlePin = A0;
int rightPaddlePin = A1;
int bitePointPin = 5;

uint16_t leftPaddleValue;
uint16_t rightPaddleValue;
uint16_t clutchOutput;

void setup()
{
  Serial.begin(115200);

  Joystick.begin();
  Joystick.setThrottleRange(0, 65535);

  pinMode(5, INPUT_PULLUP);

  updateCalibration();
}

void updateCalibration()
{
  PaddleClutchManager.setLeftPaddleRange(0, 1023);

  PaddleClutchManager.setRightPaddleRange(0, 1023);
}

void loop()
{
  leftPaddleValue = analogRead(leftPaddlePin);
  rightPaddleValue = analogRead(rightPaddlePin);

  if (digitalRead(bitePointPin) == LOW)                   //Bite point set button
    PaddleClutchManager.updateBitePoint();

  clutchOutput = PaddleClutchManager.getClutchOutput(leftPaddleValue, rightPaddleValue);

  Joystick.setThrottle(clutchOutput);

  String serialoutput = "Left Paddle: " + String(leftPaddleValue) + ", Right Paddle: " + String(rightPaddleValue) + ", Bite Point: " + String(PaddleClutchManager.calibVals.btptValue) + ", Clutch Output: " + String(clutchOutput);
  Serial.println(serialoutput);
}
