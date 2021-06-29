
/*
  PaddleClutch.h - PaddleClutch manager class
  Created by Morgan Gardner, December 10th, 2020.
  Modified by Morgan Gardner, June 29th, 2021.
  Released under the GPL-3.0 license.
*/

#ifndef PaddleClutch_h
#define PaddleClutch_h

#include "Arduino.h"
#include <Smoothed.h>
#include <EEPROM.h>

class PaddleClutch
{

  public:
  
    struct calibration
    {
      uint16_t lpMin;
      uint16_t lpMax;
      uint16_t rpMin;
      uint16_t rpMax;
      uint16_t btptMin; //for potentiometer bitepoint
      uint16_t btptMax; //for potentiometer bitepoint
      uint16_t btptValue;
    } calibVals;
  
    PaddleClutch()
    {

    }

    uint16_t getClutchOutput(uint16_t leftPaddleReading, uint16_t rightPaddleReading)
    {
      uint16_t leftPaddleConstVal = constrain(leftPaddleReading, calibVals.lpMin, calibVals.lpMax);
      uint16_t leftPaddleMappedVal = map(leftPaddleConstVal, calibVals.lpMin, calibVals.lpMax, 0, 65535);

      uint16_t rightPaddleConstVal = constrain(rightPaddleReading, calibVals.rpMin, calibVals.rpMax);
      uint16_t rightPaddleMappedVal = map(rightPaddleConstVal, calibVals.rpMin, calibVals.rpMax, 0, 65535);

      uint16_t leftPaddleMapped = map(leftPaddleMappedVal, 0, 65535, 0, calibVals.btptValue); //map left paddle max to btpt pot val

      if (rightPaddleMappedVal > leftPaddleMapped) //if right paddle val is higher than left
        return rightPaddleMappedVal; //set output val to right paddle
      else
        return leftPaddleMapped; //set output val to left paddle
    }

    uint16_t updateBitePoint(uint16_t value)
    {
      uint16_t btptConstVal = constrain(value, calibVals.btptMin, calibVals.btptMax);
      uint16_t btptMappedVal = map(btptConstVal, calibVals.btptMin, calibVals.btptMax, 0, 65535);
      calibVals.btptValue = btptMappedVal;
    }

    void setLeftPaddleRange(uint16_t min, uint16_t max)
    {
      calibVals.lpMin = min;
      calibVals.lpMax = max;
    }

    void setRightPaddleRange(uint16_t min, uint16_t max)
    {
      calibVals.rpMin = min;
      calibVals.rpMax = max;
    }

  private:

    int leftPaddlePin;
    int rightPaddlePin;

};

#endif
