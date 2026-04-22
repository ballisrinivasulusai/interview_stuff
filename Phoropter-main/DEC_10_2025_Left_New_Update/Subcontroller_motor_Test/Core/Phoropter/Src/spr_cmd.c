/*
 * process_cmd.c
 *
 *  Created on:
 *      Author:
 */

/* Header Files  */
#include "../Inc/common.h"
#include "../Inc/spr_cmd.h"

/* Global variable Declaration  */
extern private_var pv;


/** @brief  : This function processes the received command.
 *
 *  @param  : cbuff[OUT]  : Receive data buffer
 *            len         : Buffer len
 *
 *  @return : None
 *
 */
uint8_t sph_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Send the Ack to module  */
	/* Validate the max array size  */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(spr_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function  */
    if (NULL == spr_func[cbuff[1]].cb)
		return 0;

    if (MOTOR1_RUNNING == pv.cmd_status)
    {
    	return 0;
    }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    spr_func[cbuff[1]].cb(cbuff, len);

	pv.cmd_status = MOTOR_STATUS_STOP;

    return 0;
}

/* @brief  : This function operates Anti-clockwise 40 degree Disc2
 *           and 0 position for Disc 1,3,4,5.
 *
 * @param  : none
 *
 * @return : none
 *
 */
void spr_SubFun1()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 2 : 40 degree (-0.25)  */
	pv.rotate2_input = 40;
	pv.motor_flag = MOTOR1_RUNNING;
    Disc_Motor_Rotation(pv.rotate2_input,2);

    /* Rotate the disk 1,3,4,5 : 0 degree (0)  */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/** @brief  : This function operates Anti-clockwise 80 degree Disc2
 *            and 0 th position for Disc 1,3,4,5.
 *
 *  @param  : none.
 *
 *  @return : none.
 *
 */
void spr_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk2 : 80 degree (-0.5)  */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 1,3,4,5 : 0 degree (0)  */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/** @brief  : This function operates Anti-clockwise 120 degree Disc2
 *            and 0th position for Disc 1,3,4,5.
 *
 *  @param  : none.
 *
 *  @return : none.
 *
 */
void spr_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 2 : 120 degree (-0.75)  */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 1,3,4,5 : 0 degree (0)  */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/** @brief  : This function operates Anti-clockwise 160 degree Disc2
 *            and 0th position for Disc 1,3,4,5.
 *
 *  @param  : none.
 *
 *  @return : none.
 *
 */
void spr_SubFun4()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 2 : 160 degree (-1)  */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 1,3,4,5 : 0 degree (0)  */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}
/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *            clockwise 160 degree Disc2 and 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun5()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (1)  */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1
 *       	  and clockwise 120 degree Disc2 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun6()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module  */
	uint8_t scmd[]="spr_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
    pv.rotate1_input = 40;
    Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (0.75)  */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *  		  clockwise 80 degree Disc2 and 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun7()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module  */
	uint8_t scmd[]="spr_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (0.5)  */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1
 *            clockwise 40 degree Disc2 and 0th position for Disc 3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun8()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (0.25)  */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *            0th position for Disc 2,3,4,5..
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun9()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0)  */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *            Anti-clockwise 40 degree Disc2 and 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun10()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-2.25)  */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *  		  Anti-clockwise 80 degree Disc2 and 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun11()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun11\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5)  */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *  		  Anti-clockwise 120 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun12()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75)  */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 40 degree Disc1,
 *  		   Anti-clockwise 160 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun13()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25)  */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1)  */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *  		  clockwise 160 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun14()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (+1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *             clockwise 120 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun15()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (0.75)  */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *             clockwise 80 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun16()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (0.5)  */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *  		  clockwise 40 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun17()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (0.25)  */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *            & 0th position for Disc 2,3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun18()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2,3,4,5 : 0 degree (0)  */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *  		  Anti-clockwise 40 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun19()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25)  */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *             Anti-clockwise 80 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun20()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 80 degree Disc1,
 *            Anti-clockwise 120 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun21()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 80 degree Disc1,
 *            Anti-clockwise 160 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun22()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1,
 *            clockwise 160 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun23()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (+1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1,
 *            clockwise 120 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun24()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1,
 *  		  clockwise 80 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun25()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1
 *      	  clockwise 40 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun26()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1,
 *  		  & 0th position for Disc 2,3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun27()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1,
 * 		      Anti-clockwise 40 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun28()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1,
 *            Anti-clockwise 80 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun29()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates Anti-clockwise 120 degree Disc1
 *  		  Anti-clockwise 120 degree Disc2 & 0th position for Disc3,4,5.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void spr_SubFun30()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *            and Anti-clockwise 160 degree Disc2 (sph_value = -7.75)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun31()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 160 degree Disc2 (sph_value = -8)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun32()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *             and Clockwise 120 degree Disc2 (sph_value = -8.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun33()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *             and Clockwise 80 degree Disc2 (sph_value = -8.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun34()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 40 degree Disc2 (sph_value = -8.75)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun35()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 0 degree Disc2 (sph_value = -9)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun36()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2,3,4,5 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 (sph_value = -9.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun37()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *             and Anti-Clockwise 80 degree Disc2 (sph_value = -9.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun38()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *             and Anti-Clockwise 120 degree Disc2 (sph_value = -9.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun39()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *             and Anti-Clockwise 160 degree Disc2 (sph_value = -10)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun40()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Clockwise 160 degree Disc2 (sph_value = -10.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun41()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Clockwise 120 degree Disc2 (sph_value = -10.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun42()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Clockwise 80 degree Disc2 (sph_value = -10.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun43()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Clockwise 40 degree Disc2 (sph_value = -11)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun44()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Clockwise 0 degree Disc2 (sph_value = -11.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun45()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2,3,4,5 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *             and Anti-Clockwise 40 degree Disc2 (sph_value = -11.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun46()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 (sph_value = -11.75)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun47()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *            and Anti-Clockwise 120 degree Disc2 (sph_value = -12)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun48()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates clockwise 160 degree Disc1
 *             and Anti-Clockwise 160 degree Disc2 (sph_value = -12.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun49()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3,4,5 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief    : This function operates Anti-clockwise 40 degree Disc1
 *             and Anti-Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -12.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun50()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 40 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -12.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun51()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);
	HAL_Delay(50);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);
	HAL_Delay(50);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :This function operates Anti-clockwise 40 degree Disc1
 *           and Anti-Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *           (sph_value = -13)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun52()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun52\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}


/* @brief   : This function operates Anti-clockwise 40 degree Disc1
 *             and Anti-Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -13.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun53()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Anti-clockwise 80 degree Disc1
 *             and Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -13.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun54()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (+1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief    : This function operates Anti-clockwise 80 degree Disc1
 *              and Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *              (sph_value = -13.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun55()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 80 degree Disc1
 *             and Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -14)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun56()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 80 degree Disc1
 *             and Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -14.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun57()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (+0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 80 degree Disc1
 *             and Clockwise 0 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -14.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun58()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate2_input = 154.3;
	Disc_Motor_Rotation(pv.rotate2_input,3);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,2);
}

/* @brief   : This function operates Anti-clockwise 80 degree Disc1
 *             and Anti-Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -14.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun59()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 80 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -15)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun60()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Anti-clockwise 80 degree Disc1
 *             and Anti-Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -15.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun61()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5)  */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.30 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}


/* @brief   : This function operates Anti-clockwise 80 degree Disc1
 *              and Anti-Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *              (sph_value = -15.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun62()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 80 degree (-4.5) */
	pv.rotate1_input = 80;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *            and Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -15.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun63()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun63\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief    : This function operates Anti-clockwise 120 degree Disc1
 *             and Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -16)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun64()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun64\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *            and Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -16.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun65()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun65\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *              and Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *              (sph_value = -16.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun66()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun66\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *            and Clockwise 0 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -16.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun67()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun67\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 360 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -17)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun68()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun68\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Anti-clockwise 120 degree Disc1
 *             and Anti-Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -17.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun69()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun69\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *             and Anti-Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -17.5)
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun70()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun70\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 120 degree Disc1
 *             and Anti-Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -17.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun71()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun71\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 120 degree (-6.75) */
	pv.rotate1_input = 120;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree(-10)  */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/*  @brief  : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -18)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun72()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun72\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -18.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun73()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun73\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -18.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun74()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun74\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -18.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun75()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun75\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Clockwise 0 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -19)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun76()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun76\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -19.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun77()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun77\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-0.25) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Anti-clockwise 160 degree Disc1
 *             and Anti-Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -19.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun78()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun78\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Anti-Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -19.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun79()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun79\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-clockwise 160 degree Disc1
 *            and Anti-Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -20)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun80()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun80\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 160 degree (-9) */
	pv.rotate1_input = 160;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -20.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun81()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun81\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -20.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun82()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun82\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *             and Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -20.75)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun83()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun83\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -21)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun84()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun84\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Clockwise 0 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -21.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun85()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun85\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 360 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -21.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun86()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun86\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -21.75)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun87()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun87\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (1) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *             and Anti-Clockwise 120 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *             (sph_value = -22)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun88()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun88\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Anti-Clockwise 160 degree Disc2 and Anti-Clockwise -154.3 Disc3
 *            (sph_value = -22.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun89()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun89\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 200 degree (-11.25) */
	pv.rotate1_input = 200;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 154.3 degree (-10) */
	pv.rotate3_input = 154.3;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 0 degree Disc1
 *             and Anti-Clockwise 0 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *             (sph_value = 0)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun90()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun90\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 360 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Clockwise 0 degree Disc1
 *             and Clockwise 40 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *             (sph_value = 0.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun91()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun91\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 0 degree Disc1
 *            and Clockwise 80 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 0.5)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun92()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun92\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 0 degree Disc1
 *             and Clockwise 120 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *             (sph_value = 0.75)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun93()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun93\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 0 degree Disc1
 *            and Clockwise 160 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 1)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun94()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun94\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Anti-Clockwise 160 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 1.25)
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun95()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun95\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Anti-Clockwise 120 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 1.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun96()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun97\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 1.75)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun97()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun97\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 2)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun98()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun98\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Anti-Clockwise 0 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 2.25)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun99()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun99\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}


/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Clockwise 40 degree Disc2 and Anti-Clockwise 0 degree Disc3
 *            (sph_value = 2.5)
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun100()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun100\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}


/* @brief   : This function operates clockwise 40 degree Disc1
 *            and Clockwise 80 degree Disc2 (sph_value = 2.75 (0x6F))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun101()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun101\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Clockwise 120 degree Disc2 (sph_value = 3 (0x70))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun102()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun102\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 360 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1
 *            and Clockwise 160 degree Disc2 (sph_value = 3.25 (0x71))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun103()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun103\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1
 *            and Anti-clockwise 160 degree Disc2 (sph_value = 3.5 (0x72))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun104()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun104\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1
 *            and Anti-Clockwise 120 degree Disc2 (sph_value = 3.75 (0x73))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun105()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun105\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 (sph_value = 4 (0x74))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun106()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun106\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 (sph_value = 4.25(0x75))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun107()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun107\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1.
 *            (sph_value = 4.5(0x76))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun108()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun108\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);

}

/* @brief   : This function operates Clockwise 80 degree Disc1
 *            and Clockwise 40 degree Disc2 (sph_value = 4.75 (0x77)).
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun109()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun109\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Clockwise 80 degree Disc1
 *             and Clockwise 80 degree Disc2 (sph_value = 5 (0x78))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun110()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun110\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1
 *            and Clockwise 120 degree Disc2 (sph_value = 5.25 (0x79))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun111()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun111\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 160 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1
 *            and Clockwise 160 degree Disc2 (sph_value = 5.5 (0x7A))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun112()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun112\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Anti-Clockwise 160 degree Disc2 (sph_value = 5.75 (0x7B))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun113()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun113\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Anti-Clockwise 120 degree Disc2 (sph_value = 6 (0x7C))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun114()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun114\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Anti-Clockwise 80 degree Disc2 (sph_value = 6.25 (0x7D))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun115()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun115\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Anti-Clockwise 40 degree Disc2 (sph_value = 6.5 (0x7E))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun116()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun116\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            (sph_value = 6.75 (0x7F)).
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun117()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun117\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Clockwise 120 degree Disc1
 *             and Clockwise 40 degree Disc2 (sph_value = 7 (0x80))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun118()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun118\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Clockwise 80 degree Disc2 (sph_value = 7.25 (0x81))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun119()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun119\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 160 degree (1) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Clockwise 120 degree Disc2 (sph_value = 7.5 (0x82))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun120()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun120\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1
 *            and Anti-Clockwise 160 degree Disc2 (sph_value = 7.75 (0x83))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun121()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun121\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 40 degree Disc1,
 *            Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 8 (0x84))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun122()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun122\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 40 degree Disc1,
 *             Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *             (sph_value = 8.25 (0x85))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun123()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun123\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

   /* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 40 degree Disc1,
 *            Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 8.5 (0x86))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun124()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun124\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 40 degree Disc1,
 *            Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 8.75 (0x87))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun125()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun125\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 40 degree (-2.25) */
	pv.rotate1_input = 40;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 160 degree Disc1
 *            and Clockwise 154.3 degree Disc3.(sph_value = 9 (0x88)).
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun126()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun126\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 120 degree Disc1
 *            and Clockwise 154.3 degree Disc3.(sph_value = 9.25 (0x89))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun127()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun127\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 80 degree Disc1
 *            and Clockwise 154.3 degree Disc3.(sph_value = 9.5 (0x8A))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun128()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun128\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Anti-Clockwise 40 degree Disc1
 *            and Clockwise 154.3 degree Disc3.(sph_value = 9.75 (0x8B))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun129()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun129\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 0 degree Disc1
 *            and Clockwise 154.3 degree Disc3.(sph_value = 10 (0x8C))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun130()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun130\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *            and Clockwise 154.3 degree Disc3.
 *            (sph_value = 10.25 (0x8D))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun131()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun131\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            and Clockwise 154.3 degree Disc3.
 *            (sph_value = 10.5 (0x8E))
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun132()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun132\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            and Clockwise 154.3 degree Disc3.
 *            (sph_value = 10.75 (0x8F))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun133()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun133\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 160 degree Disc1,
 *            and Clockwise 154.3 degree Disc3.
 *            (sph_value = 11 (0x90))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun134()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun134\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Clockwise 40 degree Disc1,
 *             Anti-Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *             (sph_value = 11.25 (0x91))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun135()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun135\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 160 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Clockwise 40 degree Disc1,
 *             Anti-Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *             (sph_value = 11.5 (0x92))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun136()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun136\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *            Anti-Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 11.75 (0x93))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun137()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun137\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *            Anti-Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 12 (0x94))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun138()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun138\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *            and Clockwise 154.3 degree Disc3.
 *            (sph_value = 12.25 (0x95))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun139()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun139\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *            Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 12.5 (0x96))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun140()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun140\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *             Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *             (sph_value = 12.75 (0x97))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun141()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun141\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *           Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *           (sph_value = 13 (0x98))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun142()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun142\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 40 degree Disc1,
 *            Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 13.25(0x99))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun143()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun143\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 320 degree (2.25) */
	pv.rotate1_input = 320;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Anti-Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 13.5 (0x9A))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun144()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun144\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Anti-Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 13.75 (0x9B))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun145()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun145\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Anti-Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *           (sph_value = 14(0x9C))
 *
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun146()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun146\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Anti Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 14.25 (0x9D))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun147()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun147\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}


/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            and Clockwise 154.3 degree Disc3.
 *            (sph_value = 14.5 (0x9E))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun148()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun148\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}


/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 14.75 (0x9F))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun149()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun149\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 320 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}



/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 15 (0xA0))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun150()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun150\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/*  @brief  :  This function operates Clockwise 80 degree Disc1,
 *            Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 15.25 (0xA1))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun151()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun151\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 80 degree Disc1,
 *            Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 15.5 (0xA2))
 *
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun152()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun152\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 280 degree (4.5) */
	pv.rotate1_input = 280;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   :  This function operates Clockwise 120 degree Disc1,
 *             Anti-Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *             (sph_value = 15.75 (0xA3))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun153()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun153\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (-1) */
	pv.rotate2_input = 160;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Anti-Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 16 (0xA4))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun154()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun154\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 120 degree (-0.75) */
	pv.rotate2_input = 120;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Anti-Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 16.25 (0xA5))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun155()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun155\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 80 degree (-0.5) */
	pv.rotate2_input = 80;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Anti-Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 16.5 (0xA6))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun156()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun156\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 40 degree (-0.25) */
	pv.rotate2_input = 40;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *             and Clockwise 154.3 degree Disc3.
 *             (sph_value = 16.75 (0xA7))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun157()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun157\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 360 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Clockwise 40 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 17 (0xA8))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun158()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun158\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 160 degree (0.25) */
	pv.rotate2_input = 320;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Clockwise 80 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 17.25 (0xA9))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun159()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun159\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

    /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 280 degree (0.5) */
	pv.rotate2_input = 280;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Clockwise 120 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 17.5 (0xAA))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun160()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun160\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

   /* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 240 degree (0.75) */
	pv.rotate2_input = 240;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/* @brief   : This function operates Clockwise 120 degree Disc1,
 *            Clockwise 160 degree Disc2 and Clockwise 154.3 degree Disc3.
 *            (sph_value = 17.75 (0xAB))
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun161()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun161\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 240 degree (6.75) */
	pv.rotate1_input = 240;
	Disc_Motor_Rotation(pv.rotate1_input,1);
	HAL_Delay(50);

	/* Rotate the disk 2 : 200 degree (1) */
	pv.rotate2_input = 200;
	Disc_Motor_Rotation(pv.rotate2_input,2);
	HAL_Delay(50);

	/* Rotate the disk 3 : 205.7 degree (10) */
	pv.rotate3_input = 205.7;
	Disc_Motor_Rotation(pv.rotate3_input,3);
	HAL_Delay(50);
}


/* @brief   :  All disc should come to 0 position
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
  */
void spr_SubFun162()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun162\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 1 : 0 degree (0) */
	pv.rotate1_input = 360;
	Disc_Motor_Rotation(pv.rotate1_input,1);

	/* Rotate the disk 2 : 0 degree (0) */
	pv.rotate2_input = 360;
	Disc_Motor_Rotation(pv.rotate2_input,2);

	/* Rotate the disk 3 : 0 degree (0) */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function Xcyl commands
 *
 *            This function operates 51.43 degree Disc3
 *
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void spr_SubFun163()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun163\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 3 : 51.4 degree  */
	pv.rotate3_input = 51.4;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates  102.8 degree
 *            Disc3
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void spr_SubFun164()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun164\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 3 : 102.8 degree  */
	pv.rotate3_input = 102.8;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates 0 degree
 *            Disc3
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void spr_SubFun165()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun165\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 3 : 360 degree (0)  */
	pv.rotate3_input = 360;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}
void spr_SubFun166()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun166\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 3 : 308.6 degree  */
	pv.rotate3_input = 308.6;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}

/** @brief  : This function operates 0 degree
 *            Disc3
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void spr_SubFun167()
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="spr_SubFun167\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif

	/* Rotate the disk 3 : 257.2 degree (0)  */
	pv.rotate3_input = 257.2;
	Disc_Motor_Rotation(pv.rotate3_input,3);
}
