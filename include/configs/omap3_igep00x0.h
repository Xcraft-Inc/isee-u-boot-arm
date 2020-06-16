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


#define __ISEE_BOARD_TEST__	

/*
 * We are only ever GP parts and will utilize all of the "downloaded image"
 * area in SRAM which starts at 0x40200000 and ends at 0x4020FFFF (64KB).
 */
#undef CONFIG_SPL_TEXT_BASE
#define CONFIG_SPL_TEXT_BASE		0x40200000

#define CONFIG_MISC_INIT_R
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_REVISION_TAG		1

#define CONFIG_CMD_JFFS2

/* Status LED available for IGEP0020 and IGEP0030 but not IGEP0032 */
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020) || \
		       (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
#define RED_LED_GPIO 27
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
#define RED_LED_GPIO 16
#endif
#endif

#define GPIO_IGEP00X0_RDET_0				126
#define GPIO_IGEP00X0_RDET_1				127
#define GPIO_IGEP00X0_RDET_2				128
#define GPIO_IGEP00X0_RDET_3				129

#define GPIO_IGEP00X0_REVISION_DETECTION	GPIO_IGEP00X0_RDET_3

/* GPIO banks */
/*#define CONFIG_OMAP3_GPIO_1	   GPIO0..31 is in GPIO bank 1*/
#define CONFIG_OMAP3_GPIO_2		/* GPIO32..63 is in GPIO bank 2 */
#define CONFIG_OMAP3_GPIO_3		/* GPIO64..95 is in GPIO bank 3 */
#define CONFIG_OMAP3_GPIO_4	   	/* GPIO96..127 is in GPIO bank 4 */
#define CONFIG_OMAP3_GPIO_5		/* GPIO128..159 is in GPIO bank 5 */
#define CONFIG_OMAP3_GPIO_6		/* GPIO160..191 is in GPIO bank 6 */

/* CMD */
#define CONFIG_CMD_MTDPARTS

/* TWL4030 LED */
#define CONFIG_TWL4030_LED

/* USB */
#ifdef CONFIG_USB
#define CONFIG_USB_OMAP3
/* usb otg controller: FIXME only MUSB (gadget host) or either EHCI host work at 1 time */
#define CONFIG_USB_MUSB_OMAP2PLUS
#define CONFIG_USB_MUSB_PIO_ONLY

/* usb host controller */
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_OMAP
#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	8
/* TWL4030 USB */
#define CONFIG_TWL4030_USB
#endif

/* EEPROM support */
#define CONFIG_SYS_I2C_EEPROM_BUS 2 		/* Numero de Bus i2C donde esta la eeprom conectada al chip */
#define CONFIG_SYS_I2C_EEPROM_ADDR 0x51 	/* Identificador de la eeprom en el bus */

/* Only will work with igep ubi flasher developed customized
#define MTDPARTS_DEFAULT		"mtdparts=omap2-nand:512k(SPL),"\
								"20m(uboot),40m(kernel),"\
								"-(rootfs)"
*/
/* This will work with igep firmware media create and flasher tools	
#define MTDPARTS_DEFAULT		"mtdparts=omap2-nand.0:512k(spl),"\
								"1m(uboot),128k(environment),"\
								"-(filesystem)"
*/

#define MTDIDS_DEFAULT			"nand0=omap2-nand.0"
#define MTDPARTS_DEFAULT		"mtdparts=omap2-nand.0:512k(spl),"\
					"1m(uboot),12m(kernel),"\
					"-(filesystem)"

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

#define ENV_SELECTFDT \
	"selectfdt="\
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


/*
	ENABLE THIS IN ENV TO CHANGE NANDBOOT TO MAKE IT WORK FOR IGEP UBI CUSTOM FLASHER
	"ubiroot=ubi0:rootfs rw rootwait\0" \
	"ubirootfstype=ubifs rootwait fixrt\0" \
	"ubimtd=3,512\0" \
	"ubinandargs=setenv bootargs ${bootargs} mpurate=800 " \
		"ubi.mtd=${ubimtd} rootfstype=${ubirootfstype} root=${ubiroot} ${optargs}\0" \
	"nandboot= echo Booting from from NAND; " \
		"ubi part kernel; " \
			"ubifsmount ubi0:kernelfs; "\
			"run loadbootenv_nand; "\
			"run importbootenv;" \
			"run loadubifdt; "\
			"run loadubizimage; "\
			"run ubinandargs; "\
			"echo Booting Kernel...; " \
			"bootz ${loadaddr} - ${fdtaddr}\0" \
*/

#ifndef __ISEE_BOARD_TEST__

#define ENV_LOAD_ALGORYTHM \
	"bootenv=uEnv.txt\0" \
	"bootdir=\0" \
	"env_size=800\0" \
	"devnum=0\0" \
	"bootfile=zImage\0" \
	"console=ttyO2,115200n8\0" \
	"loadbootenv_mmc=fatload mmc ${devnum} ${loadaddr} ${bootenv}\0" \
	"loadbootenv_nand=ubifsload 0x82000000 ${bootdir}${bootenv}\0" \
	"loadubizimage=ubifsload ${loadaddr} ${bootdir}${bootfile}\0" \
	"loadubifdt=ubifsload ${fdtaddr} ${bootdir}${fdtfile}\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"importenv=env import -t -r ${loadaddr} ${filesize} \0" \
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"mmcroot=/dev/mmcblk0p2 rw rootwait\0" \
	"mmcrootfstype=ext4\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"importbootenv=env import -t ${loadaddr} ${filesize}\0" \
	"mmcload=load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${bootdir}${bootfile}; " \
		"load mmc ${mmcdev}:${mmcpart} ${fdtaddr} ${bootdir}${fdtfile}\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo Booting from SD/MMC; " \
			"if run loadbootenv_mmc; then " \
				"run importbootenv;" \
			"fi;" \
			"if run mmcload; then " \
				"run mmcargs; " \
				"echo Booting Kernel...; " \
				"bootz ${loadaddr} - ${fdtaddr};" \
			"fi;" \
		"fi;\0" \
	"nandroot=ubi0:filesystem rw ubi.mtd=3,512\0" \
	"nandrootfstype=ubifs rootwait\0" \
	"nandload=ubi part filesystem 512; ubifsmount ubi0; " \
		"ubifsload ${loadaddr} /boot/${bootenv}; " \
		"run importbootenv; " \
		"ubifsload ${loadaddr} /boot/${bootfile}; " \
		"ubifsload ${fdtaddr} /boot/${fdtfile} \0" \
	"nandargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${nandroot} " \
		"rootfstype=${nandrootfstype} \0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"run nandload; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \
	"netload=tftpboot ${loadaddr} ${bootfile}; " \
		"tftpboot ${fdtaddr} ${fdtfile} \0" \
	"netargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/nfs " \
		"ip=${ipaddr} nfsroot=${serverip}:${rootnfs},v3,tcp \0" \
	"netboot=echo Booting from NET; " \
		"run netload; " \
		"run netargs; " \
		"echo Booting Kernel...; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \

#else

#define ENV_LOAD_ALGORYTHM \
	"bootenv=uEnv.txt\0" \
	"bootdir=\0" \
	"env_size=800\0" \
	"devnum=0\0" \
	"bootstage=0\0" \
	"testdef=IGEP0000-D-TEST\0" \
	"console=ttyO2,115200n8\0" \
	"importbootenv=env import -t ${loadaddr} ${filesize}\0" \
	"setup_ip=setenv setup_ip ${ipaddr}:${serverip}:${gateway}:${netmask}:${hostname}:eth0:off:${dns-server}:${ntp-server} \0" \
	"loadnandzImage=fsload ${loadaddr} ${bootdir}${bootfile} \0"\
	"loadnandfdt=fsload ${fdtaddr} ${bootdir}${fdtfile} \0" \
	"nandboot=echo Booting from Nand second stage; " \
		"if run loadnandzImage; then " \
			"if run loadnandfdt; then " \
				"run setup_ip; " \
				"run netargs; " \
				"led green on; " \
				"echo Booting Kernel...; " \
				"bootz ${loadaddr} - ${fdtaddr} " \
			"else " \
				"echo Nand load failed -> go stage 3 with netboot. " \
				"led green off; " \
				"led blue on; " \
				"setenv bootstage 3; " \
				"run netboot; " \
			"fi; " \
		"else " \
			"echo Nand load failed -> go stage 3 with netboot. " \
			"led green off; " \
			"led blue on; " \
			"setenv bootstage 3; " \
			"run netboot; " \
		"fi; \0" \
	"nandboot2=echo Booting from Nand; " \
		"run loadnandzImage; " \
		"run loadnandfdt; " \
		"run setup_ip; " \
		"run netargs; " \
		"echo Booting Kernel...; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \
	"netloadzImage=tftpboot ${loadaddr} ${bootfile}; \0" \
	"netloaddtb=tftpboot ${fdtaddr} ${fdtfile} \0" \
	"netload=tftpboot ${loadaddr} ${bootfile}; " \
		"tftpboot ${fdtaddr} ${fdtfile} \0" \
	"netargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/nfs " \
		"ip=${setup_ip} nfsroot=${serverip}:${rootnfs},v3,tcp " \
		"bootstage=${bootstage} \0" \
	"netboot=echo Booting from NET; " \
		"run netloadzImage; " \
		"run netloaddtb; " \
		"run setup_ip; " \
		"run netargs; " \
		"echo Booting Kernel...; " \
		"bootz ${loadaddr} - ${fdtaddr} \0" \
	"test=run testnand; run check_stage; run goNextStage; \0" \
	"testnand=nand read ${loadaddr} SPL \0" \
	"check_stage=chpart boot; " \
		"if run testnand; then " \
			"if fsload ${loadaddr} ${bootenv}; then " \
				"setenv bootstage 2; " \
				"run importbootenv; " \
			"else " \
				"if fatload mmc ${devnum} ${loadaddr} ${bootenv} ; then " \
					"setenv bootstage 1; " \
					"run importbootenv; " \
				"else " \
					"setenv bootstage 3; " \
				"fi; " \
			"fi; " \
		"else " \
			"if fatload mmc ${devnum} ${loadaddr} ${bootenv} ; then " \
				"setenv bootstage 1; " \
				"run importbootenv; " \
			"else " \
				"setenv bootstage 3; " \
			"fi; " \
		"fi; \0" \
	"goNextStage=" \
		"if test ${bootstage} = 3 ; then " \
			"run fatalErrorTestEnv; " \
		"else " \
			"if test ${bootstage} = 1 ; then " \
				"run netboot; " \
			"else " \
				"if test ${bootstage} = 2 ; then " \
					"run nandboot; " \
				"else " \
					"run fatalErrorTestEnv; " \
				"fi; " \
			"fi; " \
		"fi; \0" \
	"fatalErrorTestEnv=Test Enviroment Fatal Error. " \
						"while true; do led all toggle; sleep 1; done; \0" \
	"fatalErrorHwTest=while true; do led blue toggle; sleep 1; done; \0" \

#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_SELECTFDT \
	ENV_DEVICE_SETTINGS \
	MEM_LAYOUT_SETTINGS \
	ENV_LOAD_ALGORYTHM \

#endif

#ifndef __ISEE_BOARD_TEST__

#define CONFIG_BOOTCOMMAND \
	"run selectfdt;" \
	"run mmcboot;" \
	"run nandboot;" \
	"run netboot;" \

#else

#define CONFIG_BOOTCOMMAND \
	"run test;" \

#endif	

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

/* OneNAND config  */
#if defined(CONFIG_CMD_ONENAND)
#define CONFIG_USE_ONENAND_BOARD_INIT
#define CONFIG_SYS_ONENAND_BASE		ONENAND_MAP
#define CONFIG_SYS_ONENAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SPL_ONENAND_SUPPORT
#endif /* (CONFIG_CMD_NET) */

/* NAND config
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
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_HAM1_CODE_HW

#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_BCH

#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000
*/
/* NAND boot config */
#define CONFIG_BCH
#define CONFIG_SPL_OMAP3_ID_NAND
#define CONFIG_SYS_NAND_BUSWIDTH_16BIT	16
#define CONFIG_SYS_NAND_MAX_ECCPOS  56
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS

#define CONFIG_SYS_NAND_ECCPOS      {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, \
					13, 14, 16, 17, 18, 19, 20, 21, 22, \
					23, 24, 25, 26, 27, 28, 30, 31, 32, \
					33, 34, 35, 36, 37, 38, 39, 40, 41, \
					42, 44, 45, 46, 47, 48, 49, 50, 51, \
					52, 53, 54, 55, 56}
#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	13
#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW_DETECTION_SW
#elif (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
/* #define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_HAM1_CODE_SW */
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW_DETECTION_SW
#endif
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000
#define CONFIG_ENV_SIZE			(32*1024)


/* UBI configuration */
#define CONFIG_CMD_UBIFS	/* Read-only UBI volume operations */
#define CONFIG_RBTREE		/* required by CONFIG_CMD_UBI */
#define CONFIG_LZO			/* required by CONFIG_CMD_UBIFS */
#define CONFIG_JFFS2_LZO	/* required by CONFIG_CMD_JFFS2 */

/* Max number of NAND devices */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
/*#define CONFIG_SYS_NAND_BUSWIDTH_16BIT*/
/* Timeout values (in ticks) */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(100 * CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT	(100 * CONFIG_SYS_HZ)

/* Flash banks JFFS2 should use */
#define CONFIG_SYS_MAX_MTD_BANKS	(CONFIG_SYS_MAX_FLASH_BANKS + \
						CONFIG_SYS_MAX_NAND_DEVICE)

#define CONFIG_SYS_JFFS2_MEM_NAND
#define CONFIG_SYS_JFFS2_FIRST_BANK	CONFIG_SYS_MAX_FLASH_BANKS
#define CONFIG_SYS_JFFS2_NUM_BANKS	1

#define CONFIG_JFFS2_NAND
/* nand device jffs2 lives on */
#define CONFIG_JFFS2_DEV		"nand0"
/* Start of jffs2 partition */
#define CONFIG_JFFS2_PART_OFFSET	0x0180000
/* Size of jffs2 partition */
#define CONFIG_JFFS2_PART_SIZE		0x00a00000


/*
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
*/

/* environment organization
#define CONFIG_ENV_UBI_PART				"kernel"
#define CONFIG_ENV_UBI_VOLUME			"kernelfs"
#define CONFIG_ENV_UBI_VOLUME_REDUND	"kernelfs_r"
#define CONFIG_UBI_SILENCE_MSG		0
#define CONFIG_UBIFS_SILENCE_MSG	0
#define CONFIG_ENV_SIZE			(32*1024)
*/
#endif /* __IGEP00X0_H */
