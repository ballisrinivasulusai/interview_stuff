/*
 * Share_disc_homing.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/Save_Flash_Values.h"
#include "../Inc/FLASH_SECTOR_H7.h"
/* Global Variable Declaration */
extern private_var pv;

/** @brief  : This function processes the received command
 *
 *  @param  cbuff[OUT]  : Receive data buffer
 *          len         : Buffer len
 *
 *  @return none
 *
 */
uint8_t Save_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Save_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Save_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    /* Call the appropriate callback function  */
    Save_func[cbuff[1]].cb(cbuff, len);
    pv.cmd_status = MOTOR_STATUS_STOP;
    return 1;
}

/**
 * @brief  This function call is used to save  the calibrated values.
 *
 * @param  None.
 * @retval None.
 *
 */
void Save_Flash_values_Fun1()
{
		pv.cal_disc[10] = 0x12345679;
		/* Save the Homing values of Disc1 t0 Disc6,axis,pd,prism in the Flash */
		Flash_Write_Data(FLASH_CONF_ADDR, (uint32_t *)&pv.cal_disc[0], 128/4);
}

/**
 * @brief  This function call is used to cancel  the calibrated values and store
 * 						the existing values in the flash memory.
 *
 * @param  None.
 * @ret val None.
 *
 */
void Save_Flash_values_Fun2()
{
	memset(&pv.cal_disc[0], 0x00, sizeof(pv.cal_disc));
	/* Store the Flash Memory values in this Buffer */
	Flash_Read_Data(FLASH_CONF_ADDR, (uint32_t *)&pv.cal_disc[0], 128/4);
}
/**
 * @brief  This function call is used to Intimation for BB function ON
 *
 * @param  None.
 * @ret val None.
 *
 */
void BB_on()
{
	pv.BB_on = 1;
}
/**
 * @brief  This function call is used to Intimation for BB function OFF
 *
 * @param  None.
 * @ret val None.
 *
 */
void BB_off()
{
	pv.BB_on = 0;
}
/**
 * @brief  This function call is used to Intimation for BB function ON
 *
 * @param  None.
 * @ret val None.
 *
 */
void PRISM_POLAR()
{
	pv.cal_disc[10] =  1;
	pv.prism = pv.cal_disc[10];
	pv.cal_disc[11] = 0x12345679;
	/* Save the Homing values of Disc1 t0 Disc6,axis,pd,prism in the Flash */
	Flash_Write_Data(FLASH_CONF_ADDR, (uint32_t *)&pv.cal_disc[0], 128/4);
}
/**
 * @brief  This function call is used to Intimation for BB function ON
 *
 * @param  None.
 * @ret val None.
 *
 */
void PRISM_XY()
{
	pv.cal_disc[10] =  0;
	pv.prism = pv.cal_disc[10];
	pv.cal_disc[11] = 0x12345679;
	/* Save the Homing values of Disc1 t0 Disc6,axis,pd,prism in the Flash */
	Flash_Write_Data(FLASH_CONF_ADDR, (uint32_t *)&pv.cal_disc[0], 128/4);
}
