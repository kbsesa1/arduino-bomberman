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
#include "IR_COMM.h"
#include "SimpleUart.h"


int main(void)
{
//IR_Init(38,38);
USART_Init(MYUBRR);	
    /* Replace with your application code */
    while (1) 
    {
    }
}

