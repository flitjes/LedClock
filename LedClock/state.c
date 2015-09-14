/*
 * state.c
 *
 *  Created on: Aug 23, 2015
 *      Author: flitjesdev
 */
#include "state.h"

CLOCK_STATE current_state = INIT;
CLOCK_STATE previous_state = INIT;

const char* CLOCK_STATE_STR[] = {
		"INIT\n",
		"SET_TIME\n",
		"PRINT_TIME\n",
		"RUNNING\n",
		"STOP\n",
		"SHOW_DATA\n"
};

void switch_state(CLOCK_STATE state){
	previous_state = current_state;
	current_state = state;

	print_state("P: ");
	print_state((char*)CLOCK_STATE_STR[previous_state]);
	print_state("C: ");
	print_state((char*)CLOCK_STATE_STR[current_state]);
}
