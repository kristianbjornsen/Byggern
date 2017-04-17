/*
 * SRAM.c
 *
 * Created: 28.09.2014 11:16:25
 *  Author: viliusc
 */ 
#include <avr/io.h>
volatile char *ext_ram= (char *) 0x1800;
void SRAM_init(){
	
	MCUCR=(1<<SRE)|(1<<SRW11);
	
	
}

void SRAM_write(char* data,int start){
		int i=0;
		while(data[i]!='\0'){
			ext_ram[start+i]=data[i];
			i++;	
		}
		ext_ram[start+i]=data[i];
}
char* SRAM_read(int start){
	char* data;
	int i=0;
	while(ext_ram[start+i]!='\0'){
		 data[i]=ext_ram[start+i];
		 i++;
	}
	return data;
}

void SRAM_test()
{
	volatile char *ext_ram= (char *) 0x1800; //Start address for the SRAM
	
	unsigned int i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;
	
	printf("Starting SRAM test...\r\n");
	

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (write phase): ext_ram[%d] = %02X (should be%02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {

			rerrors++;
		}
	}
	
	printf("SRAM test completed with %d errors in write phase and %d errors in readphase\r\n", werrors, rerrors);

}