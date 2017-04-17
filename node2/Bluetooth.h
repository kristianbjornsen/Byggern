/*
 * Bluetooth.h
 *
 * Created: 29.10.2014 19:37:06
 *  Author: ugurac
 */ 


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "MCP2515.h"

void BLUETOOTH_init();
int BLUETOOTH_read();
void BLUETOOTH_enable(int i);
int BT_MODE();
void BT_change_status(can_message_t msg);
int BLUETOOTH_write(char value);
int* BLUETOOTH_get_name1();

#endif /* BLUETOOTH_H_ */