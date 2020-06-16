/*
 * Copyright (c) 2020 IATEC
 * Manel Caro <mcaroiatec.biz>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#ifdef CONFIG_LED_STATUS
#include <status_led.h>
#endif
#include <asm/arch/cpu.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>

/* GPIO pins for the LEDs */
#define IGEP_SOPA_LED_USR0	53
#define IGEP_SOPA_LED_USR1	54

#ifndef CONFIG_LED_STATUS
typedef unsigned long led_id_t;
#endif

static int get_led_gpio(led_id_t mask)
{
#ifdef CONFIG_LED_STATUS0
	if (CONFIG_LED_STATUS_BIT == mask){
		return IGEP_SOPA_LED_USR0;
	}
#endif
#ifdef CONFIG_LED_STATUS1
	if (CONFIG_LED_STATUS_BIT1 == mask){
		return IGEP_SOPA_LED_USR1;
	}
#endif
	return -1;
}

#ifdef CONFIG_LED_STATUS_GREEN
void green_led_off(void)
{
	__led_set(IGEP_SOPA_LED_USR0, 0);
}

void green_led_on(void)
{	
	__led_set(IGEP_SOPA_LED_USR0, 1);
}
#endif

#ifdef CONFIG_LED_STATUS_BLUE
void blue_led_off(void)
{
	__led_set(IGEP_SOPA_LED_USR0, 0);
}

void blue_led_on(void)
{	
	__led_set(IGEP_SOPA_LED_USR0, 1);
}
#endif

#ifdef CONFIG_LED_STATUS_RED
void red_led_off(void)
{
	__led_set(IGEP_SOPA_LED_USR1, 0);
}

void red_led_on(void)
{	
	__led_set(IGEP_SOPA_LED_USR1, 1);
}
#endif


#ifdef CONFIG_LED_STATUS_YELLOW
void yellow_led_off(void)
{
	__led_set(IGEP_SOPA_LED_USR0, 0);
	__led_set(IGEP_SOPA_LED_USR1, 0);
}

void yellow_led_on(void)
{	
	__led_set(IGEP_SOPA_LED_USR0, 1);
	__led_set(IGEP_SOPA_LED_USR1, 1);
}
#endif

void __led_init (led_id_t mask, int state)
{
	int toggle_gpio;
	
	toggle_gpio = get_led_gpio(mask);
	if(toggle_gpio){
		if (!gpio_request(toggle_gpio, "led"))
			__led_set(mask, state);
	}
}

void __led_toggle (led_id_t mask)
{
	int state, toggle_gpio;

	toggle_gpio = get_led_gpio(mask);
	if(toggle_gpio){
		state = gpio_get_value(toggle_gpio);
		gpio_direction_output(toggle_gpio, !state);
	}
}

void __led_set (led_id_t mask, int state)
{
	int toggle_gpio;

	toggle_gpio = get_led_gpio(mask);
	if(toggle_gpio){
		gpio_direction_output(toggle_gpio, state);
	}
}