#ifndef __CONFIG_IGEP_LED_H
#define __CONFIG_IGEP_LED_H


typedef struct led_definition_t
{
	char led_name[10];
	int gpio_num;
	int led_st;
} led_definition ;


extern led_definition *ldef;

#endif