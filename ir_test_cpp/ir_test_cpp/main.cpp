/*
* ir_test_cpp.cpp
*
* Created: 28-11-2018 13:33:17
* Author : joost
*/
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "SimpleUart.h"
#include "IRcomm.h"
#include "AVRmillis.h"

UART Serial;
IR ir;

void sendData(char data);
char recieveData();

uint8_t count = 0;
bool sending = 0;
bool greenduino = 1;
char message[] = "hello infrared!\n";
uint8_t messageIndex = 0;
unsigned long sendtime = 0;
char buffer[25];
uint8_t bufferIndex;
char reply[] = "hi, i got: ";
char replyline[25];
int main(void)
{
	millis.init(F_CPU);
	Serial.Init(F_CPU,9600);
	if (greenduino) ir.Init(38,56);
	else ir.Init(56,38);
	
	
	
	/* Replace with your application code */
	while (1)
	{
		if (greenduino)
		{
			if (millis.get()>= sendtime + 20)
			{
				sendData(count);
				count ++;
				Serial.PutString("sent!");
				Serial.Transmit('/n');
			}			
		}
		else{
			char c = recieveData();
			Serial.PutUInt(c);
			Serial.Transmit('\n');
		}
		
		
	}
}
void sendData(char data){
	bool sending = 1;
	while(sending){
		ir.Transmit(count);
		if (ir.Available() == 1)
		{
			uint8_t c = ir.Recieve();
			if (c == 6)
			{
				sending = 0;
			}
			
		}
		_delay_ms(15);
	}
}
char recieveData(){
	char output = 0;
	bool recieving = 1;
	while(recieving){
		if (ir.Available() == 1)//recieved correctly
		{
			output = ir.Recieve();
			ir.Transmit(6);
			recieving = 0;
		}
		else if (ir.Available() == -1)//parity fault
		{
			ir.Transmit(21);
			
		}
	}
	return output;
}

