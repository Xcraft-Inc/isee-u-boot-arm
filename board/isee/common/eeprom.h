/*
 * Copyright (C) 2016 ISEE 2007 SL - http://www.isee.biz
 *
 * 0034 EEPROM Definitions
 *
 * Author: 
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __EEPROM_BOARD_HELPER__
#define __EEPROM_BOARD_HELPER__

int eeprom_write_setup (uint8_t s_addr, const char* data, u32 size);
int eeprom_read_setup (uint8_t s_addr, char* data, u32 size);
int check_eeprom (void);
#endif
