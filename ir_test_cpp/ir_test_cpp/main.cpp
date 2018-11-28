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

UART Serial;
IR ir;

uint8_t count = 0;
bool sending = 1;
char message[] = "hello infrared!\n";

int main(void)
{
	Serial.Init(F_CPU,9600);
	ir.Init(38,38);
    /* Replace with your application code */
    while (1) 
    {
		if (sending)
		{
			for (uint8_t i = 0;i < sizeof(message);i++)
			{
				ir.Transmit(message[i]);
				_delay_ms(10);
			}
			_delay_ms(500);
		}
		else{
			if (ir.Available())
			{
				Serial.Transmit(ir.Recieve());
			}
		}
		
    }
}

