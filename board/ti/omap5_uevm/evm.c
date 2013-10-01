/*
 * (C) Copyright 2010
 * Texas Instruments Incorporated, <www.ti.com>
 * Aneesh V       <aneesh@ti.com>
 * Steve Sakoman  <steve@sakoman.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <palmas.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mmc_host_def.h>
#include <tca642x.h>

#include "mux_data.h"

#ifdef CONFIG_USB_EHCI
#include <usb.h>
#include <asm/gpio.h>
#include <asm/arch/clock.h>
#include <asm/arch/ehci.h>
#include <asm/ehci-omap.h>

#define DIE_ID_REG_BASE     (OMAP54XX_L4_CORE_BASE + 0x2000)
#define DIE_ID_REG_OFFSET	0x200

#endif

DECLARE_GLOBAL_DATA_PTR;

const struct omap_sysinfo sysinfo = {
	"Board: OMAP5432 uEVM\n"
};


#define __tca6416__
/**
 * @brief tca642x_init - uEVM default values for the GPIO expander
 * input reg, output reg, polarity reg, configuration reg
 */
#ifndef __tca6416__ 
struct tca642x_bank_info tca642x_init[] = {
	{ .input_reg = 0x00,
	  .output_reg = 0x04,
	  .polarity_reg = 0x00,
	  .configuration_reg = 0x80 },
	{ .input_reg = 0x00,
	  .output_reg = 0x00,
	  .polarity_reg = 0x00,
	  .configuration_reg = 0xff },
	{ .input_reg = 0x00,
	  .output_reg = 0x00,
	  .polarity_reg = 0x00,
	  .configuration_reg = 0x40 },
};
#else
struct tca642x_bank_info tca642x_init[] = {
	{ .input_reg = 0x00,
	  .output_reg = 0xFF,
	  .polarity_reg = 0x00,
	  .configuration_reg = 0x00 },
	{ .input_reg = 0x00,
	  .output_reg = 0xFF,
	  .polarity_reg = 0x00,
	  .configuration_reg = 0x00 },
};
#endif

/**
 * @brief board_init
 *
 * @return 0
 */
int board_init(void)
{
	gpmc_init();
	gd->bd->bi_arch_number = MACH_TYPE_OMAP5_SEVM;
	gd->bd->bi_boot_params = (0x80000000 + 0x100); /* boot param addr */

	tca642x_set_inital_state(CONFIG_SYS_I2C_TCA642X_ADDR, tca642x_init);

	return 0;
}

int board_eth_init(bd_t *bis)
{
	return 0;
}

/**
 * @brief misc_init_r - Configure EVM board specific configurations
 * such as power configurations, ethernet initialization as phase2 of
 * boot sequence
 *
 * @return 0
 */
int misc_init_r(void)
{
#ifdef CONFIG_PALMAS_POWER
	palmas_init_settings();
#endif
	return 0;
}

void set_muxconf_regs_essential(void)
{
	do_set_mux((*ctrl)->control_padconf_core_base,
		   core_padconf_array_essential,
		   sizeof(core_padconf_array_essential) /
		   sizeof(struct pad_conf_entry));

	do_set_mux((*ctrl)->control_padconf_wkup_base,
		   wkup_padconf_array_essential,
		   sizeof(wkup_padconf_array_essential) /
		   sizeof(struct pad_conf_entry));
}

void set_muxconf_regs_non_essential(void)
{
	do_set_mux((*ctrl)->control_padconf_core_base,
		   core_padconf_array_non_essential,
		   sizeof(core_padconf_array_non_essential) /
		   sizeof(struct pad_conf_entry));

	do_set_mux((*ctrl)->control_padconf_wkup_base,
		   wkup_padconf_array_non_essential,
		   sizeof(wkup_padconf_array_non_essential) /
		   sizeof(struct pad_conf_entry));
}

#if !defined(CONFIG_SPL_BUILD) && defined(CONFIG_GENERIC_MMC)
int board_mmc_init(bd_t *bis)
{
	omap_mmc_init(0, 0, 0, -1, -1);
	omap_mmc_init(1, 0, 0, -1, -1);
	return 0;
}
#endif

#ifdef CONFIG_USB_EHCI
static struct omap_usbhs_board_data usbhs_bdata = {
	.port_mode[0] = OMAP_USBHS_PORT_MODE_UNUSED,
	.port_mode[1] = OMAP_EHCI_PORT_MODE_HSIC,
	.port_mode[2] = OMAP_EHCI_PORT_MODE_HSIC,
};

static void enable_host_clocks(void)
{
	int hs_clk_ctrl_val = (OPTFCLKEN_HSIC60M_P3_CLK |
				OPTFCLKEN_HSIC480M_P3_CLK |
				OPTFCLKEN_HSIC60M_P2_CLK |
				OPTFCLKEN_HSIC480M_P2_CLK |
				OPTFCLKEN_UTMI_P3_CLK | OPTFCLKEN_UTMI_P2_CLK);

	/* Enable port 2 and 3 clocks*/
	setbits_le32((*prcm)->cm_l3init_hsusbhost_clkctrl, hs_clk_ctrl_val);

	/* Enable port 2 and 3 usb host ports tll clocks*/
	setbits_le32((*prcm)->cm_l3init_hsusbtll_clkctrl,
			(OPTFCLKEN_USB_CH1_CLK_ENABLE | OPTFCLKEN_USB_CH2_CLK_ENABLE));
}

/*
 * Modify masked bits in register
 */
static int usb03503_breg_write(unsigned int bus, uchar chip, uint8_t addr,
		uint8_t reg_bit, uint8_t data)
{
	uint8_t valw;
	int org_bus_num;
	int ret;

	org_bus_num = i2c_get_bus_num();
	i2c_set_bus_num(bus);

	if (i2c_read(chip, addr, 1, (uint8_t *)&valw, 1)) {
		printf("Could not read before writing\n");
		ret = -1;
		goto error;
	}
	valw &= ~reg_bit;
	valw |= data;

	ret = i2c_write(chip, addr, 1, (u8 *)&valw, 1);

error:
	i2c_set_bus_num(org_bus_num);
	return ret;
}

static int usb03503_reg_write(unsigned int bus, uchar chip, uint8_t addr,
		uint8_t reg_bit, uint8_t data)
{
	uint8_t valw;
	int org_bus_num;
	int ret;

	org_bus_num = i2c_get_bus_num();
	i2c_set_bus_num(bus);
	ret = i2c_write(chip, addr, 1, (u8 *)&data, 1);

error:
	i2c_set_bus_num(org_bus_num);
	return ret;
}

static int usb3503_reg_read(unsigned int bus, uchar chip, uint8_t addr, uint8_t *data)
{
	uint8_t valw;
	int org_bus_num;
	int ret = 0;

	org_bus_num = i2c_get_bus_num();
	i2c_set_bus_num(bus);
	if (i2c_read(chip, addr, 1, (u8 *)&valw, 1)) {
		ret = -1;
		goto error;
	}
	*data = valw;
error:
	i2c_set_bus_num(org_bus_num);
	return ret;
}

static int usb3503_set_register (unsigned int bus, uchar chip, uint8_t addr, uint8_t data, int ms_timeout)
{
	int ret, tTimeout;
	do {
		u32 buf;		
		uint8_t val = 0;		
		ret = usb03503_reg_write(bus, chip, addr, 0, data);
		if(ret != 0)
			break;
		udelay(1 * 1000);
		tTimeout++;
	} while (tTimeout < ms_timeout);
	return ret;
}

//#ifdef __notdef
static void usb3503_set_battery_charger (int on)
{	
	if(on){
		usb3503_set_register(CONFIG_SYS_I2C_USB0_3503_BUS_NUM, CONFIG_SYS_I2C_USB0_3503_ADDR, 0xD0, (0x07 << 1), 10);
		usb3503_set_register(CONFIG_SYS_I2C_USB1_3503_BUS_NUM, CONFIG_SYS_I2C_USB1_3503_ADDR, 0xD0, (0x07 << 1), 10);		
	}	
	else{
		usb3503_set_register(CONFIG_SYS_I2C_USB0_3503_BUS_NUM, CONFIG_SYS_I2C_USB0_3503_ADDR, 0xD0, (~0x07 << 1), 10);
		usb3503_set_register(CONFIG_SYS_I2C_USB1_3503_BUS_NUM, CONFIG_SYS_I2C_USB1_3503_ADDR, 0xD0, (~0x07 << 1), 10);		
	}
}
// #endif


int ehci_hcd_init(int index, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	int ret;
	int auxclk;
	int reg;
	u8 val = 0;
	int timeout = 0;
	uint8_t device_mac[6];

	enable_host_clocks();
	
	// printf("ehci_hcd_init (%d)\n", index);
	
	if (!getenv("usbethaddr")) {
		reg = DIE_ID_REG_BASE + DIE_ID_REG_OFFSET;

		/*
		 * create a fake MAC address from the processor ID code.
		 * first byte is 0x02 to signify locally administered.
		 */
		device_mac[0] = 0x02;
		device_mac[1] = readl(reg + 0x10) & 0xff;
		device_mac[2] = readl(reg + 0xC) & 0xff;
		device_mac[3] = readl(reg + 0x8) & 0xff;
		device_mac[4] = readl(reg) & 0xff;
		device_mac[5] = (readl(reg) >> 8) & 0xff;

		eth_setenv_enetaddr("usbethaddr", device_mac);
	}

	auxclk = readl((*prcm)->scrm_auxclk1);
	/* Request auxilary clock */
	auxclk |= AUXCLK_ENABLE_MASK;
	writel(auxclk, (*prcm)->scrm_auxclk1);

	/* Enable LAN */ 
	gpio_direction_output(CONFIG_OMAP_USBLAN_ENABLE_GPIO, 1);

	ret = omap_ehci_hcd_init(&usbhs_bdata, hccr, hcor);
	if (ret < 0) {
		puts("Failed to initialize ehci\n");
		return ret;
	}
	udelay(1 * 1000);
	// Enable battery Charging -> SET PRTPWR Enabled on USB3503 hubs	
	usb3503_set_battery_charger(1);
#ifdef __notdef
	usb3503_reg_read(CONFIG_SYS_I2C_USB0_3503_BUS_NUM, CONFIG_SYS_I2C_USB0_3503_ADDR, 0xE6, &val);
	printf(">>>>>> 0xE6h OCS Val : 0x%x\n", val);
	usb3503_reg_read(CONFIG_SYS_I2C_USB0_3503_BUS_NUM, CONFIG_SYS_I2C_USB0_3503_ADDR, 0xD0, &val);
	printf(">>>>>> 0xD0h Battery Charging Val : 0x%x\n", val);
	usb3503_reg_read(CONFIG_SYS_I2C_USB1_3503_BUS_NUM, CONFIG_SYS_I2C_USB1_3503_ADDR, 0xE6, &val);
	printf(">>>>>> 0xE6h OCS Val : 0x%x\n", val);
	usb3503_reg_read(CONFIG_SYS_I2C_USB1_3503_BUS_NUM, CONFIG_SYS_I2C_USB1_3503_ADDR, 0xD0, &val);
	printf(">>>>>> 0xD0h Battery Charging Val : 0x%x\n", val);
#endif	
	return 0;
}

int ehci_hcd_stop(void)
{
	int ret;

	ret = omap_ehci_hcd_stop();
	return ret;
}

void usb_hub_reset_devices(int port)
{		
	/* The LAN9730 needs to be reset after the port power has been set. */
#ifdef __notdef	
	if (port == 3) {
		gpio_direction_output(CONFIG_OMAP_EHCI_PHY3_RESET_GPIO, 0);
		udelay(10);
		gpio_direction_output(CONFIG_OMAP_EHCI_PHY3_RESET_GPIO, 1);
	}
	if(port == 2) {
		gpio_direction_output(CONFIG_OMAP_EHCI_PHY2_RESET_GPIO, 0);
		udelay(10);
		gpio_direction_output(CONFIG_OMAP_EHCI_PHY2_RESET_GPIO, 1);		
	}
#endif	
}
#endif
