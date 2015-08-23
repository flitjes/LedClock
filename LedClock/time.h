/*
 * time.h
 *
 *  Created on: Jul 6, 2015
 *      Author: flitjesdev
 */

#ifndef SRC_TIME_H_
#define SRC_TIME_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FORMATED_TIME_SIZE 9

struct time_t{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};

/*Formated time buffer to be parsed in the mainloop*/
extern char time_formated[];

extern struct time_t current;

/*Flag indicating time is set and ready for parsing*/
extern uint8_t f_time_set;

extern void tick(void);
extern void parse_time(char* time, struct time_t* parsed);
extern void set_time(struct time_t* time);
extern void print_time(void);

#endif /* SRC_TIME_H_ */
