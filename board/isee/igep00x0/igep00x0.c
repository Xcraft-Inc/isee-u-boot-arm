/*
 * (C) Copyright 2010
 * ISEE 2007 SL, <www.iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <status_led.h>
#include <dm.h>
#include <ns16550.h>
#include <twl4030.h>
#include <netdev.h>
#include <spl.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-types.h>
#include <linux/errno.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/onenand.h>
#include <jffs2/load_kernel.h>
#include <mtd_node.h>
#include <fdt_support.h>
#include "igep00x0.h"

#include "../common/eeprom.h"
#include "../common/igep_common.h"

/* usb musb includes (for OTG controller currently as gadget) */
#include <usb.h>
#include <linux/usb/musb.h>
#include <asm/arch/musb.h>
#include <asm/omap_musb.h>

/* usb ehci includes (for Host subsystem) */
#ifdef CONFIG_USB_EHCI
#include <usb.h>
#include <asm/ehci-omap.h>
#endif

DECLARE_GLOBAL_DATA_PTR;


#define IGEP0020_RB	0x04
#define IGEP0020_RC	0x01
#define IGEP0020_RD	0x02
#define IGEP0020_RE_RF 0x00


/* GPIOS */
#define GPIO_TO_PIN(bank, gpio)		(32 * (bank) + (gpio))

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
#define GPIO_USER0_LED_RED  GPIO_TO_PIN(0, 27)
#define GPIO_USER1_LED_GREEN  GPIO_TO_PIN(0, 26)
#define GPIO_USER2_LED_RED  GPIO_TO_PIN(0, 28)
#endif

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
#define GPIO_USER1_LED_GREEN  GPIO_TO_PIN(6, 8)
#define GPIO_USER2_LED_RED  GPIO_TO_PIN(0, 16)
#endif



const uchar IGEP_DEFAULT_MAC_ADDRESS0 [6] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0xff };
static int igep_eeprom_valid = 0;
#define IGEP_MAGIC_ID 	0x78FC110E

static struct igep_mf_setup igep00x0_eeprom_config = {
	.magic_id = IGEP_MAGIC_ID,
	.crc32 = 0xa9f8a9f7,
	.board_uuid = {0x00},
	.board_pid = {0x00},
	.name = {0x00},
	.model = {0x00},
	.pcb_version = {0x00},
	.assembly_rev = {0x00},
	.board_manufacturer = "ISEE 2007 SL (c) 2018",
	.manf_of = {0x00},
	.manf_timestamp = {0x00},
	.bmac0 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	.bmac1 = { 0x02, 0x00, 0x00, 0x00, 0x00, 0xff },
};

static const struct ns16550_platdata igep_serial = {
	.base = OMAP34XX_UART3,
	.reg_shift = 2,
	.clock = V_NS16550_CLK,
	.fcr = UART_FCR_DEFVAL,
};

U_BOOT_DEVICE(igep_uart) = {
	"ns16550_serial",
	&igep_serial
};

#ifdef CONFIG_USB_MUSB_OMAP2PLUS
static struct musb_hdrc_config musb_config = {
	.multipoint     = 1,
	.dyn_fifo       = 1,
	.num_eps        = 16,
	.ram_bits       = 12,
};

static struct omap_musb_board_data musb_board_data = {
	.interface_type	= MUSB_INTERFACE_ULPI,
};

static struct musb_hdrc_platform_data musb_plat = {
#if defined(CONFIG_USB_MUSB_HOST)
	.mode           = MUSB_HOST,
#elif defined(CONFIG_USB_MUSB_GADGET)
	.mode		= MUSB_PERIPHERAL,
#else
#error "Please define either CONFIG_USB_MUSB_HOST or CONFIG_USB_MUSB_GADGET"
#endif
	.config         = &musb_config,
	.power          = 100,
	.platform_ops	= &omap2430_ops,
	.board_data	= &musb_board_data,
};
#endif

/* Basic function to turn on/off a gpio */
static void request_and_set_gpio(int gpio, char *name, int val)
{
	int ret;

	ret = gpio_request(gpio, name);
	if (ret < 0) {
		printf("%s: Unable to request %s\n", __func__, name);
		//puts("Unable to request");
		return;
	}

	ret = gpio_direction_output(gpio, 0);
	if (ret < 0) {
		printf("%s: Unable to set %s  as output\n", __func__, name);
		//puts("Unable to set output");
		goto err_free_gpio;
	}

	gpio_set_value(gpio, val);
	gpio_free(gpio);

	return;

err_free_gpio:
	gpio_free(gpio);
}
/* Make easy use of request_and_set_gpio function*/
#define REQUEST_AND_SET_GPIO(N)	request_and_set_gpio(N, #N, 1);
#define REQUEST_AND_CLR_GPIO(N)	request_and_set_gpio(N, #N, 0);



static int get_mac_address (void)
{
	uchar enetaddr[6];	

	if(igep_eeprom_valid)
		memcpy(enetaddr, igep00x0_eeprom_config.bmac1, 6);	
	else{
		memcpy(enetaddr, IGEP_DEFAULT_MAC_ADDRESS0, 6);	
		memcpy(igep00x0_eeprom_config.bmac0, IGEP_DEFAULT_MAC_ADDRESS0, 6);
	}

	if (!is_valid_ethaddr(enetaddr))
		return -1;	

	return eth_setenv_enetaddr("ethaddr", enetaddr);
}

/*
 * Routine: get_board_revision
 * Description: GPIO_28 and GPIO_129 are used to read board and revision from
 */

static int get_board_revision(void)
{
	int revision=0;
	gpio_request(GPIO_IGEP00X0_REVISION_DETECTION,
				"igep00x0_revision_detection");
	gpio_direction_input(GPIO_IGEP00X0_REVISION_DETECTION);
	revision = gpio_get_value(GPIO_IGEP00X0_REVISION_DETECTION);
	gpio_free(GPIO_IGEP00X0_REVISION_DETECTION);
	return revision;
}

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	u32 crc_value = 0;
    u32 crc_save_value = 0;

#if defined(CONFIG_CMD_ONENAND)
	gpmc_cs0_flash = MTD_DEV_TYPE_ONENAND;
#else
	gpmc_cs0_flash = MTD_DEV_TYPE_NAND;
#endif

	gpmc_init();

	/* find out flash memory type, assume NAND first
	int loops = 100; 
	gpmc_cs0_flash = MTD_DEV_TYPE_NAND;
	gpmc_init();
	*/
	/* Issue a RESET and then READID
	writeb(NAND_CMD_RESET, &gpmc_cfg->cs[0].nand_cmd);
	writeb(NAND_CMD_STATUS, &gpmc_cfg->cs[0].nand_cmd);
	while ((readl(&gpmc_cfg->cs[0].nand_dat) & NAND_STATUS_READY)
	                                        != NAND_STATUS_READY) {
		udelay(1);
		if (--loops == 0) {
			gpmc_cs0_flash = MTD_DEV_TYPE_ONENAND;
			gpmc_init();
			break;
		}
	}
	*/
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

/*
#if defined(CONFIG_LED_STATUS) && defined(CONFIG_LED_STATUS_BOOT_ENABLE)
	status_led_set(CONFIG_LED_STATUS_BOOT, CONFIG_LED_STATUS_ON);
#endif
*/	
	if(check_eeprom() != 0){
		printf("eeprom: not found\n");
		}
	else{
		/* Read configuration from eeprom */
		if(!eeprom_read_setup(0, (char*) &igep00x0_eeprom_config, sizeof(struct igep_mf_setup))){
		crc_save_value = igep00x0_eeprom_config.crc32;
		igep00x0_eeprom_config.crc32=0;
		crc_value = crc32(0, (const unsigned char*) &igep00x0_eeprom_config, sizeof(struct igep_mf_setup));
			/* Verify crc32 */	
			if(crc_save_value == crc_value){
				if(igep00x0_eeprom_config.magic_id == IGEP_MAGIC_ID){
					printf("eeprom: crc32 OK! Loading mac from eeprom\n");                  
					igep_eeprom_valid = 1;
				}
				else
					printf("eeprom: crc32 failed. Loading mac from environment\n");
					igep_eeprom_valid = 0;
			}
			else
                printf("eeprom: crc32 failed. Loading mac from environment\n");
				igep_eeprom_valid = 0;
		}
		else
	  		printf("EEPROM: read %d bytes fail\n", sizeof(struct igep_mf_setup));	
    }
	return 0;
}

#ifdef CONFIG_SPL_BUILD
/*
 * Routine: get_board_mem_timings
 * Description: If we use SPL then there is no x-loader nor config header
 * so we have to setup the DDR timings ourself on both banks.
 */
void get_board_mem_timings(struct board_sdrc_timings *timings)
{
	//unsigned char  data;
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	/* Turn on LED for testing purposes */
	REQUEST_AND_SET_GPIO(GPIO_USER0_LED_RED);
	REQUEST_AND_SET_GPIO(GPIO_USER1_LED_GREEN);
#endif
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	/* Turn ON RED LED for testing purposes */
	gpio_free(GPIO_USER2_LED_RED);
	REQUEST_AND_CLR_GPIO(GPIO_USER2_LED_RED);
#endif

	int mfr, id, err = identify_nand_chip(&mfr, &id);

	timings->mr = MICRON_V_MR_165;
	if (!err) {
		switch (mfr) {
		case NAND_MFR_HYNIX:
			timings->mcfg = HYNIX_V_MCFG_200(256 << 20);
			timings->ctrla = HYNIX_V_ACTIMA_200;
			timings->ctrlb = HYNIX_V_ACTIMB_200;
			break;
		case NAND_MFR_MICRON:
			timings->mcfg = MICRON_V_MCFG_200(256 << 20);
			timings->ctrla = MICRON_V_ACTIMA_200;
			timings->ctrlb = MICRON_V_ACTIMB_200;
			break;
		default:
			/* Should not happen... */
			break;
		}
		timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
		gpmc_cs0_flash = MTD_DEV_TYPE_NAND;
	} else {
		if (get_cpu_family() == CPU_OMAP34XX) {
			timings->mcfg = NUMONYX_V_MCFG_165(256 << 20);
			timings->ctrla = NUMONYX_V_ACTIMA_165;
			timings->ctrlb = NUMONYX_V_ACTIMB_165;
			timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_165MHz;
		} else {
			timings->mcfg = NUMONYX_V_MCFG_200(256 << 20);
			timings->ctrla = NUMONYX_V_ACTIMA_200;
			timings->ctrlb = NUMONYX_V_ACTIMB_200;
			timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
		}
		gpmc_cs0_flash = MTD_DEV_TYPE_ONENAND;
	}
}

#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
	/* break into full u-boot on 'c' */
	if (serial_tstc() && serial_getc() == 'c')
		return 1;

	return 0;
}
#endif
#endif

#ifdef CONFIG_CMD_ONENAND
int onenand_board_init(struct mtd_info *mtd)
{
	if (gpmc_cs0_flash == MTD_DEV_TYPE_ONENAND) {
		struct onenand_chip *this = mtd->priv;
		this->base = (void *)CONFIG_SYS_ONENAND_BASE;
		return 0;
	}
	return 1;
}
#endif

#if defined(CONFIG_CMD_NET)
static void reset_net_chip(int gpio)
{
	if (!gpio_request(gpio, "eth_nrst")) {
		gpio_direction_output(gpio, 1);
		udelay(1);
		gpio_set_value(gpio, 0);
		udelay(40);
		gpio_set_value(gpio, 1);
		mdelay(10);
	}
}

/*
 * Routine: setup_net_chip
 * Description: Setting up the configuration GPMC registers specific to the
 *		Ethernet hardware.
 */
static void setup_net_chip(void)
{
	struct ctrl *ctrl_base = (struct ctrl *)OMAP34XX_CTRL_BASE;
	static const u32 gpmc_lan_config[] = {
		NET_LAN9221_GPMC_CONFIG1,
		NET_LAN9221_GPMC_CONFIG2,
		NET_LAN9221_GPMC_CONFIG3,
		NET_LAN9221_GPMC_CONFIG4,
		NET_LAN9221_GPMC_CONFIG5,
		NET_LAN9221_GPMC_CONFIG6,
	};

	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
	enable_gpmc_cs_config(gpmc_lan_config, &gpmc_cfg->cs[5],
			CONFIG_SMC911X_BASE, GPMC_SIZE_16M);
		break;
	case MACH_TYPE_IGEP0030:
	enable_gpmc_cs_config(gpmc_lan_config, &gpmc_cfg->cs[4],
			CONFIG_SMC911X_BASE, GPMC_SIZE_16M);
		break;
	}

	/* Enable off mode for NWE in PADCONF_GPMC_NWE register */
	writew(readw(&ctrl_base->gpmc_nwe) | 0x0E00, &ctrl_base->gpmc_nwe);
	/* Enable off mode for NOE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_noe) | 0x0E00, &ctrl_base->gpmc_noe);
	/* Enable off mode for ALE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_nadv_ale) | 0x0E00,
		&ctrl_base->gpmc_nadv_ale);

	//reset_net_chip(64);
	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
		reset_net_chip(64);
		break;
	case MACH_TYPE_IGEP0030:
		reset_net_chip(42);
		break;
	}


}

int last_stage_init(){

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	//REQUEST_AND_CLR_GPIO(GPIO_USER1_LED_GREEN);
	/* Net driver turns ON GPIO_USER0_LED_RED without free, free it and clear it */
	gpio_free(GPIO_USER0_LED_RED);
	gpio_free(GPIO_USER1_LED_GREEN);
	/* First bicolor led is already yellow, colour the second one */
	REQUEST_AND_SET_GPIO(GPIO_USER2_LED_RED);

#endif

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	//gpio_free(168);
	//gpio_free(GPIO_USER2_LED_RED);
	//gpio_free(GPIO_USER1_LED_GREEN);
	gpio_free(GPIO_USER2_LED_RED);
	//REQUEST_AND_SET_GPIO(GPIO_USER1_LED_GREEN);
	REQUEST_AND_CLR_GPIO(GPIO_USER2_LED_RED);
#endif
	return 0;
}

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_SMC911X	
	get_mac_address();
	return smc911x_initialize(0, CONFIG_SMC911X_BASE);
#else
	return 0;
#endif
}
#else
static inline void setup_net_chip(void) {}
#endif

#if defined(CONFIG_GENERIC_MMC) && !defined(CONFIG_SPL_BUILD)
int board_mmc_init(bd_t *bis)
{
	return omap_mmc_init(0, 0, 0, -1, -1);
}
#endif

#if defined(CONFIG_GENERIC_MMC)
void board_mmc_power_init(void)
{
	twl4030_power_mmc_init(0);
}
#endif

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
#ifdef CONFIG_FDT_FIXUP_PARTITIONS
	static struct node_info nodes[] = {
		{ "ti,omap2-nand", MTD_DEV_TYPE_NAND, },
#ifdef CONFIG_CMD_ONENAND
		{ "ti,omap2-onenand", MTD_DEV_TYPE_ONENAND, },
#endif

	};

	fdt_fixup_mtdparts(blob, nodes, ARRAY_SIZE(nodes));
#endif
	return 0;
}
#endif

void set_default_fdt(void)
{
	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
		setenv("fdtfile", "omap3-igep0020.dtb");
		break;
	case MACH_TYPE_IGEP0030:
		setenv("fdtfile", "omap3-igep0030.dtb");
		break;
	}
}

void reset_usb_host_t(void){
	/*
	int gusbh_nrst=0;
	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
		gusbh_nrst=24;
		break;
	case MACH_TYPE_IGEP0030:
		gusbh_nrst=54;
		if (!gpio_request(23, "usbh_b0010rb_hub_rst")) {
			gpio_direction_output(23, 1);
			mdelay(2);
			gpio_set_value(23, 0);
			mdelay(2);
		}
		break;
	}
	if (!gpio_request(gusbh_nrst, "usbh_nrst")) {
		gpio_direction_output(gusbh_nrst, 0);
		mdelay(2);
		gpio_set_value(gusbh_nrst, 1);
		mdelay(2);
	}
	*/
}


void set_boardname(void)
{
	int i = get_board_revision();
	switch (i) {
	case IGEP0020_RC:
	setenv("board_rev", "C");
	puts("Board: IGEP0020-RC\n");
		break;
	case IGEP0020_RE_RF:
	setenv("board_rev", "F");
	puts("Board: IGEP0020-RF\n");
		break;
	}

	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
	setenv("board_name", "igep0020");
		break;
	case MACH_TYPE_IGEP0030:
	setenv("board_name", "igep0030");
	setenv("board_rev", "G");
	puts("Board: IGEP0030-RG\n");
		break;
	}
}

/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{
	twl4030_power_init();
	twl4030_led_init(TWL4030_LED_LEDEN_LEDAON | TWL4030_LED_LEDEN_LEDBON);
	setup_net_chip();
	reset_usb_host_t();
	omap_die_id_display();
#ifdef CONFIG_USB_MUSB_OMAP2PLUS
	musb_register(&musb_plat, &musb_board_data, (void *)MUSB_BASE);
#endif
	set_default_fdt();
	set_boardname();
	return 0;
}

void board_mtdparts_default(const char **mtdids, const char **mtdparts)
{
	struct mtd_info *mtd = get_mtd_device(NULL, 0);
	if (mtd) {
		static char ids[24];
		static char parts[78];
		const char *linux_name = "omap2-nand";
		if (strncmp(mtd->name, "onenand0", 8) == 0)
			linux_name = "omap2-onenand";
		snprintf(ids, sizeof(ids), "%s=%s", mtd->name, linux_name);
		snprintf(parts, sizeof(parts), "mtdparts=%s:%dk(SPL),1m(uboot),128k(environment),-(filesystem)",
		         linux_name, 4 * mtd->erasesize >> 10);
		*mtdids = ids;
		*mtdparts = parts;
	}
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

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	MUX_IGEP0020();
#endif

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	MUX_IGEP0030();
#endif
}





#ifdef CONFIG_USB_EHCI_OMAP
static struct omap_usbhs_board_data usbhs_bdata = {
	.port_mode[0] = OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[1] = OMAP_EHCI_PORT_MODE_PHY,
	.port_mode[2] = OMAP_USBHS_PORT_MODE_UNUSED,
};

int ehci_hcd_init(int index, enum usb_init_type init,
		  struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{

	//flush_dcache_range

	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
		/* Turn ON USB Transceiver */
		if (!gpio_request(24, "usbh_nrst")) {
			/* First we turn on power */
			twl4030_led_init(TWL4030_LED_LEDEN_LEDAON | TWL4030_LED_LEDEN_LEDBON);
			mdelay(2);
			/* Then we assert reset */
			gpio_direction_output(24, 0);
			mdelay(2);
			/* Finally we deassert reset*/
			gpio_set_value(24, 1);
			mdelay(2);
			gpio_free(24);
		}
		break;
	case MACH_TYPE_IGEP0030:
		/* Turn ON Base0010 USB HUB */
		if (!gpio_request(23, "usbh_b0010rb_hub_rst")) {
			gpio_direction_output(23, 1);
			mdelay(2);
			gpio_set_value(23, 0);
			mdelay(2);
			gpio_free(23);
		}
		/* Turn ON USB Transceiver */
		if (!gpio_request(54, "usbh_nrst")) {
			gpio_direction_output(54, 0);
			mdelay(2);
			gpio_set_value(54, 1);
			mdelay(2);
			gpio_free(54);
		}
		break;
	}

	return omap_ehci_hcd_init(index, &usbhs_bdata, hccr, hcor);
}

int ehci_hcd_stop(void)
{

	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
		/* Turn OFF USB Transceiver */
		if (!gpio_request(24, "usbh_nrst")) {
			gpio_direction_output(24, 1);
			mdelay(2);
			gpio_set_value(24, 0);
			mdelay(2);
			gpio_free(24);
			/* Reset is Asserted now we will remove power */
			twl4030_led_init(TWL4030_LED_LEDEN_LEDBON);
			mdelay(2);
		}
		break;
	case MACH_TYPE_IGEP0030:
		/* Turn OFF Base0010 USB HUB */
		if (!gpio_request(23, "usbh_b0010rb_hub_rst")) {
			gpio_direction_output(23, 0);
			mdelay(2);
			gpio_set_value(23, 1);
			mdelay(2);
			gpio_free(23);
		}
		/* Turn OFF USB Transceiver */
		if (!gpio_request(54, "usbh_nrst")) {
			gpio_direction_output(54, 1);
			mdelay(2);
			gpio_set_value(54, 0);
			mdelay(2);
			gpio_free(54);
		}
		break;
	}
	return omap_ehci_hcd_stop();
}
#endif /* CONFIG_USB_EHCI_OMAP */

