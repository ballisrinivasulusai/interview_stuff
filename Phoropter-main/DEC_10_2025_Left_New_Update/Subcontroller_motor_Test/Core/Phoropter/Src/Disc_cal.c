/*
 * Disc_cal.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/Disc_cal.h"

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
uint8_t Disc_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Disc_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Disc_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    Disc_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;
    return 1;
}

/** @brief  : This function call the homing Disc1
 *            calibrate the increase value.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc1_inc()
{
	if(pv.disc1_status == MOTOR_HIGH)
	{
		pv.user_input = 359;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,1);
		pv.cal_disc[0] += 67;
		pv.ioexp_int = 0;
		pv.D1.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc1
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc1_dec()
{
	if(pv.disc1_status == MOTOR_HIGH)
	{
		pv.user_input = 1;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,1);
		pv.cal_disc[0] -= 67;
		pv.ioexp_int = 0;
		pv.D1.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}

}

/** @brief  : This function call the homing Disc2
 *            calibrate the increase value.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc2_inc()
{
	if(pv.disc2_status == MOTOR_HIGH)
	{
		pv.user_input = 359;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,2);
		pv.cal_disc[1] += 67;
		pv.ioexp_int = 0;
		pv.D2.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}

}

/** @brief  : This function call the homing Disc2
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc2_dec()
{
	if(pv.disc2_status == MOTOR_HIGH)
	{
		pv.user_input = 1;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,2);
		pv.cal_disc[1] -= 67;
		pv.ioexp_int = 0;
		pv.D2.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc3
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc3_inc()
{
	if(pv.disc3_status == MOTOR_HIGH)
	{
		pv.user_input = 359;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,3);
		pv.cal_disc[2] += 67;
		pv.ioexp_int = 0;
		pv.D3.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc3
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc3_dec()
{
	if(pv.disc3_status == MOTOR_HIGH)
	{
		pv.user_input = 1;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,3);
		pv.cal_disc[2] -= 67;
		pv.ioexp_int = 0;
		pv.D3.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc4
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc4_inc()
{
	if(pv.disc4_status == MOTOR_HIGH)
	{
		pv.user_input = 359;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,4);
		pv.cal_disc[3] += 67;
		pv.ioexp_int = 0;
		pv.D4.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc4
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc4_dec()
{
	if(pv.disc4_status == MOTOR_HIGH)
	{
		pv.user_input = 1;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,4);
		pv.cal_disc[3] -= 67;
		pv.ioexp_int = 0;
		pv.D4.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc5
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc5_inc()
{
	if(pv.disc5_status == MOTOR_HIGH)
	{
		pv.user_input = 359;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,5);
		pv.cal_disc[4] += 67;
		pv.ioexp_int = 0;
		pv.D5.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc5
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc5_dec()
{
	if(pv.disc5_status == MOTOR_HIGH)
	{
		pv.user_input = 1;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,5);
		pv.cal_disc[4] -= 67;
		pv.ioexp_int = 0;
		pv.D5.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc6
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc6_inc()
{
	if(pv.disc6_status == MOTOR_HIGH)
	{
		pv.user_input = 359;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,6);
		pv.cal_disc[5] += 67;
		pv.ioexp_int = 0;
		pv.D6.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}

/** @brief  : This function call the homing Disc6
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void disc6_dec()
{
	if(pv.disc6_status == MOTOR_HIGH)
	{
		pv.user_input = 1;
		pv.homing = 2;
		Disc_Motor_Rotation(pv.user_input,6);
		pv.cal_disc[5] -= 67;
		pv.ioexp_int = 0;
		pv.D6.temp_buff = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}
}
/** @brief  : This function call the homing PD
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD_inc()
{
	pv.PD1_rotate_input = -1;
	PD1_motor(pv.PD1_rotate_input);
	pv.cal_disc[7] += 15000 ;
	pv.PD1_temp_old = 0;
	pv.ioexp_int = 0;
}

/** @brief  : This function call the homing PD
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD_dec()
{
	pv.PD1_rotate_input = 1;
	PD1_motor(pv.PD1_rotate_input);
	pv.cal_disc[7] -= 15000 ;
	pv.PD1_temp_old = 0;
	pv.ioexp_int = 0;
}

/** @brief  : This function call the homing axis
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Axis_inc()
{
	pv.axis_rotate_input = 1;
	Axis_motor_rotation_calibration(pv.axis_rotate_input);
	pv.cal_disc[6] += 1;
	pv.ioexp_int = 0;
}

/** @brief  : This function call the homing axis
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Axis_dec()
{
	pv.axis_rotate_input = -1;
	Axis_motor_rotation_calibration(pv.axis_rotate_input);
	pv.cal_disc[6] -= 1;
	pv.ioexp_int = 0;
}


/** @brief  : This function call the homing prism1
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void prism1_inc()
{
	pv.prism_input = 1;
	pv.prism_select = 4;
	Prism1_Motor_Rotation(pv.prism_input);
	pv.cal_disc[8] += 1;
	pv.prism_select = 0;
	pv.ioexp_int = 0;
}

/** @brief  : This function call the homing prism1
 *            calibrate the decrease value.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void prism1_dec()
{
	pv.prism_input = -1;
	pv.prism_select = 4;
	Prism1_Motor_Rotation(pv.prism_input);
	pv.cal_disc[8] -= 1;
	pv.prism_select = 0;
	pv.ioexp_int = 0;
}

/** @brief  : This function call the homing prism2
 *            calibrate the increase value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void prism2_inc()
{
	pv.prism_input = 1;
	pv.prism_select = 4;
	Prism2_Motor_Rotation(pv.prism_input);
	pv.cal_disc[9] += 1;
	pv.prism_select = 0;
	pv.ioexp_int = 0;
}

/** @brief  : This function call the homing Prism2
 *            calibrate the decrease value .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void prism2_dec()
{
	pv.prism_input = -1;
	pv.prism_select = 4;
	Prism2_Motor_Rotation(pv.prism_input);
	pv.cal_disc[9] -= 1;
	pv.prism_select = 0;
	pv.ioexp_int = 0;
}
