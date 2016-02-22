/*
 * serial.c
 *
 *  Created on: Jul 6, 2015
 *      Author: flitjesdev
 */

#include <ti/mcu/msp430/Grace.h>
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial.h"
#include "time.h"
#include "state.h"
#include "debug.h"
#include "ws2812.h"
#include "animation.h"

char input_buffer[SERIAL_BUF_SIZE];
uint8_t input_count = 0;

static void handle_input(char input);

void print_string(char* str){
	int count = 0;
	while(str[count] != '\0'){
		while (!(IFG2&UCA0TXIFG));
			char out = str[count];
			if ( out == '\n')
				UCA0TXBUF = '\r';
			else
				UCA0TXBUF = out;
			count++;
			if(count >= SERIAL_BUF_SIZE) break;
	}
	while (!(IFG2&UCA0TXIFG));
}
static uint8_t data = 0;
static uint8_t led_data[3];
static signed int led_data_count = 0;
static uint8_t led_count = 0;
void USCI0RXSerialInterruptHandler(void)
{
    /* USER CODE START (section: USCI0RX_ISR_HOOK) */
    /* replace this comment with your code */
    /* USER CODE END (section: USCI0RX_ISR_HOOK) */
	char input = UCA0RXBUF;
	if(data == 0){
		handle_input(input);
	} else {
		led_data[led_data_count] = input;
		if(led_data_count >= 2){
			led_data_count = -1;
			setLEDColor(led_count, led_data[2], led_data[1], led_data[0]);
			led_count++;

			if(led_count == NUM_LEDS){
				switch_state(SHOW_DATA);
				led_count = 0;
				data = 0;
			}
		}
		led_data_count++;
	}

}

char conv [4] = { 0, 0, '\n', '\0' };
static void handle_input(char input){
	if(input_count >= SERIAL_BUF_SIZE)
		input_count = 0;

	input_buffer[input_count + 1] = '\0';
	//UCA0TXBUF = input;
	if(input == '\n'){
		char* p = strchr(input_buffer, '\r');
		*p = '\0';
		p = strstr(input_buffer, "time=");
		if(p != NULL){
			p = strstr(input_buffer, "=");
			//value example 17:15:01
			memcpy(time_formated, p, FORMATED_TIME_SIZE);
			f_time_set = 1;
			switch_state(SET_TIME);
			p=NULL;
		}
		if(strcmp(input_buffer, "stop") == 0){
			switch_state(STOP);
		} else {
			if(strcmp(input_buffer, "start") == 0){
				switch_state(INIT);
			}
		}

		if(strcmp(input_buffer, "current_time") == 0){
			switch_state(PRINT_TIME);
		}

		if(strcmp(input_buffer, "leddata") == 0){
			switch_state(STOP);
			data=1;
			led_data_count = 0;
		}

		p = strstr(input_buffer, "setcolor=");
		if(p != NULL){
			p = strstr(input_buffer, "=");
			//example 1,0xff0x000xff
			char id_str[2];
			int r,g,b,id;
			id_str[0]=*(p+1);
			id_str[1]='\0';

			id = atoi(id_str);

			conv[0] = *(p+2);
			conv[1] = *(p+3);
			r = strtol(conv, NULL, 16);
			print_string(conv);

			conv[0] = *(p+4);
			conv[1] = *(p+5);
			print_string(conv);
			g = strtol(conv, NULL, 16);

			conv[0] = *(p+6);
			conv[1] = *(p+7);
			print_string(conv);
			b = strtol(conv, NULL, 16);

			setcolor(id, r , g, b);
			p=NULL;
		}

		input_count = 0;
		input_buffer[0] = '\0';

	} else {
		input_buffer[input_count] = input;
		input_count++;
	}
}
