/*
 * time.c
 *
 *  Created on: Jul 6, 2015
 *      Author: flitjesdev
 */


#include "time.h"
#include "serial.h"
#include <msp430.h>
#include <ti/mcu/msp430/Grace.h>

struct time_t current;
char time_formated[FORMATED_TIME_SIZE];
uint8_t f_time_set = 0;

void tick(){
	if(current.second + 1 >= 60){
		current.second = 0;
		if(current.minute + 1 >= 60){
			current.minute = 0;
			if(current.hour + 1 >= 12){
				current.hour = 0;
			} else	{
				current.hour++;
			}
		} else {
			current.minute++;
		}
	} else {
		current.second++;
	}
}

void parse_time(char* time, struct time_t* parsed){
	char * pch;
	char* delim = ":";

	print_string("time to parse:");
	print_string(time);
	print_string("\n");

	pch = strtok (time,delim);
	parsed->hour = atoi(pch);
	pch = strtok (NULL, delim);
	parsed->minute = atoi(pch);
	pch = strtok (NULL, delim);
	parsed->second = atoi(pch);
}

void set_time(struct time_t* time){
	memcpy(&current, time, sizeof(struct time_t));
	print_time();
}

void print_time(){
	char time_str[25];
	sprintf(time_str, "Current time: %d:%d:%d\n", current.hour, current.minute, current.second);
	print_string(time_str);
}

/*
 *  ======== Timer0_A3 Interrupt Service Routine ========
 */
/*#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR_HOOK(void)
{
	tick();
	sprintf(time_str, "Time: %d:%d:%d\n", current.hour, current.minute, current.second);
	print_string(time_str);
}*/
