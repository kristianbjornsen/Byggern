/*
 * GAME.h
 *
 * Created: 01.11.2014 17:31:13
 *  Author: ugurac
 */ 


#ifndef GAME_H_
#define GAME_H_
#include "CAN.h"

void GAME_init(void);
int GAME_read(can_message_t *message);
void GAME_start(void);
char* intTostring(int num, char* str);
int GAME_main(int flag); // flag = 1 starts the game
void GAME_button_handler(can_message_t msg);
void GAME_JOYSTICK(can_message_t msg);
void GAME_IR_update(can_message_t msg);
int GAME_IR_STATUS_GET();
int GAME_BUTTON_GET(int num);

void reverse(char *str , int length);
char* itoa2(int num, char* str, int base);
char* GAME_highscore_name(can_message_t read);
void GAME_move_numbers(int i, char* my_list);
void GAME_buttons_reset(void);
void GAME_send_game_condition();

#endif /* GAME_H_ */