/*
 * SENSOR.h
 *
 * Created: 22.10.2014 08:21:17
 *  Author: ugurac
 */ 


#ifndef IR_H_
#define IR_H_

void IR_init();
int IR_read_value();
bool IR_filter();
void IR_send_data();
void IR_count_point();
void GAME_modus();
#endif /* SENSOR_H_ */