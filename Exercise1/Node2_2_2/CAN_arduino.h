/*
 * CAN_arduino.h
 *
 * Created: 15.10.2014 13:52:27
 *  Author: mikaln
 */ 


#ifndef CAN_ARDUINO_H_
#define CAN_ARDUINO_H_
#include <avr/io.h>

typedef struct {
	uint16_t id;
	unsigned char length;
	int8_t data[8];
} can_message_t;

void can_Init_Normal();
void can_Init_Loopback();
void can_message_send(can_message_t *message);
int can_error();
int can_transmit_complete();
void can_data_receive(can_message_t *message);
void can_joystick_receive(can_message_t * message);

#endif /* CAN_ARDUINO_H_ */