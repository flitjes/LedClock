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

void i2c_get_value(unsigned char i2c_dev, unsigned char reg, unsigned char* dest, uint8_t count){
	__enable_interrupt();
	/*Init i2c transfer*/
	TI_USCI_I2C_transmitinit(i2c_dev, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	/*Set read pointer*/
	TI_USCI_I2C_transmit(1, &reg);
	while ( TI_USCI_I2C_notready() );
	/*Init i2c read*/
	TI_USCI_I2C_receiveinit(i2c_dev, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	/*Receive*/
	TI_USCI_I2C_receive(count, &dest[0]);
	while ( TI_USCI_I2C_notready() );
}

void i2c_set_value(unsigned char i2c_dev, unsigned char reg,unsigned char* data, uint8_t count){
	__enable_interrupt();
	unsigned char* transfer = (unsigned char*)malloc(count);
	memcpy(transfer, &reg, 1);
	memcpy(transfer+1, data, count);

	TI_USCI_I2C_transmitinit(i2c_dev, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_transmit(count + 1, transfer);
	while ( TI_USCI_I2C_notready() );
}

void ds1307_set_squarewave(uint8_t wave){
	unsigned char ctrl = 0;
	i2c_get_value(DS1307_ADDR, DS1307_ADDR_CONTROL, &ctrl, 1);
	char buf[2];
	sprintf(buf, "%x\n", ctrl);
	print_string(buf);
	if(wave & BIT0){
		ctrl |= DS1307_CTRL_SQWE;
	} else {
		ctrl &= ~(DS1307_CTRL_SQWE);
	}
	sprintf(buf, "%x\n", ctrl);
	print_string(buf);
	i2c_set_value(DS1307_ADDR, DS1307_ADDR_CONTROL, &ctrl, 1);

}


