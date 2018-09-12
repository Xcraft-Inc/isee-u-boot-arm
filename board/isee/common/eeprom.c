/*
 * Copyright (C) 2016 ISEE 2007 SL - http://www.isee.biz
 *
 * EEPROM support source file for igep0034
 *
 * Author: 
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/arch/sys_proto.h>


int eeprom_write_setup (uint8_t s_addr, const char* data, u32 size)
{
    u32 i;
    u32 remain = size % 32;
    u32 blocks = size / 32;
    for (i=0; i < blocks; i++){
        if(i2c_write(CONFIG_SYS_I2C_EEPROM_ADDR, s_addr + (i*32), 2, (uint8_t*) data + (i*32), 32)){
            return -1;
        }
        udelay(5000);
    }
    if(remain > 0){
        if(i2c_write(CONFIG_SYS_I2C_EEPROM_ADDR, s_addr + (i*32), 2, (uint8_t*) data + (i*32), remain))
            return -1;
        else
            udelay(5000);
    }
    return 0;
}

int eeprom_read_setup (uint8_t s_addr, char* data, u32 size)
{
    u32 i;
    u32 remain = size % 32;
    u32 blocks = size / 32;
    for (i=0; i < blocks; i++){
        if(i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR, s_addr + (i*32), 2, (uint8_t*) data + (i*32), 32)){
            return -1;
        }
    }
    if(remain > 0)
        if(i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR, s_addr + (i*32), 2, (uint8_t*) data + (i*32), remain))
            return -1;
    return 0;
}

int check_eeprom (void)
{
	i2c_set_bus_num(CONFIG_SYS_I2C_EEPROM_BUS);
	/* Check if baseboard eeprom is available */
	if (i2c_probe(CONFIG_SYS_I2C_EEPROM_ADDR)) {
		debug("Could not probe the EEPROM at 0x%x\n",
		       CONFIG_SYS_I2C_EEPROM_ADDR);
		return -1;
	}
	return 0;
}
