/*
 * Maintainer: Integration Software and Electronics Engineering, <www.iseebcn.com>
 *
 * Derived from Beagle Board, 3430 SDP, and OMAP3EVM code by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <khasim@ti.com>
 *	Sunil Kumar <sunilsaini05@gmail.com>
 *	Shashi Ranjan <shashiranjanmca05@gmail.com>
 *
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
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
#include "igep0020b.h"

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_IGEP0020;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/*
 * Routine: dss_init
 * Description: Draw into dss buffer.
 */
void dss_init(void)
{
	unsigned int i;
	int offset = 0;

	/* assuming a resolution of 1280x720 - draw into dss buffer */
	/* fill in the blank */

	for(i = 0; i<(720 - (offset/2/1280))*1280*2; i+=2) {
		*((unsigned short *)(0x80500000 + i + offset)) = 0xffff;
	}

	*((uint *) 0x48310034) = 0xfefffedf;
	*((uint *) 0x48310094) = 0x01000120;
	*((uint *) 0x48004D44) = 0x0001b00c;
	*((uint *) 0x48004E40) = 0x00001006;
	*((uint *) 0x48004D00) = 0x00370037;
	*((uint *) 0x48050C00) = 0x00000002;
	*((uint *) 0x48050C04) = 0x0000001B;
	*((uint *) 0x48050C08) = 0x00000040;
	*((uint *) 0x48050C0C) = 0x00000000;
	*((uint *) 0x48050C10) = 0x00000000;
	*((uint *) 0x48050C14) = 0x00008000;
	*((uint *) 0x48050C18) = 0x00000000;
	*((uint *) 0x48050C1C) = 0x00008359;
	*((uint *) 0x48050C20) = 0x0000020C;
	*((uint *) 0x48050C24) = 0x00000000;
	*((uint *) 0x48050C28) = 0x043F2631;
	*((uint *) 0x48050C2C) = 0x00000024;
	*((uint *) 0x48050C30) = 0x00000130;
	*((uint *) 0x48050C34) = 0x00000198;
	*((uint *) 0x48050C38) = 0x000001C0;
	*((uint *) 0x48050C3C) = 0x0000006A;
	*((uint *) 0x48050C40) = 0x0000005C;
	*((uint *) 0x48050C44) = 0x00000000;
	*((uint *) 0x48050C48) = 0x00000001;
	*((uint *) 0x48050C4C) = 0x0000003F;
	*((uint *) 0x48050C50) = 0x21F07C1F;
	*((uint *) 0x48050C54) = 0x00000000;
	*((uint *) 0x48050C58) = 0x00000015;
	*((uint *) 0x48050C5C) = 0x00001400;
	*((uint *) 0x48050C60) = 0x00000000;
	*((uint *) 0x48050C64) = 0x069300F4;
	*((uint *) 0x48050C68) = 0x0016020C;
	*((uint *) 0x48050C6C) = 0x00060107;
	*((uint *) 0x48050C70) = 0x008D034E;
	*((uint *) 0x48050C74) = 0x000F0359;
	*((uint *) 0x48050C78) = 0x01A00000;
	*((uint *) 0x48050C7C) = 0x020501A0;
	*((uint *) 0x48050C80) = 0x01AC0024;
	*((uint *) 0x48050C84) = 0x020D01AC;
	*((uint *) 0x48050C88) = 0x00000006;
	*((uint *) 0x48050C8C) = 0x00000000;
	*((uint *) 0x48050C90) = 0x03480079;
	*((uint *) 0x48050C94) = 0x02040024;
	*((uint *) 0x48050C98) = 0x00000000;
	*((uint *) 0x48050C9C) = 0x00000000;
	*((uint *) 0x48050CA0) = 0x0001008A;
	*((uint *) 0x48050CA4) = 0x01AC0106;
	*((uint *) 0x48050CA8) = 0x01060006;
	*((uint *) 0x48050CAC) = 0x00000000;
	*((uint *) 0x48050CB0) = 0x00140001;
	*((uint *) 0x48050CB4) = 0x00010001;
	*((uint *) 0x48050CB8) = 0x00FF0000;
	*((uint *) 0x48050CBC) = 0x00000000;
	*((uint *) 0x48050CC0) = 0x00000000;
	*((uint *) 0x48050CC4) = 0x0000000D;
	*((uint *) 0x48050CC8) = 0x00000000;
	*((uint *) 0x48050010) = 0x00000001;
	*((uint *) 0x48050040) = 0x00000078;
	*((uint *) 0x48050044) = 0x00000000;
	*((uint *) 0x48050048) = 0x00000000;
	*((uint *) 0x48050050) = 0x00000000;
	*((uint *) 0x48050058) = 0x00000000;
	*((uint *) 0x48050410) = 0x00002015;
	*((uint *) 0x48050414) = 0x00000001;
	*((uint *) 0x48050444) = 0x00000004;
	*((uint *) 0x4805044c) = 0xFFFFFFFF;
	*((uint *) 0x48050450) = 0x00000000;
	*((uint *) 0x48050454) = 0x00000000;
	*((uint *) 0x48050458) = 0x00000000;
	*((uint *) 0x48050464) = 0x0ff03f31;
	*((uint *) 0x48050468) = 0x01400504;
	*((uint *) 0x4805046c) = 0x00007028;
	*((uint *) 0x48050470) = 0x00010002;
	*((uint *) 0x48050478) = 0x00ef027f;
	*((uint *) 0x4805047c) = 0x02cf04ff;
	*((uint *) 0x48050480) = 0x80500000;
	*((uint *) 0x48050484) = 0x80500000;
	*((uint *) 0x48050488) = 0x00000000;
	*((uint *) 0x4805048c) = 0x02cf04ff;
	*((uint *) 0x480504a0) = 0x0000008d;
	*((uint *) 0x480504a4) = 0x03fc03bc;
	*((uint *) 0x480504a8) = 0x00000400;
	*((uint *) 0x480504ac) = 0x00000001;
	*((uint *) 0x480504b0) = 0x00000001;
	*((uint *) 0x480504b4) = 0x00000000;
	*((uint *) 0x480504b8) = 0x807ff000;
	udelay(1000);
	*((uint *) 0x48050440) = 0x0001836b;
	udelay(1000);
	*((uint *) 0x48050440) = 0x0001836b;
	udelay(1000);
	*((uint *) 0x48050440) = 0x0001836b;
	udelay(1000);
}

/*
 * Routine: get_prod_id
 * Description: Get id info from chips
 */
#define PRODUCT_ID_SKUID	0x4830A20C
#define CPU_35XX_PID_MASK	0x0000000F
#define CPU_35XX_600MHZ_DEV 0x0
#define CPU_35XX_720MHZ_DEV 0x8
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
	twl4030_led_init();

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

	dss_init();

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
	MUX_IGEP0020();
}

/*
 * Routine: setup_net_chip
 * Description: Setting up the configuration GPMC registers specific to the
 *		Ethernet hardware.
 */
static void setup_net_chip(void)
{
	struct gpio *gpio3_base = (struct gpio *)OMAP34XX_GPIO3_BASE;
	struct ctrl *ctrl_base = (struct ctrl *)OMAP34XX_CTRL_BASE;

	/* Configure GPMC registers */
	writel(NET_GPMC_CONFIG1, &gpmc_cfg->cs[5].config1);
	writel(NET_GPMC_CONFIG2, &gpmc_cfg->cs[5].config2);
	writel(NET_GPMC_CONFIG3, &gpmc_cfg->cs[5].config3);
	writel(NET_GPMC_CONFIG4, &gpmc_cfg->cs[5].config4);
	writel(NET_GPMC_CONFIG5, &gpmc_cfg->cs[5].config5);
	writel(NET_GPMC_CONFIG6, &gpmc_cfg->cs[5].config6);
	writel(NET_GPMC_CONFIG7, &gpmc_cfg->cs[5].config7);

	/* Enable off mode for NWE in PADCONF_GPMC_NWE register */
	writew(readw(&ctrl_base ->gpmc_nwe) | 0x0E00, &ctrl_base->gpmc_nwe);
	/* Enable off mode for NOE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_noe) | 0x0E00, &ctrl_base->gpmc_noe);
	/* Enable off mode for ALE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_nadv_ale) | 0x0E00,
		&ctrl_base->gpmc_nadv_ale);

	/* Make GPIO 64 as output pin */
	writel(readl(&gpio3_base->oe) & ~(GPIO0), &gpio3_base->oe);

	/* Now send a pulse on the GPIO pin */
	writel(GPIO0, &gpio3_base->setdataout);
	udelay(1);
	writel(GPIO0, &gpio3_base->cleardataout);
	udelay(1);
	writel(GPIO0, &gpio3_base->setdataout);
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_SMC911X_BASE);
#endif
	return rc;
}

