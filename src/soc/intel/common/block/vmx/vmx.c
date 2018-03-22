/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2017 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

//#include <assert.h>
#include <console/console.h>
#include <cpu/x86/msr.h>
//#include <cpu/x86/mtrr.h>
//#include <cpu/intel/microcode.h>
//#include <intelblocks/mp_init.h>
#include <intelblocks/msr.h>
#include <intelblocks/vmx.h>
//#include <intelblocks/systemagent.h>
#include <soc/cpu.h>
//#include <soc/pci_devs.h>
#include <string.h>

static bool vmx_param_valid;
static struct vmx_param g_vmx_param;

static const struct vmx_param *get_vmx_param(void)
{
	if (vmx_param_valid)
		return &g_vmx_param;

	memset(&g_vmx_param, 0, sizeof(g_vmx_param));
	if (soc_fill_vmx_param(&g_vmx_param) < 0) {
		printk(BIOS_ERR, "VMX : Failed to get soc vmx param\n");
		return NULL;
	}
	vmx_param_valid = true;
	printk(BIOS_INFO, "VMX : param.enable = %d\n", g_vmx_param.enable);

	return &g_vmx_param;
}

static int soc_vmx_enabled(void)
{
	const struct vmx_param *vmx_param = get_vmx_param();
	return vmx_param ? vmx_param->enable : 0;
}

static int is_vmx_supported(struct cpuid_result regs)
{
	/* Check that VMX is supported */
	if (!(regs.ecx & CPUID_VMX)) {
		printk(BIOS_DEBUG, "CPU doesn't support VMX\n");
		return 0;
	}
	return 1;
}

void vmx_configure(void)
{
	msr_t msr;
	struct cpuid_result regs;

	regs = cpuid(1);

	if (!soc_vmx_enabled() || !is_vmx_supported(regs)) {
		printk(BIOS_ERR, "VMX: pre-conditions not met\n");
		return;
	}

	msr = rdmsr(IA32_FEATURE_CONTROL);

	/* Only enable it when it is not locked */
	if ((msr.lo & FEATURE_CONTROL_LOCK) == 0) {
		/* Enable VMX (and SMX, if supported) */
		msr.lo |= FEATURE_ENABLE_VMX;
		if (regs.ecx & CPUID_SMX)
			msr.lo |= FEATURE_ENABLE_SMX;
		wrmsr(IA32_FEATURE_CONTROL, msr);
	} else {
		printk(BIOS_ERR, "VMX: feature control locked, cannot set\n");
	}

	/* Report current status */
	msr = rdmsr(IA32_FEATURE_CONTROL);
	printk(BIOS_DEBUG, "VMX status: %s, %s\n",
		(msr.lo & FEATURE_ENABLE_VMX) ? "enabled" : "disabled",
		(msr.lo & FEATURE_CONTROL_LOCK) ? "locked" : "unlocked");
}
