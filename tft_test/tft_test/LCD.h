/*
 * LCD.h
 *
 * Created: 6-12-2018 13:30:31
 *  Author: joost
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "SPI.h"

class LCD
{
	public:
	LCD();
	void init(uint8_t orientation);
	void drawPixel(uint16_t x,uint16_t y,uint16_t color);
	void fillScreen(uint16_t color);
	void drawRectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color);
	uint16_t colorTo565(uint8_t red,uint8_t green,uint8_t blue);
	uint32_t getStatus();
	uint8_t getMode();
	uint8_t getMADCTL();
	private:
	void tft_command(uint8_t command);
	void writePixel(uint16_t color);
	void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	
};




#endif /* LCD_H_ */