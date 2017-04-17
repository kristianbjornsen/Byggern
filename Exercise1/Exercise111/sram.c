/*
 * sram.c
 *
 * Created: 12.11.2014 12:11:16
 *  Author: brynjare
 */

#include "sram.h"


void sram_Init(){
    MCUCR = (1<<SRE);
    SFIOR = (1<<XMM2);
}


void SRAM_test(void)
{
    // Start address for the SRAM
    volatile char *ext_ram = (char *) 0x1800;
    
    uint16_t i, werrors, rerrors;
    werrors = 0;
    rerrors = 0;
    char testvalue;
    
    printf("Starting SRAM test...\r\n");
    
    for (i = 0; i < 0x800; i++) {
        testvalue = ~(i % 256);
        ext_ram[i] = testvalue;
        if (ext_ram[i] != testvalue) {
            werrors++;
        }
    }
    
    for (i = 0; i < 0x800; i++) {
        testvalue = ~(i % 256);
        if (ext_ram[i] != testvalue) {
            rerrors++;
        }
    }
    
    printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}

void SRAM_test_return_errors(int errors[])
{
    // Start address for the SRAM
    volatile char *ext_ram = (char *) 0x1800;
    
    uint16_t i, werrors, rerrors;
    werrors = 0;
    rerrors = 0;
    char testvalue;
    
    for (i = 0; i < 0x800; i++) {
        testvalue = ~(i % 256);
        ext_ram[i] = testvalue;
        if (ext_ram[i] != testvalue) {
            werrors++;
        }
    }
    
    for (i = 0; i < 0x800; i++) {
        testvalue = ~(i % 256);
        if (ext_ram[i] != testvalue) {
            rerrors++;
        }
    }
    
    errors[0] = werrors;
    errors[1] = rerrors;
}