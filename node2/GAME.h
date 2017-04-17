/*
 * GAME.h
 *
 * Created: 15.11.2014 14:50:33
 *  Author: ugurac
 */ 


#ifndef GAME_H_
#define GAME_H_
#include "MCP2515.h"
void GAME_MODUS_UPDATE(can_message_t msg);
void GAME_init();
int GAME_INFO_GET();




#endif /* GAME_H_ */