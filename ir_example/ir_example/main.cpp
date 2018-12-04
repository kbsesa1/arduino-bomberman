/*
* ir_example.cpp
*
* Created: 28-11-2018 17:51:24
* Author : joost
*/
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lib/AVRmillis.h"
#include "lib/IRcomm.h"
#include "lib/SimpleUart.h"

UART Serial;
IR ir;

bool sender = 1;
char message[] = "hello infrared!\n";
uint8_t messageIndex = 0;

bool sendMessage = 0;

unsigned long txTime = 0;
unsigned long messageTime = 0;

int main(void)
{
	millis.init(F_CPU);
	Serial.Init(F_CPU,9600);
	if (sender)
	{
		ir.Init(38,38);//send, recieve frequency
	}
	else{
		ir.Init(56,38);//send, recieve frequency
	}
	
	/* Replace with your application code */
	while (1)
	{
		if (sender)
		{
			if (millis.get() >= messageTime + 500)
			{
				Serial.PutString("sending!\n");
				for (uint8_t i = 0;i<sizeof(message);i++)
				{
					ir.Transmit(message[i]);
					_delay_ms(10);
				}
				messageTime = millis.get();
			}
			
			
		}
		else{
			if (ir.Available())
			{
				Serial.Transmit(ir.Recieve());
			}
		}
	}
	
}

