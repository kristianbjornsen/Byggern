/*
 * SPI.h
 *
 * Created: 01.10.2014 08:59:04
 *  Author: brynjare
 */ 


#ifndef SPI_H_
#define SPI_H_

void SPI_Send(char cData);
char SPI_read(void);
void SPI_Init_Master(void);
void SPI_Init_Slave(void);

#endif /* SPI_H_ */