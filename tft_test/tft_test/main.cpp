/*
 * tft_test.cpp
 *
 * Created: 4-12-2018 11:09:05
 * Author : joost
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "SimpleUart.h"
#include "SPI.h"
#include "LCD.h"

LCD tft;
UART Serial;

int main(void)
{
	Serial.Init(F_CPU,9600);
	tft.init(0);
	tft.fillScreen(tft.colorTo565(0xFF,0x00,0x00));
    while (1) 
    {
		Serial.PutString("status: ");
		uint32_t status = tft.getStatus();
		Serial.PutUInt((status >> 24)&0xFF);
		Serial.Transmit(',');
		Serial.PutUInt((status >> 16)&0xFF);
		Serial.Transmit(',');
		Serial.PutUInt((status >> 8)&0xFF);
		Serial.Transmit(',');
		Serial.PutUInt(status&0xFF);
		Serial.Transmit('\n');
		_delay_ms(500);
		Serial.PutString("power: ");
		Serial.PutUInt(tft.getMode());
		Serial.Transmit('\n');
		_delay_ms(500);
		Serial.PutString("mode: ");
		Serial.PutUInt(tft.getMADCTL());
		Serial.Transmit('\n');
		_delay_ms(500);
		
		
		
		/*
	tft.drawRectangle(0,0,100,100,tft.colorTo565(0x00,0xFF,0x00));
	_delay_ms(500);
	tft.drawRectangle(0,0,100,100,tft.colorTo565(0x00,0x00,0xFF));
	_delay_ms(500);
	for (uint16_t i = 100;i<200;i++)
	{
		tft.drawPixel(i,i,0xFFFF);
	}
	*/
    }
}
