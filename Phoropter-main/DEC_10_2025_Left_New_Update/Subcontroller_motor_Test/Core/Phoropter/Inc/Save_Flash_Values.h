/*
 * Save_Flash_Values.h
 *
 *  Created on:
 *      Author:
 */

#ifndef SAVE_FLASH_VALUES_CMDLIST_H_
#define SAVE_FLASH_VALUES_CMDLIST_H_

void Save_Flash_values_Fun1();
void Save_Flash_values_Fun2();
void BB_on();
void BB_off();
void PRISM_POLAR();
void PRISM_XY();
/*!
 * Command Menu list
 */

submenulist Save_func[] =
{
	{NULL},
	{Save_Flash_values_Fun1},
	{Save_Flash_values_Fun2},
	{BB_on},
	{BB_off},
	{PRISM_POLAR},
	{PRISM_XY},
};

#endif /* SAVE_FLASH_VALUES_CMDLIST_H_ */
