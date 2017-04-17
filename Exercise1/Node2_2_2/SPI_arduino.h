/*
 * SPI_arduino.h
 *
 * Created: 15.10.2014 13:39:26
 *  Author: mikaln
 */ 


#ifndef SPI_ARDUINO_H_
#define SPI_ARDUINO_H_

void SPI_Send(char cData);
char SPI_read(void);
void SPI_Init_Master(void);
void SPI_Init_Slav(void);

#endif /* SPI_ARDUINO_H_ */