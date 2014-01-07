/*
 * (C) Copyright 2001
 * Denis Peter, MPL AG Switzerland
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * SCSI support.
 */
#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <scsi.h>
#include <image.h>

#ifdef CONFIG_SCSI_DEV_LIST
#define SCSI_DEV_LIST CONFIG_SCSI_DEV_LIST
#else
#ifdef CONFIG_SCSI_SYM53C8XX
#define SCSI_VEND_ID	0x1000
#ifndef CONFIG_SCSI_DEV_ID
#define SCSI_DEV_ID		0x0001
#else
#define SCSI_DEV_ID		CONFIG_SCSI_DEV_ID
#endif
#elif defined CONFIG_SATA_ULI5288

#define SCSI_VEND_ID 0x10b9
#define SCSI_DEV_ID  0x5288

#elif !defined(CONFIG_SCSI_AHCI_PLAT)
#error no scsi device defined
#endif
#define SCSI_DEV_LIST {SCSI_VEND_ID, SCSI_DEV_ID}
#endif

static ccb tempccb;	/* temporary scsi command buffer */

static unsigned char tempbuff[512]; /* temporary data buffer */

static int scsi_max_devs; /* number of highest available scsi device */

static int scsi_curr_dev; /* current device */

static block_dev_desc_t scsi_dev_desc[CONFIG_SYS_SCSI_MAX_DEVICE];

/********************************************************************************
 *  forward declerations of some Setup Routines
 */
void spl_scsi_setup_test_unit_ready(ccb * pccb);
void spl_scsi_setup_read6(ccb * pccb, unsigned long start, unsigned short blocks);
void spl_scsi_setup_read_ext(ccb * pccb, unsigned long start, unsigned short blocks);
static void spl_scsi_setup_write_ext(ccb *pccb, unsigned long start,
			  unsigned short blocks);
void spl_scsi_setup_inquiry(ccb * pccb);
void spl_scsi_ident_cpy (unsigned char *dest, unsigned char *src, unsigned int len);


static int spl_scsi_read_capacity(ccb *pccb, lbaint_t *capacity,
			      unsigned long *blksz);
static ulong spl_scsi_read(int device, lbaint_t blknr, lbaint_t blkcnt,
		       void *buffer);
static ulong spl_scsi_write(int device, lbaint_t blknr,
			lbaint_t blkcnt, const void *buffer);


/*********************************************************************************
 * (re)-scan the scsi bus and reports scsi device info
 * to the user if mode = 1
 */
void spl_scsi_scan(int mode)
{
	unsigned char i,perq,modi,lun;
	lbaint_t capacity;
	unsigned long blksz;
	ccb* pccb=(ccb *)&tempccb;

	if(mode==1) {
		printf("scanning bus for devices...\n");
	}
	for(i=0;i<CONFIG_SYS_SCSI_MAX_DEVICE;i++) {
		scsi_dev_desc[i].target=0xff;
		scsi_dev_desc[i].lun=0xff;
		scsi_dev_desc[i].lba=0;
		scsi_dev_desc[i].blksz=0;
		scsi_dev_desc[i].log2blksz =
			LOG2_INVALID(typeof(scsi_dev_desc[i].log2blksz));
		scsi_dev_desc[i].type=DEV_TYPE_UNKNOWN;
		scsi_dev_desc[i].vendor[0]=0;
		scsi_dev_desc[i].product[0]=0;
		scsi_dev_desc[i].revision[0]=0;
		scsi_dev_desc[i].removable = false;
		scsi_dev_desc[i].if_type=IF_TYPE_SCSI;
		scsi_dev_desc[i].dev=i;
		scsi_dev_desc[i].part_type=PART_TYPE_UNKNOWN;
		scsi_dev_desc[i].block_read=spl_scsi_read;
		scsi_dev_desc[i].block_write = spl_scsi_write;
	}
	scsi_max_devs=0;
	for(i=0;i<CONFIG_SYS_SCSI_MAX_SCSI_ID;i++) {
		pccb->target=i;
		for(lun=0;lun<CONFIG_SYS_SCSI_MAX_LUN;lun++) {
			pccb->lun=lun;
			pccb->pdata=(unsigned char *)&tempbuff;
			pccb->datalen=512;
			spl_scsi_setup_inquiry(pccb);
			if (scsi_exec(pccb) != true) {
				if(pccb->contr_stat==SCSI_SEL_TIME_OUT) {
					debug ("Selection timeout ID %d\n",pccb->target);
					continue; /* selection timeout => assuming no device present */
				}
				scsi_print_error(pccb);
				continue;
			}
			perq=tempbuff[0];
			modi=tempbuff[1];
			if((perq & 0x1f)==0x1f) {
				continue; /* skip unknown devices */
			}
			if((modi&0x80)==0x80) /* drive is removable */
				scsi_dev_desc[scsi_max_devs].removable=true;
			/* get info for this device */
			spl_scsi_ident_cpy((unsigned char *)&scsi_dev_desc[scsi_max_devs].vendor[0],
				       &tempbuff[8], 8);
			spl_scsi_ident_cpy((unsigned char *)&scsi_dev_desc[scsi_max_devs].product[0],
				       &tempbuff[16], 16);
			spl_scsi_ident_cpy((unsigned char *)&scsi_dev_desc[scsi_max_devs].revision[0],
				       &tempbuff[32], 4);
			scsi_dev_desc[scsi_max_devs].target=pccb->target;
			scsi_dev_desc[scsi_max_devs].lun=pccb->lun;

			pccb->datalen=0;
			spl_scsi_setup_test_unit_ready(pccb);
			if (scsi_exec(pccb) != true) {
				if (scsi_dev_desc[scsi_max_devs].removable == true) {
					scsi_dev_desc[scsi_max_devs].type=perq;
					goto removable;
				}
				scsi_print_error(pccb);
				continue;
			}
			if (spl_scsi_read_capacity(pccb, &capacity, &blksz)) {
				scsi_print_error(pccb);
				continue;
			}
			scsi_dev_desc[scsi_max_devs].lba=capacity;
			scsi_dev_desc[scsi_max_devs].blksz=blksz;
			scsi_dev_desc[scsi_max_devs].log2blksz =
				LOG2(scsi_dev_desc[scsi_max_devs].blksz);
			scsi_dev_desc[scsi_max_devs].type=perq;
			init_part(&scsi_dev_desc[scsi_max_devs]);
removable:
			if(mode==1) {
				printf ("  Device %d: ", scsi_max_devs);
				dev_print(&scsi_dev_desc[scsi_max_devs]);
			} /* if mode */
			scsi_max_devs++;
		} /* next LUN */
	}
	if(scsi_max_devs>0)
		scsi_curr_dev=0;
	else
		scsi_curr_dev = -1;

	printf("Found %d device(s).\n", scsi_max_devs);
	// setenv_ulong("scsidevs", scsi_max_devs);
}

int spl_scsi_get_disk_count(void)
{
	return scsi_max_devs;
}



#ifdef CONFIG_PARTITIONS
block_dev_desc_t * spl_scsi_get_dev(int dev)
{
	return (dev < CONFIG_SYS_SCSI_MAX_DEVICE) ? &scsi_dev_desc[dev] : NULL;
}
#endif

#ifdef __notdef
/******************************************************************************
 * scsi boot command intepreter. Derived from diskboot
 */
int spl_do_scsiboot (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return common_diskboot(cmdtp, "scsi", argc, argv);
}

/*********************************************************************************
 * scsi command intepreter
 */
int spl_do_scsi (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	switch (argc) {
	case 0:
	case 1:
		return CMD_RET_USAGE;

	case 2:
			if (strncmp(argv[1],"res",3) == 0) {
				printf("\nReset SCSI\n");
				scsi_bus_reset();
				scsi_scan(1);
				return 0;
			}
			if (strncmp(argv[1],"inf",3) == 0) {
				int i;
				for (i=0; i<CONFIG_SYS_SCSI_MAX_DEVICE; ++i) {
					if(scsi_dev_desc[i].type==DEV_TYPE_UNKNOWN)
						continue; /* list only known devices */
					printf ("SCSI dev. %d:  ", i);
					dev_print(&scsi_dev_desc[i]);
				}
				return 0;
			}
			if (strncmp(argv[1],"dev",3) == 0) {
				if ((scsi_curr_dev < 0) || (scsi_curr_dev >= CONFIG_SYS_SCSI_MAX_DEVICE)) {
					printf("\nno SCSI devices available\n");
					return 1;
				}
				printf ("\n    Device %d: ", scsi_curr_dev);
				dev_print(&scsi_dev_desc[scsi_curr_dev]);
				return 0;
			}
			if (strncmp(argv[1],"scan",4) == 0) {
				scsi_scan(1);
				return 0;
			}
			if (strncmp(argv[1],"part",4) == 0) {
				int dev, ok;
				for (ok=0, dev=0; dev<CONFIG_SYS_SCSI_MAX_DEVICE; ++dev) {
					if (scsi_dev_desc[dev].type!=DEV_TYPE_UNKNOWN) {
						ok++;
						if (dev)
							printf("\n");
						debug ("print_part of %x\n",dev);
							print_part(&scsi_dev_desc[dev]);
					}
				}
				if (!ok)
					printf("\nno SCSI devices available\n");
				return 1;
			}
			return CMD_RET_USAGE;
	case 3:
			if (strncmp(argv[1],"dev",3) == 0) {
				int dev = (int)simple_strtoul(argv[2], NULL, 10);
				printf ("\nSCSI device %d: ", dev);
				if (dev >= CONFIG_SYS_SCSI_MAX_DEVICE) {
					printf("unknown device\n");
					return 1;
				}
				printf ("\n    Device %d: ", dev);
				dev_print(&scsi_dev_desc[dev]);
				if(scsi_dev_desc[dev].type == DEV_TYPE_UNKNOWN) {
					return 1;
				}
				scsi_curr_dev = dev;
				printf("... is now current device\n");
				return 0;
			}
			if (strncmp(argv[1],"part",4) == 0) {
				int dev = (int)simple_strtoul(argv[2], NULL, 10);
				if(scsi_dev_desc[dev].type != DEV_TYPE_UNKNOWN) {
					print_part(&scsi_dev_desc[dev]);
				}
				else {
					printf ("\nSCSI device %d not available\n", dev);
				}
				return 1;
			}
			return CMD_RET_USAGE;
    default:
			/* at least 4 args */
			if (strcmp(argv[1],"read") == 0) {
				ulong addr = simple_strtoul(argv[2], NULL, 16);
				ulong blk  = simple_strtoul(argv[3], NULL, 16);
				ulong cnt  = simple_strtoul(argv[4], NULL, 16);
				ulong n;
				printf ("\nSCSI read: device %d block # %ld, count %ld ... ",
						scsi_curr_dev, blk, cnt);
				n = spl_scsi_read(scsi_curr_dev, blk, cnt, (ulong *)addr);
				printf ("%ld blocks read: %s\n",n,(n==cnt) ? "OK" : "ERROR");
				return 0;
			} else if (strcmp(argv[1], "write") == 0) {
				ulong addr = simple_strtoul(argv[2], NULL, 16);
				ulong blk = simple_strtoul(argv[3], NULL, 16);
				ulong cnt = simple_strtoul(argv[4], NULL, 16);
				ulong n;
				printf("\nSCSI write: device %d block # %ld, "
				       "count %ld ... ",
				       scsi_curr_dev, blk, cnt);
				n = scsi_write(scsi_curr_dev, blk, cnt,
					       (ulong *)addr);
				printf("%ld blocks written: %s\n", n,
				       (n == cnt) ? "OK" : "ERROR");
				return 0;
			}
	} /* switch */
	return CMD_RET_USAGE;
}
#endif

/****************************************************************************************
 * scsi_read
 */

#define SCSI_MAX_READ_BLK 0xFFFF /* almost the maximum amount of the scsi_ext command.. */

static ulong spl_scsi_read(int device, lbaint_t blknr, lbaint_t blkcnt,
		       void *buffer)
{
	lbaint_t start, blks;
	uintptr_t buf_addr;
	unsigned short smallblks;
	ccb* pccb=(ccb *)&tempccb;
	device&=0xff;
	/* Setup  device
	 */
	pccb->target=scsi_dev_desc[device].target;
	pccb->lun=scsi_dev_desc[device].lun;
	buf_addr=(unsigned long)buffer;
	start=blknr;
	blks=blkcnt;
	debug("\nscsi_read: dev %d startblk " LBAF
	      ", blccnt " LBAF " buffer %lx\n",
	      device, start, blks, (unsigned long)buffer);
	do {
		pccb->pdata=(unsigned char *)buf_addr;
		if(blks>SCSI_MAX_READ_BLK) {
			pccb->datalen=scsi_dev_desc[device].blksz * SCSI_MAX_READ_BLK;
			smallblks=SCSI_MAX_READ_BLK;
			spl_scsi_setup_read_ext(pccb,start,smallblks);
			start+=SCSI_MAX_READ_BLK;
			blks-=SCSI_MAX_READ_BLK;
		}
		else {
			pccb->datalen=scsi_dev_desc[device].blksz * blks;
			smallblks=(unsigned short) blks;
			spl_scsi_setup_read_ext(pccb,start,smallblks);
			start+=blks;
			blks=0;
		}
		debug("scsi_read_ext: startblk " LBAF
		      ", blccnt %x buffer %lx\n",
		      start, smallblks, buf_addr);
		if (scsi_exec(pccb) != true) {
			scsi_print_error(pccb);
			blkcnt-=blks;
			break;
		}
		buf_addr+=pccb->datalen;
	} while(blks!=0);
	debug("scsi_read_ext: end startblk " LBAF
	      ", blccnt %x buffer %lx\n", start, smallblks, buf_addr);
	return(blkcnt);
}

/*******************************************************************************
 * scsi_write
 */

/* Almost the maximum amount of the scsi_ext command.. */
#define SCSI_MAX_WRITE_BLK 0xFFFF

static ulong spl_scsi_write(int device, lbaint_t blknr,
			lbaint_t blkcnt, const void *buffer)
{
	lbaint_t start, blks;
	uintptr_t buf_addr;
	unsigned short smallblks;
	ccb* pccb = (ccb *)&tempccb;
	device &= 0xff;
	/* Setup  device
	 */
	pccb->target = scsi_dev_desc[device].target;
	pccb->lun = scsi_dev_desc[device].lun;
	buf_addr = (unsigned long)buffer;
	start = blknr;
	blks = blkcnt;
	debug("\n%s: dev %d startblk " LBAF ", blccnt " LBAF " buffer %lx\n",
	      __func__, device, start, blks, (unsigned long)buffer);
	do {
		pccb->pdata = (unsigned char *)buf_addr;
		if (blks > SCSI_MAX_WRITE_BLK) {
			pccb->datalen = (scsi_dev_desc[device].blksz *
					 SCSI_MAX_WRITE_BLK);
			smallblks = SCSI_MAX_WRITE_BLK;
			spl_scsi_setup_write_ext(pccb, start, smallblks);
			start += SCSI_MAX_WRITE_BLK;
			blks -= SCSI_MAX_WRITE_BLK;
		} else {
			pccb->datalen = scsi_dev_desc[device].blksz * blks;
			smallblks = (unsigned short)blks;
			spl_scsi_setup_write_ext(pccb, start, smallblks);
			start += blks;
			blks = 0;
		}
		debug("%s: startblk " LBAF ", blccnt %x buffer %lx\n",
		      __func__, start, smallblks, buf_addr);
		if (scsi_exec(pccb) != true) {
			scsi_print_error(pccb);
			blkcnt -= blks;
			break;
		}
		buf_addr += pccb->datalen;
	} while (blks != 0);
	debug("%s: end startblk " LBAF ", blccnt %x buffer %lx\n",
	      __func__, start, smallblks, buf_addr);
	return blkcnt;
}

/* copy src to dest, skipping leading and trailing blanks
 * and null terminate the string
 */
void spl_scsi_ident_cpy (unsigned char *dest, unsigned char *src, unsigned int len)
{
	int start,end;

	start=0;
	while(start<len) {
		if(src[start]!=' ')
			break;
		start++;
	}
	end=len-1;
	while(end>start) {
		if(src[end]!=' ')
			break;
		end--;
	}
	for( ; start<=end; start++) {
		*dest++=src[start];
	}
	*dest='\0';
}


/* Trim trailing blanks, and NUL-terminate string
 */
void spl_scsi_trim_trail (unsigned char *str, unsigned int len)
{
	unsigned char *p = str + len - 1;

	while (len-- > 0) {
		*p-- = '\0';
		if (*p != ' ') {
			return;
		}
	}
}

int spl_scsi_read_capacity(ccb *pccb, lbaint_t *capacity, unsigned long *blksz)
{
	*capacity = 0;

	memset(pccb->cmd, 0, sizeof(pccb->cmd));
	pccb->cmd[0] = SCSI_RD_CAPAC10;
	pccb->cmd[1] = pccb->lun << 5;
	pccb->cmdlen = 10;
	pccb->msgout[0] = SCSI_IDENTIFY; /* NOT USED */

	pccb->datalen = 8;
	if (scsi_exec(pccb) != true)
		return 1;

	*capacity = ((lbaint_t)pccb->pdata[0] << 24) |
		    ((lbaint_t)pccb->pdata[1] << 16) |
		    ((lbaint_t)pccb->pdata[2] << 8)  |
		    ((lbaint_t)pccb->pdata[3]);

	if (*capacity != 0xffffffff) {
		/* Read capacity (10) was sufficient for this drive. */
		*blksz = ((unsigned long)pccb->pdata[4] << 24) |
			 ((unsigned long)pccb->pdata[5] << 16) |
			 ((unsigned long)pccb->pdata[6] << 8)  |
			 ((unsigned long)pccb->pdata[7]);
		return 0;
	}

	/* Read capacity (10) was insufficient. Use read capacity (16). */

	memset(pccb->cmd, 0, sizeof(pccb->cmd));
	pccb->cmd[0] = SCSI_RD_CAPAC16;
	pccb->cmd[1] = 0x10;
	pccb->cmdlen = 16;
	pccb->msgout[0] = SCSI_IDENTIFY; /* NOT USED */

	pccb->datalen = 16;
	if (scsi_exec(pccb) != true)
		return 1;

	*capacity = ((uint64_t)pccb->pdata[0] << 56) |
		    ((uint64_t)pccb->pdata[1] << 48) |
		    ((uint64_t)pccb->pdata[2] << 40) |
		    ((uint64_t)pccb->pdata[3] << 32) |
		    ((uint64_t)pccb->pdata[4] << 24) |
		    ((uint64_t)pccb->pdata[5] << 16) |
		    ((uint64_t)pccb->pdata[6] << 8)  |
		    ((uint64_t)pccb->pdata[7]);

	*blksz = ((uint64_t)pccb->pdata[8]  << 56) |
		 ((uint64_t)pccb->pdata[9]  << 48) |
		 ((uint64_t)pccb->pdata[10] << 40) |
		 ((uint64_t)pccb->pdata[11] << 32) |
		 ((uint64_t)pccb->pdata[12] << 24) |
		 ((uint64_t)pccb->pdata[13] << 16) |
		 ((uint64_t)pccb->pdata[14] << 8)  |
		 ((uint64_t)pccb->pdata[15]);

	return 0;
}


/************************************************************************************
 * Some setup (fill-in) routines
 */
void spl_scsi_setup_test_unit_ready(ccb * pccb)
{
	pccb->cmd[0]=SCSI_TST_U_RDY;
	pccb->cmd[1]=pccb->lun<<5;
	pccb->cmd[2]=0;
	pccb->cmd[3]=0;
	pccb->cmd[4]=0;
	pccb->cmd[5]=0;
	pccb->cmdlen=6;
	pccb->msgout[0]=SCSI_IDENTIFY; /* NOT USED */
}

void spl_scsi_setup_read_ext(ccb * pccb, unsigned long start, unsigned short blocks)
{
	pccb->cmd[0]=SCSI_READ10;
	pccb->cmd[1]=pccb->lun<<5;
	pccb->cmd[2]=((unsigned char) (start>>24))&0xff;
	pccb->cmd[3]=((unsigned char) (start>>16))&0xff;
	pccb->cmd[4]=((unsigned char) (start>>8))&0xff;
	pccb->cmd[5]=((unsigned char) (start))&0xff;
	pccb->cmd[6]=0;
	pccb->cmd[7]=((unsigned char) (blocks>>8))&0xff;
	pccb->cmd[8]=(unsigned char) blocks & 0xff;
	pccb->cmd[6]=0;
	pccb->cmdlen=10;
	pccb->msgout[0]=SCSI_IDENTIFY; /* NOT USED */
	debug ("scsi_setup_read_ext: cmd: %02X %02X startblk %02X%02X%02X%02X blccnt %02X%02X\n",
		pccb->cmd[0],pccb->cmd[1],
		pccb->cmd[2],pccb->cmd[3],pccb->cmd[4],pccb->cmd[5],
		pccb->cmd[7],pccb->cmd[8]);
}

void spl_scsi_setup_write_ext(ccb *pccb, unsigned long start, unsigned short blocks)
{
	pccb->cmd[0] = SCSI_WRITE10;
	pccb->cmd[1] = pccb->lun << 5;
	pccb->cmd[2] = ((unsigned char) (start>>24)) & 0xff;
	pccb->cmd[3] = ((unsigned char) (start>>16)) & 0xff;
	pccb->cmd[4] = ((unsigned char) (start>>8)) & 0xff;
	pccb->cmd[5] = ((unsigned char) (start)) & 0xff;
	pccb->cmd[6] = 0;
	pccb->cmd[7] = ((unsigned char) (blocks>>8)) & 0xff;
	pccb->cmd[8] = (unsigned char)blocks & 0xff;
	pccb->cmd[9] = 0;
	pccb->cmdlen = 10;
	pccb->msgout[0] = SCSI_IDENTIFY;  /* NOT USED */
	debug("%s: cmd: %02X %02X startblk %02X%02X%02X%02X blccnt %02X%02X\n",
	      __func__,
	      pccb->cmd[0], pccb->cmd[1],
	      pccb->cmd[2], pccb->cmd[3], pccb->cmd[4], pccb->cmd[5],
	      pccb->cmd[7], pccb->cmd[8]);
}

void spl_scsi_setup_read6(ccb * pccb, unsigned long start, unsigned short blocks)
{
	pccb->cmd[0]=SCSI_READ6;
	pccb->cmd[1]=pccb->lun<<5 | (((unsigned char)(start>>16))&0x1f);
	pccb->cmd[2]=((unsigned char) (start>>8))&0xff;
	pccb->cmd[3]=((unsigned char) (start))&0xff;
	pccb->cmd[4]=(unsigned char) blocks & 0xff;
	pccb->cmd[5]=0;
	pccb->cmdlen=6;
	pccb->msgout[0]=SCSI_IDENTIFY; /* NOT USED */
	debug ("scsi_setup_read6: cmd: %02X %02X startblk %02X%02X blccnt %02X\n",
		pccb->cmd[0],pccb->cmd[1],
		pccb->cmd[2],pccb->cmd[3],pccb->cmd[4]);
}


void spl_scsi_setup_inquiry(ccb * pccb)
{
	pccb->cmd[0]=SCSI_INQUIRY;
	pccb->cmd[1]=pccb->lun<<5;
	pccb->cmd[2]=0;
	pccb->cmd[3]=0;
	if(pccb->datalen>255)
		pccb->cmd[4]=255;
	else
		pccb->cmd[4]=(unsigned char)pccb->datalen;
	pccb->cmd[5]=0;
	pccb->cmdlen=6;
	pccb->msgout[0]=SCSI_IDENTIFY; /* NOT USED */
}


