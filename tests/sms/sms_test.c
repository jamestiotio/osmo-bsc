/*
 * (C) 2008 by Daniel Willmann <daniel@totalueberwachung.de>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <openbsc/debug.h>
#include <openbsc/msgb.h>
#include <openbsc/gsm_04_11.h>
#include <openbsc/gsm_04_08.h>

/* SMS data from MS starting with layer 3 header */
static u_int8_t sms1[] = {
	0x39, 0x01, 0x1a, 0x00, 0x01, 0x00, 0x07, 0x91, 0x55, 0x11,
	0x18, 0x31, 0x28, 0x00, 0x0e, 0x31, 0x20, 0x04, 0x81, 0x21,
	0x43, 0x00, 0x00, 0xff, 0x04, 0xd4, 0xf2, 0x9c, 0x0e
};

static u_int8_t sms2[] = {
	0x09, 0x01, 0x9c, 0x00, 0xda, 0x00, 0x07, 0x91, 0x88, 0x96, 0x13,
	0x00, 0x00, 0x99, 0x90, 0x11, 0x7b, 0x04, 0x81, 0x22, 0x22, 0x00,
	0x08, 0xff, 0x86, 0x6c, 0x38, 0x8c, 0x50, 0x92, 0x80, 0x88, 0x4c,
	0x00, 0x4d, 0x00, 0x4d, 0x00, 0x41, 0x6a, 0x19, 0x67, 0x03, 0x74,
	0x06, 0x8c, 0xa1, 0x7d, 0xb2, 0x00, 0x20, 0x00, 0x20, 0x51, 0x68,
	0x74, 0x03, 0x99, 0x96, 0x52, 0x75, 0x7d, 0xb2, 0x8d, 0xef, 0x6a,
	0x19, 0x67, 0x03, 0xff, 0x0c, 0x6a, 0x19, 0x67, 0x03, 0x96, 0xf6,
	0x98, 0xa8, 0x96, 0xaa, 0xff, 0x01, 0x8b, 0x93, 0x60, 0xa8, 0x80,
	0x70, 0x66, 0x0e, 0x51, 0x32, 0x84, 0xc4, 0xff, 0x0c, 0x97, 0x48,
	0x6d, 0x3b, 0x62, 0x95, 0x8c, 0xc7, 0xff, 0x01, 0x73, 0xfe, 0x57,
	0x28, 0x52, 0xa0, 0x51, 0x65, 0x90, 0x01, 0x96, 0x50, 0x91, 0xcf,
	0x59, 0x27, 0x80, 0x6f, 0x76, 0xdf, 0x6d, 0x0b, 0x57, 0xfa, 0x96,
	0x8a, 0x91, 0x77, 0x5e, 0x63, 0x53, 0x61, 0xff, 0x0c, 0x8a, 0xcb,
	0x4e, 0x0a, 0x7d, 0xb2, 0x64, 0x1c, 0x5c, 0x0b, 0x30, 0x0c, 0x6a,
	0x19, 0x67, 0x03, 0x30, 0x0d
};

struct sms_datum {
	u_int8_t len;
	u_int8_t *data;
};

static struct sms_datum sms_data[] = {
	{
		.len = sizeof(sms1),
		.data = sms1,
	}, {
		.len = sizeof(sms2),
		.data = sms2,
	}
};

#define SMS_NUM 2

int main(int argc, char** argv)
{
	DEBUGP(DSMS, "SMS testing\n");
	struct msgb *msg;
	u_int8_t *sms;
	u_int8_t i;

	for(i=0;i<SMS_NUM;i++) {
		/* Setup SMS msgb */
		msg = msgb_alloc(sms_data[i].len);
		sms = msgb_put(msg, sms_data[i].len);

		memcpy(sms, sms_data[i].data, sms_data[i].len);
		msg->l3h = sms;

		gsm0411_rcv_sms(msg);
		msgb_free(msg);
	}
}
