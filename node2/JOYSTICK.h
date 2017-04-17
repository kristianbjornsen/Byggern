/*
 * JOYSTICK.h
 *
 * Created: 15.10.2014 11:13:00
 *  Author: ugurac
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include "MCP2515.h"

void JOYSTICK_read(can_message_t *joy);
void JOYSTICK_button();
void JOYSTICK_init();
void JOYSTICK_button_BT();
int JOYSTICK_choose();
void JOYSTICK_change(can_message_t msg);
void JOYSTICK_update(can_message_t msg);
int JOYSTICK_get(char value);

#endif /* JOYSTICK_H_ */