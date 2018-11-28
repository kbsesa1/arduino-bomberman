/*
 * SimpleUart.h
 *
 * Created: 25-11-2018 22:16:18
 *  Author: joost
 */ 


#ifndef SIMPLEUART_H_
#define SIMPLEUART_H_

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

void USART_Init( unsigned int);
void USART_Transmit( unsigned char);
unsigned char USART_Receive();
void USART_PutString(char*);
void USART_PutInt(int);
void USART_PutUInt(int);


#endif /* SIMPLEUART_H_ */