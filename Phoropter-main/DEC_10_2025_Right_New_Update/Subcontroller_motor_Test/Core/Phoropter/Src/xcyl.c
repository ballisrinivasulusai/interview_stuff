/*
 * Special_cmd.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/xcyl.h"

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
uint8_t xcyl_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(xcyl_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == xcyl_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    xcyl_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;
    return 1;
}

/** @brief  : This function operates homing open
 *            Disc6.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun1()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	if( 0 != pv.axis_xcyl)
	{
		pv.axis_rotate_input = 0;
		axis_xcyl(pv.axis_rotate_input);
	}
	pv.rotate_input = 51.4;
	Disc_Motor_Rotation(pv.rotate_input,3);

}

/** @brief  : This function operates Clockwise 40 degree
 *            Disc6 (close)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3,4 : 0 degree (0) */
	pv.axis_rotate_input = 90;
	axis_xcyl(pv.axis_rotate_input);
	pv.rotate_input = 102.8;
	Disc_Motor_Rotation(pv.rotate_input,3);
}

/** @brief  : This function operates clockwise 40 degree
 *            Disc6 (close)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3,4 : +40 degree (0) */
	pv.axis_rotate_input = 45;
	axis_xcyl(pv.axis_rotate_input);
	pv.rotate_input = 51.4;
	Disc_Motor_Rotation(pv.rotate_input,3);
}

/** @brief  : This function operates homing open
 *            Disc6.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun4()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	pv.axis_rotate_input = 135;
	axis_xcyl(pv.axis_rotate_input);
	pv.rotate_input = 102.8;
	Disc_Motor_Rotation(pv.rotate_input,3);
}

/** @brief  : This function operates anti clockwise
 *            40 degree - Disc6.(Right pin hole)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun5()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.rotate_input = 51.4;
	Disc_Motor_Rotation(pv.rotate_input,3);
}

/** @brief  : This function operates clockwise
 *            40 degree - Disc6.(left pin hole)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun6()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.rotate_input = 102.8;
	Disc_Motor_Rotation(pv.rotate_input,3);
}

/** @brief  : This function operates clockwise
 *            80 degree - Disc6.(Red Filter ON)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun7()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.axis_rotate_input = -22;
	axis_xcyl(pv.axis_rotate_input);

}

/** @brief  : This function operates 0 degree - Disc6.
 * 			  (Red Filter OFF)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun8()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.axis_rotate_input = +22;
	axis_xcyl(pv.axis_rotate_input);

}

/** @brief  : This function operates Clockwise 200 degree
 * 			  Disc6.(left to right polarization)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun9()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.rotate_input = 257.2;
	Disc_Motor_Rotation(pv.rotate_input,3);
}
/** @brief  : This function operates Clockwise 200 degree
 * 			  Disc6.(left to right polarization)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun10()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.rotate_input = 308.6;
	Disc_Motor_Rotation(pv.rotate_input,3);
}
/** @brief  : This function operates homing open
 *            Disc6.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun11()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun11\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.axis_rotate_input = 90;
	axis_xcyl(pv.axis_rotate_input);

}

/** @brief  : This function operates Clockwise 40 degree
 *            Disc6 (close)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun12()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.axis_rotate_input = 0;
	axis_xcyl(pv.axis_rotate_input);

}

/** @brief  : This function operates clockwise 40 degree
 *            Disc6 (close)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void xcyl_SubFun13()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="xcyl_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	pv.axis_rotate_input = 0;
	axis_xcyl(pv.axis_rotate_input);
	pv.rotate_input = 360;
	Disc_Motor_Rotation(pv.rotate_input,3);
	pv.rotate_input = 360;
	Disc_Motor_Rotation(pv.rotate_input,6);
}
