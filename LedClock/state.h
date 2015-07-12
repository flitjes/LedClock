/*
 * state.h
 *
 *  Created on: Jul 12, 2015
 *      Author: flitjesdev
 */

#ifndef STATE_H_
#define STATE_H_
#include "serial.h"

#ifdef DEBUG
#define print_state(x) print_string(x)
#else
#define print_state(x)
#endif

typedef enum {
	INIT,
	SET_TIME,
	RUNNING,
	STOP,
	START
}CLOCK_STATE;

extern CLOCK_STATE current_state;

#endif /* STATE_H_ */
