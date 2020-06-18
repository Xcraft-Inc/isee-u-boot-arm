/*
 * Board functions for SOPA0000 AM335x SBC
 *
 * Copyright (C) 2016, ISEE 2007 SL - http://www.isee.biz/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <spl.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <miiphy.h>
#include <cpsw.h>
#include "../common/eeprom.h"
#include "../common/igep_common.h"
#include <power/tps65910.h>
#include "board.h"
#include "led.h"

DECLARE_GLOBAL_DATA_PTR;
static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;

static struct igep_mf_setup igep0034_eeprom_config;

#ifdef CONFIG_SPL_BUILD
#define SDRAM_K4B2G1646EBIH9 /*SOPA0000 use 256MB RAM K4B2G1646EBIH9*/
#ifdef SDRAM_H5TQ4G63AFR
/* PN H5TQ4G63AFR is equivalent to MT41K256M16HA125*/
static const struct ddr_data ddr3_data = {
	.datardsratio0 = MT41K256M16HA125E_RD_DQS,
	.datawdsratio0 = MT41K256M16HA125E_WR_DQS,
	.datafwsratio0 = MT41K256M16HA125E_PHY_FIFO_WE,
	.datawrsratio0 = MT41K256M16HA125E_PHY_WR_DATA,
};

static const struct cmd_control ddr3_cmd_ctrl_data = {
	.cmd0csratio = MT41K256M16HA125E_RATIO,
	.cmd0iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

	.cmd1csratio = MT41K256M16HA125E_RATIO,
	.cmd1iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

	.cmd2csratio = MT41K256M16HA125E_RATIO,
	.cmd2iclkout = MT41K256M16HA125E_INVERT_CLKOUT,
};

static struct emif_regs ddr3_emif_reg_data = {
	.sdram_config = MT41K256M16HA125E_EMIF_SDCFG,
	.ref_ctrl = MT41K256M16HA125E_EMIF_SDREF,
	.sdram_tim1 = MT41K256M16HA125E_EMIF_TIM1,
	.sdram_tim2 = MT41K256M16HA125E_EMIF_TIM2,
	.sdram_tim3 = MT41K256M16HA125E_EMIF_TIM3,
	.zq_config = MT41K256M16HA125E_ZQ_CFG,
	.emif_ddr_phy_ctlr_1 = MT41K256M16HA125E_EMIF_READ_LATENCY,
};

const struct ctrl_ioregs ioregs = {
	.cm0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.cm1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.cm2ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.dt0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.dt1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
};
#elif defined(SDRAM_K4B2G1646EBIH9)
static const struct ddr_data ddr3_data = {
	.datardsratio0 = K4B2G1646EBIH9_RD_DQS,
	.datawdsratio0 = K4B2G1646EBIH9_WR_DQS,
	.datafwsratio0 = K4B2G1646EBIH9_PHY_FIFO_WE,
	.datawrsratio0 = K4B2G1646EBIH9_PHY_WR_DATA,
};

static const struct cmd_control ddr3_cmd_ctrl_data = {
	.cmd0csratio = K4B2G1646EBIH9_RATIO,
	.cmd0iclkout = K4B2G1646EBIH9_INVERT_CLKOUT,

	.cmd1csratio = K4B2G1646EBIH9_RATIO,
	.cmd1iclkout = K4B2G1646EBIH9_INVERT_CLKOUT,

	.cmd2csratio = K4B2G1646EBIH9_RATIO,
	.cmd2iclkout = K4B2G1646EBIH9_INVERT_CLKOUT,
};

static struct emif_regs ddr3_emif_reg_data = {
	.sdram_config = K4B2G1646EBIH9_EMIF_SDCFG,
	.ref_ctrl = K4B2G1646EBIH9_EMIF_SDREF,
	.sdram_tim1 = K4B2G1646EBIH9_EMIF_TIM1,
	.sdram_tim2 = K4B2G1646EBIH9_EMIF_TIM2,
	.sdram_tim3 = K4B2G1646EBIH9_EMIF_TIM3,
	.zq_config = K4B2G1646EBIH9_ZQ_CFG,
	.emif_ddr_phy_ctlr_1 = K4B2G1646EBIH9_EMIF_READ_LATENCY,
};

const struct ctrl_ioregs ioregs = {
	.cm0ioctl		= K4B2G1646EBIH9_IOCTRL_VALUE,
	.cm1ioctl		= K4B2G1646EBIH9_IOCTRL_VALUE,
	.cm2ioctl		= K4B2G1646EBIH9_IOCTRL_VALUE,
	.dt0ioctl		= K4B2G1646EBIH9_IOCTRL_VALUE,
	.dt1ioctl		= K4B2G1646EBIH9_IOCTRL_VALUE,
};
#endif

#define OSC    (V_OSCK/1000000)
const struct dpll_params dpll_ddr = {
		400, OSC-1, 1, -1, -1, -1, -1};

const struct dpll_params *get_dpll_ddr_params(void)
{
	return &dpll_ddr;
}

void set_uart_mux_conf(void)
{
	enable_uart0_pin_mux();
}

void set_mux_conf_regs(void)
{
	enable_board_pin_mux();
}

void sdram_init(void)
{
	config_ddr(400, &ioregs, &ddr3_data,
		   &ddr3_cmd_ctrl_data, &ddr3_emif_reg_data, 0);
}
#endif
#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	return 0;
}
#endif

led_definition myldef[] = {
	
	{"green", 53, CONFIG_LED_STATUS_BIT},
	{"yellow", 54, CONFIG_LED_STATUS_BIT1},
	{ 0, 0, -1 }
};

led_definition *ldef = (led_definition*) &myldef;

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{
	u32 crc_value = 0;
    u32 crc_save_value = 0;

	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	gpmc_init();

	/* Select bus I2C1 and check if it works properly */
	i2c_set_bus_num(1);
	i2c_probe(TPS65910_CTRL_I2C_ADDR);	
		
	/* Check if eeprom is in the bus */	
	if(check_eeprom() != 0)
	{
		printf("eeprom: not found.\n");
	}
	else
	{
        /* Read configuration from eeprom */
        if(eeprom_read_setup(0, (char*) &igep0034_eeprom_config, sizeof(struct igep_mf_setup)))
        {
            printf("eeprom: read fail.\n");
    	}       
        else
        {     
	       	/* Verify crc32 */
	        crc_save_value = igep0034_eeprom_config.crc32;
	        igep0034_eeprom_config.crc32 = 0;
	        crc_value = crc32(0, (const unsigned char*) &igep0034_eeprom_config, sizeof(struct igep_mf_setup));
	        if(crc_save_value != crc_value)
	        {
	            printf("eeprom: crc32 failed.\n");
	        }
	        else
	        {
                printf("eeprom: crc32 OK.\n");
                printf("Board UUID: %s\n", igep0034_eeprom_config.board_uuid);
        	}
	    }
    }

	return 0;
}

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_USBETH_SUPPORT) /*If OTG USB is used to boot*/

int board_eth_init(bd_t *bis)
{
	int ret = 0;
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;

	/* If OTG Ethernet Gadget is activated generate and assign a MAC*/
	if (!eth_getenv_enetaddr("usbnet_devaddr", mac_addr)) {
		/* try reading mac address from efuse */
		mac_lo = readl(&cdev->macid0l);
		mac_hi = readl(&cdev->macid0h);
		mac_addr[0] = mac_hi & 0xFF;
		mac_addr[1] = (mac_hi & 0xFF00) >> 8;
		mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
		mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
		mac_addr[4] = mac_lo & 0xFF;
		mac_addr[5] = (mac_lo & 0xFF00) >> 8;
		/*Assign the mac to the "usbnet_devaddr" variable*/
		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("usbnet_devaddr", mac_addr);
	}
	/*Everithing is ready, usb ethernet gadget can be initialized*/
	usb_eth_initialize(bis);
	return ret;
}

#elif defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_ETH_SUPPORT)

static void cpsw_control(int enabled)
{
	/* VTP can be added here */

	return;
}

static struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs	= 0x208,
		.sliver_reg_ofs	= 0xd80,
		.phy_addr	= 1,
		.phy_if		= PHY_INTERFACE_MODE_RMII,
	},
};

static struct cpsw_platform_data cpsw_data = {
	.mdio_base			= CPSW_MDIO_BASE,
	.cpsw_base			= CPSW_BASE,
	.mdio_div			= 0xff,
	.channels			= 8,
	.cpdma_reg_ofs		= 0x800,
	.slaves				= 1,
	.slave_data			= cpsw_slaves,
	.ale_reg_ofs		= 0xd00,
	.ale_entries		= 1024,
	.host_port_reg_ofs	= 0x108,
	.hw_stats_reg_ofs	= 0x900,
	.bd_ram_ofs			= 0x2000,
	.mac_control		= (1 << 5),
	.control			= cpsw_control,
	.host_port_num		= 0,
	.version			= CPSW_CTRL_VERSION_2,
};


int board_eth_init(bd_t *bis)
{
	int rv, ret = 0;
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;

	if (!eth_getenv_enetaddr("ethaddr", mac_addr)) {
		/* try reading mac address from efuse */
		mac_lo = readl(&cdev->macid0l);
		mac_hi = readl(&cdev->macid0h);
		mac_addr[0] = mac_hi & 0xFF;
		mac_addr[1] = (mac_hi & 0xFF00) >> 8;
		mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
		mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
		mac_addr[4] = mac_lo & 0xFF;
		mac_addr[5] = (mac_lo & 0xFF00) >> 8;
		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("ethaddr", mac_addr);
	}
	writel((GMII1_SEL_RMII | RMII1_IO_CLK_EN),
	       &cdev->miisel);

	rv = cpsw_register(&cpsw_data);
	if (rv < 0)
		printf("Error %d registering CPSW switch\n", rv);
	else
		ret += rv;

	#ifdef CONFIG_SPL_USBETH_SUPPORT
		/* If OTG Ethernet Gadget is activated generate and assign a MAC*/
		if (!eth_getenv_enetaddr("usbnet_devaddr", mac_addr)) {
			/* try reading mac address from efuse */
			mac_lo = readl(&cdev->macid0l);
			mac_hi = readl(&cdev->macid0h);
			mac_addr[0] = mac_hi & 0xFF;
			mac_addr[1] = (mac_hi & 0xFF00) >> 8;
			mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
			mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
			mac_addr[4] = mac_lo & 0xFF;
			mac_addr[5] = (mac_lo & 0xFF00) >> 8;
			/*Assign the mac to the "usbnet_devaddr" variable*/
			if (is_valid_ethaddr(mac_addr))
				eth_setenv_enetaddr("usbnet_devaddr", mac_addr);
		}
		/*Everithing is ready, usb ethernet gadget can be initialized*/
		usb_eth_initialize(bis);
	#endif

	return ret;
}

#elif !defined(CONFIG_SPL_BUILD) && defined(CONFIG_DRIVER_TI_CPSW)

static void cpsw_control(int enabled)
{
	/* VTP can be added here */

	return;
}

static struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs	= 0x208,
		.sliver_reg_ofs	= 0xd80,
		.phy_addr	= 1,
		.phy_if		= PHY_INTERFACE_MODE_RMII,
	},
};

static struct cpsw_platform_data cpsw_data = {
	.mdio_base			= CPSW_MDIO_BASE,
	.cpsw_base			= CPSW_BASE,
	.mdio_div			= 0xff,
	.channels			= 8,
	.cpdma_reg_ofs		= 0x800,
	.slaves				= 1,
	.slave_data			= cpsw_slaves,
	.ale_reg_ofs		= 0xd00,
	.ale_entries		= 1024,
	.host_port_reg_ofs	= 0x108,
	.hw_stats_reg_ofs	= 0x900,
	.bd_ram_ofs			= 0x2000,
	.mac_control		= (1 << 5),
	.control			= cpsw_control,
	.host_port_num		= 0,
	.version			= CPSW_CTRL_VERSION_2,
};


int board_eth_init(bd_t *bis)
{
	int rv, ret = 0;
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;

	if (!eth_getenv_enetaddr("ethaddr", mac_addr)) {
		/* try reading mac address from efuse */
		mac_lo = readl(&cdev->macid0l);
		mac_hi = readl(&cdev->macid0h);
		mac_addr[0] = mac_hi & 0xFF;
		mac_addr[1] = (mac_hi & 0xFF00) >> 8;
		mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
		mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
		mac_addr[4] = mac_lo & 0xFF;
		mac_addr[5] = (mac_lo & 0xFF00) >> 8;
		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("ethaddr", mac_addr);
	}
	writel((GMII1_SEL_RMII | RMII1_IO_CLK_EN),
	       &cdev->miisel);

	rv = cpsw_register(&cpsw_data);
	if (rv < 0)
		printf("Error %d registering CPSW switch\n", rv);
	else
		ret += rv;

	#ifdef CONFIG_SPL_USBETH_SUPPORT
		/* If OTG Ethernet Gadget is activated generate and assign a MAC*/
		if (!eth_getenv_enetaddr("usbnet_devaddr", mac_addr)) {
			/* try reading mac address from efuse */
			mac_lo = readl(&cdev->macid0l);
			mac_hi = readl(&cdev->macid0h);
			mac_addr[0] = mac_hi & 0xFF;
			mac_addr[1] = (mac_hi & 0xFF00) >> 8;
			mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
			mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
			mac_addr[4] = mac_lo & 0xFF;
			mac_addr[5] = (mac_lo & 0xFF00) >> 8;
			/*Assign the mac to the "usbnet_devaddr" variable*/
			if (is_valid_ethaddr(mac_addr))
				eth_setenv_enetaddr("usbnet_devaddr", mac_addr);
		}
		/*Everithing is ready, usb ethernet gadget can be initialized*/
		usb_eth_initialize(bis);
	#endif

	return ret;
}
#endif
