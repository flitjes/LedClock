/*
 * state.h
 *
 *  Created on: Jul 12, 2015
 *      Author: flitjesdev
 */

#ifndef STATE_H_
#define STATE_H_
#include "serial.h"
#include "debug.h"

typedef enum {
	INIT,
	SET_TIME,
	PRINT_TIME,
	RUNNING,
	STOP,
	START
}CLOCK_STATE;

extern CLOCK_STATE current_state;
extern CLOCK_STATE previous_state;

void switch_state(CLOCK_STATE state);

#endif /* STATE_H_ */
