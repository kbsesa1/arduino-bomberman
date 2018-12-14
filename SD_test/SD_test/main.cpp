
#define F_CPU 16000000
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>

#include "SimpleUart.h"
#include "SPI.h"

SPI SD = SPI(&DDRD,&PORTD,4);
UART Serial = UART();

uint8_t SDInit(SPI SD_SPI);
void SD_command(unsigned char cmd, unsigned long arg, unsigned char crc, uint32_t read);

int main(void){
	Serial.Init(F_CPU,9600);
	
	Serial.PutCString("sd spi init!\n");
	switch(SDInit(SD)){
		case 0:
		Serial.PutCString("init failed\n");
		break;
		case 1:
		Serial.PutCString("sd init failed\n");
		break;
		case 2:
		Serial.PutCString("spi init failed\n");
		break;
		case 3:
		Serial.PutCString("succes!\n");
		break;
	}
	Serial.PutCString("set transfer size\n");
	SD_command(0x50,0x0200,0xFF,8);
	Serial.PutCString("read sector\n");
	SD_command(0x51,10,0xFF,10000);
	
	while (1)
	{
	}
}

uint8_t SDInit(SPI SD_SPI){
	bool spiInit = 0;
	bool sdInit = 0;
	
	SD_SPI.init();
	SD_SPI.endTransmission();
	for (int i = 0;i<10;i++)
	{
		SD_SPI.transfer(0xFF);
	}
	SD_SPI.beginTransmission();
	SD_SPI.transfer(0x40);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x95);
	
	for (uint8_t i = 0;i<8;i++)
	{
		
		if (SD.transfer(0xFF) == 0x01)
		{
			spiInit = 1;
		}
	}
	SD_SPI.transfer(0x41);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0x00);
	SD_SPI.transfer(0xFF);
	
	for (uint8_t i = 0;i<8;i++)
	{
		
		if (SD.transfer(0xFF) == 0x01)
		{
			sdInit = 1;
		}
	}
	
	return spiInit | (sdInit<<1);
}
void SD_command(unsigned char cmd, unsigned long arg, unsigned char crc, uint32_t read) {
	
	
	SD.beginTransmission();
	SD.transfer(cmd);
	SD.transfer(arg>>24);
	SD.transfer(arg>>16);
	SD.transfer(arg>>8);
	SD.transfer(arg);
	SD.transfer(crc);
	
	for(uint32_t i = 0; i<read; i++){
		Serial.PutHex(SD.transfer(0xFF));
		Serial.Transmit(' ');
	}
	
	
	
	SD.endTransmission();
	
	
}
