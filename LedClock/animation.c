/*
 * animation.c
 *
 *  Created on: Jul 12, 2015
 *      Author: flitjesdev
 */
#include "animation.h"
#include "ws2812.h"
#include "time.h"
#include <msp430.h>

LED time_color_hour = { 0x00, 0x00, 0xFF };
LED time_color_minute = { 0x00, 0x00, 0xFF };
LED time_color_second = { 0x00, 0xFF, 0x00 };
LED time_color_second_75 = { 0x00, 0xFF * 0.75, 0x00 };
LED time_color_second_50 = { 0x00, 0xFF * 0.50, 0x00 };
LED time_color_second_25 = { 0x00, 0xFF * 0.25, 0x00 };
LED normal_color = { 0xFF, 0x00, 0x00 };

void show_clock(u_int n, struct time_t* time){
	int i;
		char hour = time->hour * 5;
		for (i = 0; i < n; i++){			// n is number of LEDs
			if(i == hour - 1 )
				setLEDColor(i, time_color_hour.red, time_color_hour.green, time_color_hour.blue);
			else if (i == hour - 2  || i == hour)
				setLEDColor(i, time_color_hour.red / 2, time_color_hour.green / 2, time_color_hour.blue / 2);
			else if ( i == time->minute - 1 )
				setLEDColor(i, time_color_minute.red, time_color_minute.green, time_color_minute.blue);
			else if ( i == time->second - 1)
				setLEDColor(i, time_color_second.red, time_color_second.green, time_color_second.blue);
			else if ( i == time->second - 2)
				setLEDColor(i, time_color_second_75.red, time_color_second_75.green, time_color_second_75.blue);
			else if ( i == time->second - 3)
				setLEDColor(i, time_color_second_50.red, time_color_second_50.green, time_color_second_50.blue);
			else if ( i == time->second - 4)
						setLEDColor(i, time_color_second_50.red, time_color_second_50.green, time_color_second_25.blue);
			else
				setLEDColor(i, normal_color.red, normal_color.green, normal_color.blue);

		}
		showStrip();
		_delay_cycles(50000);
}

