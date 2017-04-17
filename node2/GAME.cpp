/*
 * GAME.cpp
 *
 * Created: 15.11.2014 14:50:23
 *  Author: ugurac
 */ 
#include <Arduino.h>
#include "MCP2515.h"

int game_condition;
void GAME_init(){
	game_condition=0;
	
}

void GAME_MODUS_UPDATE(can_message_t msg){
	game_condition=msg.data[0];	
	
}
int GAME_INFO_GET(){
	return game_condition;
}