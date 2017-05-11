/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Intel Corporation.
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

#include <console/console.h>
#include <soc/ramstage.h>
#include <fsp/gop.h>
#include <stdlib.h>
#include <string.h>

int init_igd_opregion(igd_opregion_t *opregion)
{
	const optionrom_vbt_t *vbt;
	uint32_t vbt_len;

	memset(opregion, 0, sizeof(igd_opregion_t));

	/* Read VBT table from flash */
	vbt = fsp_get_vbt(&vbt_len);
	if (!vbt)
		die("vbt data not found");

	memcpy(&opregion->header.signature, IGD_OPREGION_SIGNATURE,
		sizeof(IGD_OPREGION_SIGNATURE) - 1);
	memcpy(opregion->header.vbios_version, vbt->coreblock_biosbuild,
		sizeof(u32));
	memcpy(opregion->vbt.gvd1, vbt, vbt->hdr_vbt_size <
		sizeof(opregion->vbt.gvd1) ? vbt->hdr_vbt_size :
		sizeof(opregion->vbt.gvd1));

	/* Size, in KB, of the entire OpRegion structure (including header)*/
	opregion->header.size = sizeof(igd_opregion_t) / KiB;
	opregion->header.version = (IGD_OPREGION_VERSION << 24);
	opregion->header.pcon = 279;

	/* We just assume we're mobile for now */
	opregion->header.mailboxes = MAILBOXES_MOBILE;

	if (vbt){
		opregion->header.dver[0] = '9';
		opregion->header.dver[1] = '.';
		opregion->header.dver[2] = '0';
		opregion->header.dver[3] = '.';
		opregion->header.dver[4] = '1';
		opregion->header.dver[5] = '0';
		opregion->header.dver[6] = '6';
		opregion->header.dver[7] = '3';
		opregion->header.dver[8] = '\0';

		memcpy(opregion->vbt.gvd1, vbt, vbt->hdr_vbt_size <
		sizeof(opregion->vbt.gvd1) ? vbt->hdr_vbt_size :
		sizeof(opregion->vbt.gvd1));
	}

	return 0;
}
