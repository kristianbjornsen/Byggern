/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
void score_counter();
void twoPlayer_score_counter();
void addScore(can_message_t * scoreMsg);
void sendScoreToOled();
void clearHighScore();

#include "c:\Program Files\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "c:\Program Files\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "F:\Byggern\Exercise1\Node2_2_2\Node2_2_2.ino"
#include "F:\Byggern\Exercise1\Node2_2_2\CAN_arduino.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\CAN_arduino.h"
#include "F:\Byggern\Exercise1\Node2_2_2\MCP2515.h"
#include "F:\Byggern\Exercise1\Node2_2_2\Music.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\Music.h"
#include "F:\Byggern\Exercise1\Node2_2_2\PWM.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\PWM.h"
#include "F:\Byggern\Exercise1\Node2_2_2\SPI_arduino.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\SPI_arduino.h"
#include "F:\Byggern\Exercise1\Node2_2_2\highscore.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\highscore.h"
#include "F:\Byggern\Exercise1\Node2_2_2\mcp2515_arduino.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\mcp2515_arduino.h"
#include "F:\Byggern\Exercise1\Node2_2_2\motor.cpp"
#include "F:\Byggern\Exercise1\Node2_2_2\motor.h"
