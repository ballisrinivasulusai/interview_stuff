/*
 * Share_disc_homing.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/Share_disc_homing.h"

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
uint8_t Share_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Share_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Share_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    Share_func[cbuff[1]].cb(cbuff, len);


    pv.cmd_status = MOTOR_STATUS_STOP;
    return 1;
}

void Share_SubFun1()
{
	pv.share_disc = 1;
}
