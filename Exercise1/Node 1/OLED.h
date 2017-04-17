#ifndef OLED_H_
#define OLED_H_



void oled_init();
void oled_print(char* bokstav);

void write_c(unsigned char a);

void oled_clear_screen(void);
void oled_set_line(uint8_t line);
void oled_set_pos_left();
void write_d(unsigned char a);
void oled_meny(void);
void USART_Tx(char *data);
void USART_TxString_P(const char * data);
void oled_meny_init();
void oled_meny_print(void);
void oled_meny_BRYNJAR(void );

#endif