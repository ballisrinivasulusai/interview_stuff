/*
 * cmdlist.h
 *
 *  Created on:
 *      Author:
 */

#ifndef SRC_CMDLIST_H_
#define SRC_CMDLIST_H_

/*!
 * Command Menu list
 */
menulist ml[] =
{
    {0x01, sph_cmd},
	{0x02, cyl_cmd},
	{0x03, axis_cmd},
	{0x04, Prism_hor_cmd},
	{0x05, Dc_cmd},
	{0x06, PD_cmd},
	{0x07, spcl_cmd},
	{0x08, clear_cmd},
	{0x09, Disc_cmd},
	{0x10, Save_cmd},
	{0x11, Share_cmd},
	{0x12, Prism_ver_cmd},
	{0x13, xcyl_cmd},
	{0x14, Prism_polar_cmd},
	{0x15, Prism_polar2_cmd},
};

#endif /* SRC_CMDLIST_H_ */
