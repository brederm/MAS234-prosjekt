#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t ADCverdi;
int main(void)
{
	DDRC &=~(1 << ADC5D); // PC5 er input
	DDRD |= (1 << PD6);	// PD6 er output
	TCCR0A |= (1 << COM0A1); // none-inverting mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);	// fast PWM Mode
	TCCR0B |= (1 << CS01);	// setter prescaler til 8 og starter PWM

	ADMUX |= (1<<REFS0); // Bruker AVcc som referanse spenning 
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	ADCSRA | =(1<<ADEN); // ADC er aktivert
	ADCSRA &=~(1<<ADTS0)|(1<<ADTS1)|(1<<ADTS2); // free running modus
	ADCSRA |=(1<<ADSC); // starter ADC konvertering
	
	 ADCverdi = ADCH;
	
	while (1)
	
	{
		
		OCR0A =(ADCverdi/1023)*256; // konverterer fra ADCH til en PWM verdi
			_delay_ms(10);
	}
}