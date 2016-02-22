/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/Grace.h>

#include "serial.h"
#include "time.h"
#include "ws2812.h"
#include "state.h"
#include "animation.h"
#include <stdint.h>
#include "debug.h"
#include "DS1307.h"

uint8_t allignment_offset = 31;
/*
 *  ======== main ========
 */

int main(void)
{
	//During Grace init P1.6 and P1.7 are configured as I2C for the RTC

    Grace_init();                   // Activate Grace-generated configuration
    struct time_t parsed;
    struct time_t rtc = { 0, 0, 0 };
    uint8_t previous_second = 120;
    ds1307_set_squarewave(1);

    switch_state(INIT);

    // >>>>> Fill-in user code here <<<<<
    for(;;){
		switch(current_state){
		    case INIT:
		    	ds1307_get_time(&rtc);
		    	// initialize LED strip
				initStrip(allignment_offset);

				set_time(&rtc);
		    	P1IE |= BIT3;
				// set strip color red
				fillStrip(0xFF, 0x00, 0x00);

				print_string("LedClock started\n");
				switch_state(RUNNING);
		    	break;
		    case SET_TIME:
		    	P1IE &= ~(BIT3);

				parse_time(time_formated+1, &parsed);
				set_time(&parsed);

				ds1307_set_time(&parsed);

				// initialize LED strip
				initStrip(allignment_offset);

				f_time_set = 0;
				switch_state(RUNNING);
		    	break;
		    case RUNNING:
		    	//__delay_cycles(100000); //6.25 milisecond
		    	if (current.second != previous_second){
		    		previous_second = current.second;
					#ifdef DEBUG
						sprintf(debug_str, "Time: %d:%d:%d\n", current.hour, current.minute, current.second);
						print_string(debug_str);
					#endif

					if(P1IN & BIT4){
						show_clock(&current);

					} else {
						fillStrip(0x00, 0x00, 0x00);
					}
		    	}
		    	break;
		    case STOP:
		    	P1IE &= ~(BIT3);
		    	break;
		    case PRINT_TIME:
				print_time();
				switch_state(previous_state);
				break;
		    case SHOW_DATA:
		    	P1IE &= ~(BIT3);
		    	showStrip();
		    	switch_state(STOP);
		    	break;
		    default:
		    	print_string("Non defined case, possible stack corruption\n");
		    	break;
		}
    }

}
