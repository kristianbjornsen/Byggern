/*
 * CAN.h
 *
 * Created: 01.10.2014 11:46:56
 *  Author: brynjare
 */


#ifndef CAN_H_
#define CAN_H_

typedef struct {
    uint16_t id;
    unsigned char length;
    unsigned char data[8];
} can_message_t;

void can_Init_Normal();
void can_Init_Loopback();
void can_message_send(can_message_t *message);
int can_error();
int can_transmit_complete();
void can_data_receive(can_message_t *message);

#endif /* CAN_H_ */