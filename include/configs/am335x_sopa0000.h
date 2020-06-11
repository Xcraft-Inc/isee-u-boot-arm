/*
 * Copyright (C) 2016, ISEE 2007 SL - http://www.isee.biz/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_SOPA0000_H
#define __CONFIG_SOPA0000_H

#define CONFIG_NAND

#include <configs/ti_am335x_common.h>

#ifndef CONFIG_SPL_BUILD
#define CONFIG_TIMESTAMP
#define CONFIG_LZO
#endif

#define CONFIG_SYS_BOOTM_LEN		(16 << 20)

/* Mach type */
#define MACH_TYPE_SOPA0000		4521	/* Until the next sync */
#define CONFIG_MACH_TYPE		MACH_TYPE_SOPA0000

/* Clock defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB */

/* Module specific Configs --> Defined inside defconfig*/

/* #define ISEE_HARDWARETEST_ENABLED */

/* Make the verbose messages from UBI stop printing */
#define CONFIG_UBI_SILENCE_MSG
#define CONFIG_UBIFS_SILENCE_MSG

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifndef CONFIG_SPL_BUILD

#ifndef ISEE_HARDWARETEST_ENABLED

#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	"ethact=cpsw\0" \
	"ethprime=cpsw\0" \
	"bootdir=\0" \
	"usbbootdir=/boot\0" \
	"bootenv=uEnv.txt\0" \
	"bootfile=zImage\0" \
	"dtbfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"console=ttyO0,115200n8\0" \
	"loadbootenv= usb start; usb dev 0; " \
		"fatload usb 0:1 ${loadaddr} ${bootenv} \0" \
	"importbootenv= echo Importing environment from USB ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"usb_pgood_delay=2000\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"nandroot=ubi0:filesystem rw ubi.mtd=3,512\0" \
	"nandrootfstype=ubifs rootwait\0" \
	"nandload=chpart nand0,2;" \
		"fsload ${loadaddr} ${bootdir}/${bootfile}; " \
		"fsload ${fdtaddr} ${bootdir}/${dtbfile} \0" \
	"nandargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${nandroot} " \
		"rootfstype=${nandrootfstype} \0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"run nandload; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \
	"usbload= ext4load usb 0:2 ${loadaddr} ${usbbootdir}/${bootfile}; " \
		"ext4load usb 0:2 ${fdtaddr} ${usbbootdir}/${dtbfile} \0" \
	"usbargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/sda2 " \
		"rw ext3 rootwait \0" \
	"netload=tftpboot ${loadaddr} ${bootfile}; " \
		"tftpboot ${fdtaddr} ${dtbfile} \0" \
	"netargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/nfs rw " \
		"ip=${ipaddr} nfsroot=${serverip}:${rootnfs},v3,tcp \0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"run netload; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \
	"usbuenvboot= if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
			"run usbargs; " \
			"run usbload; " \
			"bootz ${loadaddr} - ${fdtaddr}; " \
		"fi;" \
		"usb stop \0"

#define CONFIG_BOOTCOMMAND \
	"run usbuenvboot;" \
	"run nandboot;"  \
	"run netboot"

#else /* defined ISEE_HARDWARETEST_ENABLED */

#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	"ethact=cpsw\0" \
	"ethprime=cpsw\0" \
	"bootdir=/boot\0" \
	"bootenv=uEnv.txt\0" \
	"bootfile=zImage\0" \
	"dtbfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
	"serverip=192.168.50.2\0" \
	"ipaddr=192.168.50.254\0" \
	"gateway=192.168.50.1\0" \
	"netmask=255.255.255.0\0" \
	"dnsserver=8.8.8.8\0" \
	"machinename=StationNone\0" \
	"usb_pgood_delay=2000\0" \
	"rootnfs=/opt/nfs-server/SOPA0000-test-rootfs-0.1-7/\0" \
	"ipconf=setenv setup_ip ${ipaddr}:${serverip}:${gateway}:${netmask}:${machinename}:eth0:off:${dnsserver}::${serverip}\0" \
	"console=ttyO0,115200n8\0" \
	"loadbootenv= usb start; usb dev 0; " \
		"fatload usb 0:1 ${loadaddr} ${bootenv}; " \
		"echo Loaded ${bootenv} file in RAM...; " \
		"usb stop \0" \
	"importbootenv= echo Importing uEnv.txt variables...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"usbload= ext4load usb 0:2 ${loadaddr} ${bootdir}/${bootfile}; " \
		"ext4load usb 0:2 ${fdtaddr} ${bootdir}/${dtbfile} \0" \
	"usbargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/sda2 " \
		"rw ext3 rootwait \0" \
	"netload=if tftpboot ${loadaddr} ${bootfile}; then " \
			"if tftpboot ${fdtaddr} ${dtbfile}; then " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"fi; " \
		"fi\0" \
	"netargs=run ipconf; setenv bootargs console=${console} root=/dev/nfs rw " \
		"ip=${setup_ip} nfsroot=${serverip}:${rootnfs},v3,tcp \0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"run netload \0" \
	"usbuenvboot= if run loadbootenv; then " \
			"run importbootenv; " \
		"fi\0"


#define CONFIG_BOOTCOMMAND \
	"run usbuenvboot;" \
	"run netboot"

#endif /*ISEE_HARDWARETEST_ENABLED*/

#endif /*!CONFIG_SPL_BUILD*/

	
/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* UART0 */
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/* Ethernet support */
#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC
#define CONFIG_DRIVER_TI_CPSW

/* EEPROM support */
#define CONFIG_SYS_I2C_EEPROM_BUS 1 		/* Numero de Bus i2C donde esta la eeprom conectada al chip */
#define CONFIG_SYS_I2C_EEPROM_ADDR 0x50 	/* Identificador de la eeprom en el bus */

/* SPL */
#define CONFIG_SPL_LDSCRIPT		"arch/arm/mach-omap2/am33xx/u-boot-spl.lds"

#ifdef CONFIG_NAND
/* NAND support */
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_ENV_OFFSET		0x180000 /* environment starts here */
#define CONFIG_SYS_ENV_SECT_SIZE	(128 << 10)	/* 128 KiB */
#define CONFIG_ENV_ADDR_REDUND		(CONFIG_ENV_OFFSET + CONFIG_SYS_ENV_SECT_SIZE)
#define CONFIG_ENV_SIZE_REDUND		(CONFIG_ENV_SIZE)
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_RBTREE
#define CONFIG_NAND_OMAP_ELM
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }

#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW
#define	CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000
#define MTDIDS_DEFAULT			"nand0=omap2-nand.0"
#define MTDPARTS_DEFAULT		"mtdparts=omap2-nand.0:512k(spl),"\
					"1m(uboot),12m(kernel),"\
					"-(filesystem)"
#define CONFIG_CMD_UBIFS
#define CONFIG_CMD_JFFS2
#define CONFIG_JFFS2_NAND
#define CONFIG_JFFS2_LZO

#define CONFIG_CMD_MTDPARTS
/* Unsupported features */
#undef CONFIG_USE_IRQ
#endif /* !CONFIG_NAND */

#ifndef CONFIG_NAND
#define CONFIG_ENV_IS_NOWHERE
#endif /*CONFIG_NAND*/

/*
 * USB configuration. We enable MUSB support, both for host and for
 * gadget. We set USB0 as peripheral and USB1 as host, based on the
 * board schematic and physical port wired to each. Then for host we
 * add mass storage support and for gadget we add both RNDIS ethernet
 * and DFU.
 */
#define CONFIG_USB_MUSB_DSPS
#define CONFIG_ARCH_MISC_INIT					
#define CONFIG_USB_MUSB_PIO_ONLY
#define	CONFIG_USB_MUSB_DISABLE_BULK_COMBINE_SPLIT
#define CONFIG_AM335X_USB0
#define CONFIG_AM335X_USB0_MODE	MUSB_PERIPHERAL
#define CONFIG_AM335X_USB1
#define CONFIG_AM335X_USB1_MODE MUSB_HOST

/*If we are booting from the USB OTG --> am335x_sopa0000_usbspl_defconfig*/
#if defined(CONFIG_USB_MUSB_GADGET)
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_USBNET_HOST_ADDR	"de:ad:be:af:00:01"
#endif /* CONFIG_USB_MUSB_GADGET && CONFIG_SPL_USBETH_SUPPORT */

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_USBETH_SUPPORT)
/* Remove other SPL modes. */
#undef CONFIG_SPL_NAND_SUPPORT
#define CONFIG_ENV_IS_NOWHERE
#undef CONFIG_ENV_IS_IN_NAND
#undef CONFIG_PARTITION_UUIDS
#undef CONFIG_EFI_PARTITION
#endif /*CONFIG_SPL_BUILD && CONFIG_SPL_USBETH_SUPPORT*/

/*If we are booting from the ETH --> am335x_sopa0000_ethspl_defconfig*/
#if defined(CONFIG_SPL_ETH_SUPPORT)
#undef CONFIG_SPL_NAND_SUPPORT
#define CONFIG_ENV_IS_NOWHERE
#undef CONFIG_ENV_IS_IN_NAND
#endif /*CONFIG_SPL_ETH_SUPPORT*/
#endif	/* ! __CONFIG_SOPA0000_H */
