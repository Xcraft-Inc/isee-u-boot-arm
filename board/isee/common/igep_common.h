/*
 * Copyright (C) 2016 ISEE 2007 SL - http://www.isee.biz
 *
 * Common resources for igep boards
 *
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */


#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__

#define IGEP_MAGIC_ID 	0x78FC110E

struct __attribute__((packed)) igep_mf_setup  {
    u32 magic_id;                   /* eeprom magic id */
    u32 crc32;                      /* eeprom crc32 */
    char board_uuid [37];           /* board identifier */
    uchar bmac0[6];                 /* MAC 0 - default */
    uchar bmac1[6];                 /* MAC 1 */
};

#endif
