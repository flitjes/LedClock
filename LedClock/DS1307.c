/*
 * DS1307.c
 *
 *  Created on: Aug 3, 2015
 *      Author: flitjesdev
 */

#include "DS1307.h"
#include "TI_USCI_I2C_master.h"
#include "serial.h"

void ds1307_set_time(struct time_t* t){
	unsigned char reg[4];

	reg[0] = DS1307_ADDR_SECONDS;
	reg[1] = ((t->second / 10 ) << 4) | ((t->second % 10) & DS1307_MASK_SEC);
	reg[2] = ((t->minute / 10 ) << 4) | ((t->minute % 10) & DS1307_MASK_MIN);
	reg[3] = ((t->hour / 10 ) << 4) | ((t->hour % 10) & DS1307_MASK_HOUR);

	__enable_interrupt();
	TI_USCI_I2C_transmitinit(DS1307_ADDR, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_transmit(4, &reg[0]);
	while ( TI_USCI_I2C_notready() );

}

void ds1307_get_time(struct time_t* t){
	unsigned char time[3];
	unsigned char reg = DS1307_ADDR_SECONDS;
	__enable_interrupt();
	TI_USCI_I2C_transmitinit(DS1307_ADDR, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_transmit(1, &reg);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_receiveinit(DS1307_ADDR, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_receive(3, &time[0]);
	while ( TI_USCI_I2C_notready() );


#ifdef DEBUG
	sprintf(debug_str, "Raw data time: %x %x %x\n", time[0], time[1],time[2]);
	print_string(debug_str);
#endif

	t->second = (((time[0] & DS1307_MASK_SEC_10) >> 4) * 10) + (time[0] & DS1307_MASK_SEC);
	t->minute = (((time[1] & DS1307_MASK_MIN_10) >> 4) * 10) + (time[1] & DS1307_MASK_MIN);
	t->hour = (((time[2] & DS1307_MASK_HOUR_10) >> 4) * 10) + (time[2] & DS1307_MASK_HOUR);

}

