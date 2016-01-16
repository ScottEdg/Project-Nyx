/*
Author: Scott Edgerly
Date: 1/14/2015
Description: This program is a simple test to blink an LED using the ATtiny85.
	     This program was heavily inspired by prb3333's instructable
	     which can be found using the folowing link.
	     http://www.instructables.com/id/Programming-the-ATtiny85-from-Raspberry-Pi/step4/Program-the-ATtiny85/
*/
#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 1<<4; // PORTB is output, all pins
	PORTB = 0x00; // Make pins low to start

//	while(1) {
//	PORTB ^= 1<<4; // invert all the pins
//	_delay_ms(500); // wait some time
//	}
	return 0;
}
