/*
 * ir_example.cpp
 *
 * Created: 28-11-2018 17:51:24
 * Author : joost
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "SimpleUart.h"
#include "AVRmillis.h"

UART Serial;

int main(void)
{
	millis.init(F_CPU);
	Serial.Init(F_CPU,9600);
    /* Replace with your application code */
    while (1) 
    {
		Serial.PutInt(millis.get());
		Serial.Transmit('\n');
		_delay_ms(100);
    }
}

