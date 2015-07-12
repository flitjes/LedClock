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

//#define DEBUG

CLOCK_STATE current_state = INIT;

/*
 *  ======== main ========
 */
//u_int numLEDs = 60;
int main(void)
{
    Grace_init();                   // Activate Grace-generated configuration
    struct time_t debug_time = { 2, 30, 00};
    struct time_t parsed;

    // >>>>> Fill-in user code here <<<<<
    for(;;){
		switch(current_state){
		    case INIT:
		    	print_state("INIT\n");
		    	// initialize LED strip
				initStrip();

				set_time(&debug_time);
				// set strip color red
				fillStrip(0xFF, 0x00, 0x00);

				// show the strip
				showStrip();

				/* Clear MCx bits to stop timer */
				TA0CTL &= ~(MC1 + MC0);
				print_string("LedClock started\n");
				current_state = STOP;
				print_state("STOP\n");
		    	break;
		    case SET_TIME:
		    	print_state("SET_TIME\n");
				parse_time(time_formated+1, &parsed);
				set_time(&parsed);
				f_time_set = 0;
				current_state = STOP;
				print_state("STOP\n");
		    	break;
		    case RUNNING:
		    	//__delay_cycles(100000); //6.25 milisecond
		    	break;
		    case START:
		    	/* Start timer in up mode */
		    	TA0CTL |= MC_1;
		    	current_state = RUNNING;
		    	print_state("RUNNING\n");
		    	break;
		    case STOP:
		    	/* Clear MCx bits to stop timer */
		    	TA0CTL &= ~(MC1 + MC0);
		    	break;
		}
    }



    return (0);
}
char time_str[25];
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR_HOOK(void)
{
	tick();
#ifdef DEBUG
	sprintf(time_str, "Time: %d:%d:%d\n", current.hour, current.minute, current.second);
	print_string(time_str);
#endif
	show_clock(&current);
}


