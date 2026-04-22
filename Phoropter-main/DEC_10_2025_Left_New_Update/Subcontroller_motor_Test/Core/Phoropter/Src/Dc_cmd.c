/*
 * DC_cmd.c
 *
 *  Created on:
 *      Author:
 */

/*** Header File ***/
#include "../Inc/common.h"
#include "../Inc/Dc_cmd.h"

/* Global variable Declaration */
extern private_var pv;

/** @brief  : Process the command.
 *            This function processes the received command.
 *
 *  @param  : cbuff[OUT]: Receive data buffer
 *            len       : Buffer len
 *
 *  @return : none
 *
 */
uint8_t Dc_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Dc\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Dc_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Dc_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    Dc_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;

    return 1;
}

/** @brief  : This function call the DC motor forward and reverse rotation.
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Dc_SubFun1()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Dc_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	if(GPIO_PIN_RESET == (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)))
	{
		 HAL_Delay(1000);
		 pv.dc_int1 = 0;
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		 while(pv.dc_int1 != 1);
		 /* Break */
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		 pv.dc_int2 = 0;
	}
	pv.ioexp_int = 0;
	pv.share_disc = 0;
	pv.cmd_status = MOTOR_STATUS_STOP;

}


/** @brief  : This function call the Dc motor forward and reverse rotation.
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Dc_SubFun2()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Dc_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	 if(GPIO_PIN_RESET == (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)))
	 {
		 HAL_Delay(1000);
		 pv.dc_int2 = 0;
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		 while(pv.dc_int2 != 1);
		 /* Break */
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		 pv.dc_int1 = 0;
	 }
	 pv.ioexp_int = 0;
	 pv.share_disc = 0;
	 pv.cmd_status = MOTOR_STATUS_STOP;
}

