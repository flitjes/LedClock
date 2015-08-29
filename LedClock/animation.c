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

static void compensate_brightness(LED* led){
	u_int c;
	c = ((led->red * brightness) / 100);
	led->red = c & 0xFF;
	c = ((led->green * brightness) / 100);
	led->green = c & 0xFF;
	c = ((led->blue * brightness) / 100);
	led->blue = c & 0xFF;

}

static void setLedColorBrCtrl(uint8_t index, LED* led){
	LED set_color;
	memcpy(&set_color, led, sizeof(LED));
	compensate_brightness(&set_color);
	setLEDColor(index, set_color.red, set_color.green, set_color.blue);
}
u_char brightness = 10;
void show_clock(struct time_t* time){
	int8_t i;
	uint8_t led_count, led_after_glow, led_after_glow_left;
	LED set_color;
	for (i = 0; i < LED_COUNT; i++){			// n is number of LEDs
		setLedColorBrCtrl(i, &normal_color);
	}

	led_count  = time->hour * 5;
	set_color.red = time_color_hour.red >> 3;
	set_color.green = time_color_hour.green >> 3;
	set_color.blue = time_color_hour.blue >> 3;

	if(led_count == NUM_LEDS){
		setLedColorBrCtrl(led_count - 2, &set_color);
		setLedColorBrCtrl(led_count - 1, &time_color_hour);
		setLedColorBrCtrl(0, &set_color);
	} else if(led_count != NUM_LEDS) {
		setLedColorBrCtrl(led_count - 1, &set_color);
		setLedColorBrCtrl(led_count, &time_color_hour);
		setLedColorBrCtrl(led_count + 1, &set_color);
	}

	setLedColorBrCtrl(time->minute, &time_color_minute);

	led_after_glow = 4;
	led_after_glow_left = led_after_glow;

	for(i = time->second; i >= 0 && i > time->second - led_after_glow; i--){
		setLedColorBrCtrl(i, &time_color_second);
		led_after_glow_left--;
	}
	/*Compensate for the fact it's a linear strip and a clock is round*/
	if(led_after_glow_left > 0){
		for(i = NUM_LEDS - 1; i > (NUM_LEDS - 1) - led_after_glow_left; i--){
			setLedColorBrCtrl(i, &time_color_second);
		}
	}

	showStrip();
}
