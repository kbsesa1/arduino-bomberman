/*
* IR_test.cpp
*
* Created: 28-11-2018 12:11:09
* Author : joost
*/
#define F_CPU 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)

#include <avr/io.h>
#include <util/delay.h>
#include "IR_COMM.h"
#include "SimpleUart.h"

bool raw = 0;
bool sending = 0;
char message[] = "hello infrared!\n";

int main(void)
{
	USART_Init(MYUBRR);
	IR_Init(38,38);

	
	/* Replace with your application code */
	while (1)
	{
		if (sending)
		{
			for (int i = 0;i<sizeof(message);i++)
			{
				IR_Transmit(message[i]);
				_delay_ms(10);
			}
			_delay_ms(500);
		}
		else{
			if (raw)
			{
				
				if (IR_rawAvailable())
				{
					IR_printRaw();
				}
			}
			else{
				if (IR_Available())
				{
					
					USART_Transmit(IR_Recieve());
					
				}
			}
			
		}
		
		
		
		
		
	}
}

