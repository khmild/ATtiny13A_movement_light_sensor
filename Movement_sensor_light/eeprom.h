#ifndef EEPROM_LIB
#define EEPROM_LIB

#include <avr/io.h>

void save_eeprom();
uint16_t read_eeprom();

#endif