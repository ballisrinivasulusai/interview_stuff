/*
 * cmdlist.h
 *
 *  Created on:
 *      Author:
 */

#ifndef SPCL_CMDLIST_H_
#define SPCL_CMDLIST_H_

void spcl_SubFun1();
void spcl_SubFun2();
void spcl_SubFun3();
void spcl_SubFun4();
void spcl_SubFun5();
void spcl_SubFun6();
void spcl_SubFun7();
void spcl_SubFun8();
void spcl_SubFun9();
#if 0
void spcl_SubFun10();
void spcl_SubFun11();
void spcl_SubFun12();
void spcl_SubFun13();
void spcl_SubFun14();
void spcl_SubFun15();
void spcl_SubFun16();
#endif


/*!
 * Command Menu list
 */
submenulist spcl_func[] =
{
	{NULL},
	{spcl_SubFun1},
	{spcl_SubFun2},
	{spcl_SubFun3},
	{spcl_SubFun4},
	{spcl_SubFun5},
	{spcl_SubFun6},
	{spcl_SubFun7},
	{spcl_SubFun8},
	{spcl_SubFun9},
#if 0
	{spcl_SubFun10},
	{spcl_SubFun11},
	{spcl_SubFun12},
	{spcl_SubFun13},
	{spcl_SubFun14},
	{spcl_SubFun15},
	{spcl_SubFun16},
#endif

};

#endif /* AXIS_CMDLIST_H_ */

