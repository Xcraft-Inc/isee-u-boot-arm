/*
 * Timing and Organization details of the ddr device parts used in OMAP5
 * EVM
 *
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *
 * Aneesh V <aneesh@ti.com>
 * Sricharan R <r.sricharan@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/emif.h>
#include <asm/arch/sys_proto.h>

/*
 * This file provides details of the LPDDR2 SDRAM parts used on OMAP5
 * EVM. Since the parts used and geometry are identical for
 * evm for a given OMAP5 revision, this information is kept
 * here instead of being in board directory. However the key functions
 * exported are weakly linked so that they can be over-ridden in the board
 * directory if there is a OMAP5 board in the future that uses a different
 * memory device or geometry.
 *
 * For any new board with different memory devices over-ride one or more
 * of the following functions as per the CONFIG flags you intend to enable:
 * - emif_get_reg_dump()
 * - emif_get_dmm_regs()
 * - emif_get_device_details()
 * - emif_get_device_timings()
 */

#ifdef CONFIG_SYS_EMIF_PRECALCULATED_TIMING_REGS
const struct emif_regs emif_regs_532_mhz_2cs = {
	.sdram_config_init		= 0x80800EBA,
	.sdram_config			= 0x808022BA,
	.ref_ctrl			= 0x0000081A,
	.sdram_tim1			= 0x772F6873,
	.sdram_tim2			= 0x304a129a,
	.sdram_tim3			= 0x02f7e45f,
	.read_idle_ctrl			= 0x00050000,
	.zq_config			= 0x000b3215,
	.temp_alert_config		= 0x08000a05,
	.emif_ddr_phy_ctlr_1_init	= 0x0E28420d,
	.emif_ddr_phy_ctlr_1		= 0x0E28420d,
	.emif_ddr_ext_phy_ctrl_1	= 0x04020080,
	.emif_ddr_ext_phy_ctrl_2	= 0x28C518A3,
	.emif_ddr_ext_phy_ctrl_3	= 0x518A3146,
	.emif_ddr_ext_phy_ctrl_4	= 0x0014628C,
	.emif_ddr_ext_phy_ctrl_5	= 0x04010040
};

const struct emif_regs emif_regs_532_mhz_2cs_es2 = {
	.sdram_config_init		= 0x80800EBA,
	.sdram_config			= 0x808022BA,
	.ref_ctrl			= 0x0000081A,
	.sdram_tim1			= 0x772F6873,
	.sdram_tim2			= 0x304a129a,
	.sdram_tim3			= 0x02f7e45f,
	.read_idle_ctrl			= 0x00050000,
	.zq_config			= 0x100b3215,
	.temp_alert_config		= 0x08000a05,
	.emif_ddr_phy_ctlr_1_init	= 0x0E30400d,
	.emif_ddr_phy_ctlr_1		= 0x0E30400d,
	.emif_ddr_ext_phy_ctrl_1	= 0x04020080,
	.emif_ddr_ext_phy_ctrl_2	= 0x28C518A3,
	.emif_ddr_ext_phy_ctrl_3	= 0x518A3146,
	.emif_ddr_ext_phy_ctrl_4	= 0x0014628C,
	.emif_ddr_ext_phy_ctrl_5	= 0xC330CC33,
};

const struct emif_regs emif_regs_266_mhz_2cs = {
	.sdram_config_init		= 0x80800EBA,
	.sdram_config			= 0x808022BA,
	.ref_ctrl			= 0x0000040D,
	.sdram_tim1			= 0x2A86B419,
	.sdram_tim2			= 0x1025094A,
	.sdram_tim3			= 0x026BA22F,
	.read_idle_ctrl			= 0x00050000,
	.zq_config			= 0x000b3215,
	.temp_alert_config		= 0x08000a05,
	.emif_ddr_phy_ctlr_1_init	= 0x0E28420d,
	.emif_ddr_phy_ctlr_1		= 0x0E28420d,
	.emif_ddr_ext_phy_ctrl_1	= 0x04020080,
	.emif_ddr_ext_phy_ctrl_2	= 0x0A414829,
	.emif_ddr_ext_phy_ctrl_3	= 0x14829052,
	.emif_ddr_ext_phy_ctrl_4	= 0x000520A4,
	.emif_ddr_ext_phy_ctrl_5	= 0x04010040
};

const struct emif_regs emif_regs_ddr3_532_mhz_1cs = {
	.sdram_config_init		= 0x61851B32,
	.sdram_config			= 0x61851B32,
	.sdram_config2			= 0x0,
	.ref_ctrl			= 0x00001035,
	.sdram_tim1			= 0xCCCF36B3,
	.sdram_tim2			= 0x308F7FDA,
	.sdram_tim3			= 0x027F88A8,
	.read_idle_ctrl			= 0x00050000,
	.zq_config			= 0x0007190B,
	.temp_alert_config		= 0x00000000,
	.emif_ddr_phy_ctlr_1_init	= 0x0020420A,
	.emif_ddr_phy_ctlr_1		= 0x0024420A,
	.emif_ddr_ext_phy_ctrl_1	= 0x04040100,
	.emif_ddr_ext_phy_ctrl_2	= 0x00000000,
	.emif_ddr_ext_phy_ctrl_3	= 0x00000000,
	.emif_ddr_ext_phy_ctrl_4	= 0x00000000,
	.emif_ddr_ext_phy_ctrl_5	= 0x04010040,
	.emif_rd_wr_lvl_rmp_win		= 0x00000000,
	.emif_rd_wr_lvl_rmp_ctl		= 0x80000000,
	.emif_rd_wr_lvl_ctl			= 0x00000000,
	.emif_rd_wr_exec_thresh		= 0x00000305
};

/* Register Fields definition to DDR3 531,8 Mhz */

/* Register sdram_config_init = sdram_config = EMIF_SDRAM_CONFIG (0x4C000008) */

#define SDRAM_TYPE				3		/* 3 = DDR3 */
#define IBANK_POS				0		
#define DDR_TERM				1		/* RZQ / 4 */
#define DDR2_DDQS				1
#define DYN_ODT					0
#define DDR_DISABLE_DLL			0
#define SDRAM_DRIVE				1		/* RZQ/6 = 0 (34 ohm), RZQ/7 = 1 (40 Ohm) */

/* CAS WRITE Latency */
/* Micron MT41K256M16HA-125IT:
 * CL = 7 -> CLW = 6 -> tCK [1.875 .. < 2.5] nS
 *  */
#define CWL_5					0
#define CWL_6					1
#define CWL_7					2
#define CWL_8					3
#define CWL						CWL_6		/* DDR3 CAS Write latency. Value of 0, 1, 2, and 3 (CAS write latency of 5, 6, 7, and 8) are supported. */

#define CL_5					2
#define CL_6					4
#define CL_7					6
#define CL_8					8
#define CL_9					10
#define CL_10					12
#define CL_11					14
#define CL						CL_7		/* DDR3 CAS Latency (referred to as read latency (RL) in some SDRAM specs
											   Values of 2, 4, 6, 8, 10, 12, and 14 (CAS latency of 5, 6, 7, 8, 9, 10, and 11) are supported for DDR3 */
#define NORROW_MODE				0
#define ROWSIZE					6			/* 6 = 15 row bits */
#define IBANK					3			/* 3 = 8 banks */
#define EBANK_CS0_ENABLE			0
#define EBANK_CS0_CS1_ENABLE	1
#define EBANK					EBANK_CS0_ENABLE	/* External chip select setup. Defines whether SDRAM accesses will use 1 or 2 chip select lines. Set to 0 to use NCS0 only. Set to 1 to use NCS[1:0] */
#define PAGESIZE				2			/* 1024 word page - 10 column bits */

#define SDRAM_CONFIG_1			(SDRAM_TYPE << 29) | (IBANK_POS << 27) | (DDR_TERM << 24) | \
								(DDR2_DDQS << 23) | (DYN_ODT << 21) | (DDR_DISABLE_DLL << 20) | \
								(SDRAM_DRIVE << 18) | (CWL << 16) | (NORROW_MODE << 14) | (CL << 10) | \
								(ROWSIZE << 7) | (IBANK << 4) | (EBANK << 3) | (PAGESIZE)

/* Register sdram_config2 = EMIF_SDRAM_CONFIG (0x4C00000C) */
#define EBANK_POS				0
#define SDRAM_CONFIG_2			(EBANK_POS << 27) | 0

/* Register ref_ctrl = EMIF_SDRAM_REFRESH_CONTROL (0x4C000010) */
#define INITREF_DIS 			0
#define SRT 					0
#define ASR 					0
#define PASR 					0
#define REFRESH_RATE 			4154 /* (531,8 Mhz * 7,8uS) */

#define REFRESH_CONTROL			(INITREF_DIS << 31) | (SRT << 29) | (ASR << 28) | (PASR << 24) | REFRESH_RATE


/* Register sdram_tim1 = EMIF_SDRAM_TIMMING_1 (0x4C000018) */

#define T_RTW					6	/* Minimum number of DDR clock cycles between Read to Write data phases, minus one.*/
#define T_RP					6	/* Minimum number of DDR clock cycles from Precharge to Activate or Refresh, minus one. */
#define T_RCD					6	/* Minimum number of DDR clock cycles from Activate to Read or Write,  minus one. */
#define T_WR					7	/* Minimum number of DDR clock cycles from last Write transfer to Precharge,  minus one. */
#define T_RAS					19	/* Minimum number of DDR clock cycles from Activate to Precharge, minus one. T_RAS value needs to be bigger than or equal to T_RDC value */
#define T_RC					26	/* Minimum number of DDR clock cycles from Activate to Activate,  minus one. */
#define T_RRD					6	/* Minimum number of DDR clock cycles from Activate to Activate for a different bank, minus one. For an 8-bank, this field must be equal to ((tFAW / (4 × tCK)) - 1) */
#define T_WTR					3	/* Minimum number of DDR clock cycles from last Write to Read, minus one. */
#define T_SDRAM_TIM1			(T_RTW << 29) | (T_RP << 25) | (T_RCD << 21) | (T_WR << 17) | (T_RAS << 12) | (T_RC << 6) | (T_RRD << 3) | (T_WTR)

/* Register sdram_tim2 = EMIF_SDRAM_TIMMING_2 (0x4C000020) */
#define T_XP					3	/* Minimum number of DDR clock cycles from power-down exit to any command other than a read command, minus one */
#define T_ODT					0	/* Minimum number of DDR clock cycles from ODT enable to write data driven for DDR3. Must be equal to tAOND */
#define T_XSNR					143	/* Minimum number of DDR clock cycles from Self-Refresh exit to any command other than a Read command, minus one. For DDR3, the value of tXS must be programmed. */
#define T_XSRD					511	/* Minimum number of DDR clock cycles from Self-Refresh exit to a Read command, minus one. For DDR3, the value of tXSDLL must be programmed. */
#define T_RTP					3	/* Minimum number of DDR clock cycles for the last read command to a Precharge command, minus one */
#define T_CKE					2	/* Minimum number of DDR clock cycles between CKE pin changes, minus one. */
#define T_SDRAM_TIM2			(T_XP << 28) | (T_ODT << 25) | (T_XSNR << 16) | (T_XSRD << 6) | (T_RTP << 3) | (T_CKE)

/* Register sdram_tim3 = EMIF_SDRAM_TIMMING_3 (0x4C000028) */
#define	T_PDLL_UL				5	/* Minimum number of DDR clock cycles for PHY DLL to unlock. A value of N will be equal to N x 128 clocks */
#define T_CSTA					5	/* Minimum number of DDR clock cycles between write-to-write or read-to-read data phases to different chip selects, minus one. */
#define T_CKESR					3	/* Minimum number of DDR clock cycles for which SDRAM must remain in Self Refresh, minus one */
#define T_ZQ_ZQCS				63	/* Number of DDR clock cycles for a ZQCS command, minus one. */ 
#define T_TDQSCKMAX				0	/* Number of DDR clock that satisfies tDQSCKmax for LPDDR2, minus one. */
#define T_RFC					138	/* Minimum number of DDR clock cycles from Refresh or Load Mode to Refresh or Activate, minus one.*/
#define T_RAS_MAX				15	/* Maximum number of REFRESH_RATE intervals from Activate to Precharge command. 
										This field must be equal to ((tRASmax / tREFI)-1) rounded down to the next lower integer. 
										Value for T_RAS_MAX can be calculated as follows: 
											If tRASmax = 120 us and tREFI = 15.7 us, 
												then T_RAS_MAX = ((120/15.7)-1) = 6.64. Round down to the next lower integer. 
									Therefore, the programmed value must be 6.*/
#define T_SDRAM_TIM3			(T_PDLL_UL << 28) | (T_CSTA << 24) | (T_CKESR << 21) | (T_ZQ_ZQCS << 15) | (T_TDQSCKMAX << 13) | (T_RFC << 4) | T_RAS_MAX

/* Register zq_config = EMIF_SDRAM_OUTPUT_IMPEDANCE_CALIBRATION_CONFIG (0x4C0000C8) */

#define ZQ_CS1EN				0	/* Writing a 1 enables ZQ calibration for CS1. */
#define ZQ_CS0EN 				1	/* Writing a 1 enables ZQ calibration for CS0. */
#define ZQ_DUALCALEN 			0	/* ZQ Dual Calibration enable. Allows both ranks to be ZQ calibrated simultaneously. 
									   Setting this bit requires both chip selects to have a separate calibration resistor per device. */
#define ZQ_SFEXITEN 			1	/* Writing a 1 enables the issuing of ZQCL on Self-Refresh exit. */
#define ZQ_PDEXITEN 			0	/* Writing a 1 enables the issuing of ZQCL on Active Power-Down, and Precharge Power-Down exit. */
#define ZQ_ZQINIT_MULT 			1	/* Indicates the number of ZQCL durations that make up a ZQINIT duration, minus one. */
#define ZQ_ZQCL_MULT 			3	/* Indicates the number of ZQCS intervals that make up a ZQCL duration, minus one. 
									   ZQCS interval is defined by ZQ_ZQCS inEMIF_SDRAM_TIMING_3: see T_SDRAM_TIM3[T_ZQ_ZQCS] */
#define ZQ_REFINTERVAL 			0x190B	/* Number of refresh periods between ZQCS commands */

#define ZQ_CONFIG				(ZQ_CS1EN << 31) | (ZQ_CS0EN << 30) | (ZQ_DUALCALEN << 29) | \
								(ZQ_SFEXITEN << 28) | (ZQ_PDEXITEN << 27) | (ZQ_ZQINIT_MULT << 18) | \
								(ZQ_ZQCL_MULT << 16) | (ZQ_REFINTERVAL)
								
/* Register emif_ddr_phy_ctlr_1_init (init) = EMIF_DDR_PHY_CONTROL_1 (0x4C0000E4) */
#define RDLVL_MASK_INIT 			1	/* Writing a 1 to this field will mask read data eye training during full leveling command, 
										   plus drives reg_phy_use_rd_data_eye_level control low to allow user to use programmed ratio values. 
										   Incremental training needs to be disabled using incremental training registers. */
#define RDLVLGATE_MASK_INIT 		1	/* Writing a 1 to this field will mask dqs gate training during full leveling command,
										   plus drives reg_phy_use_rd_dqs_level control low to allow user to use programmed ratio values. 
										   Incremental training needs to be disabled using incremental training registers. */
#define WRLVL_MASK_INIT 			1	/* Writing a 1 to this field will mask write leveling training during full leveling command, 
										   plus drives reg_phy_use_wr_level control low to allow user to use programmed ratio values. 
										   Incremental training needs to be disabled using incremental training registers. */
#define PHY_HALF_DELAYS_INIT 		1	/* Adjust slave delay line delays to support 2× 
										   mode 1: 2× mode (MDLL clock is half the rate of PHY) - OPP_NOM 
										   mode 0: 1× mode (MDLL clock rate is same as PHY) - OPP_BOOT */
#define PHY_CLK_STALL_LEVEL_INIT 	1	/* Enable variable idle value for delay lines. Enable during normal operations to avoid differential aging in the delay lines. */
#define PHY_DIS_CALIB_RST_INIT 		0	/* Disable the dll_calib (internally generated) signal from resetting the Read Capture FIFO pointers and portions of data PHYs. Debug only. */
#define PHY_INVERT_CLKOUT_INIT		0	/* Inverts the polarity of DRAM clock. This bit must be set to 0 when in LDDDR2 mode.
										   0: core clock is passed on to DRAM 
										   1: inverted core clock is passed on to DRAM */
#define PHY_DLL_LOCK_DIFF_INIT		0x10	/* The maximum number of delay line taps variation while maintaining the master DLL lock. 
											   When the PHY is in locked state and the variation on the clock exceeds the variation indicated by this field, 
											   the lock signal is de-asserted and a dll_calib signal is generated. 
											   To prevent the dll_calib signal from being asserted in the middle of traffic when the clock jitter exceeds the variation, 
											   this register needs to be set to a value which will ensure that the lock will not be lost. Recommended value is 16. */
#define PHY_FAST_DLL_LOCK_INIT 		0		/* Controls master DLL to lock fast or average logic must be part of locking process. 
											   Set to 1 before OPP transition commences, and set back to 0 after OPP transition completes. 
											   1: MDLL lock is asserted based on single sample 
											   0: MDLL lock is asserted based on average of 16 samples. */
#define READ_LATENCY_INIT 			0x0A	/* This field defines the read latency for the read data from SDRAM in number of DDR clock cycles 
											   This field is used by the EMIF as well as the PHY. READ_LATENCY = RL + reg_phy_rdc_we_to_re -1. 
											   EMIF uses above equation to calculate reg_phy_rdc_we_to_re and forward it to the PHY. 
											   For DDR3, the true RL is used, not the decoded value. See JEDEC spec. */


#define EMIF_DDR_PHY_CONTROL_1_INIT		(RDLVL_MASK_INIT << 27) | (RDLVLGATE_MASK_INIT << 26) | (WRLVL_MASK_INIT << 25) | \
										(PHY_HALF_DELAYS_INIT << 21) | (PHY_CLK_STALL_LEVEL_INIT << 20) | (PHY_DIS_CALIB_RST_INIT << 19) | \
										(PHY_INVERT_CLKOUT_INIT << 18) | (PHY_DLL_LOCK_DIFF_INIT << 10) | (PHY_FAST_DLL_LOCK_INIT << 9) | \
										(READ_LATENCY_INIT)
								
/* Register emif_ddr_phy_ctlr_1 (init) = EMIF_DDR_PHY_CONTROL_1 (0x4C0000E4) */
#define RDLVL_MASK		 			1	/* Writing a 1 to this field will mask read data eye training during full leveling command, 
										   plus drives reg_phy_use_rd_data_eye_level control low to allow user to use programmed ratio values. 
										   Incremental training needs to be disabled using incremental training registers. */
#define RDLVLGATE_MASK		 		1	/* Writing a 1 to this field will mask dqs gate training during full leveling command,
										   plus drives reg_phy_use_rd_dqs_level control low to allow user to use programmed ratio values. 
										   Incremental training needs to be disabled using incremental training registers. */
#define WRLVL_MASK		 			1	/* Writing a 1 to this field will mask write leveling training during full leveling command, 
										   plus drives reg_phy_use_wr_level control low to allow user to use programmed ratio values. 
										   Incremental training needs to be disabled using incremental training registers. */
#define PHY_HALF_DELAYS		 		1	/* Adjust slave delay line delays to support 2× 
										   mode 1: 2× mode (MDLL clock is half the rate of PHY) - OPP_NOM 
										   mode 0: 1× mode (MDLL clock rate is same as PHY) - OPP_BOOT */
#define PHY_CLK_STALL_LEVEL		 	1	/* Enable variable idle value for delay lines. Enable during normal operations to avoid differential aging in the delay lines. */
#define PHY_DIS_CALIB_RST	 		0	/* Disable the dll_calib (internally generated) signal from resetting the Read Capture FIFO pointers and portions of data PHYs. Debug only. */
#define PHY_INVERT_CLKOUT			1	/* Inverts the polarity of DRAM clock. This bit must be set to 0 when in LDDDR2 mode.
										   0: core clock is passed on to DRAM 
										   1: inverted core clock is passed on to DRAM */
#define PHY_DLL_LOCK_DIFF			0x16	/* The maximum number of delay line taps variation while maintaining the master DLL lock. 
											   When the PHY is in locked state and the variation on the clock exceeds the variation indicated by this field, 
											   the lock signal is de-asserted and a dll_calib signal is generated. 
											   To prevent the dll_calib signal from being asserted in the middle of traffic when the clock jitter exceeds the variation, 
											   this register needs to be set to a value which will ensure that the lock will not be lost. Recommended value is 16. */
#define PHY_FAST_DLL_LOCK	 		0		/* Controls master DLL to lock fast or average logic must be part of locking process. 
											   Set to 1 before OPP transition commences, and set back to 0 after OPP transition completes. 
											   1: MDLL lock is asserted based on single sample 
											   0: MDLL lock is asserted based on average of 16 samples. */
#define READ_LATENCY	 			0x0A	/* This field defines the read latency for the read data from SDRAM in number of DDR clock cycles 
											   This field is used by the EMIF as well as the PHY. READ_LATENCY = RL + reg_phy_rdc_we_to_re -1. 
											   EMIF uses above equation to calculate reg_phy_rdc_we_to_re and forward it to the PHY. 
											   For DDR3, the true RL is used, not the decoded value. See JEDEC spec. */


#define EMIF_DDR_PHY_CONTROL_1	(RDLVL_MASK << 27) | (RDLVLGATE_MASK << 26) | (WRLVL_MASK << 25) | \
								(PHY_HALF_DELAYS << 21) | (PHY_CLK_STALL_LEVEL << 20) | (PHY_DIS_CALIB_RST << 19) | \
								(PHY_INVERT_CLKOUT << 18) | (PHY_DLL_LOCK_DIFF << 10) | (PHY_FAST_DLL_LOCK << 9) | \
								(READ_LATENCY)
								
							
								
							
#ifdef __notdef
const struct emif_regs emif_regs_ddr3_532_mhz_1cs_es2 = {
	.sdram_config_init              = 0x61851B32,
	.sdram_config                   = 0x61851B32,
	.sdram_config2					= 0x0,
	.ref_ctrl                       = 0x00001035,
	.sdram_tim1                     = 0xCCCF36B3,
	.sdram_tim2                     = 0x308F7FDA,
	.sdram_tim3                     = 0x027F88A8,
	.read_idle_ctrl                 = 0x00050000,
	.zq_config                      = 0x1007190B,
	.temp_alert_config              = 0x00000000,
	.emif_ddr_phy_ctlr_1_init       = 0x0020420A,
	.emif_ddr_phy_ctlr_1            = 0x0024420A,
	.emif_ddr_ext_phy_ctrl_1        = 0x04040100,
	.emif_ddr_ext_phy_ctrl_2        = 0x00000000,
	.emif_ddr_ext_phy_ctrl_3        = 0x00000000,
	.emif_ddr_ext_phy_ctrl_4        = 0x00000000,
	.emif_ddr_ext_phy_ctrl_5        = 0x04010040,
	.emif_rd_wr_lvl_rmp_win         = 0x00000000,
	.emif_rd_wr_lvl_rmp_ctl         = 0x80000000,
	.emif_rd_wr_lvl_ctl             = 0x00000000,
	.emif_rd_wr_exec_thresh         = 0x40000305,
};
#else


#define RDWRLVINC_RMP_WIN			0
#define EMIF_READ_WRITE_LEVELING_RAMP_WINDOW	RDWRLVINC_RMP_WIN

#define RDWRLVL_EN					1
#define RDWRLVLINC_RMP_PRE 			0
#define RDLVLINC_RMP_INT 			0
#define RDLVLGATEINC_RMP_INT 		0
#define WRLVLINC_RMP_INT			0
#define EMIF_READ_WRITE_LEVELING_RAMP_CONTROL	(RDWRLVL_EN << 31) | (RDWRLVLINC_RMP_PRE << 24) | (RDLVLINC_RMP_INT << 16) | \
												(RDLVLGATEINC_RMP_INT << 8) | (WRLVLINC_RMP_INT)

#define RDWRLVLFULL_START			0
#define RDWRLVLINC_PRE 				0
#define RDLVLINC_INT 				0
#define RDLVLGATEINC_INT 			0
#define WRLVLINC_INT 				0
#define EMIF_READ_WRITE_LEVELING_CONTROL		(RDWRLVLFULL_START << 31) | (RDWRLVLINC_PRE << 24) | (RDLVLINC_INT << 16) | \
												(RDLVLGATEINC_INT << 8) | (WRLVLINC_INT)

#define	MASK_0_10_BITS					0x7FF
#define MASK_0_9_BITS					(MASK_0_10_BITS >> 1)
#define MASK_BIT_10						(1 << 10)
#define MASK_0_8_BITS					(MASK_0_10_BITS >> 2)			
#define MASK_BIT_10_9					(3 << 9)
#define MASK_BIT_8						(1 << 7)
#define MASK_BIT_0_1					0x3
#define MASK_BIT_0_3					0xF
#define MASK_0_7_BITS					(MASK_0_10_BITS >> 3)
#define MASK_0_6_BITS					(MASK_0_10_BITS >> 4)

/* PHY_FIFO_WE_SLAVE_RATIO */
#define PHY_FIFO_WE_SLAVE_RATIO_0_0		0xB0
#define PHY_FIFO_WE_SLAVE_RATIO_0_1		0xB0
#define PHY_FIFO_WE_SLAVE_RATIO_1_0		0xBC
#define EMIF_EXT_PHY_CONTROL_2			((PHY_FIFO_WE_SLAVE_RATIO_1_0 & MASK_0_9_BITS)  << 22) | \
										((PHY_FIFO_WE_SLAVE_RATIO_0_1 & MASK_0_10_BITS) << 11) | \
										(PHY_FIFO_WE_SLAVE_RATIO_0_0 & MASK_0_10_BITS)

#define PHY_FIFO_WE_SLAVE_RATIO_1_1		0xBC
#define PHY_FIFO_WE_SLAVE_RATIO_2_0		0xBC
#define PHY_FIFO_WE_SLAVE_RATIO_2_1		0xBC
#define EMIF_EXT_PHY_CONTROL_3			((PHY_FIFO_WE_SLAVE_RATIO_2_1 & MASK_0_8_BITS) << 23) | \
										((PHY_FIFO_WE_SLAVE_RATIO_2_0 & MASK_0_10_BITS) << 12)| \
										((PHY_FIFO_WE_SLAVE_RATIO_1_1 & MASK_0_10_BITS) << 1) | \
										((PHY_FIFO_WE_SLAVE_RATIO_1_0 & MASK_BIT_10) >> 10)

#define PHY_FIFO_WE_SLAVE_RATIO_3_0		0xBC
#define PHY_FIFO_WE_SLAVE_RATIO_3_1		0xBC
#define EMIF_EXT_PHY_CONTROL_4			((PHY_FIFO_WE_SLAVE_RATIO_3_1 & MASK_0_10_BITS) << 13) | \
										((PHY_FIFO_WE_SLAVE_RATIO_3_0 & MASK_0_10_BITS) << 2) | \
 										((PHY_FIFO_WE_SLAVE_RATIO_2_1 & MASK_BIT_10_9) >> 9)

/* EMIF_EXT_PHY_CONTROL_5 = PHY_RD_DQS_SLAVE_RATIO */
#define PHY_RD_DQS_SLAVE_RATIO_0_0		0x34
#define PHY_RD_DQS_SLAVE_RATIO_0_1		0x34
#define PHY_RD_DQS_SLAVE_RATIO_1_0		0x34
#define PHY_RD_DQS_SLAVE_RATIO_1_1		0x34
#define EMIF_EXT_PHY_CONTROL_5			((PHY_RD_DQS_SLAVE_RATIO_1_1 & MASK_BIT_0_1) << 30) | \
										((PHY_RD_DQS_SLAVE_RATIO_1_0 & MASK_0_9_BITS) << 20) | \
										((PHY_RD_DQS_SLAVE_RATIO_0_1 & MASK_0_9_BITS) << 10) | \
										(PHY_RD_DQS_SLAVE_RATIO_0_0 & MASK_0_9_BITS)


#define PHY_RD_DQS_SLAVE_RATIO_2_0		0x34
#define PHY_RD_DQS_SLAVE_RATIO_2_1		0x34
#define PHY_RD_DQS_SLAVE_RATIO_3_0		0x34
#define EMIF_EXT_PHY_CONTROL_6			((PHY_RD_DQS_SLAVE_RATIO_3_0 & MASK_BIT_0_3) << 28) | \
										((PHY_RD_DQS_SLAVE_RATIO_2_1 & MASK_0_9_BITS) << 18)| \
										((PHY_RD_DQS_SLAVE_RATIO_2_0 & MASK_0_9_BITS) << 8) | \
										((PHY_RD_DQS_SLAVE_RATIO_1_1 & ~MASK_BIT_0_1) >> 2)

#define PHY_RD_DQS_SLAVE_RATIO_3_1		0x34
#define EMIF_EXT_PHY_CONTROL_7			((PHY_RD_DQS_SLAVE_RATIO_3_1 & MASK_0_9_BITS) << 6) | \
										((PHY_RD_DQS_SLAVE_RATIO_3_0 & (~MASK_BIT_0_3)) >> 4)

/* PHY_WR_DATA_SLAVE_RATIO */
#define PHY_WR_DATA_SLAVE_RATIO_0_0		0xA7
#define PHY_WR_DATA_SLAVE_RATIO_0_1		0xA7
#define PHY_WR_DATA_SLAVE_RATIO_1_0		0xA7
#define PHY_WR_DATA_SLAVE_RATIO_1_1		0xA7
#define EMIF_EXT_PHY_CONTROL_8			((PHY_WR_DATA_SLAVE_RATIO_1_1 & MASK_BIT_0_1) << 30) | \
										((PHY_WR_DATA_SLAVE_RATIO_1_0 & MASK_0_9_BITS) << 20) | \
										((PHY_WR_DATA_SLAVE_RATIO_0_1 & MASK_0_9_BITS) << 10) | \
										(PHY_WR_DATA_SLAVE_RATIO_0_0 & MASK_0_9_BITS)

#define PHY_WR_DATA_SLAVE_RATIO_2_0		0xA7
#define PHY_WR_DATA_SLAVE_RATIO_2_1		0xA7
#define PHY_WR_DATA_SLAVE_RATIO_3_0		0xA7
#define EMIF_EXT_PHY_CONTROL_9			((PHY_WR_DATA_SLAVE_RATIO_3_0 & MASK_BIT_0_3) << 28) | \
										((PHY_WR_DATA_SLAVE_RATIO_2_1 & MASK_0_9_BITS) << 18)| \
										((PHY_WR_DATA_SLAVE_RATIO_2_0 & MASK_0_9_BITS)<< 8) | \
										((PHY_WR_DATA_SLAVE_RATIO_1_1 & (~MASK_BIT_0_1)) >> 2)

#define PHY_WR_DATA_SLAVE_RATIO_3_1		0xA7
#define EMIF_EXT_PHY_CONTROL_10			((PHY_WR_DATA_SLAVE_RATIO_3_1 & MASK_0_9_BITS) << 6) | \
										((PHY_WR_DATA_SLAVE_RATIO_3_0 & (~MASK_BIT_0_3)) >> 4)

/* EMIF_EXT_PHY_CONTROL_11 - PHY_WR_DQS_SLAVE_RATIO */
#define PHY_WR_DQS_SLAVE_RATIO_0_0		0x67
#define PHY_WR_DQS_SLAVE_RATIO_0_1		0x67
#define PHY_WR_DQS_SLAVE_RATIO_1_0		0x67
#define PHY_WR_DQS_SLAVE_RATIO_1_1		0x67

#define EMIF_EXT_PHY_CONTROL_11			((PHY_WR_DQS_SLAVE_RATIO_1_1 & MASK_BIT_0_1) << 30) | \
										((PHY_WR_DQS_SLAVE_RATIO_1_0 & MASK_0_9_BITS) << 20) | \
										((PHY_WR_DQS_SLAVE_RATIO_0_1 & MASK_0_9_BITS) << 10) | \
										(PHY_WR_DQS_SLAVE_RATIO_0_0 & MASK_0_9_BITS)
								
#define PHY_WR_DQS_SLAVE_RATIO_2_0		0x67
#define PHY_WR_DQS_SLAVE_RATIO_2_1		0x67
#define PHY_WR_DQS_SLAVE_RATIO_3_0		0x67

#define EMIF_EXT_PHY_CONTROL_12			((PHY_WR_DQS_SLAVE_RATIO_3_0 & MASK_BIT_0_3) << 28) | \
										((PHY_WR_DQS_SLAVE_RATIO_2_1 & MASK_0_9_BITS) << 18)| \
										((PHY_WR_DQS_SLAVE_RATIO_2_0 & MASK_0_9_BITS) << 8) | \
										((PHY_WR_DQS_SLAVE_RATIO_1_1 & (~MASK_BIT_0_1)) >> 2)

#define PHY_WR_DQS_SLAVE_RATIO_3_1		0x67
#define EMIF_EXT_PHY_CONTROL_13			((PHY_WR_DQS_SLAVE_RATIO_3_1 & MASK_0_9_BITS) << 6) | \
										((PHY_WR_DQS_SLAVE_RATIO_3_0 & (~MASK_BIT_0_3)) >> 4)

/* EMIF_EXT_PHY_CONTROL_14 */
#define PHY_RD_DQS_SLAVE_DELAY 			0x80
#define PHY_FIFO_WE_IN_DELAY 			0x80
#define PHY_CTRL_SLAVE_DELAY 			0x80
#define EMIF_EXT_PHY_CONTROL_14			((PHY_RD_DQS_SLAVE_DELAY & MASK_0_7_BITS) << 24) | \
										((PHY_FIFO_WE_IN_DELAY & MASK_0_8_BITS) << 12) | \
										(PHY_CTRL_SLAVE_DELAY & MASK_0_8_BITS)

/* EMIF_EXT_PHY_CONTROL_15 */
#define PHY_WR_DATA_SLAVE_DELAY 		0x80
#define PHY_WR_DQS_SLAVE_DELAY  		0x80
#define EMIF_EXT_PHY_CONTROL_15			((PHY_WR_DATA_SLAVE_DELAY & MASK_0_8_BITS) << 16) | \
										((PHY_WR_DQS_SLAVE_DELAY & MASK_0_8_BITS) << 4) | \
										((PHY_RD_DQS_SLAVE_DELAY & MASK_BIT_8) >> 7)

/* EMIF_EXT_PHY_CONTROL_16 */	
#define PHY_DQ_OFFSET_3					0x20
#define PHY_DQ_OFFSET_2					0x20
#define PHY_DQ_OFFSET_1					0x20
#define PHY_DQ_OFFSET_0					0x20
#define EMIF_EXT_PHY_CONTROL_16			((PHY_DQ_OFFSET_3 & MASK_0_6_BITS) << 21) | \
										((PHY_DQ_OFFSET_2 & MASK_0_6_BITS) << 14) | \
										((PHY_DQ_OFFSET_1 & MASK_0_6_BITS) << 7) | \
										(PHY_DQ_OFFSET_0 & MASK_0_6_BITS)

/* EMIF_EXT_PHY_CONTROL_17 */
#define PHY_GATELVL_INIT_MODE			1
#define PHY_USERANK0_DELAYS				0
#define EMIF_EXT_PHY_CONTROL_17			(PHY_GATELVL_INIT_MODE << 1) | PHY_USERANK0_DELAYS

/* EMIF_EXT_PHY_CONTROL_18 */
#define PHY_GATE_INIT_RATIO_0_0			0x77
#define PHY_GATE_INIT_RATIO_0_1			0x77
#define PHY_GATE_INIT_RATIO_1_0			0x82
#define EMIF_EXT_PHY_CONTROL_18			((PHY_GATE_INIT_RATIO_1_0 & MASK_0_9_BITS) << 22) | \
										((PHY_GATE_INIT_RATIO_0_1 & MASK_0_10_BITS) << 11) | \
										(PHY_GATE_INIT_RATIO_0_0 & MASK_0_10_BITS)

/* EMIF_EXT_PHY_CONTROL_19 */
#define PHY_GATE_INIT_RATIO_1_1			0x82
#define PHY_GATE_INIT_RATIO_2_0			0x7E
#define PHY_GATE_INIT_RATIO_2_1			0x7E
#define EMIF_EXT_PHY_CONTROL_19			((PHY_GATE_INIT_RATIO_2_1 & MASK_0_8_BITS) << 23) | \
										((PHY_GATE_INIT_RATIO_2_0 & MASK_0_10_BITS) << 12)| \
										((PHY_GATE_INIT_RATIO_1_1 & MASK_0_10_BITS) << 1) | \
										((PHY_GATE_INIT_RATIO_1_0 & MASK_BIT_10) >> 10)

/* EMIF_EXT_PHY_CONTROL_20 */
#define PHY_GATE_INIT_RATIO_3_0			0x7F
#define PHY_GATE_INIT_RATIO_3_1			0x7F
#define EMIF_EXT_PHY_CONTROL_20			((PHY_GATE_INIT_RATIO_3_1 & MASK_0_10_BITS) << 13) |  \
										((PHY_GATE_INIT_RATIO_3_0 & MASK_0_10_BITS) << 2) | \
 										((PHY_GATE_INIT_RATIO_2_1 & MASK_BIT_10_9) >> 9)

/* EMIF_EXT_PHY_CONTROL_21 */
#define PHY_WRLVL_INIT_RATIO_0_0		0x22
#define PHY_WRLVL_INIT_RATIO_0_1		0x22
#define PHY_WRLVL_INIT_RATIO_1_0		0x2c
#define PHY_WRLVL_INIT_RATIO_1_1		0x2c
#define EMIF_EXT_PHY_CONTROL_21			((PHY_WRLVL_INIT_RATIO_1_1 & MASK_BIT_0_1) << 30) | \
										((PHY_WRLVL_INIT_RATIO_1_0 & MASK_0_9_BITS) << 20) | \
										((PHY_WRLVL_INIT_RATIO_0_1 & MASK_0_9_BITS) << 10) | \
										(PHY_WRLVL_INIT_RATIO_0_0 & MASK_0_9_BITS)

/* EMIF_EXT_PHY_CONTROL_22 */
#define PHY_WRLVL_INIT_RATIO_2_0		0x28
#define PHY_WRLVL_INIT_RATIO_2_1		0x28
#define PHY_WRLVL_INIT_RATIO_3_0		0x2a
#define EMIF_EXT_PHY_CONTROL_22			((PHY_WRLVL_INIT_RATIO_3_0 & MASK_BIT_0_3) << 28) | \
										((PHY_WRLVL_INIT_RATIO_2_1 & MASK_0_9_BITS) << 18)| \
										((PHY_WRLVL_INIT_RATIO_2_0 & MASK_0_9_BITS) << 8) | \
										((PHY_WRLVL_INIT_RATIO_1_1 & ~MASK_BIT_0_1) >> 2)
/* EMIF_EXT_PHY_CONTROL_23 */
#define PHY_WRLVL_INIT_RATIO_3_1		0x2a
#define EMIF_EXT_PHY_CONTROL_23			((PHY_WRLVL_INIT_RATIO_3_1 & MASK_0_9_BITS) << 6) | \
										((PHY_WRLVL_INIT_RATIO_3_0 & (~MASK_BIT_0_3)) >> 4)


const struct emif_regs emif_regs_ddr3_532_mhz_1cs_es2 = {
	.sdram_config_init              = SDRAM_CONFIG_1,/*0x61851B32, */
	.sdram_config                   = SDRAM_CONFIG_1,/*0x61851B32, */ 
	.sdram_config2					= SDRAM_CONFIG_2,
	.ref_ctrl                       = REFRESH_CONTROL,
	.sdram_tim1                     = T_SDRAM_TIM1,
	.sdram_tim2                     = T_SDRAM_TIM2,
	.sdram_tim3                     = T_SDRAM_TIM3,
	.read_idle_ctrl                 = 0x00050000,
	.zq_config                      = ZQ_CONFIG, /*0x1007190B,*/
	.temp_alert_config              = 0x00000000,
	.emif_ddr_phy_ctlr_1_init       = EMIF_DDR_PHY_CONTROL_1_INIT, /*0x0030400A,*/
	.emif_ddr_phy_ctlr_1            = EMIF_DDR_PHY_CONTROL_1, /*0x0034400A,*/
	.emif_ddr_ext_phy_ctrl_1        = 0x04040100,
	.emif_ddr_ext_phy_ctrl_2        = EMIF_EXT_PHY_CONTROL_2,
	.emif_ddr_ext_phy_ctrl_3        = EMIF_EXT_PHY_CONTROL_3,
	.emif_ddr_ext_phy_ctrl_4        = EMIF_EXT_PHY_CONTROL_4,
	.emif_ddr_ext_phy_ctrl_5        = EMIF_EXT_PHY_CONTROL_5, /*0x4350D435,*/
	.emif_rd_wr_lvl_rmp_win         = EMIF_READ_WRITE_LEVELING_RAMP_WINDOW,
	.emif_rd_wr_lvl_rmp_ctl         = EMIF_READ_WRITE_LEVELING_RAMP_CONTROL,
	.emif_rd_wr_lvl_ctl             = EMIF_READ_WRITE_LEVELING_CONTROL,
	.emif_rd_wr_exec_thresh         = 0x00000305, /*0x40000305,*/
};
#endif

const struct emif_regs emif_1_regs_ddr3_532_mhz_1cs_dra_es1 = {
	.sdram_config_init              = 0x61851ab2,
	.sdram_config                   = 0x61851ab2,
	.sdram_config2					= 0x08000000,
	.ref_ctrl                       = 0x00001035,
	.sdram_tim1                     = 0xCCCF36B3,
	.sdram_tim2                     = 0x308F7FDA,
	.sdram_tim3                     = 0x027F88A8,
	.read_idle_ctrl                 = 0x00050000,
	.zq_config                      = 0x0007190B,
	.temp_alert_config              = 0x00000000,
	.emif_ddr_phy_ctlr_1_init       = 0x0E20400A,
	.emif_ddr_phy_ctlr_1            = 0x0E24400A,
	.emif_ddr_ext_phy_ctrl_1        = 0x04040100,
	.emif_ddr_ext_phy_ctrl_2        = 0x009E009E,
	.emif_ddr_ext_phy_ctrl_3        = 0x009E009E,
	.emif_ddr_ext_phy_ctrl_4        = 0x009E009E,
	.emif_ddr_ext_phy_ctrl_5        = 0x009E009E,
	.emif_rd_wr_lvl_rmp_win         = 0x00000000,
	.emif_rd_wr_lvl_rmp_ctl         = 0x80000000,
	.emif_rd_wr_lvl_ctl             = 0x00000000,
	.emif_rd_wr_exec_thresh         = 0x00000305
};

const struct emif_regs emif_2_regs_ddr3_532_mhz_1cs_dra_es1 = {
	.sdram_config_init              = 0x61851B32,
	.sdram_config                   = 0x61851B32,
	.sdram_config2					= 0x08000000,
	.ref_ctrl                       = 0x00001035,
	.sdram_tim1                     = 0xCCCF36B3,
	.sdram_tim2                     = 0x308F7FDA,
	.sdram_tim3                     = 0x027F88A8,
	.read_idle_ctrl                 = 0x00050000,
	.zq_config                      = 0x0007190B,
	.temp_alert_config              = 0x00000000,
	.emif_ddr_phy_ctlr_1_init       = 0x0020400A,
	.emif_ddr_phy_ctlr_1            = 0x0E24400A,
	.emif_ddr_ext_phy_ctrl_1        = 0x04040100,
	.emif_ddr_ext_phy_ctrl_2        = 0x009D009D,
	.emif_ddr_ext_phy_ctrl_3        = 0x009D009D,
	.emif_ddr_ext_phy_ctrl_4        = 0x009D009D,
	.emif_ddr_ext_phy_ctrl_5        = 0x009D009D,
	.emif_rd_wr_lvl_rmp_win         = 0x00000000,
	.emif_rd_wr_lvl_rmp_ctl         = 0x80000000,
	.emif_rd_wr_lvl_ctl             = 0x00000000,
	.emif_rd_wr_exec_thresh         = 0x00000305
};

#define LISA_MAP_1_SYS_ADDR 		0x80	
#define LISA_MAP_1_SYS_SIZE			7		/* 0x6: 1-GiB section */
#define LISA_MAP_1_SDRC_INTL		0 		/* 128-byte interleaving */
#define LISA_MAP_1_SDRC_ADDRSPC		0
#define LISA_MAP_1_SDRC_MAP 		2		/* 0x1: Mapped on EMIF1 only (not interleaved) */
#define LISA_MAP_1_SDRC_ADDR 		0x00
//#ifdef __notdef
// #define DMM_LISA_MAP1				(LISA_MAP_1_SYS_ADDR << 24) | (LISA_MAP_1_SYS_SIZE << 20) | (LISA_MAP_1_SDRC_INTL << 18) | \
									(LISA_MAP_1_SDRC_ADDRSPC << 16) | (LISA_MAP_1_SDRC_MAP << 8) | LISA_MAP_1_SDRC_ADDR
//#endif
#define DMM_LISA_MAP1				0

#define LISA_MAP_2_SYS_ADDR 		0x80	
#define LISA_MAP_2_SYS_SIZE		7		/* 0x6: 1-GiB section */
#define LISA_MAP_2_SDRC_INTL		0 		/* 128-byte interleaving */
#define LISA_MAP_2_SDRC_ADDRSPC		0
#define LISA_MAP_2_SDRC_MAP 		3		/* 0x1: Mapped on EMIF1 only (not interleaved) */
#define LISA_MAP_2_SDRC_ADDR 		0x00

#define DMM_LISA_MAP2				(LISA_MAP_2_SYS_ADDR << 24) | (LISA_MAP_2_SYS_SIZE << 20) | (LISA_MAP_2_SDRC_INTL << 18) | \
									(LISA_MAP_2_SDRC_ADDRSPC << 16) | (LISA_MAP_2_SDRC_MAP << 8) | LISA_MAP_2_SDRC_ADDR

const struct dmm_lisa_map_regs lisa_map_4G_x_2_x_2 = {
	.dmm_lisa_map_0 = 0x0,
	.dmm_lisa_map_1 = DMM_LISA_MAP1,
	.dmm_lisa_map_2 = DMM_LISA_MAP2, /* 0x80740300, */
	/*.dmm_lisa_map_2 = 0x80640100,*/
	//.dmm_lisa_map_3 = 0xFF020100,
	.dmm_lisa_map_3 = 0xFF020100,
	.is_ma_present	= 0x1
};

/*
 * DRA752 EVM board has 1.5 GB of memory
 * EMIF1 --> 2Gb * 2 =  512MB
 * EMIF2 --> 2Gb * 4 =  1GB
 * so mapping 1GB interleaved and 512MB non-interleaved
 */
const struct dmm_lisa_map_regs lisa_map_2G_x_2_x_2_2G_x_1_x_2 = {
	.dmm_lisa_map_0 = 0x0,
	.dmm_lisa_map_1 = 0x80640300,
	.dmm_lisa_map_2 = 0xC0500220,
	.dmm_lisa_map_3 = 0xFF020100,
	.is_ma_present	= 0x1
};

/*
 * DRA752 EVM EMIF1 ONLY CONFIGURATION
 */
const struct dmm_lisa_map_regs lisa_map_2G_x_1_x_2 = {
	.dmm_lisa_map_0 = 0x0,
	.dmm_lisa_map_1 = 0x0,
	.dmm_lisa_map_2 = 0x80500100,
	.dmm_lisa_map_3 = 0xFF020100,
	.is_ma_present	= 0x1
};

/*
 * DRA752 EVM EMIF2 ONLY CONFIGURATION
 */
const struct dmm_lisa_map_regs lisa_map_2G_x_2_x_2 = {
	.dmm_lisa_map_0 = 0x0,
	.dmm_lisa_map_1 = 0x0,
	.dmm_lisa_map_2 = 0x80600200,
	.dmm_lisa_map_3 = 0xFF020100,
	.is_ma_present	= 0x1
};

static void emif_get_reg_dump_sdp(u32 emif_nr, const struct emif_regs **regs)
{
	switch (omap_revision()) {
	case OMAP5430_ES1_0:
		*regs = &emif_regs_532_mhz_2cs;
		break;
	case OMAP5432_ES1_0:
		*regs = &emif_regs_ddr3_532_mhz_1cs;
		break;
	case OMAP5430_ES2_0:
		*regs = &emif_regs_532_mhz_2cs_es2;
		break;
	case OMAP5432_ES2_0:
		*regs = &emif_regs_ddr3_532_mhz_1cs_es2;
		break;
	case DRA752_ES1_0:
		switch (emif_nr) {
		case 1:
			*regs = &emif_1_regs_ddr3_532_mhz_1cs_dra_es1;
			break;
		case 2:
			*regs = &emif_2_regs_ddr3_532_mhz_1cs_dra_es1;
			break;
		}
		break;
	default:
		*regs = &emif_1_regs_ddr3_532_mhz_1cs_dra_es1;
	}
}

void emif_get_reg_dump(u32 emif_nr, const struct emif_regs **regs)
	__attribute__((weak, alias("emif_get_reg_dump_sdp")));

static void emif_get_dmm_regs_sdp(const struct dmm_lisa_map_regs
						**dmm_lisa_regs)
{
	switch (omap_revision()) {
	case OMAP5430_ES1_0:
	case OMAP5430_ES2_0:
	case OMAP5432_ES1_0:
	case OMAP5432_ES2_0:
		*dmm_lisa_regs = &lisa_map_4G_x_2_x_2;
		break;
	case DRA752_ES1_0:
	default:
		*dmm_lisa_regs = &lisa_map_2G_x_2_x_2_2G_x_1_x_2;
	}

}

void emif_get_dmm_regs(const struct dmm_lisa_map_regs **dmm_lisa_regs)
	__attribute__((weak, alias("emif_get_dmm_regs_sdp")));
#else

static const struct lpddr2_device_details dev_4G_S4_details = {
	.type		= LPDDR2_TYPE_S4,
	.density	= LPDDR2_DENSITY_4Gb,
	.io_width	= LPDDR2_IO_WIDTH_32,
	.manufacturer	= LPDDR2_MANUFACTURER_SAMSUNG
};

static void emif_get_device_details_sdp(u32 emif_nr,
		struct lpddr2_device_details *cs0_device_details,
		struct lpddr2_device_details *cs1_device_details)
{
	/* EMIF1 & EMIF2 have identical configuration */
	*cs0_device_details = dev_4G_S4_details;
	*cs1_device_details = dev_4G_S4_details;
}

void emif_get_device_details(u32 emif_nr,
		struct lpddr2_device_details *cs0_device_details,
		struct lpddr2_device_details *cs1_device_details)
	__attribute__((weak, alias("emif_get_device_details_sdp")));

#endif /* CONFIG_SYS_EMIF_PRECALCULATED_TIMING_REGS */

const u32 ext_phy_ctrl_const_base[EMIF_EXT_PHY_CTRL_CONST_REG] = {
	0x01004010,
	0x00001004,
	0x04010040,
	0x01004010,
	0x00001004,
	0x00000000,
	0x00000000,
	0x00000000,
	0x80080080,
	0x00800800,
	0x08102040,
	0x00000001,
	0x540A8150,
	0xA81502a0,
	0x002A0540,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000077,
	0x0
};

const u32 ddr3_ext_phy_ctrl_const_base_es1[EMIF_EXT_PHY_CTRL_CONST_REG] = {
	0x01004010,
	0x00001004,
	0x04010040,
	0x01004010,
	0x00001004,
	0x00000000,
	0x00000000,
	0x00000000,
	0x80080080,
	0x00800800,
	0x08102040,
	0x00000002,
	0x0,
	0x0,
	0x0,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000057,
	0x0
};

#ifdef __notdef
const u32 ddr3_ext_phy_ctrl_const_base_es2[EMIF_EXT_PHY_CTRL_CONST_REG] = {
	0x01004010, 
	0x00001004, 
	0x04010040, 
	0x01004010, 
	0x00001004,
	0x00000000,
	0x00000000,
	0x00000000,
	0x80080080,
	0x00800800,
	0x08102040,
	0x00000002,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000057,
	0x0
};
#else
const u32 ddr3_ext_phy_ctrl_const_base_es2[EMIF_EXT_PHY_CTRL_CONST_REG] = {
	EMIF_EXT_PHY_CONTROL_6, /*0x50D4350D,*/
	EMIF_EXT_PHY_CONTROL_7, /*0x00000D43,*/
	EMIF_EXT_PHY_CONTROL_8, /*0x09024090,*/ /*0x04010040,*/
	EMIF_EXT_PHY_CONTROL_9, /*0x02409024,*/ /*0x01004010,*/
	EMIF_EXT_PHY_CONTROL_10, /*0x00002409,*/ /*0x00001004,*/
	EMIF_EXT_PHY_CONTROL_11,
	EMIF_EXT_PHY_CONTROL_12,
	EMIF_EXT_PHY_CONTROL_13,
	EMIF_EXT_PHY_CONTROL_14,
	EMIF_EXT_PHY_CONTROL_15,
	EMIF_EXT_PHY_CONTROL_16,
	EMIF_EXT_PHY_CONTROL_17,
	EMIF_EXT_PHY_CONTROL_18,
	EMIF_EXT_PHY_CONTROL_19,
	EMIF_EXT_PHY_CONTROL_20,
	EMIF_EXT_PHY_CONTROL_21,
	EMIF_EXT_PHY_CONTROL_22,
	EMIF_EXT_PHY_CONTROL_23,
	0x00000057,
	0x0
};
#endif

const u32
dra_ddr3_ext_phy_ctrl_const_base_es1_emif1[EMIF_EXT_PHY_CTRL_CONST_REG] = {
	0x009E009E,
	0x002E002E,
	0x002E002E,
	0x002E002E,
	0x002E002E,
	0x002E002E,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x004D004D,
	0x0,
	0x600020,
	0x40010080,
	0x8102040
};

const u32
dra_ddr3_ext_phy_ctrl_const_base_es1_emif2[EMIF_EXT_PHY_CTRL_CONST_REG] = {
	0x009D009D,
	0x002D002D,
	0x002D002D,
	0x002D002D,
	0x002D002D,
	0x002D002D,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x00570057,
	0x0,
	0x600020,
	0x40010080,
	0x8102040
};

const struct lpddr2_mr_regs mr_regs = {
	.mr1	= MR1_BL_8_BT_SEQ_WRAP_EN_NWR_8,
	.mr2	= 0x6,
	.mr3	= 0x1,
	.mr10	= MR10_ZQ_ZQINIT,
	.mr16	= MR16_REF_FULL_ARRAY
};

static void emif_get_ext_phy_ctrl_const_regs(u32 emif_nr, const u32 **regs)
{
	switch (omap_revision()) {
	case OMAP5430_ES1_0:
	case OMAP5430_ES2_0:
		*regs = ext_phy_ctrl_const_base;
		break;
	case OMAP5432_ES1_0:
		*regs = ddr3_ext_phy_ctrl_const_base_es1;
		break;
	case OMAP5432_ES2_0:
		*regs = ddr3_ext_phy_ctrl_const_base_es2;
		break;
	case DRA752_ES1_0:
		if (emif_nr == 1)
			*regs = dra_ddr3_ext_phy_ctrl_const_base_es1_emif1;
		else
			*regs = dra_ddr3_ext_phy_ctrl_const_base_es1_emif2;
		break;
	default:
		*regs = ddr3_ext_phy_ctrl_const_base_es2;

	}
}

void get_lpddr2_mr_regs(const struct lpddr2_mr_regs **regs)
{
	*regs = &mr_regs;
}

void do_ext_phy_settings(u32 base, const struct emif_regs *regs)
{
	u32 *ext_phy_ctrl_base = 0;
	u32 *emif_ext_phy_ctrl_base = 0;
	u32 emif_nr;
	const u32 *ext_phy_ctrl_const_regs;
	u32 i = 0;

	emif_nr = (base == EMIF1_BASE) ? 1 : 2;

	struct emif_reg_struct *emif = (struct emif_reg_struct *)base;

	ext_phy_ctrl_base = (u32 *) &(regs->emif_ddr_ext_phy_ctrl_1);
	emif_ext_phy_ctrl_base = (u32 *) &(emif->emif_ddr_ext_phy_ctrl_1);

	/* Configure external phy control timing registers */
	for (i = 0; i < EMIF_EXT_PHY_CTRL_TIMING_REG; i++) {
		writel(*ext_phy_ctrl_base, emif_ext_phy_ctrl_base++);
		/* Update shadow registers */
		writel(*ext_phy_ctrl_base++, emif_ext_phy_ctrl_base++);
	}

	/*
	 * external phy 6-24 registers do not change with
	 * ddr frequency
	 */
	emif_get_ext_phy_ctrl_const_regs(emif_nr, &ext_phy_ctrl_const_regs);
	for (i = 0; i < EMIF_EXT_PHY_CTRL_CONST_REG; i++) {
		writel(ext_phy_ctrl_const_regs[i],
		       emif_ext_phy_ctrl_base++);
		/* Update shadow registers */
		writel(ext_phy_ctrl_const_regs[i],
		       emif_ext_phy_ctrl_base++);
	}
}

#ifndef CONFIG_SYS_DEFAULT_LPDDR2_TIMINGS
static const struct lpddr2_ac_timings timings_jedec_532_mhz = {
	.max_freq	= 532000000,
	.RL		= 8,
	.tRPab		= 21,
	.tRCD		= 18,
	.tWR		= 15,
	.tRASmin	= 42,
	.tRRD		= 10,
	.tWTRx2		= 15,
	.tXSR		= 140,
	.tXPx2		= 15,
	.tRFCab		= 130,
	.tRTPx2		= 15,
	.tCKE		= 3,
	.tCKESR		= 15,
	.tZQCS		= 90,
	.tZQCL		= 360,
	.tZQINIT	= 1000,
	.tDQSCKMAXx2	= 11,
	.tRASmax	= 70,
	.tFAW		= 50
};

static const struct lpddr2_min_tck min_tck = {
	.tRL		= 3,
	.tRP_AB		= 3,
	.tRCD		= 3,
	.tWR		= 3,
	.tRAS_MIN	= 3,
	.tRRD		= 2,
	.tWTR		= 2,
	.tXP		= 2,
	.tRTP		= 2,
	.tCKE		= 3,
	.tCKESR		= 3,
	.tFAW		= 8
};

static const struct lpddr2_ac_timings *ac_timings[MAX_NUM_SPEEDBINS] = {
	&timings_jedec_532_mhz
};

static const struct lpddr2_device_timings dev_4G_S4_timings = {
	.ac_timings	= ac_timings,
	.min_tck	= &min_tck,
};

void emif_get_device_timings_sdp(u32 emif_nr,
		const struct lpddr2_device_timings **cs0_device_timings,
		const struct lpddr2_device_timings **cs1_device_timings)
{
	/* Identical devices on EMIF1 & EMIF2 */
	*cs0_device_timings = &dev_4G_S4_timings;
	*cs1_device_timings = &dev_4G_S4_timings;
}

void emif_get_device_timings(u32 emif_nr,
		const struct lpddr2_device_timings **cs0_device_timings,
		const struct lpddr2_device_timings **cs1_device_timings)
	__attribute__((weak, alias("emif_get_device_timings_sdp")));

#endif /* CONFIG_SYS_DEFAULT_LPDDR2_TIMINGS */
