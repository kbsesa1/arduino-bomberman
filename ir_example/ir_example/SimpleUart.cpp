/*
 * SimpleUart.cpp
 *
 * Created: 28-11-2018 13:34:47
 *  Author: joost
 */ 
/*
 * SimpleUart.c
 *
 * Created: 25-11-2018 22:16:00
 *  Author: joost
 */ 

#include "SimpleUart.h"

UART::UART(){
	
}

void UART::Init( unsigned long f_cpu,unsigned long baud)
{
	/*calculate baud register*/
unsigned int ubrr = f_cpu/16/baud-1;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (3<<UCSZ00);
}
void UART::Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
unsigned char UART::Receive()
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}
void UART::PutString(char* string){
	for (int i = 0;i<strlen(string);i++)
	{
	Transmit(string[i]);
	}
}
void UART::PutInt(int input){
	char str[20];
	sprintf(str, "%d", input);
	PutString(str);
}
void UART::PutUInt(int input){
	char str[20];
	sprintf(str, "%u", input);
	PutString(str);
}