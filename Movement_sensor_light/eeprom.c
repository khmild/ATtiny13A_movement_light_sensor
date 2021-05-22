#include "eeprom.h"

void eeprom_write_byte(unsigned char ucAddress, unsigned char ucData){
	while(EECR & (1<<EEPE));
	
	EECR = (0<<EEPM1)|(0<<EEPM0);//set programming mode
	
	EEARL = ucAddress;	//set address
	EEDR = ucData;		//data to write
	
	EECR |= (1<<EEMPE); //write enable
	
	EECR |= (1<<EEPE);	//start eeprom write
}

uint8_t eeprom_read_byte(uint8_t ucAddress){
	while(EECR & (1<<EEPE));
	
	EEARL = ucAddress; //Set up address register
	
	EECR |= (1<<EERE); //Start eeprom read by writing EERE 

	return EEDR;
}

void save_eeprom(uint16_t data){
	uint8_t dataH = data >> 8;
	uint8_t dataL = data;
	
	eeprom_write_byte(0, dataH);
	eeprom_write_byte(1, dataL);
}

uint16_t read_eeprom(){
	uint16_t data_out = 0;
	
	uint8_t dataH = eeprom_read_byte(0);
	uint8_t dataL = eeprom_read_byte(1);
	
	data_out |= (uint16_t)(dataH << 8);
	data_out |= (uint16_t)(dataL);
	
	return data_out;
}