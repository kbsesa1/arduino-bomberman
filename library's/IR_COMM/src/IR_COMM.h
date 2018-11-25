/*
 * IR_COMM.h
 *
 * Created: 23-11-2018 17:01:00
 *  Author: joost
 */ 


#ifndef IR_COMM_H_
#define IR_COMM_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdbool.h>


	void Init();
	char Recieve();
	bool Available();
	/*
	void IR_printRaw();
	void IR_printFrame();
	*/
	void flush();
	bool rawAvailable();
	void Transmit(char);
	
	
	
	

#endif /* IR_COMM_H_ */