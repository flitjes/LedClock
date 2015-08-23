/*
 * DS1307.h
 *
 *  Created on: Aug 3, 2015
 *      Author: flitjesdev
 */

#ifndef DS1307_H_
#define DS1307_H_

#include "time.h"
#include <msp430.h>

#define DS1307_ADDR 0x68
#define DS1307_ADDR_READ (DS1307_ADDR << 1)
#define DS1307_ADDR_WRITE ((DS1307_ADDR << 1) | 0x01)

#define DS1307_ADDR_SECONDS 	0x00
#define DS1307_ADDR_MINTUES		0x01
#define DS1307_ADDR_HOURS		0x02
#define DS1307_ADDR_DAYS		0x03
#define DS1307_ADDR_DATE		0x04
#define DS1307_ADDR_MONTH		0x05
#define DS1307_ADDR_YEAR		0x06
#define DS1307_ADDR_CONTROL		0x07

#define DS1307_MASK_CH			(BIT7)  					//Powerfailure
#define DS1307_MASK_SEC_10		(BIT6 | BIT5 | BIT4) 		//10's of seconds
#define DS1307_MASK_SEC			(BIT3 | BIT2 | BIT1 | BIT0) //Seconds
#define DS1307_MASK_MIN_10		(BIT6 | BIT5 | BIT4)  		//10's of minutes
#define DS1307_MASK_MIN			(BIT3 | BIT2 | BIT1 | BIT0) //Minutes
#define DS1307_MASK_HOUR		(BIT3 | BIT2 | BIT1 | BIT0)	//Hours
#define DS1307_MASK_HOUR_12_24	(BIT6)						//12 or 24 hour format
#define DS1307_MASK_HOUR_AM_PM	(BIT5)						//AM or PM
#define DS1307_MASK_HOUR_10		(BIT4)						//10's of hours
#define DS1307_MASK_DAY			(BIT2 | BIT1 | BIT0)		//Day
#define DS1307_MASK_DATE		(BIT3 | BIT2 | BIT1 | BIT0)	//Date
#define DS1307_MASK_DATE_10		(BIT5 | BIT4)				//10's of days
#define DS1307_MASK_MONT		(BIT3 | BIT2 | BIT1 | BIT0)	//Year
#define DS1307_MASK_MONT_10		(BIT4)						//10's of year
#define DS1307_MASK_YEAR		(BIT3 | BIT2 | BIT1 | BIT0)	//Year
#define DS1307_MASK_YEAR_10		(BIT7 | BIT6 | BIT5 | BIT4)	//10's of year

#define DS1307_CTRL_RS0			(BIT0)
#define DS1307_CTRL_RS1			(BIT1)
#define DS1307_CTRL_SQWE		(BIT4)
#define DS1307_CTRL_OUT			(BIT7)

extern void ds1307_set_time(struct time_t* t);
extern void ds1307_get_time(struct time_t* t);
extern void ds1307_set_squarewave(uint8_t wave);

#endif /* DS1307_H_ */
