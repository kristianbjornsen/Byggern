/*
 * SPI.h
 *
 * Created: 01.10.2014 08:24:21
 *  Author: viliusc
 */ 


#ifndef SPI_H_
#define SPI_H_


void SPI_init(void);
void SPI_send(char cData);
char SPI_read(void);
void SPI_select(int select);
#endif /* SPI_H_ */