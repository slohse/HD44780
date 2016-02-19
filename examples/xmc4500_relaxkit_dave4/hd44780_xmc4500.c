/*
 * hd44780_xmc4500.c
 *
 *  Created on: Feb 7, 2016
 *      Author: fraggy
 */

#include "hd44780_xmc4500.h"

#ifndef NDEBUG
#define HD44780_XMC4500_ASSERT(x) \
		{ \
	if (!(x)) \
	{ \
		HD44780_XMC4500_GPIO_Driver *driver = ((HD44780_XMC4500_GPIO_Driver*)interface); \
		if (driver->assert_failure_handler != NULL) \
		driver->assert_failure_handler(__FILE__, __LINE__); \
	} \
		}

#define HD44780_XMC4500_RETURN_ASSERT(x,ret) \
		do { \
			int condition = (x); \
			HD44780_XMC4500_ASSERT(condition) \
			if (!condition) \
			return (ret); \
		} while (0)
#else
#define HD44780_XMC4500_ASSERT(x)
#define HD44780_XMC4500_RETURN_ASSERT(x,ret)
#endif


static HD44780_Result xmc4500_default_pin_configure(
		HD44780_GPIO_Interface *interface, HD44780_Pin pin, HD44780_PinMode mode)
{
	HD44780_XMC4500_RETURN_ASSERT(interface != NULL, HD44780_RESULT_ERROR);

	HD44780_XMC4500_GPIO_Driver *driver = (HD44780_XMC4500_GPIO_Driver*)interface;
	DIGITAL_IO_t const *hw_pin = driver->pinout.pins[pin];

	HD44780_XMC4500_RETURN_ASSERT(hw_pin != NULL, HD44780_RESULT_ERROR);

	// both input an output can be done in open drain mode, so the only thing that needs to be done is
	// set the pin high in case of input mode
	if(mode == HD44780_PINMODE_INPUT)
	{
		DIGITAL_IO_SetOutputHigh(hw_pin);
	}

	return HD44780_RESULT_OK;
}

static HD44780_Result xmc4500_default_pin_write(
		HD44780_GPIO_Interface *interface, HD44780_Pin pin, HD44780_PinState value)
{
	HD44780_XMC4500_RETURN_ASSERT(interface != NULL, HD44780_RESULT_ERROR);

	HD44780_XMC4500_GPIO_Driver *driver = (HD44780_XMC4500_GPIO_Driver*)interface;
	DIGITAL_IO_t const *hw_pin = driver->pinout.pins[pin];

	HD44780_XMC4500_RETURN_ASSERT(hw_pin != NULL, HD44780_RESULT_ERROR);

	if(value)
	{
		DIGITAL_IO_SetOutputHigh(hw_pin);
	}
	else
	{
		DIGITAL_IO_SetOutputLow(hw_pin);
	}

	return HD44780_RESULT_OK;
}

static HD44780_Result xmc4500_default_pin_read(
		HD44780_GPIO_Interface *interface, HD44780_Pin pin, HD44780_PinState *value)
{
	HD44780_XMC4500_RETURN_ASSERT(interface != NULL, HD44780_RESULT_ERROR);
	HD44780_XMC4500_RETURN_ASSERT(value != NULL, HD44780_RESULT_ERROR);

	HD44780_XMC4500_GPIO_Driver *driver = (HD44780_XMC4500_GPIO_Driver*)interface;
	DIGITAL_IO_t const *hw_pin = driver->pinout.pins[pin];

	HD44780_XMC4500_RETURN_ASSERT(hw_pin != NULL, HD44780_RESULT_ERROR);

	uint32_t out_bit = DIGITAL_IO_GetInput(hw_pin);

	*value = (out_bit ? HD44780_PINSTATE_HIGH : HD44780_PINSTATE_LOW);

	return HD44780_RESULT_OK;
}

const HD44780_GPIO_Interface HD44780_XMC4500_PINDRIVER_INTERFACE =
{
		xmc4500_default_pin_configure,
		xmc4500_default_pin_write,
		xmc4500_default_pin_read
};

