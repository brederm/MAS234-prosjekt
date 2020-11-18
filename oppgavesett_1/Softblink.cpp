#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD |= (1 << DDD6);	// PD6 er output
	TCCR0A |= (1 << COM0A1);	// setter none-inverting mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);	// setter fast PWM Mode
	TCCR0B |= (1 << CS01);	// setter prescaler til 8 og starter PWM
	
	while (1)
        {
		
		for(int i=1; i<256; i++)
		{OCR0A =0+i;
			_delay_ms(10);
		}
	    }
}