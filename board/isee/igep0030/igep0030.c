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
#include <asm/arch/gpio.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-types.h>
#include "igep0030.h"

/* OMAP35x GPMC definitions for LAN9221 chips on Tobi expansion boards */
static const u32 gpmc_35x_lan_config[] = {
    NET_GPMC_CONFIG1,
    NET_GPMC_CONFIG2,
    NET_GPMC_CONFIG3,
    NET_GPMC_CONFIG4,
    NET_GPMC_CONFIG5,
    NET_GPMC_CONFIG6,
    /*CONFIG7- computed as params */
};

/* DM37x GPMC definitions for LAN9221 chips on Tobi expansion boards */
static const u32 gpmc_37x_lan_config[] = {
    NET_37X_LAN9221_GPMC_CONFIG1,
    NET_37X_LAN9221_GPMC_CONFIG2,
    NET_37X_LAN9221_GPMC_CONFIG3,
    NET_37X_LAN9221_GPMC_CONFIG4,
    NET_37X_LAN9221_GPMC_CONFIG5,
    NET_37X_LAN9221_GPMC_CONFIG6,
    /*CONFIG7- computed as params */
};

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
 * Routine: setup_net_chip
 * Description: Setting up the configuration GPMC registers specific to the
 *		Ethernet hardware.
 */
static void setup_net_chip()
{
	struct ctrl *ctrl_base = (struct ctrl *)OMAP34XX_CTRL_BASE;

	/* Configure GPMC registers */
	if (get_cpu_family() == CPU_OMAP36XX) {
		enable_gpmc_cs_config(gpmc_37x_lan_config, &gpmc_cfg->cs[5],
			0x2C000000, GPMC_SIZE_16M);

		enable_gpmc_cs_config(gpmc_37x_lan_config, &gpmc_cfg->cs[4],
			0x2B000000, GPMC_SIZE_16M);
	} else {
		enable_gpmc_cs_config(gpmc_35x_lan_config, &gpmc_cfg->cs[5],
			0x2C000000, GPMC_SIZE_16M);

		enable_gpmc_cs_config(gpmc_35x_lan_config, &gpmc_cfg->cs[4],
			0x2B000000, GPMC_SIZE_16M);

	}

	/* Enable off mode for NWE in PADCONF_GPMC_NWE register */
	writew(readw(&ctrl_base ->gpmc_nwe) | 0x0E00, &ctrl_base->gpmc_nwe);
	/* Enable off mode for NOE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_noe) | 0x0E00, &ctrl_base->gpmc_noe);
	/* Enable off mode for ALE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_nadv_ale) | 0x0E00,
		&ctrl_base->gpmc_nadv_ale);

	/* Make GPIO as output pin and send a magic pulse through it */
	if (!omap_request_gpio(42)) {
		omap_set_gpio_direction(42, 0);
		omap_set_gpio_dataout(42, 1);
		udelay(1);
		omap_set_gpio_dataout(42, 0);
		udelay(1);
		omap_set_gpio_dataout(42, 1);
	}
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
	/* set VDD1 to 1.35 for DM37x */
	if (get_cpu_family() == CPU_OMAP36XX)
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VDD1_VSEL,
				VDD1_VSEL_135,
				TWL4030_PM_RECEIVER_VDD1_DEV_GRP, DEV_GRP_P1);

	twl4030_power_init();

	/* Select TWL4030 VSEL to support 720Mhz */
	if (get_prod_id() == CPU_35XX_720MHZ_DEV) {
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VDD1_VSEL,
				VDD1_VSEL_135,
				TWL4030_PM_RECEIVER_VDD1_DEV_GRP, DEV_GRP_P1);
		prcm_config_720mhz();
	}

#if defined(CONFIG_CMD_NET)
	setup_net_chip();
#endif

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
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, 0x2B000000);
	rc = smc911x_initialize(1, 0x2C000000);
#endif
	return rc;
}

