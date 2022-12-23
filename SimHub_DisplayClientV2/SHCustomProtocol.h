#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>

class SHCustomProtocol {
private:

  #if(GAMEPAD_AXIS_CLUTCH_BITEPOINT_TYPE == 2)
  uint16_t bitePoint;
  #endif

public:

  /*
	CUSTOM PROTOCOL CLASS
	SEE https://github.com/zegreatclan/SimHub/wiki/Custom-Arduino-hardware-support

	GENERAL RULES :
		- ALWAYS BACKUP THIS FILE, reinstalling/updating SimHub would overwrite it with the default version.
		- Read data AS FAST AS POSSIBLE in the read function
		- NEVER block the arduino (using delay for instance)
		- Make sure the data read in "read()" function READS ALL THE DATA from the serial port matching the custom protocol definition
		- Idle function is called hundreds of times per second, never use it for slow code, arduino performances would fall
		- If you use library suspending interrupts make sure to use it only in the "read" function when ALL data has been read from the serial port.
			It is the only interrupt safe place

	COMMON FUNCTIONS :
		- FlowSerialReadStringUntil('\n')
			Read the incoming data up to the end (\n) won't be included
		- FlowSerialReadStringUntil(';')
			Read the incoming data up to the separator (;) separator won't be included
		- FlowSerialDebugPrintLn(string)
			Send a debug message to simhub which will display in the log panel and log file (only use it when debugging, it would slow down arduino in run conditions)

	*/

  // Called when starting the arduino (setup method in main sketch)
  void setup() {
  }

  // Called when new data is coming from computer
  void read() {
    //RACEX PADDLE CLUTCH LOGIC
    #if(GAMEPAD_AXIS_CLUTCH_BITEPOINT_TYPE == 2)
    bitePoint = FlowSerialReadStringUntil('\n').toInt();
    #endif


  }

  #if(GAMEPAD_AXIS_CLUTCH_BITEPOINT_TYPE == 2)
  uint16_t getBitePoint() {
    return bitePoint;
  }
  #endif

  // Called once per arduino loop, timing can't be predicted,
  // but it's called between each command sent to the arduino
  void loop() {
  }

  // Called once between each byte read on arduino,
  // THIS IS A CRITICAL PATH :
  // AVOID ANY TIME CONSUMING ROUTINES !!!
  // PREFER READ OR LOOP METHOS AS MUCH AS POSSIBLE
  // AVOID ANY INTERRUPTS DISABLE (serial data would be lost!!!)
  void idle() {
  }
};

#endif