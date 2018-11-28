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
	  void Init(unsigned long,unsigned long);
	  void Transmit( unsigned char);
	  unsigned char Receive();
	  void PutString(char*);
	  void PutInt(int);
	  void PutUInt(int);
	private:
	
};



#endif /* SIMPLEUART_H_ */