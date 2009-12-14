/*
 * Configuration settings for the IGEP v2.x series rev. B board.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H
#include <asm/sizes.h>

/* ----------------------------------------------------------------------------
 * High Level Configuration Options
 * ----------------------------------------------------------------------------
 */
#define CONFIG_ARMCORTEXA8	1	/* This is an ARM V7 CPU core */
#define CONFIG_OMAP		1	/* in a TI OMAP core */
#define CONFIG_OMAP34XX		1	/* which is a 34XX */
#define CONFIG_OMAP3430		1	/* which is in a 3430 */
#define CONFIG_OMAP3_IGEP0020	1	/* working with IGEP0020 */

#include <asm/arch/cpu.h>		/* get chip and board defs */
#include <asm/arch/omap3.h>

/* ----------------------------------------------------------------------------
 * Display CPU and Board information
 * ----------------------------------------------------------------------------
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

/* Clock Defines */
#define V_OSCK			26000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

#undef CONFIG_USE_IRQ				/* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

/* ----------------------------------------------------------------------------
 * NS16550 Configuration
 * ----------------------------------------------------------------------------
 */

#define V_NS16550_CLK			48000000	/* 48MHz (APLL96/2) */

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		V_NS16550_CLK

/* select serial console configuration */
#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		OMAP34XX_UART3
#define CONFIG_SERIAL3			3

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,115200}
#define CONFIG_MMC			1
#define CONFIG_OMAP3_MMC		1
#define CONFIG_DOS_PARTITION		1

/* commands to include */
#include <config_cmd_default.h>

#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_MMC		/* MMC support			*/
#define CONFIG_CMD_ONENAND	/* ONENAND support		*/
#define CONFIG_CMD_NET		/* bootp, tftpboot, rarpboot	*/
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_CMD_NFS		/* NFS support			*/

#undef CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
#undef CONFIG_CMD_IMI		/* iminfo			*/
#undef CONFIG_CMD_IMLS		/* List all found images	*/
#undef CONFIG_CMD_JFFS2		/* JFFS2 Support		*/
#undef CONFIG_CMD_NAND	  	/* NAND support			*/

#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_SYS_I2C_BUS		0
#define CONFIG_SYS_I2C_BUS_SELECT	1
#define CONFIG_DRIVER_OMAP34XX_I2C	1

/* ----------------------------------------------------------------------------
 * TWL4030
 * ----------------------------------------------------------------------------
 */
#define CONFIG_TWL4030_POWER		1
#define CONFIG_TWL4030_LED		1

/* Environment information */
#define CONFIG_BOOTDELAY		3
#define CONFIG_EXTRA_ENV_SETTINGS	"\0"
#define CONFIG_BOOTCOMMAND		"mmc init 0 ; fatload mmc 0 0x80000000 setup.ini ; source \0"

#define CONFIG_AUTO_COMPLETE		1

/* ----------------------------------------------------------------------------
 * Miscellaneous configurable options
 * ----------------------------------------------------------------------------
 */
#define V_PROMPT			"U-Boot # "

#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		V_PROMPT
#define CONFIG_SYS_CBSIZE		256	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)

#define CONFIG_SYS_MEMTEST_START	(OMAP34XX_SDRC_CS0)	/* memtest */
								/* works on */
#define CONFIG_SYS_MEMTEST_END		(OMAP34XX_SDRC_CS0 + \
					0x01F00000) /* 31MB */

#define CONFIG_SYS_LOAD_ADDR		(OMAP34XX_SDRC_CS0)	/* default */
							/* load address */

/*
 * OMAP3 has 12 GP timers, they can be driven by the system clock
 * (12/13/16.8/19.2/38.4MHz) or by 32KHz clock. We use 13MHz (V_SCLK).
 * This rate is divided by a local divisor.
 */
#define CONFIG_SYS_TIMERBASE		(OMAP34XX_GPT2)
#define CONFIG_SYS_PTV			2       /* Divisor: 2^(PTV+1) => 8 */
#define CONFIG_SYS_HZ			1000

/*-----------------------------------------------------------------------------
 * Stack sizes
 * ----------------------------------------------------------------------------
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	SZ_128K	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	SZ_4K	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	SZ_4K	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------------
 * Physical Memory Map
 * ----------------------------------------------------------------------------
 */
#define CONFIG_NR_DRAM_BANKS	2	/* CS1 may or may not be populated */
#define PHYS_SDRAM_1		OMAP34XX_SDRC_CS0
#define PHYS_SDRAM_1_SIZE	SZ_32M	/* at least 32 meg */
#define PHYS_SDRAM_2		OMAP34XX_SDRC_CS1

/* SDRAM Bank Allocation method */
#define SDRC_R_B_C		1

/* ----------------------------------------------------------------------------
 * FLASH and environment organization
 * ----------------------------------------------------------------------------
 */

#define PISMO1_ONEN_SIZE		GPMC_SIZE_128M /* Configure the PISMO */

#define CONFIG_SYS_FLASH_BASE		boot_flash_base

#define CONFIG_SYS_ONENAND_BASE		ONENAND_MAP

#define CONFIG_ENV_IS_IN_ONENAND	1

#define CONFIG_ENV_SIZE			SZ_512K	/* Total Size Environment */

#define CONFIG_SYS_ENV_SECT_SIZE	boot_flash_sec
#define CONFIG_ENV_OFFSET		boot_flash_off
#define ONENAND_ENV_OFFSET		0x240000 /* environment starts here */

#define CONFIG_ENV_ADDR			ONENAND_ENV_OFFSET

#define CONFIG_MTD_ONENAND_2X_PROGRAM

/* Monitor at start of flash */
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_MONITOR_LEN		SZ_256K	/* Reserve 2 sectors */

/* ----------------------------------------------------------------------------
 * Size of malloc() pool
 * ----------------------------------------------------------------------------
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + SZ_128K)
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* bytes reserved for */
						/* initial data */

/* ----------------------------------------------------------------------------
 * SMSC911x Ethernet
 * ----------------------------------------------------------------------------
 */
#if defined(CONFIG_CMD_NET)

#define CONFIG_NET_MULTI
#define CONFIG_SMC911X
#define CONFIG_SMC911X_32_BIT
#define CONFIG_SMC911X_BASE	0x2C000000


#endif /* (CONFIG_CMD_NET) */

#ifndef __ASSEMBLY__
extern struct gpmc *gpmc_cfg;
extern unsigned int boot_flash_base;
extern volatile unsigned int boot_flash_env_addr;
extern unsigned int boot_flash_off;
extern unsigned int boot_flash_sec;
extern unsigned int boot_flash_type;
#endif

#endif /* __CONFIG_H */
