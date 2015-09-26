/*
 * colors.h
 *
 *  Created on: Jul 12, 2015
 *      Author: flitjesdev
 */

#ifndef COLORS_H_
#define COLORS_H_
#include "ws2812.h"
#include "time.h"

typedef enum {
	HOUR=0,
	MINUTE=1,
	SECOND=2,
	BACKGROUND=3
}COLOR_STATES;

extern LED time_color_hour;
extern LED time_color_minute;
extern LED time_color_second;
extern LED normal_color;
extern uint16_t brightness;

extern void show_clock(struct time_t* time);
void setcolor(uint8_t id, uint8_t r, uint8_t g, uint8_t b);

#endif /* COLORS_H_ */
