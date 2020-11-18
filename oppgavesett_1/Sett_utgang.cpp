#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    
    DDRC |= (1<<PC2); //Velg PC2 til utgang
	//PORTC|= (1<<PC2); PC2 settes høy 
	PORTC&=~(1<<PC2); //PC2 settes lav 
}

