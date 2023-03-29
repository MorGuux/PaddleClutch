#ifndef __SHGAMEPADCLUTCH_H__
#define __SHGAMEPADCLUTCH_H__

#include <Arduino.h>
#include <Joystick.h>
#include "PaddleClutch.h"

class SHGamepadClutch {
private:

  PaddleClutch paddleClutch;
  bool isBitePointButton = false;
  bool isRaceXControlled = false;

  bool bitePointPullup = false;

  int lastAxisValue = -1;

  int leftPaddlePin = -1;
  int leftMinimumInputValue;
  int leftMaximumInputValue;
  double leftExponentialFactor;

  int rightPaddlePin = -1;
  int rightMinimumInputValue;
  int rightMaximumInputValue;
  double rightExponentialFactor;

  int bitePointPin = -1;
  int bitePointMinimumInputValue;
  int bitePointMaximumInputValue;

  int samplingRate;
  Joystick_* joystick;

  float analogReadXXbit(uint8_t analogPin, uint8_t bits_of_precision) {
    uint8_t n = bits_of_precision - 10;
    unsigned long oversample_num = 1 << (2 * n);
    uint8_t divisor = 1 << n;
    unsigned long reading_sum = 0;
    unsigned long inner_sum = 0;
    for (unsigned long j = 0; j < oversample_num; j++) {
      inner_sum += analogRead(analogPin);
    }
    unsigned int reading = (inner_sum + (unsigned long)divisor / 2UL) >> n;
    reading_sum += reading;
    return (float)reading_sum;
  }

  void setAxis(int value) {
    //Serial.println(value);
    joystick->setRxAxis(value);
    joystick->sendState();
  }

public:

  //RaceX
  SHGamepadClutch(byte leftPaddlePin, byte rightPaddlePin, int leftMinimumInputValue, int leftMaximumInputValue, double leftExponentialFactor, int rightMinimumInputValue, int rightMaximumInputValue, double rightExponentialFactor) {

    this->leftPaddlePin = leftPaddlePin;
    this->rightPaddlePin = rightPaddlePin;
    this->leftExponentialFactor = leftExponentialFactor;
    this->leftMinimumInputValue = leftMinimumInputValue;
    this->leftMaximumInputValue = leftMaximumInputValue;
    this->rightMinimumInputValue = rightMinimumInputValue;
    this->rightMaximumInputValue = rightMaximumInputValue;
    this->rightExponentialFactor = rightExponentialFactor;

    this->paddleClutch.setLeftPaddleRange(0, 1023);
    this->paddleClutch.setRightPaddleRange(0, 1023);
    this->isRaceXControlled = true;

    pinMode(bitePointPin, INPUT_PULLUP);

    this->samplingRate = 10;
  }

  //Button
  SHGamepadClutch(byte leftPaddlePin, byte rightPaddlePin, byte bitePointPin, int leftMinimumInputValue, int leftMaximumInputValue, double leftExponentialFactor, int rightMinimumInputValue, int rightMaximumInputValue, double rightExponentialFactor, bool pullup) {

    this->leftPaddlePin = leftPaddlePin;
    this->rightPaddlePin = rightPaddlePin;
    this->bitePointPin = bitePointPin;
    this->leftExponentialFactor = leftExponentialFactor;
    this->leftMinimumInputValue = leftMinimumInputValue;
    this->leftMaximumInputValue = leftMaximumInputValue;
    this->rightMinimumInputValue = rightMinimumInputValue;
    this->rightMaximumInputValue = rightMaximumInputValue;
    this->rightExponentialFactor = rightExponentialFactor;

    this->paddleClutch.setLeftPaddleRange(0, 1023);
    this->paddleClutch.setRightPaddleRange(0, 1023);
    this->isBitePointButton = true;
    this->bitePointPullup = pullup;

    pinMode(bitePointPin, bitePointPullup ? INPUT_PULLUP : INPUT);

    this->samplingRate = 10;
  }

  //Potentiometer
  SHGamepadClutch(byte leftPaddlePin, byte rightPaddlePin, byte bitePointPin, int leftMinimumInputValue, int leftMaximumInputValue, double leftExponentialFactor, int rightMinimumInputValue, int rightMaximumInputValue, double rightExponentialFactor, int bitePointMinimumInputValue, int bitePointMaximumInputValue) {

    this->leftPaddlePin = leftPaddlePin;
    this->rightPaddlePin = rightPaddlePin;
    this->bitePointPin = bitePointPin;
    this->bitePointMinimumInputValue = bitePointMinimumInputValue;
    this->bitePointMaximumInputValue = bitePointMaximumInputValue;
    this->leftExponentialFactor = leftExponentialFactor;
    this->leftMinimumInputValue = leftMinimumInputValue;
    this->leftMaximumInputValue = leftMaximumInputValue;
    this->rightMinimumInputValue = rightMinimumInputValue;
    this->rightMaximumInputValue = rightMaximumInputValue;
    this->rightExponentialFactor = rightExponentialFactor;

    this->paddleClutch.setLeftPaddleRange(0, 1023);
    this->paddleClutch.setRightPaddleRange(0, 1023);
    this->paddleClutch.setBitePointPotRange(0, 1023);

    this->samplingRate = 10;
  }

  void SetJoystick(Joystick_* joystick) {
    this->joystick = joystick;
    joystick->setRxAxisRange(0, 1023);
    update();
  }

  // RaceX bitepoint set (percentage 0-100)
  void setBitePoint(uint16_t value) {
    this->paddleClutch.updateBitePointPercent(value);
  }

  int read(byte pin, double exponentialFactor, int minimumInputValue, int maximumInputValue) {
    int pot = analogReadXXbit(pin, samplingRate);

    if (lastAxisValue != pot) {
      lastAxisValue = pot;

      int mapped = map(pot, minimumInputValue, maximumInputValue, 0, 1023);
      float mapped2 = min(1, max(0, (float)mapped / (float)1023));

      if (exponentialFactor != 1) {
        mapped2 = pow(mapped2, 1.0 / (float)exponentialFactor);
      }

      mapped2 *= 1023;
      return mapped2;
    }
  }

  bool update() {
    uint16_t left = read(leftPaddlePin, leftExponentialFactor, leftMinimumInputValue, leftMaximumInputValue);
    uint16_t right = read(rightPaddlePin, rightExponentialFactor, rightMinimumInputValue, rightMaximumInputValue);
    uint16_t clutch = this->paddleClutch.getClutchOutput(left, right);

    if (!isRaceXControlled) {
      if (isBitePointButton) {
        if (digitalRead(bitePointPin) == (bitePointPullup ? LOW : HIGH)) {
          this->paddleClutch.updateBitePoint();
        }
      } else {
        this->paddleClutch.updateBitePoint(read(bitePointPin, 1, bitePointMinimumInputValue, bitePointMaximumInputValue));
      }
    }

    setAxis(clutch);
  }
};

#endif