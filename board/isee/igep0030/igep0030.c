/*
 * Copyright (C) 2010, ISEE 2007 SL
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <twl4030.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-types.h>
#include "igep0030.h"

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_IGEP0030;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/*
 * Routine: get_prod_id
 * Description: Get id info from chips
 */

#define PRODUCT_ID_SKUID	0x4830A20C
#define CPU_35XX_PID_MASK	0x0000000F
#define CPU_35XX_600MHZ_DEV	0x0
#define CPU_35XX_720MHZ_DEV	0x8

static u32 get_prod_id(void)
{
	u32 p;

	/* get production ID */
	p = __raw_readl(PRODUCT_ID_SKUID);

	return (p & CPU_35XX_PID_MASK);
}

/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{
	twl4030_power_init();
/*	twl4030_led_init(); */

	/* Select TWL4030 VSEL to support 720Mhz */
	if (get_prod_id() == CPU_35XX_720MHZ_DEV) {
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
				VAUX2_VSEL_18,
				TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
				DEV_GRP_P1);

		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VDD1_VSEL,
				VDD1_VSEL_14,
				TWL4030_PM_RECEIVER_VDD1_DEV_GRP,
				DEV_GRP_P1);
		prcm_config_720mhz();
	}

	dieid_num_r();

	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_DEFAULT();
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;

	return rc;
}

