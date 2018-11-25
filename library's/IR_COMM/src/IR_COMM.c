/*
 * IR_COMM.c
 *
 * Created: 23-11-2018 17:00:47
 *  Author: joost
 */ 

#include "IR_COMM.h"
#include <stdbool.h>

const int sendTimes[] = {20,40,60,80,20};//number of ticks to set led(low,high,start,stop,divider)
uint8_t sendFrame[23];//array to save lengths of pulses
int sendIndex = 0;//index for pulsearray

int newFrame = 0;//bool if arduino should start sending
int sendingFrame = 0;//bool if arduino is busy sending

int recievelookup[4];
const int recieveTimes38_38[] = {43,65,90,110};
const int recieveTimes38_56[] = {32,49,65,81};
const int recieveTimes56_56[] = {47,70,96,118};
const int recieveTimes56_38[] = {63,94,129,158};
const int devPos = 3;
const int devNeg = 3;

//private recieve variables
volatile unsigned long IR_ticks;
volatile unsigned long last_IR_ticks;
volatile unsigned long pulseBegin = 0;
volatile unsigned long pulseEnd = 0;
volatile uint8_t frameBuffer[15];
volatile int frameIndex = 0;
volatile bool irAvaliable = 0;


//debug recieve variables
volatile bool unknownTime = 0;
volatile int rawFrame[15];
volatile int rawindex;
volatile bool rawNew = 0;


void IR_Init(int sendFreq,int recFreq){
	//set ir led port to output
	DDRD |= (1 << PORTD3);
	//set timer to toggle on compare match
	TCCR2A |= (1 << COM2B0);
	TCCR2A &= ~(1 << COM2B1);
	//set waveform generation to normal mode
	TCCR2A &= ~(1 << WGM20);
	TCCR2A &= ~(1 << WGM21);
	TCCR2B &= ~(1 << WGM22);
	
	
	//set prescaler to 1
	TCCR2B |= (1 << CS20);
	TCCR2B &= ~(1 << CS21);
	TCCR2B &= ~(1 << CS22);
	//set output compare register to 209 for 38KHz
	if (sendFreq == 38)
	{
		OCR2B = 192;
	}
	else if(sendFreq == 56)
	{
		OCR2B = 125;
	}
	//enable output compare interrupt
	TIMSK2 |= (1 << OCIE2B);
	//enable reciever interrupt
	//setup reciever interrupts
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	
	if (recFreq == 38 && sendFreq == 38)
	{
		for (int i = 0;i<4;i++)
		{
			recievelookup[i] = recieveTimes38_38[i];
		}
	}
	else if (recFreq == 38 && sendFreq == 56)
	{
		for (int i = 0;i<4;i++)
		{
			recievelookup[i] = recieveTimes56_38[i];
		}
	}
	else if(recFreq == 56 && sendFreq == 38)
	{
		for (int i = 0;i<4;i++)
		{
			recievelookup[i] = recieveTimes38_56[i];
		}
	}
	else if(recFreq == 56 && sendFreq == 56)
	{
		for (int i = 0;i<4;i++)
		{
			recievelookup[i] = recieveTimes56_56[i];
		}
	}
	
	
	
	
	//enable global interrupt
	sei();
	
}
char IR_Recieve(){
	irAvaliable = 0;
	char output = 0;
	for (int i = 0;i<8;i++)
	{
		output |= (frameBuffer[i+1] << i);
	}
	uint8_t outParity = (output & 1);
	return output;
	
	if (outParity == frameBuffer[9])
	{
		
		return output;
	}
	else{
		
		return 0;
	}
	
}
/*
void IR_printRaw(){
char output[30];
for (int i = 0;i<13;i++)
{
USART_PutInt(rawFrame[i]);
USART_PutString(", ");

}
USART_Transmit('\n');
rawAvailable = 0;
}
void IR_printFrame(){
for (int i = 0;i<13;i++)
{
USART_PutInt(frameBuffer[i]);
USART_PutString(", ");

}
USART_Transmit('\n');

}
*/
bool IR_Available(){
	return irAvaliable;
}
void IR_flush(){
	while (sendingFrame)
	{
	}
}
bool IR_rawAvailable(){
	return rawNew;
}
void IR_Transmit(char input){
	sendFrame[0] = 2;
	uint8_t framePos = 1;
	sendFrame[framePos] = 4;
	framePos ++;
	for (int i = 0;i<8;i++)
	{
		uint8_t bitState = (input >> i)&1;
		sendFrame[framePos] = bitState;
		framePos ++;
		sendFrame[framePos] = 4;
		framePos ++;
		
	}
	uint8_t parity = (input & 1);
	sendFrame[framePos] = parity;
	framePos ++;
	sendFrame[framePos] = 4;
	framePos ++;
	sendFrame[framePos] = 3;
	framePos ++;
	sendFrame[framePos] = 4;
	framePos ++;
	sendFrame[framePos] = 0;
	newFrame = 1;
	
}

ISR(TIMER2_COMPB_vect){
	TCNT2 = 0;
	IR_ticks ++;
	if (newFrame)
	{
		IR_ticks = 0;
		newFrame = 0;
		sendingFrame = 1;
		last_IR_ticks = IR_ticks;
		TCCR2A |= (1 << COM2B0);
		
	}
	if (sendingFrame)
	{
		if (IR_ticks >= last_IR_ticks + sendTimes[sendFrame[sendIndex]])
		{
			if (sendFrame[sendIndex] == 4)//detect if it is a divider
			{
				TCCR2A |= (1 << COM2B0);
			}
			else{
				TCCR2A &= ~(1 << COM2B0);
				
			}
			
			sendIndex++;
			last_IR_ticks = IR_ticks;
		}
		if (sendIndex >= 23)
		{
			sendIndex = 0;
			sendingFrame = 0;
			TCCR2A &= ~(1 << COM2B0);
		}
	}
	
	
}

ISR(INT0_vect){
	pulseEnd = IR_ticks;
	int pulsTime = pulseEnd-pulseBegin;
	pulseBegin = pulseEnd;
	
	//raw code
	if (pulsTime > 200)
	{
		rawFrame[rawindex] = pulsTime;
		rawindex = 0;
		rawNew = 1;
	}
	rawFrame[rawindex] = pulsTime;
	rawindex++;
	
	//recieve code
	if (pulsTime <= (recievelookup[0]+devPos) && pulsTime >= (recievelookup[0]-devNeg))
	{
		frameBuffer[frameIndex] = 0;
		frameIndex ++;
	}
	else if (pulsTime <= (recievelookup[1]+devPos) && pulsTime >= (recievelookup[1]-devNeg))
	{
		frameBuffer[frameIndex] = 1;
		frameIndex ++;
	}
	else if (pulsTime <= (recievelookup[2]+devPos) && pulsTime >= (recievelookup[2]-devNeg))
	{
		frameIndex = 0;
		
		frameBuffer[frameIndex] = 2;
		frameIndex ++;
	}
	else if (pulsTime <= (recievelookup[3]+devPos) && pulsTime >= (recievelookup[3]-devNeg))
	{
		irAvaliable = 1;
		frameBuffer[frameIndex] = 3;
		frameIndex ++;
	}
	else{
		frameBuffer[frameIndex] = 5;
		frameIndex ++;
		unknownTime = 1;
	}
	
}