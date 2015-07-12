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

#define LED_COUNT 60

LED time_color_hour = { 0x00, 0x00, 0xFF };
LED time_color_minute = { 0x00, 0x00, 0xFF };
LED time_color_second = { 0x00, 0xFF, 0x00 };
LED time_color_second_75 = { 0x00, 0xFF >> 1, 0x00 };
LED time_color_second_50 = { 0x00, 0xFF >> 3, 0x00 };
LED time_color_second_25 = { 0x00, 0xFF >> 5, 0x00 };
LED normal_color = { 0xFF, 0x00, 0x00 };

u_char brightness = 10;
void show_clock(struct time_t* time){
	int i;
		char hour = time->hour * 5;
		LED set_color;
		for (i = 0; i < LED_COUNT; i++){			// n is number of LEDs
			if(i == hour - 1 ){
				set_color.red = time_color_hour.red;
				set_color.green = time_color_hour.green;
				set_color.blue = time_color_hour.blue;
			}
			else if (i == hour - 2  || i == hour){
				set_color.red = time_color_hour.red >> 3;
				set_color.green = time_color_hour.green >> 3;
				set_color.blue = time_color_hour.blue >> 3;
			}
			else if ( i == time->minute - 1 ){
				set_color.red = time_color_minute.red;
				set_color.green = time_color_minute.green;
				set_color.blue = time_color_minute.blue;
			}
			else if ( i == time->second - 1){
				set_color.red = time_color_second.red;
				set_color.green = time_color_second.green;
				set_color.blue = time_color_second.blue;
			}
			else if ( i == time->second - 2){
				set_color.red = time_color_second.red >> 1;
				set_color.green = time_color_second.green >> 1;
				set_color.blue = time_color_second.blue >> 1;
			}
			else if ( i == time->second - 3){
				set_color.red = time_color_second.red >> 3;
				set_color.green = time_color_second.green >> 3;
				set_color.blue = time_color_second.blue >> 3;
			}
			else if ( i == time->second - 4){
				set_color.red = time_color_second.red >> 5;
				set_color.green = time_color_second.green >> 5;
				set_color.blue = time_color_second.blue >> 5;
			}
			else {
				set_color.red = normal_color.red;
				set_color.green = normal_color.green;
				set_color.blue = normal_color.blue;
			}

			u_int c;
			c = ((set_color.red * brightness) / 100);
			set_color.red = c & 0xFF;
			c = ((set_color.green * brightness) / 100);
			set_color.green = c & 0xFF;
			c = ((set_color.blue * brightness) / 100);
			set_color.blue = c & 0xFF;

			setLEDColor(i,set_color.red, set_color.green, set_color.blue);

		}
		showStrip();
}

