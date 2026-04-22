/*
 * Disc_cal.h
 *
 *  Created on:
 *      Author:
 */

#ifndef DISC_CAL_CMDLIST_H_
#define DISC_CAL_CMDLIST_H_

void disc1_inc();
void disc1_dec();
void disc2_inc();
void disc2_dec();
void disc3_inc();
void disc3_dec();
void disc4_inc();
void disc4_dec();
void disc5_inc();
void disc5_dec();
void disc6_inc();
void disc6_dec();
void PD_inc();
void PD_dec();
void Axis_inc();
void Axis_dec();
void prism1_inc();
void prism1_dec();
void prism2_inc();
void prism2_dec();

/*!
 * Command Menu list
 */
submenulist Disc_func[] =
{
	{NULL},
	{disc1_inc},
	{disc1_dec},
	{disc2_inc},
	{disc2_dec},
	{disc3_inc},
	{disc3_dec},
	{disc4_inc},
	{disc4_dec},
	{disc5_inc},
	{disc5_dec},
	{disc6_inc},
	{disc6_dec},
	{PD_inc},
	{PD_dec},
	{Axis_inc},
	{Axis_dec},
	{prism1_inc},
	{prism1_dec},
	{prism2_inc},
	{prism2_dec},
};

#endif /* DISC_CAL_CMDLIST_H_ */

