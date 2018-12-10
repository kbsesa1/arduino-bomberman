/*
 * LCD.c
 *
 * Created: 6-12-2018 13:30:15
 *  Author: joost
 */ 
#include "LCD.h"


SPI _spi(&DDRB,&PORTB,2);


LCD::LCD(){
	
}
void LCD::tft_command(uint8_t command){
	PORTB &= ~(1<<PORTB1);
	_spi.transmit(command);
	PORTB |= (1<<PORTB1);
}
void LCD::writePixel(uint16_t color){
	_spi.transmit16(color);
}
void LCD::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	uint32_t xa = ((uint32_t)x << 16) | (x+w-1);
	uint32_t ya = ((uint32_t)y << 16) | (y+h-1);
	tft_command(0x2A); // Column addr set
	_spi.transmit32(xa);
	tft_command(0x2B); // Row addr set
	_spi.transmit32(ya);
	tft_command(0x2C); // write to RAM
}
void LCD::drawPixel(uint16_t x, uint16_t y, uint16_t color){
	 if((x < 0) ||(x >= 240) || (y < 0) || (y >= 320)) return;
	 _spi.beginTransmission();
	 setAddrWindow(x,y,1,1);
	 writePixel(color);
	 _spi.endTransmission();
}
void LCD::init(uint8_t orientation){
	_spi.init();
	//set data/command pin to output
	DDRB |= (1<<PORTB1);
	//set screen to data mode by default
	PORTB |= (1<<PORTB1);
	_spi.beginTransmission();
	
	tft_command(0xC0);//power control 1
	_spi.transmit(0x23);
	tft_command(0xC1);// power control 2
	_spi.transmit(0x10);
	
	tft_command(0xC5);    //VCM control
	_spi.transmit(0x3e);
	_spi.transmit(0x28);

	tft_command(0xC7);    //VCM control2
	_spi.transmit(0x86);
	
	tft_command(0x36);    // Memory Access Control
	_spi.transmit(0x48);

	tft_command(0x37); // Vertical Scrolling Start Address
	_spi.transmit(0);                 // Zero
	_spi.transmit(0);

	tft_command(0x3A);  // COLMOD: Pixel Format Set
	_spi.transmit(0x55);

	tft_command(0xB1); //Frame Rate Control (In Normal Mode/Full Colors)
	_spi.transmit(0x00);
	_spi.transmit(0x18);

	tft_command(0xB6);    // Display Function Control
	_spi.transmit(0x08);
	_spi.transmit(0x82);
	_spi.transmit(0x27);

	tft_command(0xF2);    // 3Gamma Function Disable
	_spi.transmit(0x00);

	tft_command(0x26);    //Gamma curve selected
	_spi.transmit(0x01);

	tft_command(0xE0);    //Set Gamma
	_spi.transmit(0x0F);
	_spi.transmit(0x31);
	_spi.transmit(0x2B);
	_spi.transmit(0x0C);
	_spi.transmit(0x0E);
	_spi.transmit(0x08);
	_spi.transmit(0x4E);
	_spi.transmit(0xF1);
	_spi.transmit(0x37);
	_spi.transmit(0x07);
	_spi.transmit(0x10);
	_spi.transmit(0x03);
	_spi.transmit(0x0E);
	_spi.transmit(0x09);
	_spi.transmit(0x00);

	tft_command(0xE1);    //Set Gamma
	_spi.transmit(0x00);
	_spi.transmit(0x0E);
	_spi.transmit(0x14);
	_spi.transmit(0x03);
	_spi.transmit(0x11);
	_spi.transmit(0x07);
	_spi.transmit(0x31);
	_spi.transmit(0xC1);
	_spi.transmit(0x48);
	_spi.transmit(0x08);
	_spi.transmit(0x0F);
	_spi.transmit(0x0C);
	_spi.transmit(0x31);
	_spi.transmit(0x36);
	_spi.transmit(0x0F);

	tft_command(0x11);    //Exit Sleep
	tft_command(0x29);    //Display on
	
	_spi.endTransmission();
}
void LCD::fillScreen(uint16_t color){
	_spi.beginTransmission();
	setAddrWindow(0,0,240,320);
	for (uint32_t i = 0;i<76800;i++)
	{
		writePixel(color);
	}
	_spi.endTransmission();
}
void LCD::drawRectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color){
	_spi.beginTransmission();
	setAddrWindow(x,y,width,height);
	uint32_t pixelCount = width*height;
	for (uint32_t i = 0;i<pixelCount;i++)
	{
		writePixel(color);
	}
	_spi.endTransmission();
}
uint16_t LCD::colorTo565(uint8_t red,uint8_t green,uint8_t blue){
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}
uint32_t LCD::getStatus(){
	uint32_t output = 0;
	_spi.beginTransmission();
	tft_command(0x09);
	output |= (_spi.read(0x01)<<24);
	output |= (_spi.read(0x01)<<16);
	output |= (_spi.read(0x01)<<8);
	output |= _spi.read(0x01);
	_spi.endTransmission();
	return output;
}
uint8_t LCD::getMode(){
	uint8_t output = 0;
	_spi.beginTransmission();
	tft_command(0x0A);
	output = _spi.read(0x00);
	
	_spi.endTransmission();
	return output;
}
uint8_t LCD::getMADCTL(){
	uint8_t output = 0;
	_spi.beginTransmission();
	tft_command(0x0B);
	output = _spi.read(0x00);
	
	_spi.endTransmission();
	return output;
}
