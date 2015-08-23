/*
 * i2c.h
 *
 *  Created on: Aug 23, 2015
 *      Author: flitjesdev
 */

#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
#define I2C_PRESCALER 160

extern void i2c_get_value(unsigned char i2c_dev, unsigned char reg, unsigned char* dest, uint8_t count);
extern void i2c_set_value(unsigned char i2c_dev, unsigned char* data, uint8_t count);

#endif /* I2C_H_ */
