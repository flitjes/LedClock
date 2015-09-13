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
#include "TI_USCI_I2C_master.h"

uint8_t allignment_offset = 30;
/*
 *  ======== main ========
 */

int main(void)
{
	//During Grace init P1.6 and P1.7 are configured as I2C for the RTC

    Grace_init();                   // Activate Grace-generated configuration
    struct time_t parsed;
    struct time_t rtc = { 0, 0, 0 };

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

				// show the strip
				showStrip();

				/* Clear MCx bits to stop timer */
				TA0CTL &= ~(MC1 + MC0);
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
		    	break;
		    case STOP:
		    	P1IE &= ~(BIT3);
		    	fillStrip(0x00, 0x00, 0x00);
		    	showStrip();
		    	break;
		    case PRINT_TIME:
				print_time();
				switch_state(previous_state);
				break;
		    default:
		    	print_string("Non defined case, possible stack corruption\n");
		    	break;
		}
    }

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void){
	if(IFG2 & UCA0RXIFG){
		USCI0RXSerialInterruptHandler();
	}

	if(IFG2 & UCB0RXIFG){
		USCI0RXI2CInterruptHandler();
	}
}


#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){
	USCI0TXI2CInterruptHandler();
}

static uint8_t moving_average[10];
static uint8_t moving_average_i = 0;

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR_HOOK(void)
{
	if(P1IFG & BIT3){
		uint16_t LDR_value = 0;
		P1IFG &= ~(BIT3);
		tick();

		if(P1IN & BIT4){
			//ADC start conversion - software trigger
			ADC10CTL0 |= ADC10SC;

			// Loop until ADC10IFG is set indicating ADC conversion complete
			while ((ADC10CTL0 & ADC10IFG) == 0);

			LDR_value = ADC10MEM;
			/* 1024 == complete darkness
			 * 0 == light overload
			 * Converting the ADC value straight to a percentage 0 - 100
			 */
			LDR_value = LDR_value / 10 + 30;

			if(moving_average_i >= 10){
				moving_average_i = 0;
			}

			if(LDR_value > 100){
				moving_average[moving_average_i] = 100;
			} else {
				moving_average[moving_average_i] = LDR_value;
			}

			brightness = (moving_average[0] + moving_average[1] + moving_average[2] + moving_average[3] + moving_average[4] + moving_average[5] + moving_average[6] + moving_average[7] + moving_average[8] + moving_average[9]) / 10;
			moving_average_i++;

		#ifdef DEBUG
			sprintf(debug_str, "LDR: %d brightness %d\n", LDR_value, brightness);
			print_string(debug_str);
			sprintf(debug_str, "Time: %d:%d:%d\n", current.hour, current.minute, current.second);
			print_string(debug_str);
		#endif
			show_clock(&current);
		} else {
			switch_state(STOP);
		}

	}
}
