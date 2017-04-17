/*
 * SRAM.h
 *
 * Created: 28.09.2014 11:16:41
 *  Author: viliusc
 */ 


#ifndef SRAM_H_
#define SRAM_H_

void SRAM_init();
void SRAM_test();
void SRAM_write(char* value,int);
char* SRAM_read(int i);

#endif /* SRAM_H_ */