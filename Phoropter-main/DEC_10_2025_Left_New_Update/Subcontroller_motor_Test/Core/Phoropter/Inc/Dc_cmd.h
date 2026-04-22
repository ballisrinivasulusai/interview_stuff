/*
 * Dc_cmd.h
 *
 *  Created on:
 *      Author:
 */

#ifndef INC_DC_CMD_H_
#define INC_DC_CMD_H_

void Dc_SubFun1();
void Dc_SubFun2();


/*!
 * Command Menu list
 */
submenulist Dc_func[] =
{

	{NULL},
	{Dc_SubFun1},
	{Dc_SubFun2},

};

#endif /* INC_DC_CMD_H_ */
