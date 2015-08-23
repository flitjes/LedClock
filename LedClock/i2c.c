/*
 * i2c.c
 *
 *  Created on: Aug 23, 2015
 *      Author: flitjesdev
 */

#include "i2c.h"
#include "TI_USCI_I2C_master.h"
#include <stdlib.h>
#include "serial.h"
#include <stdio.h>

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

void i2c_set_value(unsigned char i2c_dev, unsigned char* data, uint8_t count){

	__enable_interrupt();
	TI_USCI_I2C_transmitinit(i2c_dev, I2C_PRESCALER);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_transmit(count, data);
	while ( TI_USCI_I2C_notready() );
}
