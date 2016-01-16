/*
Author: Scott Edgerly
Date: 1/14/2016
Description: This program is written to practice using the ADC on the ATtiny85
	     which will be used on Project Nyx
*/
#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void){
	int val = 0;
	ADMUX &= ~(0xFF); //ensure all bits are zero before we write.
        ADMUX = (0<<REFS2)|(0<<REFS1)|(0<<REFS0) | //Set voltage reference here. (See Table 17-3)
                (1<<ADLAR)                       | //Set Left (1) or Right (0) Adjust.
                (0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0); //select input channel. (See Table 17-4)

	ADCSRA &= ~(0xFF); //ensure all bits are zero before we write
        ADCSRA = (1<<ADEN)| //Enable ADC bit. (1 = enabled, 0 = disabled
                 (0<<ADSC)| //ADC Start Conversion bit. (write to 1 to start co$
                (0<<ADATE)| //ADC Auto Trigger Enable bit. (1 = enabled, 0 = di$
                 (0<<ADIF)| //ADC Interrupt Flag. (bit is set when ADC conversi$
                 (0<<ADIE)| //ACD Interrupt Enable bit. (1 = enabled, 0 = disabled
		(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler Select bits. Needs to be set such the ADC clock is 50kHz-200kHz. (see Section 17.5 and Table 17-5) 1MHz configuration
		//(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0); //Prescaler Select bits. Needs to be set such the ADC clock is 50kHz-200kHz. (see Section 17.5 and Table 17-5) 8MHz configuration.

	DDRB &= ~(0xFF);
	DDRB |= 1<<DDB4; //make PB4 an output
	PORTB &= ~(1 << PORTB4); //ensure output of PB4 is low

	DIDR0 |= (0<<ADC0D)|(1<<ADC2D)|(1<<ADC3D)|(1<<ADC1D); //Disable inputs to save power


	while(1){
		ADCSRA |= (1<<ADSC); //start conversion.
		while((ADCSRA & (1<<ADSC))); //wait for the conversion to complete
	//	ADCSRA |= (1<<ADIF); //clear flag to srart next conversion
//		_delay_ms(5);
//		val = (ADCH<<2)|ADCL;
		val = ADC;
		if(val >= 512){
			PORTB |= (1<<PORTB4); //turn on LED
		}
		else if(val < 512){
			PORTB &= ~(1<<PORTB4);
		}
	//	ADCSRA |= (1<<ADSC); //start conversion.

//		_delay_ms(25);
		//PORTB ^= (1<<PB4);
	}
	return 0;
}
