#ifndef __CONFIG_IGEP_LED_H
#define __CONFIG_IGEP_LED_H


typedef struct led_definition_t
{
	char led_name[25];	/* led name id */
	int gpio_num;		/* gpio number */
	int led_st;			/* led status */
} led_definition ;

/* Extern definition */
extern led_definition *ldef;

#endif