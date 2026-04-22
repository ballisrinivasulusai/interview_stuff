/*
 * process_cmd.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files */
#include "../Inc/common.h"
#include "../Inc/cyl_cmd.h"

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
uint8_t cyl_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="CYL\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(cyl_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == cyl_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    cyl_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;

    return 1;
}

/** @brief  : This function operates Anti-Clockwise 51.43 degree
 *            Disc5 (cyl_value = -0.25 (0xC9))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun1()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/* Rotate the disk 5 : 51.4 degree (-0.25) */
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Anti-Clockwise 102.8 degree
 *            Disc5 (cyl_value = -0.5 (0xCA))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/* Rotate the disk 4 : 0 degree (0) */
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/* Rotate the disk 5 : 102.8 degree (-0.5) */
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Anti-Clockwise 154.2
 *            degree Disc5 (cyl_value = -0.75 (0xCB))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/* Rotate the disk 4 : 0 degree (0) */
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);
	HAL_Delay(50);

	/* Rotate the disk 5 : 154.2 degree(-0.75) */
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Clockwise 154.2 degree Disc5.
 *            (cyl_value = -1 (0xCC))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun4()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/* Rotate the disk 1,2,3,4 : 0 degree (0) */
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/* Rotate the disk 5 : 205.6 degree(-1) */
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Clockwise 102.8
 *            degree Disc5.(cyl_value = -1.25 (0xCD))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun5()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/* Rotate the disk 1,2,3,4 : 0 degree (0)*/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/* Rotate the disk 5 : 257 degree (-1.25) */
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Clockwise 51.43
 *            degree Disc5 (cyl_value = -1.5 (0xCE))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun6()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/* Rotate the disk 1,2,3,4 : 0 degree (0) */
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/* Rotate the disk 5 : 308.4 degree (-1.5) */
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief  : This function operates Clockwise 51.4
 *           degree Disc4.(cyl_value = -1.75 (0xCF))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun7()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}
	/* Rotate the disk 4 : 308.4 degree (-1.75)*/
	pv.cyl_rotate_input = 308.6;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);
	/* Rotate the disk 5 : 0 degree (0) */
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);

}

/* @brief : This function operates Clockwise 51.4 degree Disc4.
            and Anti-Clockwise 51.43 degree Disc5.
 *          (cyl_value = -2 (0xD0))
 *
 * @param  : none
 *
 * @return : none
 *
 */
void cyl_SubFun8()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 308.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 51.4 degree (-0.25)****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/*  @brief : This function operates Clockwise 51.43 degree Disc4
            and Anti-Clockwise 102.8 degree Disc5.
 *          (cyl_value = -2.25 (0xD1))
 *
 * @param  : none
 *
 * @return : none
 *
 */
void cyl_SubFun9()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 308.4 degree(-1.75) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 102.8 (-0.5) degree ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates Clockwise 51.4 degree Disc4
   	   	   	 and Anti-Clockwise 154.2 degree Disc5.
 *           (cyl_value = -2.5 (0xD2))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun10()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 308.4 degree(-1.75) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 102.8 (-0.75) degree ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/*   @brief : This function operates Clockwise 51.4 degree Disc4
              and Clockwise 154.2 degree Disc5.
 *            (cyl_value = -2.75 (0xD3))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun11()
{
	/* Send the Ack to module  */
	#ifdef __CMD_DEBUG__
		uint8_t scmd[]="cyl_SubFun11\r\n";
		dump_packet(scmd, strlen((const char *)scmd));
	#endif
		/* Rotate the disk 1,2,3 : 0 degree (0) */
		if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
				((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
				((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
		{
			pv.cyl_rotate_input = 360;
			Disc_Motor_Rotation(pv.cyl_rotate_input,1);
			Disc_Motor_Rotation(pv.cyl_rotate_input,2);
			Disc_Motor_Rotation(pv.cyl_rotate_input,3);
		}

	/**** Rotate the disk 4 : 308.4 degree(-1.75) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 205.6 degree (-1) ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);

}

/**  @brief : This function operates Clockwise 51.4 degree Disc4
              and Clockwise 102.8 degree Disc5.
 *            (cyl_value = -3 (0xD4))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun12()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 308.4 degree(-1.75) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 257 degree (-1.25)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/**  @brief : This function operates Clockwise 51.4 degree Disc4
             and Clockwise 51.43 degree Disc5.
 *           (cyl_value = -3.25 (0xD5))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun13()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 308.4 degree(-1.75) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 102.8 degree (-1.5)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates Clockwise 102.8 degree Disc4
  	  	  	 (cyl_value = -3.5 (0xD6))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun14()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 257 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 102.8 degree (0) ****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/**  @brief :  This function operates Clockwise 102.8 degree Disc5
               and Anti-Clockwise 51.43 degree Disc5.
 *             (cyl_value = -3.75 (0xD7))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun15()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 257 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 51.4 degree (-0.25) ****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/**  @brief : This function operates Clockwise 102.8 degree Disc5
              and Anti-Clockwise 102.8 degree Disc5.
 *            (cyl_value = -4 (0xD8))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun16()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 257 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree (-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief  : This function operates Clockwise 102.8 degree Disc5
              and Anti-Clockwise 154.2 degree Disc5.
 *            (cyl_value = -4.25 (0xD9))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun17()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 257 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75) ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/**  @brief : This function operates Clockwise 102.8 degree Disc5
              and Clockwise 154.2 degree Disc5.
 *            (cyl_value = -4.5 (0xDA))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun18()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 257 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree(-1) ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Clockwise 102.8 degree Disc5
              and Clockwise 102.8 degree Disc5.
 *            (cyl_value = -4.75 (0xDB))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun19()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 257 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +102.8 degree (-1.25)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates Clockwise 102.8 degree Disc5
             and Clockwise 51.43 degree Disc5.
 *           (cyl_value = -5 (0xDC))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun20()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +51.4 degree (-1.5)  ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Clockwise 154.2 degree Disc4
              and Clockwise 0 degree Disc5.
 *            (cyl_value = -5.25 (0xDD))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun21()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree (0) ****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/**  @brief :  This function operates Clockwise 154.2 degree Disc4
               and Anti-Clockwise 51.43 degree Disc5.
 *             (cyl_value = -5.5 (0xDE))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun22()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25) ****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/**  @brief : This function operates Clockwise 154.2 degree Disc4
              and Anti-Clockwise 102.8 degree Disc5.
 *            (cyl_value = -5.75 (0xDF))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun23()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 205.7 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);
	HAL_Delay(50);

	/**** Rotate the disk 5 : 360 degree (-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/**  @brief : This function operates Clockwise 154.2 degree Disc4
              and Anti-Clockwise 154.2 degree Disc5.
 *            (cyl_value = -6 (0xAC))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun24()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : 205.7 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75) ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief  : This function operates Clockwise 154.2 degree Disc4
              and Clockwise 154.2 degree Disc5.
 *            (cyl_value = -6.25 (0xAD))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun25()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree (-1) ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/**  @brief :  This function operates Clockwise 154.2 degree Disc4
               and Clockwise 102.8 degree Disc5.
 *             (cyl_value = -6.5 (0xAE))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun26()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +102.8 degree (-1.25)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  : This function operates Clockwise 154.2 degree Disc4
              and Clockwise 51.43 degree Disc5.
 *            (cyl_value = -6.75 (0xAF))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun27()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +51.4 degree (-1.5) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/*   @brief : This function operates Anti-Clockwise 154.2 degree Disc4
              and Clockwise 0 degree Disc5.
 *            (cyl_value = -7 (0xB0))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun28()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  :  This function operates Anti-Clockwise 154.2 degree Disc4
              and Anti-Clockwise 51.43 degree Disc5.
 *            (cyl_value = -7.25 (0xB1))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun29()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25)****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief  :  This function operates Anti-Clockwise 154.2 degree Disc4
              and Anti-Clockwise 102.8 degree Disc5.
 *            (cyl_value = -7.5 (0xB2))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun30()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree (-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/*   @brief : This function operates Anti-Clockwise 154.2 degree Disc4
              and Anti-Clockwise 154.2 degree Disc5.
 *            (cyl_value = -7.75 (0xB3))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun31()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75) ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/*   @brief : This function operates Anti-Clockwise 154.2 degree Disc4
              and Clockwise 154.2 degree Disc5.
 *           (cyl_value = -8 (0xB4))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun32()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1,2,3 : 0 degree (0) */
	if(((pv.rotate1_input == 360) || (pv.rotate1_input == 0))&&
			((pv.rotate2_input == 360) || (pv.rotate2_input == 0))&&
			((pv.rotate3_input == 360) || (pv.rotate3_input == 0)))
	{
		pv.cyl_rotate_input = 360;
		Disc_Motor_Rotation(pv.cyl_rotate_input,1);
		Disc_Motor_Rotation(pv.cyl_rotate_input,2);
		Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	}

	/**** Rotate the disk 4 : -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree (-1)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             0 degree Disc1
             Clockwise 40 degree Disc2
             0 degree Disc3
             0 degree Disc4
 *           Anti-clockwise 51.43 degree Disc5.
 *           (cyl_value = 0.25 (0xB6))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun33()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +40 degree (0.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25) ****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             0 degree Disc1
             Clockwise 80 degree Disc2
             0 degree Disc3
             0 degree Disc4 Anti-clockwise
             102.8 degree Disc5.
 *
 *  (cyl_value = 0.5 (0xB7))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun34()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +80 degree (0.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree (-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief: This function operates
           0 degree Disc1
           Clockwise 120 degree Disc2
           0 degree Disc3
           0 degree Disc4
           Anti-clockwise 154.2 degree Disc5.

 * (cyl_value = 0.75 (0xB8))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun35()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +120 degree (0.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75) ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/* @brief : This function operates
            0 degree Disc1
            Clockwise 160 degree Disc2
            0 degree Disc3
            0 degree Disc4
            Clockwise 154.2 degree Disc5.

 *  (cyl_value = 1 (0xB9))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun36()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 1: +160 degree (1)****/
	pv.cyl_rotate_input = 200;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 (-1) degree ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/* @brief : This function operates
             Clockwise 40 degree Disc1
             Anti-Clockwise 160 degree Disc2
             0 degree Disc3
             0 degree Disc4
             Clockwise 102.8 degree Disc5.
 *
 *  (cyl_value = 1.25 (0xBA))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun37()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -160 degree (-1)****/
	pv.cyl_rotate_input = 160;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +102.8 degree (-1.25) ****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 40 degree Disc1
             Anti-Clockwise 120 degree Disc2
             0 degree Disc3
             0 degree Disc4
             Clockwise 51.4 degree Disc5
 *  (cyl_value = 1.5 (0xBB))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun38()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 1: -120 degree (-0.75)****/
	pv.cyl_rotate_input = 120;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +51.4 degree (-1.5)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 40 degree Disc1
            Anti-Clockwise 80 degree Disc2
            0 degree Disc3
            Clockwise 51.4 degree Disc4
            Clockwise 0 degree Disc5.

 *  (cyl_value = 1.75 (0xBC))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun39()
{

	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -80 degree (-0.5)****/
	pv.cyl_rotate_input = 80;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree ()****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree ****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief : This function operates
            Clockwise 40 degree Disc1
            Anti-Clockwise 40 degree Disc2
            0 degree Disc3
            Clockwise 51.4 degree Disc4
            Anti-Clockwise 51.4 degree Disc5.
 *
 *  (cyl_value = 2 (0xBD))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun40()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -40 degree (-0.25)****/
	pv.cyl_rotate_input = 40;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree ()****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25) ****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 40 degree Disc1
             Anti-Clockwise 40 degree Disc2
             0 degree Disc3
             Clockwise 51.4 degree Disc4
             Anti-Clockwise 102.8 degree Disc5.

 *  (cyl_value = 2.25 (0xBE))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun41()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree(-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 40 degree Disc1
            Clockwise 40 degree Disc2
            0 degree Disc3
            Clockwise 51.4 degree Disc4
            Anti-Clockwise 154.2 degree Disc5.
 *
 *  (cyl_value = 2.5 (0xBF))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun42()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +40 degree (0.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75) ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief: This function operates
            Clockwise 40 degree Disc1
            Clockwise 80 degree Disc2
            0 degree Disc3
            Clockwise 51.4 degree Disc4
            Clockwise 154.2 degree Disc5.

 *  (cyl_value = 2.75 (0xC0))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun43()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +80 degree (0.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree (-1)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief : This function operates
             Clockwise 40 degree Disc1
             Clockwise 120 degree Disc2
             0 degree Disc3
             Clockwise 51.4 degree Disc4
             Clockwise 102.8 degree Disc5.

 *  (cyl_value = 3 (0xC1))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun44()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +120 degree (0.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +102.8 degree (-1.25)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 40 degree Disc1
             Clockwise 160 degree Disc2
             0 degree Disc3
             Clockwise 51.4 degree Disc4
             Clockwise 51.4 degree Disc5.
 *
 *  (cyl_value = 3.25 (0xC2))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun45()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +40 degree (2.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +160 degree (1)****/
	pv.cyl_rotate_input = 200;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +51.4 degree (-1.75)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +51.4 degree (-1.5)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 80 degree Disc1
             Anti-Clockwise 160 degree Disc2
             0 degree Disc3
             Clockwise 102.8 degree Disc4
             Clockwise 0 degree Disc5.

 *  (cyl_value = 3.5 (0xC3))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun46()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -160 degree (-1)****/
	pv.cyl_rotate_input = 160;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 80 degree Disc1
            Anti-Clockwise 120 degree Disc2
            0 degree Disc3
            Clockwise 102.8 degree Disc4
            Anti-Clockwise 51.4 degree Disc5.

 *  (cyl_value = 3.75 (0xC4))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun47()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -120 degree (-0.75)****/
	pv.cyl_rotate_input = 120;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25)****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 80 degree Disc1
            Anti-Clockwise 80 degree Disc2
            0 degree Disc3
            Clockwise 102.8 degree Disc4
            Anti-Clockwise 102.8 degree Disc5.
 *
 *  (cyl_value = 4 (0xC5))
 *
 *  @param  : none
 *
 *  @return : none
 */
void cyl_SubFun48()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -80 degree (-0.5)****/
	pv.cyl_rotate_input = 80;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree (-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 80 degree Disc1
            Anti-Clockwise 40 degree Disc2
            0 degree Disc3
            Clockwise 102.8 degree Disc4
            Anti-Clockwise 154.2 degree Disc5.

 *  (cyl_value = 4.25 (0xC6))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun49()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -40 degree (-0.25)****/
	pv.cyl_rotate_input = 40;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree(-0.75) ****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief : This function operates
             Clockwise 80 degree Disc1
             Anti-Clockwise 0 degree Disc2
             0 degree Disc3
             Clockwise 102.8 degree Disc4
             Clockwise 154.2 degree Disc5.
 *
 *  (cyl_value = 4.5 (0xC7))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun50()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree(-1) ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
            Clockwise 80 degree Disc1
            Clockwise 40 degree Disc2
            0 degree Disc3
            Clockwise 102.8 degree Disc4
            Clockwise 102.8 degree Disc5.

 *  (cyl_value = 4.75 (0xF0))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun51()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +40 degree (0.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +102.8 degree(-1.25) ****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 80 degree Disc1
             Clockwise 80 degree Disc2
             0 degree Disc3
             Clockwise 102.8 degree Disc4
             Clockwise 51.4 degree Disc5.

 *  (cyl_value = 5 (0xF1))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun52()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun52\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +80 degree (0.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +102.8 degree (-3.5)****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +51.4 degree (-1.5)****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 80 degree Disc1
            Clockwise 120 degree Disc2
            0 degree Disc3
            Clockwise 154.2 degree Disc4
            Clockwise 0 degree Disc5.

 *  (cyl_value = 5.25 (0xF2))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun53()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +120 degree (0.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 80 degree Disc1
             Clockwise 160 degree Disc2
             0 degree Disc3
             Clockwise 154.2 degree Disc4
             Anti-Clockwise 51.4 degree Disc5.

 *  (cyl_value = 5.5 (0xF3))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun54()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +80 degree (4.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +160 degree (1)****/
	pv.cyl_rotate_input = 200;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25)****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief : This function operates
             Clockwise 120 degree Disc1
             Anti-Clockwise 160 degree Disc2
             0 degree Disc3
             Clockwise 154.2 degree Disc4
             Anti-Clockwise 102.8 degree Disc5.

 *  (cyl_value = 5.75 (0xF4))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun55()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -160 degree (-1)****/
	pv.cyl_rotate_input = 160;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree(-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 120 degree Disc1
             Anti-Clockwise 120 degree Disc2
             0 degree Disc3
             Clockwise 154.2 degree Disc4
             Anti-Clockwise 154.2 degree Disc5.

 *  (cyl_value = 6 (0xF5))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun56()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -120 degree (-0.75)****/
	pv.cyl_rotate_input = 120;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 120 degree Disc1
             Anti-Clockwise 80 degree Disc2
             0 degree Disc3
             Clockwise 154.2 degree Disc4
             Clockwise 154.2 degree Disc5.

 *  (cyl_value = 6.25 (0xF6))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun57()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -80 degree (-0.5)****/
	pv.cyl_rotate_input = 80;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree(-1) ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/** @brief : This function operates
             Clockwise 120 degree Disc1
             Anti-Clockwise 40 degree Disc2
             0 degree Disc3
             Clockwise 154.2 degree Disc4
             Clockwise 102.8 degree Disc5.

 *  (cyl_value = 6.5 (0xF7))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun58()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: -40 degree (-0.25)****/
	pv.cyl_rotate_input = 40;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +102.8 degree(-1.25) ****/
	pv.cyl_rotate_input = 257.1;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 120 degree Disc1
            Anti-Clockwise 0 degree Disc2
            0 degree Disc3
            Clockwise 154.2 degree Disc4
            Clockwise 51.43 degree Disc5.

 *  (cyl_value = 6.75 (0xF8))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun59()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: +154.2 degree (-5.25)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +51.4 degree (-1.5) ****/
	pv.cyl_rotate_input = 308.5;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief : This function operates
             Clockwise 120 degree Disc1
             Clockwise 40 degree Disc2
             0 degree Disc3
             Anti-Clockwise 154.2 degree Disc4
             Clockwise 0 degree Disc5.

 *  (cyl_value = 7 (0xF9))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun60()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +40 degree (0.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 120 degree Disc1
            Clockwise 80 degree Disc2
            0 degree Disc3
            Anti-Clockwise 154.2 degree Disc4
            Anti-Clockwise 51.4 degree Disc5.

 *  (cyl_value = 7.25 (0xFA))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun61()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +80 degree (0.5)****/
	pv.cyl_rotate_input = 280;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -51.4 degree (-0.25) ****/
	pv.cyl_rotate_input = 51.4;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 120 degree Disc1
            Clockwise 120 degree Disc2
            0 degree Disc3
            Anti-Clockwise 154.2 degree Disc4
            Anti-Clockwise 102.8 degree Disc5.

 *  (cyl_value = 7.5 (0xFB))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun62()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +120 degree (0.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -102.8 degree(-0.5) ****/
	pv.cyl_rotate_input = 102.8;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}

/* @brief : This function operates
            Clockwise 120 degree Disc1
            Clockwise 160 degree Disc2
            0 degree Disc3
            Anti-Clockwise 154.2 degree Disc4
            Anti-Clockwise 154.2 degree Disc5.

 *  (cyl_value = 7.75 (0xFC))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun63()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun63\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: +120 degree (6.75)****/
	pv.cyl_rotate_input = 240;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +160 degree (1)****/
	pv.cyl_rotate_input = 200;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: 360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : -154.2 degree (-0.75)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);

}

/** @brief :  This function operates
              Anti-Clockwise 40 degree Disc1
              Clockwise 40 degree Disc2
              Clockwise 154.2 degree Disc3
              Anti-Clockwise 154.2 degree Disc4
              Clockwise 154.2 degree Disc5.

 *  (cyl_value = 8 (0xFD))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun64()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun64\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: -40 degree (-2.25)****/
	pv.cyl_rotate_input = 40;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +40 degree (0.25)****/
	pv.cyl_rotate_input = 320;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3: +154.2 degree (10)****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);

	/**** Rotate the disk 4: -154.2 degree (-7)****/
	pv.cyl_rotate_input = 154.2;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : +154.2 degree (-1) ****/
	pv.cyl_rotate_input = 205.7;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);

}

/** @brief : This function operates  0 degree Disc1, 0 degree Disc2, 0 degree Disc3
             0 degree Disc4
             0 degree Disc5.
 *  (cyl_value = 0 (0xB5))
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void cyl_SubFun65()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun65\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree ****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}


/** @brief  : This function operates All disc 0 degree.

 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void cyl_SubFun66()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="cyl_SubFun66\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/**** Rotate the disk 1: 0 degree (0)****/
#if 1
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,1);

	/**** Rotate the disk 2: +360 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,2);

	/**** Rotate the disk 3,4: 0 degree (0)****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,3);
#endif

	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,4);

	/**** Rotate the disk 5 : 360 degree ****/
	pv.cyl_rotate_input = 360;
	Disc_Motor_Rotation(pv.cyl_rotate_input,5);
}
