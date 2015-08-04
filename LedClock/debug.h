/*
 * Debug.h
 *
 *  Created on: Jul 25, 2015
 *      Author: flitjesdev
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include "serial.h"
#define DEBUG

#ifdef DEBUG
char debug_str[25];
#endif

#ifdef DEBUG
#define print_state(x) print_string(x)
#else
#define print_state(x)
#endif

#endif /* DEBUG_H_ */
