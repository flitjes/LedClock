/*
 * seral.h
 *
 *  Created on: Jul 6, 2015
 *      Author: flitjesdev
 */

#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_
#include <stdint.h>
#define SERIAL_BUF_SIZE 128

extern char input_buffer[];
extern uint8_t input_count;

extern void print_string(char* str);
extern void USCI0RXSerialInterruptHandler(void);

#endif /* SRC_SERIAL_H_ */
