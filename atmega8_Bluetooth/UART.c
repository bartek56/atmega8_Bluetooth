/*
 * UART.c
 *
 * Created: 2016-04-23 18:37:33
 *  Author: Bartek
 */

#include <avr/interrupt.h>
#include "UART.h"

//volatile unsigned int bufferSendIndex;
volatile char bufferReceive[80];
uint8_t bufferReceiveIndex;
char* buforSend;

void ClearBufor()
{
	for(int i=0; i<80; i++)
	{
	bufferReceive[i]='\0';
	}
}

//interrupt generated after receiving the byte
ISR(USART_RXC_vect)
{
	
	bufferReceive [bufferReceiveIndex] = UDR;
	
	/* 
	0x0D, \r  CR
	0x0A, \n  LF
	0x4B  K
	0x4F  O
	*/

	if(bufferReceive[bufferReceiveIndex]=='\r' || bufferReceive[bufferReceiveIndex]=='\n')
	{
		bufferReceive[bufferReceiveIndex]=0x5F;
	}

	bufferReceiveIndex++;
}

//przerwanie generowane, gdy bufor nadawania jest ju� pusty
ISR(USART_UDRE_vect)
{
	static int bufferSendIndex = 0;
	
	//czy bufor nadawania nie ma ostatniego znaku
	if(buforSend[bufferSendIndex]!= 0)
	{
		//za�aduj znak do rejestru wysy�ki i ustaw indeks na nast�pny znak
		UDR = buforSend[bufferSendIndex++];
	}
	else
	{
		//osi�gni�to koniec napisu w tablicy bufor_nad
		UCSRB &= ~(1<<UDRIE); //wy��cz przerwania pustego bufora nadawania
		bufferSendIndex = 0; 
	}
}

//definiowanie parametr�w transmisji
void UsartInit(void)
{
	#define BAUD 9600
	//#define BAUD 19200       //warto�� pr�dko�� transmisji
	
	#include <util/setbaud.h>
	
	//obliczone przez makro warto�ci
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	
	UCSRA &= ~(1<<U2X);
	
	//modu� GSM wymaga 8 bit�w, brak bitu parzysto�ci, 1 bit stopu
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); 
	//bit�w danych: 8
	//bity stopu:  1
	//parzysto��:  brak
	
	//w��czenie nadajnika i odbiornika oraz przerwanie odbiornika
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
}

//wysy�anie danych
void Send(char data[])
{
	
	buforSend=data; // wska�nik na tablice, dzi�ki temu mo�na mie� r�ne d�ugo�ci znak�w
	
	//a� bufor nadawania b�dzie pusty
	while (!(UCSRA & (1<<UDRE)));
	
	//w��cz przerwanie nadawania
	UCSRB |= (1<<UDRIE);
}

