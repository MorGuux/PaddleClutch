
/*
  PaddleClutch.h - PaddleClutch manager class
  Created by Morgan Gardner, December 10th, 2020.
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
    //initialise PaddleClutch
    PaddleClutch(int leftPaddlePin, int rightPaddlePin)
    {
		this->leftPaddlePin = leftPaddlePin;
		this->rightPaddlePin = rightPaddlePin;
    }
	
	uint16_t getClutchOutput(uint16_t leftPaddleReading, uint16_t rightPaddleReading)
	{
		int leftPaddleVal = analogRead(leftPaddlePin);
		int leftPaddleConstVal = constrain(leftPaddleVal, calibVals.lpMin, calibVals.lpMax);
		int leftPaddleMappedVal = map(leftPaddleConstVal, calibVals.lpMin, calibVals.lpMax, 0, 65535);

		int rightPaddleVal = analogRead(rightPaddlePin);
		int rightPaddleConstVal = constrain(rightPaddleVal, calibVals.rpMin, calibVals.rpMax);
		int rightPaddleMappedVal = map(rightPaddleConstVal, calibVals.rpMin, calibVals.rpMax, 0, 65535);

		int leftPaddleMapped = map(leftPaddleMappedVal, 0, 65535, 0, btptValue); //map left paddle max to btpt pot val

		if (rightPaddleMappedVal > leftPaddleMapped) //if right paddle val is higher than left
			return rightPaddleMappedVal; //set output val to right paddle
		else
			return leftPaddleMapped; //set output val to left paddle
	}
	
	uint16_t updateBitePoint(uint16_t value)
	{
		int btptConstVal = constrain(value, calibVals.btptMin, calibVals.btptMax);
		int btptMappedVal = map(btptConstVal, calibVals.btptMin, calibVals.btptMax, 0, 65535);
	}
	
	void setLeftPaddleRange(uint16_t min, uint16_t max)
	{
		lpMin = min;
		lpMax = max;
	}
	
	void setLeftPaddleRange(uint16_t min, uint16_t max)
	{
		rpMin = min;
		rpMax = max;
	}

  private:
  
  int leftPaddlePin;
  int rightPaddlePin;
  uint16_t btptValue;
  
  struct calibration
  {
	uint16_t lpMin;
	uint16_t lpMax;
	uint16_t rpMin;
	uint16_t rpMax;
	uint16_t btptValue;
  } calibVals;

};

#endif
