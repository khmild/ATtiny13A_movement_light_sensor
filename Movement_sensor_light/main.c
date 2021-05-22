#define F_CPU 9600000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "eeprom.h"

void adc_init();
uint16_t check_sens();
uint8_t check_but();
uint8_t check_mov();

uint16_t sensor_val = 0;
uint16_t set_val = 0;
uint16_t hysteresis = 15;

int main(void)
{
	//relay output (PB1)
	DDRB |= (1 << DDB1);
	PORTB &= ~(1 << DDB1);
	
	//button input pullup (PB4)
	DDRB &= ~(1 << DDB4);
	PORTB |= (1 << DDB4);
	
	//movement sensor input
	DDRB &= ~(1 << DDB0);
	//PORTB |= (1 << DDB0);
	
	set_val = read_eeprom();
	if (set_val > 1024) set_val = 500;

	adc_init();
	wdt_enable(WDTO_8S);
	
	while (1)
	{
		sensor_val = check_sens();
		
		if (check_but())
		{
			save_eeprom(sensor_val);
			set_val = sensor_val;
			_delay_ms(100);
		}
		
		
		if (check_mov())
		{
			if (sensor_val > (set_val + hysteresis))
			{
				PORTB &= ~(1 << DDB1);
			}
		}
		else
		{
			PORTB |= (1 << DDB1);
		}
		
		wdt_reset();
	}
}


void adc_init(){
	ADCSRA = (1 << ADEN)|(1 << ADPS1)|(1 << ADPS0); // Enable ADC, prescaler 64
}


uint16_t check_sens(){
	
	ADMUX = (1 << MUX0)|(1 << MUX1); //PB3 select
	ADCSRA |= (1 << ADSC); //start conversion
	
	while((ADCSRA & (1 << ADSC))); //wait for conversion
	
	return (uint16_t)ADC;
}


uint8_t check_but(){
	if (PINB & (1 << DDB4)) return 0; //inverse
	else return 1;
}


uint8_t check_mov(){
	if (PINB & (1 << DDB0)) return 1;
	else return 0;
}