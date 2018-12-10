/*
 * SimpleUart.h
 *
 * Created: 28-11-2018 13:35:06
 *  Author: joost
 */ 


#ifndef SIMPLEUART_H_
#define SIMPLEUART_H_

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "SimpleUart.h"

class UART
{
	public:
	UART();
	  void Init(unsigned long f_cpu,unsigned long baud);
	  void Transmit( unsigned char data);
	  unsigned char Receive();
	  void PutString(char* C_string);
	  void PutInt(int number);
	  void PutUInt(int number);
	private:
	
};



#endif /* SIMPLEUART_H_ */