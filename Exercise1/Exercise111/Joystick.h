#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "CAN.h"

#define x_volt_max 255
#define y_volt_max 255
#define x_volt_min 0
#define y_volt_min 0
#define margin 10
#define Joystick_pos_2player 10



typedef struct {
	int x_percent;
	int y_percent;
	int x_percent_joystick2;
	int y_percent_joystick2;
	
} anal_pos;

typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} Position;

void joystick_init();
Position joystick_dir(anal_pos);
anal_pos joystick_anal();
int joystick_button();
int joy_button_left();
int joy_button_right();
int joy_button2_right();
//int left_slider();
//int right_slider();
int joystick_send_to_arduino(can_message_t * message);
//void hit();

#endif