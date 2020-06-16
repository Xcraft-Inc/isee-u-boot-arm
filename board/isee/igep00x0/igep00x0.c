/*
 * (C) Copyright 2010
 * ISEE 2007 SL, <www.iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#ifdef CONFIG_LED_STATUS
#include <status_led.h>
#endif
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

#define CONTROL_WKUP_CTRL		0x48002a5c
#define GPIO_IO_PWRDNZ			(1 << 6)
#define PBIASLITEVMODE1			(1 << 8)

DECLARE_GLOBAL_DATA_PTR;


#define IGEP0020_RE_RF 	0x07
#define IGEP0020_RD		0x0B
#define IGEP0020_RC	   	0x01
#define IGEP0020_RB		0x04

#define IGEP0030_RG		0x07
#define IGEP0030_RF		0x0B
#define IGEP0030_RE		0x0D
#define IGEP0030_RD_RC	0x0E
/*
There are 4 ranges of Locally Administered Address Ranges that can be used on a local network:

x2-xx-xx-xx-xx-xx
x6-xx-xx-xx-xx-xx
xA-xx-xx-xx-xx-xx
xE-xx-xx-xx-xx-xx
*/

static unsigned int board_rev = 0;
static int igep_eeprom_valid = 0;

static struct igep_mf_setup igep00x0_eeprom_config = {
	.magic_id = IGEP_MAGIC_ID,
	.crc32 = 0,
	.board_uuid = "00000000-0000-0000-0000-000000000000",
	.bmac0 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	.bmac1 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
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

static int get_mac_address (void)
{
	uchar enetaddr[6];	

	/* Check if the enviroment have ethaddr defined */
	if (!eth_getenv_enetaddr("ethaddr", enetaddr)) {
		memcpy(enetaddr, igep00x0_eeprom_config.bmac0, 6);
	}
	
	if (!is_valid_ethaddr(enetaddr)){
		printf("MAC Address: Error or not set\n");
		return -1;
	}

	return eth_setenv_enetaddr("ethaddr", enetaddr);
}

/*
 * Routine: get_board_revision
 * Description: GPIO_28 and GPIO_129 are used to read board and revision from
 */

static u32 get_board_revision(void)
{
	u32 revision=0;

	gpio_request(GPIO_IGEP00X0_RDET_0, "igep-id-0");
	gpio_request(GPIO_IGEP00X0_RDET_1, "igep-id-1");
	gpio_request(GPIO_IGEP00X0_RDET_2, "igep-id-2");
	gpio_request(GPIO_IGEP00X0_RDET_3, "igep-id-3");

	gpio_direction_input(GPIO_IGEP00X0_RDET_0);
	gpio_direction_input(GPIO_IGEP00X0_RDET_1);
	gpio_direction_input(GPIO_IGEP00X0_RDET_2);
	gpio_direction_input(GPIO_IGEP00X0_RDET_3);


#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)	
	gpio_direction_input(28);
	revision |= (u32) (gpio_get_value(28) ? 1 : 0 ) << 4;	
	// gpio_free(28);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	// gpio_direction_input(16);
	// revision |= (u32) (gpio_get_value(16) ? 1 : 0 ) << 4;	
	// Not Yet Tested
#endif

	revision |= (u32) (gpio_get_value(GPIO_IGEP00X0_RDET_0) ? 1 : 0 ) << 0;
	revision |= (u32) (gpio_get_value(GPIO_IGEP00X0_RDET_1) ? 1 : 0 ) << 1;
	revision |= (u32) (gpio_get_value(GPIO_IGEP00X0_RDET_2) ? 1 : 0 ) << 2;
	revision |= (u32) (gpio_get_value(GPIO_IGEP00X0_RDET_3) ? 1 : 0 ) << 3;

	gpio_free(GPIO_IGEP00X0_RDET_0);
	gpio_free(GPIO_IGEP00X0_RDET_1);
	gpio_free(GPIO_IGEP00X0_RDET_2);
	gpio_free(GPIO_IGEP00X0_RDET_3);

	return revision;
}

static void set_boardserial (void)
{
	char *serial_string = getenv("serial#");
	if(!serial_string){
		setenv("serial#", igep00x0_eeprom_config.board_uuid);
		serial_string = igep00x0_eeprom_config.board_uuid;
	}
	printf("Board uuid: %s\n", serial_string);
}

static int load_eeprom (void)
{
	int result = -1;
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
    igep_eeprom_valid = 0;
	if(check_eeprom() != 0)
		printf("eeprom: not found\n");
	else{
		struct igep_mf_setup eeprom_cfg;
		/* Read configuration from eeprom */
		if(!eeprom_read_setup(0, (char*) &eeprom_cfg, sizeof(struct igep_mf_setup))){
		/* if(!eeprom_read_setup(0, (char*) &igep00x0_eeprom_config, sizeof(struct igep_mf_setup))){ */			
    		u32 crc_save_value = 0;			
			crc_save_value = eeprom_cfg.crc32;
			eeprom_cfg.crc32=0;
			u32 crc_value = crc32(0, (const unsigned char*) &eeprom_cfg, sizeof(struct igep_mf_setup));
			/* Verify crc32 */
			if((crc_save_value == crc_value) && (eeprom_cfg.magic_id == IGEP_MAGIC_ID)){
				memcpy(&igep00x0_eeprom_config, &eeprom_cfg, sizeof(struct igep_mf_setup));				
				igep_eeprom_valid = 1;
				result = 0;
				printf("eeprom: crc32 OK! Loading mac from eeprom\n");
				printf("Board UUID: %s\n", igep00x0_eeprom_config.board_uuid);
			}
			else
				printf("eeprom: crc32 Failed, using defaults\n");	
		}
    }
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
    /* NO Eeprom */
    printf("eeprom: not found\n");
#endif
    return result;
}

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
#if defined(CONFIG_CMD_ONENAND)
	gpmc_cs0_flash = MTD_DEV_TYPE_ONENAND;
#else
	gpmc_cs0_flash = MTD_DEV_TYPE_NAND;
#endif

	gpmc_init();

	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

#if defined(CONFIG_LED_STATUS) && defined(CONFIG_LED_STATUS_BOOT_ENABLE)
	status_led_set(CONFIG_LED_STATUS_BOOT, CONFIG_LED_STATUS_ON);
#endif

	load_eeprom();

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
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	enable_gpmc_cs_config(gpmc_lan_config, &gpmc_cfg->cs[5],
			CONFIG_SMC911X_BASE, GPMC_SIZE_16M);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	enable_gpmc_cs_config(gpmc_lan_config, &gpmc_cfg->cs[4],
			CONFIG_SMC911X_BASE, GPMC_SIZE_16M);
#endif

	/* Enable off mode for NWE in PADCONF_GPMC_NWE register */
	writew(readw(&ctrl_base->gpmc_nwe) | 0x0E00, &ctrl_base->gpmc_nwe);
	/* Enable off mode for NOE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_noe) | 0x0E00, &ctrl_base->gpmc_noe);
	/* Enable off mode for ALE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_nadv_ale) | 0x0E00,
		&ctrl_base->gpmc_nadv_ale);

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	reset_net_chip(64);
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	reset_net_chip(42);
#endif	
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
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	setenv("fdtfile", "omap3-igep0020.dtb");
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	setenv("fdtfile", "omap3-igep0030.dtb");
#endif
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
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	setenv("board_name", "igep0020");
	switch(board_rev){
		case IGEP0020_RC:
			setenv("board_rev", "C");
			puts("Board: IGEP0020-RC\n");
			break;
		case IGEP0020_RE_RF:
			setenv("board_rev", "F");
			puts("Board: IGEP0020-RF\n");
			break;
		default:
			printf("rev: 0x%x\n", board_rev);
			break;
	}	
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	setenv("board_name", "igep0030");
	switch(board_rev){
		case IGEP0030_RG:			
			setenv("board_rev", "G");
			puts("Board: IGEP0030-RG\n");
			break;
		case IGEP0030_RF:
			setenv("board_rev", "F");
			puts("Board: IGEP0030-RF\n");
			break;
		case IGEP0030_RE:
			setenv("board_rev", "E");
			puts("Board: IGEP0030-RE\n");		
			break;
		case IGEP0030_RD_RC:
			setenv("board_rev", "C-D");
			puts("Board: IGEP0030-RC-D\n");
			break;
		default:
			setenv("board_rev", "A-B");
			puts("Board: IGEP0030-RA-B\n");
			break;
	}
#endif	
}

/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{		
	t2_t *t2_base = (t2_t *)T2_BASE;
	u32 pbias_lite;	
	
	twl4030_power_init();

	/* set VSIM to 1.8V */
	twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VSIM_DEDICATED,
				TWL4030_PM_RECEIVER_VSIM_VSEL_18,
				TWL4030_PM_RECEIVER_VSIM_DEV_GRP,
				TWL4030_PM_RECEIVER_DEV_GRP_P1);

	/* set up dual-voltage GPIOs to 1.8V */
	pbias_lite = readl(&t2_base->pbias_lite);
	pbias_lite &= ~PBIASLITEVMODE1;
	pbias_lite |= PBIASLITEPWRDNZ1;
	writel(pbias_lite, &t2_base->pbias_lite);
	if (get_cpu_family() == CPU_OMAP36XX)
		writel(readl(OMAP34XX_CTRL_WKUP_CTRL) |
					 OMAP34XX_CTRL_WKUP_CTRL_GPIO_IO_PWRDNZ,
					 OMAP34XX_CTRL_WKUP_CTRL);		
	
	board_rev = get_board_revision();
	/* printf("board_rev: 0x%x\n", board_rev); */

	/* Enable USB Power*/	
#if 0	
	twl4030_set_ledA(1);
#endif	
	setup_net_chip();
	reset_usb_host_t();
	omap_die_id_display();
#ifdef CONFIG_USB_MUSB_OMAP2PLUS
	musb_register(&musb_plat, &musb_board_data, (void *)MUSB_BASE);
#endif
	set_default_fdt();
	set_boardname();
	set_boardserial();
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
		snprintf(parts, sizeof(parts), "mtdparts=%s:%dk(SPL),1m(uboot),10m(boot),-(rootfs)",
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
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	/* Turn ON USB Transceiver */
	if (!gpio_request(24, "usbh_nrst")) {
		/* First we turn on power */
#if 0		
		twl4030_set_ledA(1);
#endif		
		mdelay(2);
		/* Then we assert reset */
		gpio_direction_output(24, 0);
		mdelay(2);
		/* Finally we deassert reset*/
		gpio_set_value(24, 1);
		mdelay(2);
		gpio_free(24);
	}
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
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
#endif
	return omap_ehci_hcd_init(index, &usbhs_bdata, hccr, hcor);
}

int ehci_hcd_stop(void)
{
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	/* Turn OFF USB Transceiver */
	if (!gpio_request(24, "usbh_nrst")) {
		gpio_direction_output(24, 1);
		mdelay(2);
		gpio_set_value(24, 0);
		mdelay(2);
		gpio_free(24);
		/* Reset is Asserted now we will remove power */
#if 0		
		twl4030_set_ledA(0);
#endif		
		mdelay(2);
	}
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
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
#endif	
	return omap_ehci_hcd_stop();
}
#endif /* CONFIG_USB_EHCI_OMAP */

