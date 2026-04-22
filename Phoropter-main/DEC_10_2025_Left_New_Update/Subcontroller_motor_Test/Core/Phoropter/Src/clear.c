/*
 * Clear_cmd.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/clear.h"

/* Global Variable Declaration */
extern private_var pv;

void Dc_SubFun2();

/** @brief  : This function processes the received command
 *
 *  @param  cbuff[OUT]  : Receive data buffer
 *          len         : Buffer len
 *
 *  @return none
 *
 */
uint8_t clear_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="clear\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(clear_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == clear_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    clear_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;

    return 1;
}

/** @brief  : This function operates reset homing position for Disc1 to Disc6
 *            Axis and DC motor.
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void clear_SubFun1()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="clear_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

#if 1

	PD_motor();
	HAL_Delay(100);

	Disc1_homing_position();
	HAL_Delay(10);

	Disc2_homing_position();
	HAL_Delay(10);

	Disc3_homing_position();
	HAL_Delay(10);

	Disc4_homing_position();
	HAL_Delay(10);

	Disc5_homing_position();
	HAL_Delay(10);

	Disc6_homing_position();
	HAL_Delay(10);

    Axis_motor_homing();
	HAL_Delay(100);

	DC_motor_Out();
	HAL_Delay(100);

	DC_motor_In();
	HAL_Delay(100);

#endif
	pv.motor_error      = MOTOR_SUCCESS;
	pv.ioexp_int        = 0;
	pv.share_disc       = 0;
	pv.D1.temp_buff     = 0;
	pv.D2.temp_buff     = 0;
	pv.D3.temp_buff     = 0;
	pv.D4.temp_buff     = 0;
	pv.D5.temp_buff     = 0;
	pv.D6.temp_buff     = 0;
	pv.PD1_temp_old     = 0;
	pv.axis_temp        = 0;
	pv.axis_xcyl        = 0;
	pv.x_temp           = 0;
	pv.y_temp           = 0;
	pv.ver_temp         = 0;
	pv.hor_temp         = 0;
	pv.prism_select     = 0;
	pv.prism_polar_temp = 0;
	pv.rotate1_input     = 360;
	pv.rotate2_input     = 360;
	pv.rotate3_input     = 360;


}


/** @brief  : This function operates clear .
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void clear_SubFun2()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="clear_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
#if 1
	/* DISC 1 Clear */
	pv.user_input = 360;
	Disc_Motor_Rotation(pv.user_input,1);

	/* DISC 2 Clear */
	pv.user_input = 360;
	Disc_Motor_Rotation(pv.user_input,2);

	/* DISC 3 Clear */
	pv.user_input = 360;
	Disc_Motor_Rotation(pv.user_input,3);

	/* DISC 4 Clear */
	pv.user_input = 360;
	Disc_Motor_Rotation(pv.user_input,4);

	/* DISC 5 Clear */
	pv.user_input = 360;
	Disc_Motor_Rotation(pv.user_input,5);

	/* DISC 6 Clear */
	pv.user_input = 360;
	Disc_Motor_Rotation(pv.user_input,6);


	/* Axis_xcyl clear */
	if(pv.axis_xcyl != 0)
	{
		pv.axis_rotate_input = 0;
		axis_xcyl(pv.axis_rotate_input);
	}
	/* Axis clear */
	if(pv.axis_temp != 0)
	{
		pv.axis_rotate_input = 0;
		Axis_motor_rotation(pv.axis_rotate_input);
	}

	/* PD clear */
	if(pv.PD1_temp_old != 0)
		PD1_motor(pv.PD1_temp_old);
	if(( 0 != pv.x_temp) || (0 != pv.y_temp))
	{
		pv.prism_ver_input = 0;
		pv.prism_hor_input = 0;
		pv.prism_select = 1;
		prism_function(pv.prism_select);
	}
	if(0 != pv.prism_polar_temp)
	{
		pv.prism_polar_input = 0;
		pv.prism_select = 4;
		if(pv.polar_func == 1)
		{
			pv.prism_polar_temp = 180 - pv.prism_polar_temp;
		}
		prism_function(pv.prism_select);
	}
	/* DC Clear */
	Dc_SubFun2();
#endif


	pv.motor_error      = MOTOR_SUCCESS;
	pv.ioexp_int        = 0;
	pv.share_disc       = 0;
	pv.PD1_temp_old     = 0;
	pv.axis_temp        = 0;
	pv.axis_xcyl        = 0;
	pv.x_temp           = 0;
	pv.y_temp           = 0;
	pv.prism_select     = 0;
	pv.prism_polar_temp = 0;
	pv.ver_temp         = 0;
	pv.hor_temp         = 0;
	pv.rotate1_input     = 360;
	pv.rotate2_input     = 360;
	pv.rotate3_input     = 360;

}
/** @brief  : This function operates reset homing position for Prism1 & Prism2.
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_Homing()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_Homing\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	if(( 0 != pv.x_temp) || (0 != pv.y_temp))
	{
		pv.prism_ver_input = 0;
		pv.prism_hor_input = 0;
		pv.prism_select = 1;
		prism_function(pv.prism_select);
	}
	if(0 != pv.prism_polar_temp)
	{
		pv.prism_polar_input = 0;
		pv.prism_select = 4;
		prism_function(pv.prism_select);
	}
	pv.motor_error      = MOTOR_SUCCESS;
	pv.ioexp_int        = 0;
	pv.share_disc       = 0;
	pv.x_temp           = 0;
	pv.y_temp           = 0;
	pv.prism_polar_temp = 0;
	pv.ver_temp         = 0;
	pv.hor_temp         = 0;
}

/** @brief  : This function operates Initial homing position for Disc1 to Disc6
 *            Axis,Prism1,Prism2 and DC motor.
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Initial_Homing()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Initial_Homing\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

#if 1

	PD_motor();
	HAL_Delay(100);

	Disc1_homing_position();
	HAL_Delay(10);

	Disc2_homing_position();
	HAL_Delay(10);

	Disc3_homing_position();
	HAL_Delay(10);

	Disc4_homing_position();
	HAL_Delay(10);

	Disc5_homing_position();
	HAL_Delay(10);

	Disc6_homing_position();
	HAL_Delay(10);

    Axis_motor_homing();


	DC_motor_Out();
	HAL_Delay(100);

	Prism1_motor();
	HAL_Delay(100);

	Prism2_motor();
	HAL_Delay(100);

	DC_motor_In();
	HAL_Delay(100);
#endif

	pv.motor_error      = MOTOR_SUCCESS;
	pv.ioexp_int        = 0;
	pv.share_disc       = 0;
	pv.D1.temp_buff     = 0;
	pv.D2.temp_buff     = 0;
	pv.D3.temp_buff     = 0;
	pv.D4.temp_buff     = 0;
	pv.D5.temp_buff     = 0;
	pv.D6.temp_buff     = 0;
	pv.PD1_temp_old     = 0;
	pv.axis_temp        = 0;
	pv.axis_xcyl        = 0;
	pv.x_temp           = 0;
	pv.y_temp           = 0;
	pv.ver_temp         = 0;
	pv.hor_temp         = 0;
	pv.prism_polar_temp = 0;
	pv.rotate1_input     = 360;
	pv.rotate2_input     = 360;
	pv.rotate3_input     = 360;
}
