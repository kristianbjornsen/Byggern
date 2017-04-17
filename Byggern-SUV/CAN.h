/*
 * CAN.h
 *
 * Created: 28.09.2014 11:15:09
 *  Author: viliusc
 */ 


#ifndef CAN_H_
#define CAN_H_
#include <avr/io.h>
typedef struct{
	uint8_t id;
	//unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

int GAME_play;
void CAN_init();
int CAN_message_send(can_message_t msg);
void CAN_data_receive(can_message_t *receive);
void CAN_transmit_complete();
void CAN_allowed_to_send();
void CAN_recieve();
void CAN_recieve_new(can_message_t *msg);

#endif /* CAN_H_ */