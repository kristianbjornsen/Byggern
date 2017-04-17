
#include <SPI/SPI.h>
#include "MCP2515.h"
#include "PWM.h"
#include <Servo/Servo.h>
#include "IR.h"
#include "MOTOR.h"
#include "JOYSTICK.h"
#include "Bluetooth.h"
#include <Wire/Wire.h>
#include "GAME.h"
#define SLAVESELECT 53
int analogpin=13;
int value;
void setup()
{
 
 Serial.begin(9600); /* add setup code here */
 MCP2515init();
 PWM_init();
 GAME_init();
 IR_init();
 MOTOR_init();
 JOYSTICK_init();
 BLUETOOTH_init();
 //BLUETOOTH_enable(0);
}


can_message_t data ;
unsigned int data2;
void loop()
{	
// 	CAN_data_receive(& data);
// 	if (data.id == 69)
// 		if (data.data[0])
// 			IR_init();
	IR_send_data();
	//BLUETOOTH_read();
	JOYSTICK_button();
	//MOTOR_set_speed(1);
	CAN_read();
	//BLUETOOTH_write('h');
	MOTOR_CONTROL();
// 	PWM_generate();
// 	MOTOR_joystick();
	//MOTOR_BLUETOOTH();
	//Serial.println(BLUETOOTH_read());
}
