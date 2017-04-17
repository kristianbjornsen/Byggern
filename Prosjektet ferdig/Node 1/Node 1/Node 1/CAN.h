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
    int8_t data[8];
} can_message_t;

void can_Init_Normal();
void can_Init_Loopback();
void can_message_send(can_message_t *message);
int can_error();
int can_transmit_complete();
void can_data_receive(can_message_t *message);

void receive_score(can_message_t * msg,int * playingGame);

#define scoreId 1
#define onePlayer 2
#define twoPlayer 3
#define SCORE_REQUEST_ID 4
#define SCORE_RECEIVE_ID 5
#define SCORE_SEND_ID 6
#define END_TWO_PLAYER_GAME 8
#define PLAY_MUSIC_ID 9
#define musicStop 10

#endif /* CAN_H_ */