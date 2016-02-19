/*
 * main.c
 *
 *  Created on: 2016 Feb 19 01:42:38
 *  Author: fraggy
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <stdio.h>
#include <stdlib.h>

#include "hd44780.h"
#include "hd44780_xmc4500.h"

void init_lcd(void);
void delay_microseconds(uint16_t us);
uint32_t uint32_time_diff(uint32_t now, uint32_t before);
void hd44780_assert_failure_handler(const char *filename, unsigned long line);

HD44780 lcd;
HD44780_XMC4500_GPIO_Driver lcd_pindriver;

int main(void)
{
	DAVE_STATUS_t status;

	status = DAVE_Init();           /* Initialization of DAVE APPs  */

	if(status == DAVE_STATUS_FAILURE)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U)
		{

		}
	}

	init_lcd();

	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while(1U)
	{
		static uint32_t lcd_update_ms = 0;

		if (uint32_time_diff(SYSTIMER_GetTime(), lcd_update_ms) >= 1000)
		{
			lcd_update_ms = SYSTIMER_GetTime();

			static unsigned counter = 0;

			const size_t buf_size = lcd.columns_amount + 1;
			char buf[buf_size];
			snprintf(buf, buf_size, "%d", counter);

			++counter;

			hd44780_clear(&lcd);
			hd44780_move_cursor(&lcd, 0, 0);
			hd44780_write_string(&lcd, buf);
		}
	}
}


void init_lcd(void)
{
	HD44780_XMC4500_Pinout lcd_pinout =
	{
			{
				/* RS        */  &IO_LCD_RS,
				/* ENABLE    */  &IO_LCD_E,
				/* RW        */  &IO_LCD_RW,
				/* Backlight */  (NULL),
				/* DP0       */  (NULL),
				/* DP1       */  (NULL),
				/* DP2       */  (NULL),
				/* DP3       */  (NULL),
				/* DP4       */  &IO_LCD_D4,
				/* DP5       */  &IO_LCD_D5,
				/* DP6       */  &IO_LCD_D6,
				/* DP7       */  &IO_LCD_D7,
			}
	};

	lcd_pindriver.interface = HD44780_XMC4500_PINDRIVER_INTERFACE;
	lcd_pindriver.pinout = lcd_pinout;
	lcd_pindriver.assert_failure_handler = hd44780_assert_failure_handler;

	const HD44780_Config lcd_config =
	{
			(HD44780_GPIO_Interface*)&lcd_pindriver,
			delay_microseconds,
			hd44780_assert_failure_handler,
			HD44780_OPT_USE_RW
	};

	hd44780_init(&lcd, HD44780_MODE_4BIT, &lcd_config, 5, 2, HD44780_CHARSIZE_5x8);
}


void hd44780_assert_failure_handler(const char *filename, unsigned long line)
{
	(void)filename; (void)line;
	do {} while (1);
}

uint32_t uint32_time_diff(uint32_t now, uint32_t before)
{
	return (now >= before) ? (now - before) : (UINT32_MAX - before + now);
}

void delay_microseconds(uint16_t us)
{
	const uint32_t systick_ms_start = SYSTIMER_GetTime();

	while (1)
	{
		uint32_t diff = uint32_time_diff(SYSTIMER_GetTime(), systick_ms_start);

		if (diff >= ((uint32_t)us / 1000) + (us % 1000 ? 1 : 0))
			break;
	}
}


