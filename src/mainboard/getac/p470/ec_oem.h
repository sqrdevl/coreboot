/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2008-2009 coresystems GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _MAINBOARD_EC_OEM_H
#define _MAINBOARD_EC_OEM_H

#define EC_OEM_DATA	0x68
#define EC_OEM_SC	0x6c

/* EC_SC input */
#define   EC_SMI_EVT	(1 << 6) // 1: SMI event pending
#define   EC_SCI_EVT	(1 << 5) // 1: SCI event pending
#define   EC_BURST	(1 << 4) // controller is in burst mode
#define   EC_CMD	(1 << 3) // 1: byte in data register is command
				 // 0: byte in data register is data
#define   EC_IBF	(1 << 1) // 1: input buffer full (data ready for ec)
#define   EC_OBF	(1 << 0) // 1: output buffer full (data ready for host)
/* EC_SC output */
#define   RD_EC		0x80 // Read Embedded Controller
#define   WR_EC		0x81 // Write Embedded Controller
#define   BE_EC		0x82 // Burst Enable Embedded Controller
#define   BD_EC 	0x83 // Burst Disable Embedded Controller
#define   QR_EC 	0x84 // Query Embedded Controller

int send_ec_oem_command(u8 command);
int send_ec_oem_data(u8 data);
int send_ec_oem_data_nowait(u8 data);
u8 recv_ec_oem_data(void);
u8 ec_oem_read(u8 addr);
int ec_oem_write(u8 addr, u8 data);

int ec_dump_status(void);
int ec_oem_dump_status(void);
#endif
