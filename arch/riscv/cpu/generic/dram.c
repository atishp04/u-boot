// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018, Bin Meng <bmeng.cn@gmail.com>
 */

#include <common.h>
#include <fdtdec.h>
#include <init.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	int rc;
#ifdef CONFIG_32BIT
	unsigned long usable_mem_end = 0;
	unsigned long dram_end;
#endif
	rc = fdtdec_setup_mem_size_base();
	if (rc < 0)
		return rc;

#ifdef CONFIG_32BIT
	dram_end = gd->ram_base + gd->ram_size - 1;
	usable_mem_end = gd->ram_base + SZ_1G;
	if (usable_mem_end < dram_end)
		gd->ram_size = gd->ram_size - (dram_end - usable_mem_end - 1);
#endif

	return 0;
}

int dram_init_banksize(void)
{
	return fdtdec_setup_memory_banksize();
}

ulong board_get_usable_ram_top(ulong total_size)
{
#ifdef CONFIG_64BIT
	/*
	 * Ensure that we run from first 4GB so that all
	 * addresses used by U-Boot are 32bit addresses.
	 *
	 * This in-turn ensures that 32bit DMA capable
	 * devices work fine because DMA mapping APIs will
	 * provide 32bit DMA addresses only.
	 */
	if (gd->ram_top > SZ_4G)
		return SZ_4G;
#endif
	return gd->ram_top;
}
