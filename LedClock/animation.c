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
LED time_color_minute = { 0x00, 0xFF, 0x00 };
LED time_color_second = { 0x00, 0x00, 0x00 };
LED normal_color = { 0xCA, 0xBD, 0x80 };

const uint8_t gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


static void compensate_brightness(LED* led){
	u_int c;
	c = ((led->red * brightness) / 100);
	led->red = gamma[c & 0xFF];
	c = ((led->green * brightness) / 100);
	led->green = gamma[c & 0xFF];
	c = ((led->blue * brightness) / 100);
	led->blue = gamma[c & 0xFF];

}

static void setLedColorBrCtrl(uint8_t index, LED* led){
	LED set_color;
	memcpy(&set_color, led, sizeof(LED));
	compensate_brightness(&set_color);
	setLEDColor(index, set_color.red, set_color.green, set_color.blue);
}

uint16_t brightness = 10;
void setColorLength(uint8_t start, uint8_t end, LED *color){
	uint8_t length = 0, count = 0, index = 0;

	//8:15 -> start 41 end 15
	if (start < 60 && end < start)
		length = (60 - start) + end;
	else
		length = end - start;

	index = start;
	while(count < length){
		setLedColorBrCtrl(index, color);
		count++;
		index++;
		if(index == 60){
			index = 0;
		}
	}
}
uint8_t ongoing = 0;
void show_clock(struct time_t* time){
	if(!ongoing){
		uint8_t led_hour;
		uint8_t led_minute;
		ongoing = 1;
		led_hour = (time->hour * 5) + (time->minute / 12);
		led_minute = time->minute;

		if(led_hour > led_minute){
			setColorLength(led_minute, led_hour, &time_color_minute);
			setColorLength(led_hour, led_minute, &time_color_hour);
		} else if (led_hour < led_minute){
			setColorLength(led_hour, led_minute, &time_color_minute);
			setColorLength(led_minute, led_hour, &time_color_hour);
		}
		showStrip();
		ongoing = 0;
	}
}

void setcolor(uint8_t id, uint8_t r, uint8_t g, uint8_t b){
	LED setcolor_to;
	setcolor_to.red = r;
	setcolor_to.green = g;
	setcolor_to.blue = b;
	switch(id){
	case HOUR:
		memcpy(&time_color_hour, &setcolor_to, sizeof(LED));
		break;
	case MINUTE:
		memcpy(&time_color_minute, &setcolor_to, sizeof(LED));
		break;
	case SECOND:
		memcpy(&time_color_second, &setcolor_to, sizeof(LED));
		break;
	case BACKGROUND:
		memcpy(&normal_color, &setcolor_to, sizeof(LED));
		break;
	}

}
