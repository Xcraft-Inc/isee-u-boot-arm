/*
 * Copyright (c) 2018 ISEE 2007 SL
 * Manel Caro <mcaro@iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <status_led.h>
#include <asm/arch/cpu.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <twl4030.h>

#include "led.h"

/* Return gpio number from Name */
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


#ifdef CONFIG_LED_STATUS_RED
void red_led_off(void)
{
#if (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)
	__led_set(gpio_name_to_num("red"), CONFIG_LED_STATUS_OFF);
#endif
}

void red_led_on(void)
{	
#if (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)	
	__led_set(gpio_name_to_num("red"), CONFIG_LED_STATUS_ON);
#endif	
#endif


#ifdef CONFIG_LED_STATUS_YELLOW
void yellow_led_off(void)
{
#if (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)	
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_OFF);
	__led_set(gpio_name_to_num("yellow"), CONFIG_LED_STATUS_OFF);
#endif	
}

void yellow_led_on(void)
{	
#if (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)	
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_ON);
	__led_set(gpio_name_to_num("yellow"), CONFIG_LED_STATUS_ON);
#endif	
}
#endif

/* GPIO pins for the LEDs */
#define IGEP0030_LED_USR0	16
#define IGEP0030_LED_USR1	168

/* TWL pins for leds  */
#define IGEP0030_LED_USR2	400
#define IGEP0030_LED_USR3	401

/* GPIO pins for the LEDs */
#define IGEP0020_LED_USR0	26
#define IGEP0020_LED_USR1	27
#define IGEP0020_LED_USR2	28
/* TWL pins for leds  */
#define IGEP0020_LED_USR3	400


#ifdef CONFIG_LED_STATUS_GREEN
void green_led_off(void)
{
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	__led_set(IGEP0030_LED_USR1, 0);
	__led_set(IGEP0030_LED_USR3, 0);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	__led_set(IGEP0020_LED_USR0, 0);
	__led_set(IGEP0020_LED_USR3, 0);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_OFF);
#endif	
}

void green_led_on(void)
{	
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	__led_set(IGEP0030_LED_USR1, 1);
	__led_set(IGEP0030_LED_USR3, 1);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	__led_set(IGEP0020_LED_USR0, 1);
	__led_set(IGEP0020_LED_USR3, 1);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)
	__led_set(gpio_name_to_num("green"), CONFIG_LED_STATUS_ON);	
#endif	
}
#endif

#ifdef CONFIG_LED_STATUS_BLUE
void blue_led_off(void)
{
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	__led_set(IGEP0030_LED_USR0, 1);
	__led_set(IGEP0030_LED_USR2, 0);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	__led_set(IGEP0020_LED_USR1, 0);
	__led_set(IGEP0020_LED_USR2, 0);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)
	__led_set(gpio_name_to_num("blue"), CONFIG_LED_STATUS_OFF);
#endif	
}

void blue_led_on(void)
{	
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	__led_set(IGEP0030_LED_USR0, 0);
	__led_set(IGEP0030_LED_USR2, 1);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	__led_set(IGEP0020_LED_USR1, 1);
	__led_set(IGEP0020_LED_USR2, 1);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)
	__led_set(gpio_name_to_num("blue"), CONFIG_LED_STATUS_ON);
#endif	
}
#endif


int IsLed_in_TWL (int ledNo)
{
	switch(ledNo){
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
		case IGEP0030_LED_USR2:
		case IGEP0030_LED_USR3:
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
		case IGEP0020_LED_USR3:		
#endif		
			return 1;
	}
	return 0;
}

u8 twl4030_get_leds (void)
{
	u8 val = 0;
	twl4030_i2c_read_u8(TWL4030_CHIP_LED, TWL4030_LED_LEDEN,
			&val);
	return val;
}

void twl4030_set_ledA (int on_off)
{
	u8 val = twl4030_get_leds();
	if(on_off){	/* on lead A */
		val |= (TWL4030_LED_LEDEN_LEDAON | TWL4030_LED_LEDEN_LEDAPWM);
	}
	else{	/* off led A*/
		val &= ~(TWL4030_LED_LEDEN_LEDAON | TWL4030_LED_LEDEN_LEDAPWM);
	}
	twl4030_i2c_write_u8(TWL4030_CHIP_LED, TWL4030_LED_LEDEN,
			     val);	
}

void twl4030_set_ledB (int on_off)
{
	u8 val = twl4030_get_leds();
	if(on_off){	/* on lead A */
		val |= (TWL4030_LED_LEDEN_LEDBON | TWL4030_LED_LEDEN_LEDBPWM);
	}
	else{	/* off led A*/
		val &= ~(TWL4030_LED_LEDEN_LEDBON | TWL4030_LED_LEDEN_LEDBPWM);
	}
	twl4030_i2c_write_u8(TWL4030_CHIP_LED, TWL4030_LED_LEDEN,
			     val);	
}

void twl4030_set_led (int led, int on_off)
{
	switch(led){
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)		
		case IGEP0030_LED_USR2:			
			twl4030_set_ledA(on_off);
			break;
		case IGEP0030_LED_USR3:
			twl4030_set_ledB(on_off);
			break;
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
		case IGEP0020_LED_USR3:
			twl4030_set_ledB(on_off);
			break;
#endif			
	}
}

int twl4030_get_led(int led)
{
	u8 value = twl4030_get_leds();
	switch(led){
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
		case IGEP0030_LED_USR2:
			value = value & TWL4030_LED_LEDEN_LEDAON;
			break;
		case IGEP0030_LED_USR3:
			value = value & TWL4030_LED_LEDEN_LEDBON;
			break;
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
		case IGEP0020_LED_USR3:
			value = value & TWL4030_LED_LEDEN_LEDBON;
			break;
#endif			
		default:
			value = 0;
			break;
	}
	return value;
}

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
static int get_led_gpio(led_id_t mask)
{
#ifdef CONFIG_LED_STATUS0
	if (CONFIG_LED_STATUS_BIT == mask){
		return IGEP0030_LED_USR0;
	}
#endif
#ifdef CONFIG_LED_STATUS1
	if (CONFIG_LED_STATUS_BIT1 == mask){
		return IGEP0030_LED_USR1;
	}
#endif
#ifdef CONFIG_LED_STATUS2	
	if (CONFIG_LED_STATUS_BIT2 == mask){
		return IGEP0030_LED_USR2;
	}
#endif
#ifdef CONFIG_LED_STATUS3
	if (CONFIG_LED_STATUS_BIT3 == mask){
		return IGEP0030_LED_USR3;
	}
#endif
	return 0;
}
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
static int get_led_gpio(led_id_t mask)
{	
#ifdef CONFIG_LED_STATUS0
	if (CONFIG_LED_STATUS_BIT == mask){
		return IGEP0020_LED_USR0;
	}
#endif
#ifdef CONFIG_LED_STATUS1
	if (CONFIG_LED_STATUS_BIT1 == mask){
		return IGEP0020_LED_USR1;
	}
#endif
#ifdef CONFIG_LED_STATUS2	
	if (CONFIG_LED_STATUS_BIT2 == mask){
		return IGEP0020_LED_USR2;
	}
#endif
#ifdef CONFIG_LED_STATUS3
	if (CONFIG_LED_STATUS_BIT3 == mask){
		return IGEP0020_LED_USR3;
	}
#endif
	return 0;
}
#elif (CONFIG_MACH_TYPE == MACH_TYPE_SOPA0000)

/* Convert Status from gpio mas */
static int get_led_gpio(led_id_t mask)
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

#endif

void __led_init (led_id_t mask, int state)
{
	int toggle_gpio;
	
	toggle_gpio = get_led_gpio(mask);
	if(toggle_gpio){
		if(IsLed_in_TWL(toggle_gpio)){
			__led_set(mask, state);
		}
		else{
			if (!gpio_request(toggle_gpio, "led"))
				__led_set(mask, state);
		}
	}
}

void __led_toggle (led_id_t mask)
{
	int state, toggle_gpio;

	toggle_gpio = get_led_gpio(mask);
	if(toggle_gpio){
		if(IsLed_in_TWL(toggle_gpio)){
			state = twl4030_get_led(toggle_gpio);
			twl4030_set_led(toggle_gpio, !state);
		}
		else{
			state = gpio_get_value(toggle_gpio);
			gpio_direction_output(toggle_gpio, !state);
		}
	}
}

void __led_set (led_id_t mask, int state)
{
	int toggle_gpio;

	toggle_gpio = get_led_gpio(mask);
	if(toggle_gpio){
		if(IsLed_in_TWL(toggle_gpio))
			twl4030_set_led(toggle_gpio, state);
		else
			gpio_direction_output(toggle_gpio, state);
	}
}
