/*
 * DS1307.c
 *
 *  Created on: Aug 3, 2015
 *      Author: flitjesdev
 */

#include "DS1307.h"
#include "TI_USCI_I2C_master.h"
#include "serial.h"
#include "i2c.h"

void ds1307_set_time(struct time_t* t){
	unsigned char reg[4];

	reg[0] = DS1307_ADDR_SECONDS;
	reg[1] = ((t->second / 10 ) << 4) | ((t->second % 10) & DS1307_MASK_SEC);
	reg[2] = ((t->minute / 10 ) << 4) | ((t->minute % 10) & DS1307_MASK_MIN);
	reg[3] = ((t->hour / 10 ) << 4) | ((t->hour % 10) & DS1307_MASK_HOUR);

	i2c_set_value(DS1307_ADDR, reg, 4);

}

void ds1307_get_time(struct time_t* t){
	unsigned char time[3];

	i2c_get_value(DS1307_ADDR, DS1307_ADDR_SECONDS, time, 3);


#ifdef DEBUG
	sprintf(debug_str, "Raw data time: %x %x %x\n", time[0], time[1],time[2]);
	print_string(debug_str);
#endif

	t->second = (((time[0] & DS1307_MASK_SEC_10) >> 4) * 10) + (time[0] & DS1307_MASK_SEC);
	t->minute = (((time[1] & DS1307_MASK_MIN_10) >> 4) * 10) + (time[1] & DS1307_MASK_MIN);
	t->hour = (((time[2] & DS1307_MASK_HOUR_10) >> 4) * 10) + (time[2] & DS1307_MASK_HOUR);

}

void ds1307_set_squarewave(uint8_t wave){
	unsigned char ctrl = 0;
	i2c_get_value(DS1307_ADDR, DS1307_ADDR_CONTROL, &ctrl, 1);

	if(wave & BIT0){
		ctrl |= DS1307_CTRL_SQWE;
	} else {
		ctrl &= ~(DS1307_CTRL_SQWE);
	}

	ctrl &= ~(DS1307_CTRL_RS0 | DS1307_CTRL_RS1);

	unsigned char data[2];
	data[0] = DS1307_ADDR_CONTROL;
	data[1] = ctrl;
	i2c_set_value(DS1307_ADDR, data, 2);

}


