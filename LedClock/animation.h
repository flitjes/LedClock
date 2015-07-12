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

extern LED time_color_hour;
extern LED time_color_minute;
extern LED time_color_second;
extern LED time_color_second_75;
extern LED time_color_second_50;
extern LED time_color_second_25;
extern LED normal_color;
extern u_char brightness;

extern void show_clock(u_int n, struct time_t* time);

#endif /* COLORS_H_ */
