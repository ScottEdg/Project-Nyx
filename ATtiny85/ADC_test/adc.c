/*
Author: Scott Edgerly
Date: 1/14/2016
Description: This program is written to practice using the ADC on the ATtiny85
	     which will be used on Project Nyx
*/
#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>
void ADC_init(void);
void ADC_light(void);
void ADC_battery(void);
int main(void){

	ADC_init();

	while(1){
		ADC_light();
		_delay_ms(1);
		ADC_battery();
		_delay_ms(1);

	}
	return 0;
}

void ADC_init(void)
{
	ADMUX &= ~(0xFF); //ensure all bits are zero before we write.
        ADMUX = (0<<REFS2)|(0<<REFS1)|(0<<REFS0) | //Set voltage reference here. (See Table 17-3)
                (1<<ADLAR)                       | //Set Left (1) or Right (0) Adjust.
                (0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0); //select input channel. (See Table 17-4)

	ADCSRA &= ~(0xFF); //ensure all bits are zero before we write
        ADCSRA = (1<<ADEN)| //Enable ADC bit. (1 = enabled, 0 = disabled
                 (0<<ADSC)| //ADC Start Conversion bit. (write to 1 to start co$
                (0<<ADATE)| //ADC Auto Trigger Enable bit. (1 = enabled, 0 = di$
                 (0<<ADIF)| //ADC Interrupt Flag. (bit is set when ADC conversi$
                 (0<<ADIE)| //ACD Interrupt Enable bit. (1 = enabled, 0 = disabled
		(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler Select bits. Needs to be set such the ADC clock is 50kHz-200kHz. (see Section 17.5 and Table 17-5) 1MHz configuration

	DDRB &= ~(0xFF);
	DDRB |= (1<<DDB4)|(1<<DDB1)|(1<<DDB0); //make PB4 an output
	PORTB &= ~((1<<PORTB4)|(1<<PORTB1)|(1<<PORTB0)); //ensure output of PB4 is low

	DIDR0 |= (1<<ADC0D)|(1<<ADC2D)|(0<<ADC3D)|(0<<ADC1D); //Disable inputs to save power


}


void ADC_light(void)
{
	int val = 0;
	ADMUX &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
	ADMUX |= (0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0); //Select ADC3
	ADCSRA |= (1<<ADSC); //start conversion.
	while((ADCSRA & (1<<ADSC))); //wait for the conversion to complete
		val = ADC; //store ADC value.
		if(val <= 350){
			PORTB |= (1<<PORTB4); //turn on LED
		}
		if(val > 910){
			PORTB &= ~(1<<PORTB4);
		}

}


void ADC_battery(void)
{
	static int stage = 0;
	int val = 0;
	int sum = 0;
	int i;

	ADMUX &= ~((0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0));
	ADMUX |= (0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0); //Select ADC1
	for(i=0;i<9;i++){
		ADCSRA |= (1<<ADSC); //start conversion.
		while((ADCSRA & (1<<ADSC))); //wait for the conversion to complete
		sum += ADC; //store ADC value.
		_delay_ms(1);
	}
	val = sum/10;
	sum = 0; //reset for next iteration.
	if(val>680 && stage == 0){
		PORTB &= ~((1<<PORTB0)|(1<<PORTB1));
		stage = 1;
	}
	else if(val > 340 && val <= 680 && stage == 1){
		PORTB &= ~((1<<PORTB0)|(1<<PORTB1));
		PORTB |= (1<<PORTB0); //turn on LED
		stage = 2;
	}
	else if(val <= 340 && val > 100 && stage == 2){
		PORTB &= ~((1<<PORTB0)|(1<<PORTB1));
		PORTB |= (1<<PORTB1);
		stage = 3;
	}
	else if(val <= 100 && stage == 3){
		PORTB &= ~((1<<PORTB0)|(1<<PORTB1));
		PORTB |= (1<<PORTB0)|(1<<PORTB1);
		stage = 0;
	}
}
