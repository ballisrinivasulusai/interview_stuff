/*
 * clear.h
 *
 *  Created on:
 *      Author:
 */

#ifndef CLEAR_CMDLIST_H_
#define CLEAR_CMDLIST_H_

void clear_SubFun1();
void clear_SubFun2();
void Prism_Homing();
void Initial_Homing();

/*!
 * Command Menu list
 */
submenulist clear_func[] =
{
	{NULL},
	{clear_SubFun1},
	{clear_SubFun2},
	{Prism_Homing},
};

#endif /* CLEAR_CMDLIST_H_ */

