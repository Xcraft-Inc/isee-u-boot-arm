/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 * (C) Copyright 2013
 * ISEE, <www.isee.biz>
 *
 * Aneesh V <aneesh@ti.com>
 * Manel Caro <mcaro@iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <spl.h>
#include <asm/u-boot.h>
#include <fat.h>
#include <version.h>
#include <image.h>

DECLARE_GLOBAL_DATA_PTR;

static int sata_load_image_raw(block_dev_desc_t *sata, unsigned long sector)
{
	unsigned long err;
	u32 image_size_sectors;
	struct image_header *header;

	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE -
						sizeof(struct image_header));

	/* read image header to find the image size & load address */
	err = sata->block_read(0, sector, 1, header);
	if (err == 0)
		goto end;

	if (image_get_magic(header) != IH_MAGIC)
		return -1;

	spl_parse_image_header(header);

	/* convert size to sectors - round up */
	image_size_sectors = (spl_image.size + sata->blksz - 1) /
				sata->blksz;

	/* Read the header too to avoid extra memcpy */
	err = sata->block_read(0, sector, image_size_sectors,
					(void *)spl_image.load_addr);

end:
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
	if (err == 0)
		printf("spl: SATA blk read err - %lu\n", err);
#endif

	return (err == 0);
}

#ifdef CONFIG_SPL_OS_BOOT
static int sata_load_image_raw_os(block_dev_desc_t *sata)
{
	if (!sata->block_read(0,
				       CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR,
				       CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS,
				       (void *)CONFIG_SYS_SPL_ARGS_ADDR)) {
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
		printf("SATA args blk read error\n");
#endif
		return -1;
	}

	return sata_load_image_raw(sata, CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR);
}
#endif

#ifdef CONFIG_SPL_FAT_SUPPORT
static int sata_load_image_fat(block_dev_desc_t * sata, const char *filename)
{
	int err;
	struct image_header *header;

	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE -
						sizeof(struct image_header));

	err = file_fat_read(filename, header, sizeof(struct image_header));
	if (err <= 0)
		goto end;

	spl_parse_image_header(header);

	err = file_fat_read(filename, (u8 *)spl_image.load_addr, 0);

end:
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
	if (err <= 0)
		printf("spl: error reading image %s, err - %d\n",
		       filename, err);
#endif

	return (err <= 0);
}

#ifdef CONFIG_SPL_OS_BOOT
static int sata_load_image_fat_os(block_dev_desc_t *sata)
{
	int err;

	err = file_fat_read(CONFIG_SPL_FAT_LOAD_ARGS_NAME,
			    (void *)CONFIG_SYS_SPL_ARGS_ADDR, 0);
	if (err <= 0) {
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
		printf("spl: error reading image %s, err - %d\n",
		       CONFIG_SPL_FAT_LOAD_ARGS_NAME, err);
#endif
		return -1;
	}

	return sata_load_image_fat(sata, CONFIG_SPL_FAT_LOAD_KERNEL_NAME);
}
#endif

#endif

void spl_sata_load_image(void)
{
	block_dev_desc_t *sata = spl_scsi_get_dev(0);
	int err;
	u32 boot_mode;

	boot_mode = spl_boot_mode();
	if (boot_mode == MMCSD_MODE_UNDEFINED)
		boot_mode = MMCSD_MODE_FAT;	/* If boot_mode is undefined then try with FAT */
	if (boot_mode == MMCSD_MODE_RAW) {
		debug("boot mode - RAW\n");
#ifdef CONFIG_SPL_OS_BOOT
		if (spl_start_uboot() || sata_load_image_raw_os(sata))
#endif
		err = sata_load_image_raw(sata,
					 CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR);
#ifdef CONFIG_SPL_FAT_SUPPORT
	} else if (boot_mode == MMCSD_MODE_FAT) {
		debug("boot mode - FAT\n");

		err = fat_register_device(sata,
					  CONFIG_SYS_MMC_SD_FAT_BOOT_PARTITION);
		if (err) {
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
			printf("spl: fat register err - %d\n", err);
#endif
			hang();
		}

#ifdef CONFIG_SPL_OS_BOOT
		if (spl_start_uboot() || sata_load_image_fat_os(sata))
#endif
		err = sata_load_image_fat(sata, CONFIG_SPL_FAT_LOAD_PAYLOAD_NAME);
#endif
	} else {
#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
		puts("spl: wrong SATA boot mode\n");
#endif
		hang();
	}

	if (err)
		hang();
}
