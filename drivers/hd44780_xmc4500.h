/*
 * hd44780_xmc4500.h
 *
 *  Created on: Feb 7, 2016
 *      Author: fraggy
 */

#ifndef SRC_HD44780_HD44780_XMC4500_H_
#define SRC_HD44780_HD44780_XMC4500_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Dave.h>
#include "hd44780.h"

typedef struct
{
	DIGITAL_IO_t const * pins[HD44780_PINS_AMOUNT];
} HD44780_XMC4500_Pinout;

typedef struct
{
	HD44780_GPIO_Interface interface;
	HD44780_XMC4500_Pinout pinout;
	HD44780_AssertFn assert_failure_handler;
} HD44780_XMC4500_GPIO_Driver;


extern const HD44780_GPIO_Interface HD44780_XMC4500_PINDRIVER_INTERFACE;


#ifdef __cplusplus
}
#endif

#endif /* SRC_HD44780_HD44780_XMC4500_H_ */
