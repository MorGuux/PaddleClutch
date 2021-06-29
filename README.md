# PaddleClutch
Arduino library to enable a dual paddle clutch for sim racing

## Usage

* Copy the PaddleClutch.h class into your working directory
* Import the PaddleClutch class into your arduino sketch
  ```C
  #include "PaddleClutch.h"
  ```
* Declare the PaddleClutch class in your sketch
  ```C
  PaddleClutch PaddleClutchManager;
  ```
* In your arduino loop function, call ".getClutchOutput" with the two paddle values passed in
  ```C
  clutchOutput = PaddleClutchManager.getClutchOutput(leftPaddleValue, rightPaddleValue);
  ```
  
  You can use
  ```C
  PaddleClutchManager.updateBitePoint(bitePointValue)
  ```
  to update the bite point value.
  
  You can also specify the clutch axis ranges by settings the variables in the associated struct
  ```C
  PaddleClutchManager.calibVals.lpMin = 0;      //left paddle minimum
  PaddleClutchManager.calibVals.lpMax = 1023;   //left paddle maximum
  
  PaddleClutchManager.calibVals.rpMin = 0;      //right paddle minimum
  PaddleClutchManager.calibVals.rpMax = 1023;   //right paddle maximum
  
  PaddleClutchManager.calibVals.btptMin = 0;    //bitepoint minimum (for potentiometer bitepoint)
  PaddleClutchManager.calibVals.btptMax = 1023; //bitepoint maximum (for potentiometer bitepoint)
  PaddleClutchManager.calibVals.btptValue = 0;  //bitepoint value
  ```
  
  There is no hardware implementation in the PaddleClutch.h class, so that different hardware with different resolutions can be used (potentiometers, hall effect sensors, ADS10XX modules etc). The resulting clutch output value is scaled to 16-bit for use with the Joystick library.
  
  ## Example
  
  I have attached a sample .ino sketch (PaddleClutchMain.ino) that demonstrates usage of the class. It uses a third potentiometer as the bitepoint value and updates the Throttle axis on the Joystick.
  
