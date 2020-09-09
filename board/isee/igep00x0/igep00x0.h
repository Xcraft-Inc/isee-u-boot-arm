/*
 * (C) Copyright 2010
 * ISEE 2007 SL, <www.iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef _IGEP00X0_H_
#define _IGEP00X0_H_

/*
 * IEN  - Input Enable
 * IDIS - Input Disable
 * PTD  - Pull type Down
 * PTU  - Pull type Up
 * DIS  - Pull type selection is inactive
 * EN   - Pull type selection is active
 * M0   - Mode 0
 * The commented string gives the final mux configuration for that pin
 */
#define MUX_DEFAULT()\
	/* SDRC */\
	MUX_VAL(CP(SDRC_D0),        (IEN  | PTD | DIS | M0)) /* SDRC_D0 */\
	MUX_VAL(CP(SDRC_D1),        (IEN  | PTD | DIS | M0)) /* SDRC_D1 */\
	MUX_VAL(CP(SDRC_D2),        (IEN  | PTD | DIS | M0)) /* SDRC_D2 */\
	MUX_VAL(CP(SDRC_D3),        (IEN  | PTD | DIS | M0)) /* SDRC_D3 */\
	MUX_VAL(CP(SDRC_D4),        (IEN  | PTD | DIS | M0)) /* SDRC_D4 */\
	MUX_VAL(CP(SDRC_D5),        (IEN  | PTD | DIS | M0)) /* SDRC_D5 */\
	MUX_VAL(CP(SDRC_D6),        (IEN  | PTD | DIS | M0)) /* SDRC_D6 */\
	MUX_VAL(CP(SDRC_D7),        (IEN  | PTD | DIS | M0)) /* SDRC_D7 */\
	MUX_VAL(CP(SDRC_D8),        (IEN  | PTD | DIS | M0)) /* SDRC_D8 */\
	MUX_VAL(CP(SDRC_D9),        (IEN  | PTD | DIS | M0)) /* SDRC_D9 */\
	MUX_VAL(CP(SDRC_D10),       (IEN  | PTD | DIS | M0)) /* SDRC_D10 */\
	MUX_VAL(CP(SDRC_D11),       (IEN  | PTD | DIS | M0)) /* SDRC_D11 */\
	MUX_VAL(CP(SDRC_D12),       (IEN  | PTD | DIS | M0)) /* SDRC_D12 */\
	MUX_VAL(CP(SDRC_D13),       (IEN  | PTD | DIS | M0)) /* SDRC_D13 */\
	MUX_VAL(CP(SDRC_D14),       (IEN  | PTD | DIS | M0)) /* SDRC_D14 */\
	MUX_VAL(CP(SDRC_D15),       (IEN  | PTD | DIS | M0)) /* SDRC_D15 */\
	MUX_VAL(CP(SDRC_D16),       (IEN  | PTD | DIS | M0)) /* SDRC_D16 */\
	MUX_VAL(CP(SDRC_D17),       (IEN  | PTD | DIS | M0)) /* SDRC_D17 */\
	MUX_VAL(CP(SDRC_D18),       (IEN  | PTD | DIS | M0)) /* SDRC_D18 */\
	MUX_VAL(CP(SDRC_D19),       (IEN  | PTD | DIS | M0)) /* SDRC_D19 */\
	MUX_VAL(CP(SDRC_D20),       (IEN  | PTD | DIS | M0)) /* SDRC_D20 */\
	MUX_VAL(CP(SDRC_D21),       (IEN  | PTD | DIS | M0)) /* SDRC_D21 */\
	MUX_VAL(CP(SDRC_D22),       (IEN  | PTD | DIS | M0)) /* SDRC_D22 */\
	MUX_VAL(CP(SDRC_D23),       (IEN  | PTD | DIS | M0)) /* SDRC_D23 */\
	MUX_VAL(CP(SDRC_D24),       (IEN  | PTD | DIS | M0)) /* SDRC_D24 */\
	MUX_VAL(CP(SDRC_D25),       (IEN  | PTD | DIS | M0)) /* SDRC_D25 */\
	MUX_VAL(CP(SDRC_D26),       (IEN  | PTD | DIS | M0)) /* SDRC_D26 */\
	MUX_VAL(CP(SDRC_D27),       (IEN  | PTD | DIS | M0)) /* SDRC_D27 */\
	MUX_VAL(CP(SDRC_D28),       (IEN  | PTD | DIS | M0)) /* SDRC_D28 */\
	MUX_VAL(CP(SDRC_D29),       (IEN  | PTD | DIS | M0)) /* SDRC_D29 */\
	MUX_VAL(CP(SDRC_D30),       (IEN  | PTD | DIS | M0)) /* SDRC_D30 */\
	MUX_VAL(CP(SDRC_D31),       (IEN  | PTD | DIS | M0)) /* SDRC_D31 */\
	MUX_VAL(CP(SDRC_CLK),       (IEN  | PTD | DIS | M0)) /* SDRC_CLK */\
	MUX_VAL(CP(SDRC_DQS0),      (IEN  | PTD | DIS | M0)) /* SDRC_DQS0 */\
	MUX_VAL(CP(SDRC_DQS1),      (IEN  | PTD | DIS | M0)) /* SDRC_DQS1 */\
	MUX_VAL(CP(SDRC_DQS2),      (IEN  | PTD | DIS | M0)) /* SDRC_DQS2 */\
	MUX_VAL(CP(SDRC_DQS3),      (IEN  | PTD | DIS | M0)) /* SDRC_DQS3 */\
	/* GPMC Address */\
	MUX_VAL(CP(GPMC_A1),        (IDIS | PTD | DIS | M0)) /* GPMC_A1 */\
	MUX_VAL(CP(GPMC_A2),        (IDIS | PTD | DIS | M0)) /* GPMC_A2 */\
	MUX_VAL(CP(GPMC_A3),        (IDIS | PTD | DIS | M0)) /* GPMC_A3 */\
	MUX_VAL(CP(GPMC_A4),        (IDIS | PTD | DIS | M0)) /* GPMC_A4 */\
	MUX_VAL(CP(GPMC_A5),        (IDIS | PTD | DIS | M0)) /* GPMC_A5 */\
	MUX_VAL(CP(GPMC_A6),        (IDIS | PTD | DIS | M0)) /* GPMC_A6 */\
	MUX_VAL(CP(GPMC_A7),        (IDIS | PTD | DIS | M0)) /* GPMC_A7 */\
	MUX_VAL(CP(GPMC_A8),        (IDIS | PTD | DIS | M0)) /* GPMC_A8 */\
	MUX_VAL(CP(GPMC_A9),        (IDIS | PTD | DIS | M0)) /* GPMC_A9 */\
	MUX_VAL(CP(GPMC_A10),       (IDIS | PTD | DIS | M0)) /* GPMC_A10 */\
	/* GPMC Data */\
	MUX_VAL(CP(GPMC_D0),        (IEN  | PTD | DIS | M0)) /* GPMC_D0 */\
	MUX_VAL(CP(GPMC_D1),        (IEN  | PTD | DIS | M0)) /* GPMC_D1 */\
	MUX_VAL(CP(GPMC_D2),        (IEN  | PTD | DIS | M0)) /* GPMC_D2 */\
	MUX_VAL(CP(GPMC_D3),        (IEN  | PTD | DIS | M0)) /* GPMC_D3 */\
	MUX_VAL(CP(GPMC_D4),        (IEN  | PTD | DIS | M0)) /* GPMC_D4 */\
	MUX_VAL(CP(GPMC_D5),        (IEN  | PTD | DIS | M0)) /* GPMC_D5 */\
	MUX_VAL(CP(GPMC_D6),        (IEN  | PTD | DIS | M0)) /* GPMC_D6 */\
	MUX_VAL(CP(GPMC_D7),        (IEN  | PTD | DIS | M0)) /* GPMC_D7 */\
	MUX_VAL(CP(GPMC_D8),        (IEN  | PTD | DIS | M0)) /* GPMC_D8 */\
	MUX_VAL(CP(GPMC_D9),        (IEN  | PTD | DIS | M0)) /* GPMC_D9 */\
	MUX_VAL(CP(GPMC_D10),       (IEN  | PTD | DIS | M0)) /* GPMC_D10 */\
	MUX_VAL(CP(GPMC_D11),       (IEN  | PTD | DIS | M0)) /* GPMC_D11 */\
	MUX_VAL(CP(GPMC_D12),       (IEN  | PTD | DIS | M0)) /* GPMC_D12 */\
	MUX_VAL(CP(GPMC_D13),       (IEN  | PTD | DIS | M0)) /* GPMC_D13 */\
	MUX_VAL(CP(GPMC_D14),       (IEN  | PTD | DIS | M0)) /* GPMC_D14 */\
	MUX_VAL(CP(GPMC_D15),       (IEN  | PTD | DIS | M0)) /* GPMC_D15 */\
	/* GPMC Signals */\
	MUX_VAL(CP(GPMC_NCS0),      (IDIS | PTU | EN  | M0)) /* GPMC_nCS0 */\
	MUX_VAL(CP(GPMC_NCS1),      (IDIS | PTU | EN  | M0)) /* GPMC_nCS1 */\
	MUX_VAL(CP(GPMC_NCS2),      (IDIS | PTU | EN  | M0)) /* GPIO_nCS2 */\
 	MUX_VAL(CP(GPMC_NCS3),      (IDIS | PTU | EN  | M0)) /* GPIO_nCS3 */\
	MUX_VAL(CP(GPMC_NCS4),      (IDIS | PTU | EN  | M0)) /* GPMC_nCS4 */\
	MUX_VAL(CP(GPMC_NCS5),      (IDIS | PTU | EN  | M0)) /* GPMC_nCS5 */\
	MUX_VAL(CP(GPMC_NCS6),      (IDIS | PTU | EN  | M0)) /* GPMC_nCS6 */\
	MUX_VAL(CP(GPMC_NCS7),      (IDIS | PTU | EN  | M0)) /* GPMC_nCS7 */\
	MUX_VAL(CP(GPMC_CLK),       (IDIS | PTD | DIS | M0)) /* GPMC_CLK */\
	MUX_VAL(CP(GPMC_NADV_ALE),  (IDIS | PTD | DIS | M0)) /* GPMC_nADV_ALE */\
	MUX_VAL(CP(GPMC_NOE),       (IDIS | PTD | DIS | M0)) /* GPMC_nOE */\
	MUX_VAL(CP(GPMC_NWE),       (IDIS | PTD | DIS | M0)) /* GPMC_nWE */\
	MUX_VAL(CP(GPMC_NBE0_CLE),  (IDIS | PTD | DIS | M0)) /* GPMC_nBE0_CLE */\
	MUX_VAL(CP(GPMC_NBE1),      (IEN  | PTD | DIS | M0)) /* GPMC_nBE1 */\
	MUX_VAL(CP(GPMC_NWP),       (IEN  | PTD | DIS | M0)) /* GPMC_nWP */\
	MUX_VAL(CP(GPMC_WAIT0),     (IEN  | PTU | EN  | M0)) /* GPMC_WAIT0 */\
	MUX_VAL(CP(MMC1_CLK),       (IDIS | PTU | EN  | M0)) /* MMC1_CLK */\
	MUX_VAL(CP(MMC1_CMD),       (IEN  | PTU | EN  | M0)) /* MMC1_CMD */\
	MUX_VAL(CP(MMC1_DAT0),      (IEN  | PTU | EN  | M0)) /* MMC1_DAT0 */\
	MUX_VAL(CP(MMC1_DAT1),      (IEN  | PTU | EN  | M0)) /* MMC1_DAT1 */\
	MUX_VAL(CP(MMC1_DAT2),      (IEN  | PTU | EN  | M0)) /* MMC1_DAT2 */\
	MUX_VAL(CP(MMC1_DAT3),      (IEN  | PTU | EN  | M0)) /* MMC1_DAT3 */\
	MUX_VAL(CP(UART3_TX_IRTX),  (IDIS | PTD | DIS | M0)) /* UART3_TX */\
	MUX_VAL(CP(UART3_RX_IRRX),  (IEN  | PTD | DIS | M0)) /* UART3_RX */\
	MUX_VAL(CP(I2C1_SCL),       (IEN  | PTU | EN  | M0)) /* I2C1_SCL */\
	MUX_VAL(CP(I2C1_SDA),       (IEN  | PTU | EN  | M0)) /* I2C1_SDA */\
	MUX_VAL(CP(I2C4_SCL),       (IEN  | PTU | EN  | M0)) /* I2C4_SCL */\
	MUX_VAL(CP(I2C4_SDA),       (IEN  | PTU | EN  | M0)) /* I2C4_SDA */\
	MUX_VAL(CP(SYS_32K),        (IEN  | PTD | DIS | M0)) /* SYS_32K */\
	MUX_VAL(CP(SYS_BOOT0),      (IEN  | PTD | DIS | M4)) /* GPIO_2 */\
	MUX_VAL(CP(SYS_BOOT1),      (IEN  | PTD | DIS | M4)) /* GPIO_3 */\
	MUX_VAL(CP(SYS_BOOT2),      (IEN  | PTD | DIS | M4)) /* GPIO_4 */\
	MUX_VAL(CP(SYS_BOOT3),      (IEN  | PTD | DIS | M4)) /* GPIO_5 */\
	MUX_VAL(CP(SYS_BOOT4),      (IEN  | PTD | DIS | M4)) /* GPIO_6 */\
	MUX_VAL(CP(SYS_BOOT5),      (IEN  | PTD | DIS | M4)) /* GPIO_7 */\
	MUX_VAL(CP(SYS_BOOT6),      (IEN  | PTD | DIS | M4)) /* GPIO_8 */\
	MUX_VAL(CP(GPIO126),        (IEN  | PTU | DIS  | M4)) /* GPIO_126 */\
	MUX_VAL(CP(GPIO127),        (IEN  | PTU | DIS  | M4)) /* GPIO_127 */\
	MUX_VAL(CP(GPIO128),        (IEN  | PTU | DIS  | M4)) /* GPIO_128 */\
	MUX_VAL(CP(GPIO129),        (IEN  | PTU | DIS  | M4)) /* GPIO_129 */\
	MUX_VAL(CP(SDRC_CKE0),      (IDIS | PTU | EN  | M0)) /* SDRC_CKE0 */\
	MUX_VAL(CP(SDRC_CKE1),      (IDIS | PTU | EN  | M0)) /* SDRC_CKE1 */\
	/* USB 0 OTG */\
	MUX_VAL(CP(HSUSB0_CLK),		(IEN  | PTD | DIS | M0)) /*HSUSB0_CLK*/\
	MUX_VAL(CP(HSUSB0_STP),		(IDIS | PTU | EN  | M0)) /*HSUSB0_STP*/\
	MUX_VAL(CP(HSUSB0_DIR),		(IEN  | PTD | DIS | M0)) /*HSUSB0_DIR*/\
	MUX_VAL(CP(HSUSB0_NXT),		(IEN  | PTD | DIS | M0)) /*HSUSB0_NXT*/\
	MUX_VAL(CP(HSUSB0_DATA0),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA0*/\
	MUX_VAL(CP(HSUSB0_DATA1),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA1*/\
	MUX_VAL(CP(HSUSB0_DATA2),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA2*/\
	MUX_VAL(CP(HSUSB0_DATA3),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA3*/\
	MUX_VAL(CP(HSUSB0_DATA4),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA4*/\
	MUX_VAL(CP(HSUSB0_DATA5),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA5*/\
	MUX_VAL(CP(HSUSB0_DATA6),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA6*/\
	MUX_VAL(CP(HSUSB0_DATA7),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA7*/\
	/* UARTS */\
	MUX_VAL(CP(UART1_TX),  		(IDIS | PTD | DIS | M0)) /* UART1_TX */\
	MUX_VAL(CP(UART1_RX),  		(IEN  | PTD | DIS | M0)) /* UART1_RX */\
	MUX_VAL(CP(UART1_RTS),  	(IDIS | PTD | DIS | M0)) /* UART1_RTS */\
	MUX_VAL(CP(UART1_CTS),  	(IEN  | PTU | DIS | M0)) /* UART1_CTS */\
	MUX_VAL(CP(UART2_TX),  		(IDIS | PTD | DIS | M0)) /* UART2_TX */\
	MUX_VAL(CP(UART2_RX),  		(IEN  | PTD | DIS | M0)) /* UART2_RX */\
	MUX_VAL(CP(UART3_TX_IRTX),  (IDIS | PTD | DIS | M0)) /* UART3_TX */\
	MUX_VAL(CP(UART3_RX_IRRX),  (IEN  | PTD | DIS | M0)) /* UART3_RX */

#endif

#define MUX_IGEP0020() \
	/* USB 1 HOST */\
	MUX_VAL(CP(ETK_CLK_ES2),	(IDIS | PTU | EN  | M3)) /*HSUSB1_STP*/\
	MUX_VAL(CP(ETK_CTL_ES2),	(IDIS | PTU | DIS | M3)) /*HSUSB1_CLK*/\
	MUX_VAL(CP(ETK_D0_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA0*/\
	MUX_VAL(CP(ETK_D1_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA1*/\
	MUX_VAL(CP(ETK_D2_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA2*/\
	MUX_VAL(CP(ETK_D3_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA7*/\
	MUX_VAL(CP(ETK_D4_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA4*/\
	MUX_VAL(CP(ETK_D5_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA5*/\
	MUX_VAL(CP(ETK_D6_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA6*/\
	MUX_VAL(CP(ETK_D7_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DATA3*/\
	MUX_VAL(CP(ETK_D8_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_DIR*/\
	MUX_VAL(CP(ETK_D9_ES2),		(IEN  | PTU | DIS | M3)) /*HSUSB1_NXT*/\
	/* MISC */\
	MUX_VAL(CP(ETK_D14_ES2),	(IEN  | PTD | EN  | M4)) /* gpio_28 */ \
	MUX_VAL(CP(ETK_D12_ES2),	(IEN  | PTD | EN  | M4)) /* gpio_26 */ \
	MUX_VAL(CP(ETK_D13_ES2),	(IEN  | PTD | EN  | M4)) /* gpio_27 */ \
	MUX_VAL(CP(GPMC_WAIT2),     (IEN  | PTU | DIS | M4)) /* GPIO_64-ETH_NRST */\
	MUX_VAL(CP(I2C2_SCL),       (IEN  | PTU | EN  | M0)) /* I2C2_SCL */\
	MUX_VAL(CP(I2C2_SDA),       (IEN  | PTU | EN  | M0)) /* I2C2_SDA */\
	MUX_VAL(CP(I2C3_SCL),       (IEN  | PTU | EN  | M0)) /* I2C3_SCL */\
	MUX_VAL(CP(I2C3_SDA),       (IEN  | PTU | EN  | M0)) /* I2C3_SDA */\
	MUX_VAL(CP(ETK_D10_ES2),	(IDIS | PTD | EN  | M4)) /* GPIO_24-USB_HOST_RST*/


#define MUX_IGEP0030() \
	/* USB 2 HOST */\
	MUX_VAL(CP(ETK_D14_ES2),	(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA0*/\
	MUX_VAL(CP(ETK_D15_ES2),	(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA1*/\
	MUX_VAL(CP(MCSPI1_CS3),		(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA2*/\
	MUX_VAL(CP(MCSPI2_CS1),		(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA3*/\
	MUX_VAL(CP(MCSPI2_SIMO),	(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA4*/\
	MUX_VAL(CP(MCSPI2_SOMI),	(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA5*/\
	MUX_VAL(CP(MCSPI2_CS0),		(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA6*/\
	MUX_VAL(CP(MCSPI2_CLK),		(IEN  | PTD | EN  | M3)); /*HSUSB2_DATA7*/\
	MUX_VAL(CP(ETK_D12_ES2),	(IEN  | PTD | EN  | M3)); /*HSUSB2_DIR*/\
	MUX_VAL(CP(ETK_D13_ES2),	(IEN  | PTD | EN  | M3)); /*HSUSB2_NXT*/\
	MUX_VAL(CP(ETK_D10_ES2),	(IDIS | PTD | DIS | M3)); /*HSUSB2_CLK*/\
	MUX_VAL(CP(ETK_D11_ES2),	(IDIS | PTU | DIS | M3)); /*HSUSB2_STP*/\
	/* MISC */\
	MUX_VAL(CP(I2C2_SCL),       (IEN  | PTU | EN  | M4)) /* I2C2_SCL -> gpio 168*/\
	MUX_VAL(CP(I2C2_SDA),       (IEN  | PTU | EN  | M4)) /* I2C2_SDA -> gpio 183*/\
	MUX_VAL(CP(GPMC_A9),        (IEN  | PTU | DIS | M4)) /* GPIO 42 ETH0_NRST BASE0010-RB */\
	MUX_VAL(CP(GPMC_A10),       (IDIS | PTD | DIS | M4)) /* GPIO 43 ETH1_NRST BASE0010-RB */\
	MUX_VAL(CP(UART1_TX),       (IDIS | PTD | DIS | M0)) /* UART1_TX */\
	MUX_VAL(CP(UART1_RX),       (IEN  | PTD | DIS | M0)) /* UART1_RX */\
	MUX_VAL(CP(I2C3_SCL),       (IEN  | PTU | EN  | M0)) /* I2C3_SCL */\
	MUX_VAL(CP(I2C3_SDA),       (IEN  | PTU | EN  | M0)) /* I2C3_SDA */\
	MUX_VAL(CP(GPMC_NCS3),		(IDIS | PTU | EN  | M4)) /* GPIO 54 */\
	MUX_VAL(CP(ETK_D9),			(IDIS | PTU | EN  | M4)) /* GPIO 23 BASE0010-RB HUB_RST*/\
	MUX_VAL(CP(GPMC_NCS1),      (IEN | PTU | EN  | M4)) /* GPIO 52 ETH0_IRQ GPMC_nCS1 */


