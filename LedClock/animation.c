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
#include "serial.h"
#include "debug.h"

#define LED_COUNT 60

LED time_color_hour = { 0x00, 0x00, 0xFF };
LED time_color_minute = { 0x00, 0xFF, 0x00 };
LED time_color_off = { 0x00, 0x00, 0x00 };


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

static uint8_t gradientInc(uint8_t start, uint8_t end, uint8_t length, uint8_t index){
	uint8_t p_index = 1 * 100 / length * 100;
	uint16_t p = index * p_index / 100;

	return (start * p + end * (100 - p)) / 100;
}
uint16_t brightness = 10;
void setColorLength(uint8_t start, uint8_t end, LED *color, LED *color_gr, uint8_t gr){
	uint8_t length = 0, count = 0, index = 0;
	LED c;
	//8:15 -> start 41 end 15
	if (start < 60 && end < start)
		length = (60 - start) + end;
	else
		length = end - start;

	index = start;

	while(count < length){
		if(gr){
			c.red = gradientInc(color->red, color_gr->red,length, count);
			c.green = gradientInc(color->green, color_gr->green,length, count);
			c.blue = gradientInc(color->blue, color_gr->blue,length, count);
			setLedColorBrCtrl(index, &c);
		} else {
			setLedColorBrCtrl(index, color);
		}

		count++;
		index++;
		if(index == 60){
			index = 0;
		}
	}
}
#define minute_off 2
#define hour_off 3
#define max_led_cout 59
uint8_t ongoing = 0;
void show_clock(struct time_t* time){
    if(!ongoing){
        uint8_t led_hour, led_minute, minute_fill_start, minute_fill_stop, hour_fill_start, hour_fill_stop;
        ongoing = 1;

        led_hour = (time->hour * 5) + (time->minute / 12)  + max_led_cout;
        led_minute = time->minute + max_led_cout;

        minute_fill_start = led_minute + minute_off;
        minute_fill_stop = led_minute - minute_off;
        hour_fill_start = led_hour + hour_off;
        hour_fill_stop = led_hour - hour_off;


        if(minute_fill_start > max_led_cout)
        	minute_fill_start -= max_led_cout;

        if(minute_fill_stop > max_led_cout)
        	minute_fill_stop -= max_led_cout;

        if(hour_fill_start > max_led_cout)
        	hour_fill_start -= max_led_cout;

        if(hour_fill_stop > max_led_cout)
        	hour_fill_stop -= max_led_cout;


        if(minute_fill_start >= NUM_LEDS)
        	minute_fill_start -= 60;

        if(minute_fill_stop >= NUM_LEDS)
        	minute_fill_stop -= 60;

        if(hour_fill_start >= NUM_LEDS)
        	hour_fill_start -= 60;

        if(hour_fill_stop >= NUM_LEDS)
        	hour_fill_stop -= 60;

        if(led_hour > led_minute){
            setColorLength(minute_fill_start, hour_fill_stop, &time_color_minute, &time_color_minute, 0);
            setColorLength(minute_fill_stop, minute_fill_start, &time_color_off, &time_color_off, 0);
            setColorLength(hour_fill_start, minute_fill_stop, &time_color_hour, &time_color_hour, 0);
            setColorLength(hour_fill_stop, hour_fill_start, &time_color_off, &time_color_off, 0);

        } else if (led_hour < led_minute){

            setColorLength(hour_fill_start, minute_fill_stop, &time_color_minute, &time_color_minute, 0);
            setColorLength(minute_fill_stop, minute_fill_start, &time_color_off, &time_color_off, 0);
            setColorLength(minute_fill_start, hour_fill_stop, &time_color_hour, &time_color_hour, 0);
            setColorLength(hour_fill_stop, hour_fill_start, &time_color_off, &time_color_off, 0);

        }

       	if(led_minute <= (led_hour + minute_off * 2 + hour_off) && led_minute >= (led_hour - minute_off * 2- hour_off)){
        	setLedColorBrCtrl(led_minute - max_led_cout, &time_color_minute);
        }
		showStrip();
		ongoing = 0;
	}
}

void setcolor(uint8_t id, uint8_t r, uint8_t g, uint8_t b){

	switch(id){
		case HOUR:
			time_color_hour.red = r;
			time_color_hour.green = g;
			time_color_hour.blue = b;
			break;
		case MINUTE:
			time_color_minute.red = r;
			time_color_minute.green = g;
			time_color_minute.blue = b;
			break;
	}
}
