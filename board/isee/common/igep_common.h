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

struct __attribute__((packed)) igep_mf_setup  {
    u32 magic_id;            /* eeprom magic id */
    u32 crc32;                /* eeprom crc32 */
    char board_uuid [37];        /* board identifier */
    char board_pid [20];        /* product identifier */
    char name [20];            /* board name */
    char model [10];            /* board model */
    char pcb_version [10];        /* board version */
    char assembly_rev[10];        /* board revision */
    char board_manufacturer[30];    /* board manufacturer */
    char manf_of[10];            /* manufacturer order of fabrication */
    char manf_timestamp[16];        /* manufacturer timestamp */
    uchar bmac0[17];            /* MAC 0 - default */
    uchar bmac1[17];            /* MAC 1 */
};



#endif
