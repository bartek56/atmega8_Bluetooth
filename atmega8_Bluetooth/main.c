/*
 * Atmega8_lcd.c
 *
 * Created: 2016-11-02 21:08:37
 * Author : Bartek
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "UART.h"



int main(void)
{
	int i=0;
	int j=0;
	UsartInit();
	sei();
		
	DDRC |= (1<<PINC0) | (1<<PINC1) | (1<<PINC2) | (1<<PINC3);
	
	PORTC |= (1<<PINC0) | (1<<PINC1) | (1<<PINC2) | (1<<PINC3); // wy³¹cz
	
	
	
    while (1) 
    {
		
		if(j<1)
		{
			
			j++;
			_delay_ms(500);
			Send("AT");	
		}
		
		
		if(bufferReceiveIndex>1)
		{
			
			_delay_ms(200);
			bufferReceiveIndex=0;
			
			if(bufferReceive[0]=='D' && bufferReceive[1]=='1')
			{
				PORTC ^= (1<<PINC0); //stan przeciwny
			}
			else if(bufferReceive[0]=='D' && bufferReceive[1]=='2')
			{
				PORTC ^= (1<<PINC1);
			}
			else if(bufferReceive[0]=='D' && bufferReceive[1]=='3')
			{
				PORTC ^= (1<<PINC2);
			}
			else if(bufferReceive[0]=='D' && bufferReceive[1]=='4')
			{
				PORTC ^= (1<<PINC3);
			}		
			
			i++;
			ClearBufor();
		}
		
		
    }
}

