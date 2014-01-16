/*
 * (C) Copyright 2014
 * ISEE 2007 SL
 * Manel Caro	  <mcaro@iseebcn.com>
 *
 * Configuration settings for the IGEPv5 board.
 * See omap5_common.h for omap5 common settings.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_IGEP0050_H
#define __CONFIG_IGEP0050_H

/* Define the default GPT table for eMMC */
#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=rootfs,start=2MiB,size=-,uuid=${uuid_gpt_rootfs}"

#include <configs/omap5_common.h>

#define CONFIG_OMAP5_IGEPv5

#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		UART3_BASE
#define CONFIG_BAUDRATE			115200

/* MMC ENV related defines */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		1	/* SLOT2: eMMC(1) */
#define CONFIG_ENV_OFFSET		0xE0000
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_CMD_SAVEENV

/* Enhance our eMMC support / experience. */
#define CONFIG_CMD_GPT
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_PART

/* Required support for the TCA642X GPIO we have on the uEVM */
#define CONFIG_TCA642X
#define CONFIG_TCA641X			/* Modify TCA642X driver to be used with TCA641x */
#define CONFIG_CMD_TCA642X
#define CONFIG_SYS_I2C_TCA642X_BUS_NUM 3
#define CONFIG_SYS_I2C_TCA642X_ADDR 0x21

/* eeprom board configuration */
#define CONFIG_IGEPV5_CFG_EEPROM
#define CONFIG_SYS_I2C_IGEPV5_CFG_BUS_NUM   0
#define CONFIG_SYS_I2C_IGEPV5_CFG_BUS_ADDR  0x50

/* USB UHH support options */
#define CONFIG_CMD_USB
#define CONFIG_USB_HOST
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_OMAP
#define CONFIG_USB_STORAGE
#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS 3
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET

#define CONFIG_OMAP_EHCI_PHY2_RESET_GPIO 80
#define CONFIG_OMAP_EHCI_PHY3_RESET_GPIO 79

#define CONFIG_OMAP_USBLAN_ENABLE_GPIO	 78
#define CONFIG_OMAP_USBLAN_RESET_GPIO	 15

#define CONFIG_SYS_I2C_USB0_3503_BUS_NUM 	4
#define CONFIG_SYS_I2C_USB0_3503_ADDR 		0x08
#define CONFIG_SYS_I2C_USB1_3503_BUS_NUM 	2
#define CONFIG_SYS_I2C_USB1_3503_ADDR 		0x08

/* Implement SPL board init for OMAP5 IGEPv5 */
#define CONFIG_SPL_BOARD_INIT

/* Enabled commands */
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP		/* DHCP Support			*/
#define CONFIG_CMD_NET		/* bootp, tftpboot, rarpboot	*/
#define CONFIG_CMD_NFS		/* NFS support			*/

/* USB Networking options */
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_SMSC75XX

#define CONSOLEDEV		"ttyO2"

/* Max time to hold reset on this board, see doc/README.omap-reset-time */
#define CONFIG_OMAP_PLATFORM_RESET_TIME_MAX_USEC	16296

#define CONFIG_SPL_SATA_SUPPORT
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_CMD_SCSI
#define CONFIG_LIBATA
#define CONFIG_SCSI_AHCI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_SYS_SCSI_MAX_SCSI_ID	1
#define CONFIG_SYS_SCSI_MAX_LUN		1
#define CONFIG_SYS_SCSI_MAX_DEVICE	(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
						CONFIG_SYS_SCSI_MAX_LUN)

#endif /* __CONFIG_IGEP0050_H */
