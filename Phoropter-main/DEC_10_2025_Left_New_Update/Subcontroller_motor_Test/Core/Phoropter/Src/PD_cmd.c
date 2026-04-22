/*
 * PD_cmd.c
 *
 *  Created on:
 *      Author:
 */

/*** Header File ***/
#include "../Inc/common.h"
#include "../Inc/PD_cmd.h"

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
uint8_t PD_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(PD_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == PD_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    PD_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;

    return 1;
}


/** @brief  :This function call the PD motor rotation
 *           function with 0.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void PD1_SubFun1()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 0.5 mm */
	pv.PD1_rotate_input = 1;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 1;
}

/** @brief  :This function call the PD motor rotation
 *           function with 1 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun2()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 1 mm */
	pv.PD1_rotate_input = 2;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 2;
}

/** @brief  : This function call the PD motor rotation
 *            function with 1.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun3()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 1.5 mm */
    pv.PD1_rotate_input = 3;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 3;
}

/** @brief  : This function call the PD motor rotation
 *            function with 2 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun4()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 2 mm */
	pv.PD1_rotate_input = 4;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 4;
}

/** @brief  : This function call the PD motor rotation
 *            function with 2.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun5()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 2.5 mm */
	pv.PD1_rotate_input = 5;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 5;
}

/** @brief  : This function call the PD motor rotation
 *            function with 3 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun6()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 3 mm */
	pv.PD1_rotate_input = 6;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 6;
}


/** @brief  : This function call the PD motor rotation
 *            function with 3.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun7()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 3.5 mm */
	pv.PD1_rotate_input = 7;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 7;
}

/** @brief  : This function call the PD motor rotation
 *            function with 4 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun8()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 4 mm */
	pv.PD1_rotate_input = 8;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 8;
}

/** @brief  : This function call the PD motor rotation
 *            function with 4.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun9()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 4.5 mm */
	pv.PD1_rotate_input = 9;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 9;
}

/** @brief  : This function call the PD motor rotation
 *            function with 5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun10()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 5 mm */
	pv.PD1_rotate_input = 10;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 10;
}

/** @brief  : This function call the PD motor rotation
 *            function with 5.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun11()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun11\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 5.5 mm */
	pv.PD1_rotate_input = 11;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 11;
}

/** @brief  : This function call the PD motor rotation
 *            function with 6 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun12()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 6 mm */
	pv.PD1_rotate_input = 12;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 12;
}

/** @brief  : This function call the PD motor rotation
 *            function with 6.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun13()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 6.5 mm */
	pv.PD1_rotate_input = 13;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 13;
}

/** @brief  : This function call the PD motor rotation
 *            function with 7 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun14()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 7 mm */
	pv.PD1_rotate_input = 14;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 14;
}

/** @brief  : This function call the PD motor rotation
 *            function with 7.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun15()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 7.5 mm */
	pv.PD1_rotate_input = 15;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 15;
}

/** @brief  : This function call the PD motor rotation
 *            function with 8 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun16()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 8 mm */
	pv.PD1_rotate_input = 16;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 16;
}

/** @brief  : This function call the PD motor rotation
 *            function with 8.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun17()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 8.5 mm */
	pv.PD1_rotate_input = 17;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 17;
}

/** @brief  : This function call the PD motor rotation
 *            function with  9 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun18()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 9 mm */
	pv.PD1_rotate_input = 18;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 18;
}

/** @brief  : This function call the PD motor rotation
 *            function with 9.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun19()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 9.5 mm */
	pv.PD1_rotate_input = 19;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 19;
}

/** @brief  : This function call the PD motor rotation
 *            function with 10 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun20()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 10 mm */
	pv.PD1_rotate_input = 20;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 20;
}

/** @brief  : This function call the PD motor rotation
 *            function with 10.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun21()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 10.5 mm */
	pv.PD1_rotate_input = 21;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 21;
}

/** @brief  : This function call the PD motor rotation
 *            function with 11 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun22()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 11 mm */
	pv.PD1_rotate_input = 22;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 22;
}

/** @brief  : This function call the PD motor rotation
 *            function with 11.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun23()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 11.5 mm */
	pv.PD1_rotate_input = 23;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 23;
}

/** @brief  : This function call the PD motor rotation
 *            function with 12 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun24()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 12 mm */
	pv.PD1_rotate_input = 24;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 24;
}

/** @brief  : This function call the PD motor rotation
 *            function with 12.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun25()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 12.5 mm */
    pv.PD1_rotate_input = 25;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 25;
}

/** @brief  : This function call the PD motor rotation
 *            function with 13 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun26()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 13 mm */
	pv.PD1_rotate_input = 26;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 26;
}

/** @brief  : This function call the PD motor rotation
 *            function with 13.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun27()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 13.5 mm */
	pv.PD1_rotate_input = 27;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 27;
}

/** @brief  : This function call the PD motor rotation
 *            function with 14 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun28()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 14 mm */
	pv.PD1_rotate_input = 28;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 28;
}

/** @brief  : This function call the PD motor rotation
 *            function with 14.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun29()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 14.5 mm */
	pv.PD1_rotate_input = 29;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 29;
}

/** @brief  : This function call the PD motor rotation
 *            function with  15 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *0
 */

void PD1_SubFun30()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to 15 mm */
	pv.PD1_rotate_input = 30;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old - 30;
}

/** @brief  : This function call the PD motor rotation
 *            function with -0.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun31()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -0.5 mm */
	pv.PD1_rotate_input = -1;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 1;
}

/** @brief  : This function call the PD motor rotation
 *            function with -1 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void PD1_SubFun32()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -1 mm */
	pv.PD1_rotate_input = -2;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 2;
}

/** @brief  : This function call the PD motor rotation
 *            function with -1.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun33()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -1.5 mm */
    pv.PD1_rotate_input = -3;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 3;
}

/** @brief  : This function call the PD motor rotation
 *            function with -2 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun34()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -2 mm */
	pv.PD1_rotate_input = -4;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 4;
}

/** @brief  : This function call the PD motor rotation
 *            function with -2.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun35()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -2.5 mm */
	pv.PD1_rotate_input = -5;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 5;
}

/** @brief  : This function call the PD motor rotation
 *            function with -3 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun36()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -3 mm */
	pv.PD1_rotate_input = -6;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 6;
}

/** @brief  : This function call the PD motor rotation
 *            function with -3.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun37()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -3.5 mm */
	pv.PD1_rotate_input = -7;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 7;
}

/** @brief  : This function call the PD motor rotation
 *            function with -4 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun38()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -4 mm */
	pv.PD1_rotate_input = -8;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 8;
}

/** @brief  : This function call the PD motor rotation
 *            function with -4.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun39()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -4.5 mm */
	pv.PD1_rotate_input = -9;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 9;
}

/** @brief  : This function call the PD motor rotation
 *            function with -5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun40()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -5 mm */
	pv.PD1_rotate_input = -10;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 10;
}

/** @brief  : This function call the PD motor rotation
 *            function with -5.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun41()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -5.5 mm */
	pv.PD1_rotate_input = -11;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 11;
}

/** @brief  : This function call the PD motor rotation
 *            function with -6 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun42()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -6 mm */
	pv.PD1_rotate_input = -12;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 12;
}

/** @brief  : This function call the PD motor rotation
 *            function with -6.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun43()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -6.5 mm */
	pv.PD1_rotate_input = -13;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 13;
}

/** @brief  : This function call the PD motor rotation
 *            function with -7 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun44()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -7 mm */
	pv.PD1_rotate_input = -14;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 14;
}

/** @brief  : This function call the PD motor rotation
 *            function with -7.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun45()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -7.5 mm */
	pv.PD1_rotate_input = -15;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 15;
}

/** @brief  : This function call the PD motor rotation
 *            function with -8 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun46()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -8 mm */
	pv.PD1_rotate_input = -16;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 16;
}

/** @brief  : This function call the PD motor rotation
 *            function with -8.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun47()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -8.5 mm */
	pv.PD1_rotate_input = -17;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 17;
}

/** @brief  : This function call the PD motor rotation
 *            function with -9 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun48()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -9 mm */
	pv.PD1_rotate_input = -18;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 18;
}

/** @brief  : This function call the PD motor rotation
 *            function with -9.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun49()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -9.5 mm */
	pv.PD1_rotate_input = -19;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 19;
}

/** @brief  : This function call the PD motor rotation
 *            function with -10 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun50()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -10 mm */
	pv.PD1_rotate_input = -20;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 20;
}

/** @brief  : This function call the PD motor rotation
 *            function with -10.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun51()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -10.5 mm */
	pv.PD1_rotate_input = -21;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 21;
}

/** @brief  : This function call the PD motor rotation
 *            function with -11 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun52()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun52\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -11 mm */
	pv.PD1_rotate_input = -22;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 22;
}

/** @brief  : This function call the PD motor rotation
 *            function with -11.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun53()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -11.5 mm */
	pv.PD1_rotate_input = -23;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 23;
}

/** @brief  : This function call the PD motor rotation
 *            function with -12 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun54()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -12 mm */
	pv.PD1_rotate_input = -24;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 24;
}

/** @brief  : This function call the PD motor rotation
 *            function with -12.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun55()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -12.5 mm */
    pv.PD1_rotate_input = -25;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 25;
}

/** @brief  : This function call the PD motor rotation
 *            function with -13 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun56()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -13 mm */
	pv.PD1_rotate_input = -26;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 26;
}

/** @brief  : This function call the PD motor rotation
 *            function with -13.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun57()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -13.5 mm */
	pv.PD1_rotate_input = -27;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 27;
}

/** @brief  : This function call the PD motor rotation
 *            function with -14 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun58()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -14 mm */
	pv.PD1_rotate_input = -28;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 28;
}

/** @brief  : This function call the PD motor rotation
 *            function with -14.5 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void PD1_SubFun59()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -14.5 mm */
	pv.PD1_rotate_input = -29;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 29;
}

/** @brief  : This function call the PD motor rotation
 *            function with  -15 mm.
 *
 *  @param  : none
 *
 *  @return : none
 *0
 */

void PD1_SubFun60()
{
	/* Send the Ack to module */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD1_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
    /* Move the motor to -15 mm */
	pv.PD1_rotate_input = -30;
	PD1_motor(pv.PD1_rotate_input);
	pv.PD1_temp_old = pv.PD1_temp_old + 30;
}

/** @brief  : This function power on the Disk LED.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void PD1_SubFun61()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		pv.spcl_rotate_input = 200;
		Disc_Motor_Rotation(pv.spcl_rotate_input,6);
	}
}

void PD1_SubFun62()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="PD_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		pv.spcl_rotate_input = 360;
		Disc_Motor_Rotation(pv.spcl_rotate_input,6);
	}
}

