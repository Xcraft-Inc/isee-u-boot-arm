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
#include "led.h"

static int get_led_gpio (led_id_t mask)
{
	int i = 0;
	while(ldef[i].led_name[0] != 0){
		if(mask == ldef[i].led_st){
			return ldef[i].gpio_num;
		}
		i++;
	}
	return 0;
}

int gpio_name_to_num(char* name)
{
	int i = 0;
	while(ldef[i].led_name[0] != 0){
		if(!strcmp(name, ldef[i].led_name)){
			return ldef[i].gpio_num;
		}
		i++;
	}
	return 0;
}

#ifdef CONFIG_LED_STATUS_GREEN
void green_led_off(void)
{
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_OFF);
}

void green_led_on(void)
{	
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_ON);
}
#endif

#ifdef CONFIG_LED_STATUS_BLUE
void blue_led_off(void)
{
	__led_set(gpio_name_to_num("blue"), CONFIG_LED_STATUS_OFF);
}

void blue_led_on(void)
{	
	__led_set(gpio_name_to_num("blue"), CONFIG_LED_STATUS_ON);
}
#endif

#ifdef CONFIG_LED_STATUS_RED
void red_led_off(void)
{
	__led_set(gpio_name_to_num("red"), CONFIG_LED_STATUS_OFF);
}

void red_led_on(void)
{	
	__led_set(gpio_name_to_num("red"), CONFIG_LED_STATUS_ON);
#endif


#ifdef CONFIG_LED_STATUS_YELLOW
void yellow_led_off(void)
{
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_OFF);
	__led_set(gpio_name_to_num("yellow"), CONFIG_LED_STATUS_OFF);
}

void yellow_led_on(void)
{	
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_ON);
	__led_set(gpio_name_to_num("yellow"), CONFIG_LED_STATUS_ON);
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