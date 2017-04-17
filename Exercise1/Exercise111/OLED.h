
#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>


void oled_init();

void write_c(unsigned char a);

void oled_clear_screen(void);
void oled_set_line(uint8_t line);
void oled_set_pos_left();
void write_d(unsigned char a);
void oled_meny(int * playingGame, uint16_t * id);
void USART_Tx(char *data);
void USART_TxString_P(const char * data);
void oled_meny_init();
void oled_meny_print(void);
void print_Sram_test();
void write_dnumber(unsigned char a);
void oled_print_number(char *data);
void stopGame(int * playingGame);

void printHighScore(int*arr);
void endTwoPlayerGame(can_message_t * winner);

#endif