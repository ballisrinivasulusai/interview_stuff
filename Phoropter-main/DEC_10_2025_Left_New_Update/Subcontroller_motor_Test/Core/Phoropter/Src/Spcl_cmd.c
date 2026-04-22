/*
 * Special_cmd.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/spcl_cmd.h"

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
uint8_t spcl_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(spcl_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == spcl_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    spcl_func[cbuff[1]].cb(cbuff, len);

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
void spcl_SubFun1()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	pv.spcl_rotate_input = 360;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates Clockwise 40 degree
 *            Disc6 (close)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3,4 : 0 degree (0) */
	pv.spcl_rotate_input = 320;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates clockwise 40 degree
 *            Disc6 (close)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3,4 : +40 degree (0) */
	pv.spcl_rotate_input = 80;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates homing open
 *            Disc6.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun4()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	pv.spcl_rotate_input = 120;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates anti clockwise
 *            40 degree - Disc6.(Right pin hole)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun5()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : -40 degree (0) */
	pv.spcl_rotate_input = 40;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates clockwise
 *            40 degree - Disc6.(left pin hole)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun6()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : +40 degree (0) */
	pv.spcl_rotate_input = 240;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates clockwise
 *            80 degree - Disc6.(Red Filter ON)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun7()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : +80 degree (0) */
	pv.spcl_rotate_input = 280;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates 0 degree - Disc6.
 * 			  (Red Filter OFF)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun8()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	pv.spcl_rotate_input = 160;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates Clockwise 200 degree
 * 			  Disc6.(left to right polarization)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun9()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 200 degree (0) */
	pv.spcl_rotate_input = 200;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

#if 0
/** @brief  : This function operates Anti clockwise 80 degree
 * 			  - Disc6.(right to left polarization)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun10()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : -80 degree (0) */
	pv.spcl_rotate_input = 80;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates Anti clockwise 80 degree
 * 			  - Disc6.(right to right polarization)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun11()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun11\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : -80 degree (0) */
	pv.spcl_rotate_input = 80;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates Anti clockwise 120 degree
 * 			  - Disc6.(right to left polarization)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun12()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : -120 degree (0) */
	pv.spcl_rotate_input = 120;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates 0 degree
 * 			  - Disc6.(right MADdix)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun13()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	pv.spcl_rotate_input = 360;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates clockwise 120 degree
 * 			  - Disc6.(Left MADdix)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun14()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : +120 degree (0) */
	pv.spcl_rotate_input = 240;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates 0 degree
 * 			  - Disc6.(Right Prism)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun15()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : 0 degree (0) */
	pv.spcl_rotate_input = 360;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}

/** @brief  : This function operates -160 degree
 * 			  - Disc6.(Left Prism)
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spcl_SubFun16()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spcl_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 6 : -160 degree (0) */
	pv.spcl_rotate_input = 160;
	Disc_Motor_Rotation(pv.spcl_rotate_input,6);
}
#endif
