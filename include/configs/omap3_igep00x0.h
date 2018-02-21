/*
 * Common configuration settings for IGEP technology based boards
 *
 * (C) Copyright 2012
 * ISEE 2007 SL, <www.iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __IGEP00X0_H
#define __IGEP00X0_H

#define CONFIG_NR_DRAM_BANKS            2
#define CONFIG_NAND

#include <configs/ti_omap3_common.h>
#include <asm/mach-types.h>

/*
 * We are only ever GP parts and will utilize all of the "downloaded image"
 * area in SRAM which starts at 0x40200000 and ends at 0x4020FFFF (64KB).
 */
#undef CONFIG_SPL_TEXT_BASE
#define CONFIG_SPL_TEXT_BASE		0x40200000

#define CONFIG_MISC_INIT_R
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_REVISION_TAG		1

/* Status LED available for IGEP0020 and IGEP0030 but not IGEP0032 */
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020) || \
		       (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
#define RED_LED_GPIO 27
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
#define RED_LED_GPIO 16
#endif
#endif

/* GPIO banks */
#define CONFIG_OMAP3_GPIO_3		/* GPIO64 .. 95 is in GPIO bank 3 */
#define CONFIG_OMAP3_GPIO_5		/* GPIO128..159 is in GPIO bank 5 */
#define CONFIG_OMAP3_GPIO_6		/* GPIO160..191 is in GPIO bank 6 */

/* TWL4030 LED */
#define CONFIG_TWL4030_LED

#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_ONENAND

/* EEPROM support */
#define CONFIG_SYS_I2C_EEPROM_BUS 2 		/* Numero de Bus i2C donde esta la eeprom conectada al chip */
#define CONFIG_SYS_I2C_EEPROM_ADDR 0x51 	/* Identificador de la eeprom en el bus */

/*"nand0=omap2-nand.0"*/
/* Parititons */
#define MTDIDS_DEFAULT			"nand0=omap2-nand"
#define MTDPARTS_DEFAULT		"mtdparts=omap2-nand:512k(SPL),"\
								"20m(uboot),40m(kernel),"\
								"-(rootfs)"
#ifndef CONFIG_SPL_BUILD

/* Environment */
#define ENV_DEVICE_SETTINGS \
	"stdin=serial\0" \
	"stdout=serial\0" \
	"stderr=serial\0"

#define MEM_LAYOUT_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	"scriptaddr=0x87E00000\0" \
	"pxefile_addr_r=0x87F00000\0"

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0)

#define ENV_FINDFDT \
	"findfdt="\
		"if test ${board_name} = igep0020; then " \
			"if test ${board_rev} = F; then " \
				"setenv fdtfile omap3-igep0020-rev-f.dtb; " \
			"else " \
				"setenv fdtfile omap3-igep0020.dtb; fi; fi; " \
		"if test ${board_name} = igep0030; then " \
			"if test ${board_rev} = G; then " \
				"setenv fdtfile omap3-igep0030-rev-g.dtb; " \
			"else " \
				"setenv fdtfile omap3-igep0030.dtb; fi; fi; " \
		"if test ${fdtfile} = ''; then " \
			"echo WARNING: Could not determine device tree to use; fi; \0"

#define ENV_LOAD_ALGORYTHM \
	"bootenv=uEnv.txt\0" \
	"env_size=800\0" \
	"devnum=0\0" \
	"bootdir=/boot\0" \
	"bootdir2=/\0" \
	"bootfile=zImage\0" \
	"console=ttyO2,115200n8\0" \
	"loadbootenv_mmc=fatload mmc ${devnum} ${loadaddr} ${bootenv}\0" \
	"loadbootenv_nand=ubifsload 0x82000000 uEnv.txt\0" \
	"loadubizimage=ubifsload ${loadaddr} ${bootfile}\0" \
	"loadubifdt=ubifsload ${fdtaddr} ${fdtfile}\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"importenv=env import -t -r $loadaddr $filesize \0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 rw\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
		"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=env import -t ${loadaddr} ${filesize}\0" \
	"mmcload=load mmc ${mmcdev}:1 ${loadaddr} ${bootfile}; " \
		"load mmc ${mmcdev}:1 ${fdtaddr} ${fdtfile}\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo Trying to load environment from MMC; " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootenv; then " \
				"echo Loaded environment from ${bootenv};" \
				"run importbootenv;" \
			"fi;" \
			"if test -n $uenvcmd; then " \
				"echo Running uenvcmd ...;" \
				"run uenvcmd;" \
			"fi;" \
			"if run mmcload; then " \
				"run mmcargs; " \
				"bootz ${loadaddr} - ${fdtaddr};" \
			"fi;" \
		"fi;\0" \
	"ubinandargs=setenv bootargs ${bootargs} mpurate=800 " \
		"ubi.mtd=${ubimtd} rootfstype=${ubirootfstype} root=${ubiroot} ${optargs}\0" \
	"loadbootenv_nand=ubifsload ${loadaddr} ${bootfile}\0" \
	"nandboot= echo Trying to boot from NAND; " \
		"echo Trying UBIFS;" \
		"if ubi part kernel; then " \
			"ubifsmount ubi0:kernelfs; "\
			"run loadbootenv_nand; "\
			"run importbootenv;" \
			"run loadubifdt; "\
			"run loadubizimage; "\
			"run ubinandargs; "\
			"bootz ${loadaddr} - ${fdtaddr}\0" \
		"fi;" \
		"echo Could not load from UBIFS;\0" \
	"netload=tftpboot ${loadaddr} ${bootfile}; " \
		"tftpboot ${fdtaddr} ${fdtfile} \0" \
	"netargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/nfs " \
		"ip=${ipaddr} nfsroot=${serverip}:${rootnfs},v3,tcp \0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"run netload; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \

#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_FINDFDT \
	ENV_DEVICE_SETTINGS \
	MEM_LAYOUT_SETTINGS \
	ENV_LOAD_ALGORYTHM \
	BOOTENV
#endif

#define CONFIG_BOOTCOMMAND \
	"run findfdt; " \
	"run mmcboot;" \
	"run nandboot;" \
	"run netboot;" \

#include <config_distro_bootcmd.h>

/*
 * SMSC911x Ethernet
 */
#if defined(CONFIG_CMD_NET)
#define CONFIG_SMC911X
#define CONFIG_SMC911X_32_BIT
#define CONFIG_SMC911X_BASE		0x2C000000
#endif /* (CONFIG_CMD_NET) */

#define CONFIG_SYS_MTDPARTS_RUNTIME
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS

/* OneNAND config */
#define CONFIG_USE_ONENAND_BOARD_INIT
#define CONFIG_SYS_ONENAND_BASE		ONENAND_MAP
#define CONFIG_SYS_ONENAND_BLOCK_SIZE	(128*1024)

/* NAND config */
#define CONFIG_SPL_OMAP3_ID_NAND
#define CONFIG_SYS_NAND_BUSWIDTH_16BIT
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCPOS		{ 2,  3,  4,  5,  6,  7,  8,  9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }
#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW_DETECTION_SW
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_BCH

#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000

/* UBI configuration */
#define CONFIG_CMD_UBIFS	/* Read-only UBI volume operations */
#define CONFIG_RBTREE		/* required by CONFIG_CMD_UBI */
#define CONFIG_LZO		/* required by CONFIG_CMD_UBIFS */
#define CONFIG_SPL_UBI			1
#define CONFIG_SPL_UBI_MAX_VOL_LEBS	256
#define CONFIG_SPL_UBI_MAX_PEB_SIZE	(256*1024)
#define CONFIG_SPL_UBI_MAX_PEBS		4096
#define CONFIG_SPL_UBI_VOL_IDS		8
#define CONFIG_SPL_UBI_LOAD_MONITOR_ID	0
#define CONFIG_SPL_UBI_LOAD_KERNEL_ID	3
#define CONFIG_SPL_UBI_LOAD_ARGS_ID	4
#define CONFIG_SPL_UBI_PEB_OFFSET	4
#define CONFIG_SPL_UBI_VID_OFFSET	512
#define CONFIG_SPL_UBI_LEB_START	2048
#define CONFIG_SPL_UBI_INFO_ADDR	0x88080000

/* environment organization */
#define CONFIG_ENV_UBI_PART				"kernel"
#define CONFIG_ENV_UBI_VOLUME			"kernelfs"
#define CONFIG_ENV_UBI_VOLUME_REDUND	"kernelfs_r"
#define CONFIG_UBI_SILENCE_MSG		0
#define CONFIG_UBIFS_SILENCE_MSG	0
#define CONFIG_ENV_SIZE			(32*1024)

#endif /* __IGEP00X0_H */
