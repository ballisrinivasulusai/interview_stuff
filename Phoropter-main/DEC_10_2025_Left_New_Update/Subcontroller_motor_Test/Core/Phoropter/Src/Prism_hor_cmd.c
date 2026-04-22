/*
 * prism1_cmd.c
 *
 *  Created on:
 *      Author:
 */

/*** Header File ***/
#include "../Inc/common.h"
#include "../Inc/Prism_hor_cmd.h"

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
uint8_t Prism_hor_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_cmd\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Prism_hor_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Prism_hor_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    Prism_hor_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;

    return 1;
}
/** @brief  : This function operates clockwise 2.25 degree
 *  		  Rotate the lens 0.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun1()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 0.25 command at 2.25 degree */
	pv.prism_hor_input = 1;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 4.50 degree
 *  		  Rotate the lens 0.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 0.50 command at 4.50 degree */
	pv.prism_hor_input = 2;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}
/** @brief  : This function operates clockwise 6.75 degree
 *  		  Rotate the lens 0.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 0.75 command at 6.75 degree */
	pv.prism_hor_input = 3;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}
/** @brief  : This function operates clockwise 9.00 degree
 *  		  Rotate the lens 1.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun4()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.00 command at 9.00 degree */
	pv.prism_hor_input = 4;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 11.25 degree
 *  		  Rotate the lens 1.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun5()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.25 command at 11.25 degree */
	pv.prism_hor_input = 5;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}
/** @brief  : This function operates clockwise 13.50 degree
 *  		  Rotate the lens 1.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun6()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.50 command at 13.50 degree */
	pv.prism_hor_input = 6;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}
/** @brief  : This function operates clockwise 15.75 degree
 *  		  Rotate the lens 1.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun7()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.75 command at 15.75 degree */
	pv.prism_hor_input = 7;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 18.00 degree
 *  		  Rotate the lens 2.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun8()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.00 command at 18.00 degree */
	pv.prism_hor_input = 8;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 20.25 degree
 *  		  Rotate the lens 2.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun9()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.25 command at 20.25 degree */
	pv.prism_hor_input = 9;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 22.50 degree
 *  		  Rotate the lens 2.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun10()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.50 command at 22.50 degree */
	pv.prism_hor_input = 10;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 24.75 degree
 *  		  Rotate the lens 2.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun11()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.75 command at 24.75 degree */
	pv.prism_hor_input = 11;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 27.00 degree
 *  		  Rotate the lens 3.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun12()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.00 command at 27.00 degree */
	pv.prism_hor_input = 12;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 29.25 degree
 *  		  Rotate the lens 3.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun13()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.25 command at 29.25 degree */
	pv.prism_hor_input = 13;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 31.50 degree
 *  		  Rotate the lens 3.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun14()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.50 command at 31.50 degree */
	pv.prism_hor_input = 14;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 33.75 degree
 *  		  Rotate the lens 3.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun15()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.75 command at 33.75 degree */
	pv.prism_hor_input = 15;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 36.00 degree
 *  		  Rotate the lens 4.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun16()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.00 command at 36.00 degree */
	pv.prism_hor_input = 16;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 38.25 degree
 *  		  Rotate the lens 4.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun17()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.25 command at 38.25 degree */
	pv.prism_hor_input = 17;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 40.50 degree
 *  		  Rotate the lens 4.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun18()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.50 command at 40.50 degree */
	pv.prism_hor_input = 18;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 42.75 degree
 *  		  Rotate the lens 4.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun19()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.75 command at 42.75 degree */
	pv.prism_hor_input = 19;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 45.00 degree
 *  		  Rotate the lens 5.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun20()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.00 command at 45.00 degree */
	pv.prism_hor_input = 20;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 47.25 degree
 *  		  Rotate the lens 5.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun21()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.25 command at 47.25 degree */
	pv.prism_hor_input = 21;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 49.50 degree
 *  		  Rotate the lens 5.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun22()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.50 command at 49.50 degree */
	pv.prism_hor_input = 22;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 51.75 degree
 *  		  Rotate the lens 5.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun23()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.75 command at 51.75 degree */
	pv.prism_hor_input = 23;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 54.00 degree
 *  		  Rotate the lens 6.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun24()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.00 command at 54.00 degree */
	pv.prism_hor_input = 24;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 56.25 degree
 *  		  Rotate the lens 6.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun25()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.25 command at 56.25 degree */
	pv.prism_hor_input = 25;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 58.50 degree
 *  		  Rotate the lens 6.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun26()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.50 command at 58.50 degree */
	pv.prism_hor_input = 26;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 60.75 degree
 *  		  Rotate the lens 6.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun27()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.75 command at 60.75 degree */
	pv.prism_hor_input = 27;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 63.00 degree
 *  		  Rotate the lens 7.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun28()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.00 command at 63.00 degree */
	pv.prism_hor_input = 28;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 65.25 degree
 *  		  Rotate the lens 7.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun29()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.25 command at 65.25 degree */
	pv.prism_hor_input = 29;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 67.50 degree
 *  		  Rotate the lens 7.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun30()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.50 command at 67.50 degree */
	pv.prism_hor_input = 30;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 69.75 degree
 *  		  Rotate the lens 7.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun31()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.75 command at 69.75 degree */
	pv.prism_hor_input = 31;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 72.00 degree
 *  		  Rotate the lens 8.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun32()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.00 command at 72.00 degree */
	pv.prism_hor_input = 32;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 74.25 degree
 *  		  Rotate the lens 8.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun33()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.25 command at 74.25 degree */
	pv.prism_hor_input = 33;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 76.50 degree
 *  		  Rotate the lens 8.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void Prism_hor_SubFun34()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.50 command at 76.50 degree */
	pv.prism_hor_input = 34;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 78.75 degree
 *  		  Rotate the lens 8.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun35()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.75 command at 78.75 degree */
	pv.prism_hor_input = 35;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 81.00 degree
 *  		  Rotate the lens 9.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun36()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.00 command at 81.00 degree */
	pv.prism_hor_input = 36;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 83.25 degree
 *  		  Rotate the lens 9.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun37()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.25 command at 83.25 degree */
	pv.prism_hor_input = 37;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 85.50 degree
 *  		  Rotate the lens 9.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun38()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.50 command at 85.50 degree */
	pv.prism_hor_input = 38;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 87.75 degree
 *  		  Rotate the lens 9.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun39()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.75 command at 87.75 degree */
	pv.prism_hor_input = 39;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 90.00 degree
 *  		  Rotate the lens 10.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun40()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.00 command at 90.00 degree */
	pv.prism_hor_input = 40;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 92.25 degree
 *  		  Rotate the lens 10.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun41()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.25 command at 92.25 degree */
	pv.prism_hor_input = 41;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 94.50 degree
 *  		  Rotate the lens 10.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun42()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.50 command at 94.50 degree */
	pv.prism_hor_input = 42;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 96.75 degree
 *  		  Rotate the lens 10.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun43()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.75 command at 96.75 degree */
	pv.prism_hor_input = 43;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 99.00 degree
 *  		  Rotate the lens 11.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun44()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.00 command at 99.00 degree */
	pv.prism_hor_input = 44;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 101.25 degree
 *  		  Rotate the lens 11.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun45()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.25 command at 101.25 degree */
	pv.prism_hor_input = 45;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 103.50 degree
 *  		  Rotate the lens 11.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun46()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.50 command at 103.50 degree */
	pv.prism_hor_input = 46;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 105.75 degree
 *  		  Rotate the lens 11.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun47()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.75 command at 105.75 degree */
	pv.prism_hor_input = 47;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 108.00 degree
 *  		  Rotate the lens 12.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun48()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.00 command at 108.00 degree */
	pv.prism_hor_input = 48;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 110.25 degree
 *  		  Rotate the lens 12.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun49()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.25 command at 110.25 degree */
	pv.prism_hor_input = 49;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 112.50 degree
 *  		  Rotate the lens 12.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun50()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.50 command at 112.50 degree */
	pv.prism_hor_input = 50;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 114.75 degree
 *  		  Rotate the lens 12.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun51()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.75 command at 114.75 degree */
	pv.prism_hor_input = 51;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 117.00 degree
 *  		  Rotate the lens 13.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun52()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun52\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.00 command at 117.00 degree */
	pv.prism_hor_input = 52;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 119.25 degree
 *  		  Rotate the lens 13.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun53()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.25 command at 119.25 degree */
	pv.prism_hor_input = 53;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 121.50 degree
 *  		  Rotate the lens 13.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun54()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.50 command at 121.50 degree */
	pv.prism_hor_input = 54;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 123.75 degree
 *  		  Rotate the lens 13.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun55()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.75 command at 123.75 degree */
	pv.prism_hor_input = 55;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 126.00 degree
 *  		  Rotate the lens 14.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun56()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.00 command at 126.00 degree */
	pv.prism_hor_input = 56;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 128.25 degree
 *  		  Rotate the lens 14.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun57()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.25 command at 128.25 degree */
	pv.prism_hor_input = 57;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 130.50 degree
 *  		  Rotate the lens 14.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun58()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.50 command at 130.50 degree */
	pv.prism_hor_input = 58;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 132.75 degree
 *  		  Rotate the lens 14.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun59()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.75 command at 132.75 degree */
	pv.prism_hor_input = 59;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 135.00 degree
 *  		  Rotate the lens 15.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun60()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.00 command at 135.00 degree */
	pv.prism_hor_input = 60;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 137.25 degree
 *  		  Rotate the lens 15.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun61()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.25 command at 137.25 degree */
	pv.prism_hor_input = 61;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 139.50 degree
 *  		  Rotate the lens 15.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun62()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.50 command at 139.50 degree */
	pv.prism_hor_input = 62;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 141.75 degree
 *  		  Rotate the lens 15.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun63()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun63\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.75 command at 141.75 degree */
	pv.prism_hor_input = 63;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 144.00 degree
 *  		  Rotate the lens 16.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun64()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun64\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.00 command at 144.00 degree */
	pv.prism_hor_input = 64;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 146.25 degree
 *  		  Rotate the lens 16.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun65()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun65\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.25 command at 146.25 degree */
	pv.prism_hor_input = 65;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 148.50 degree
 *  		  Rotate the lens 16.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun66()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun66\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.50 command at 148.50 degree */
	pv.prism_hor_input = 66;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 150.75 degree
 *  		  Rotate the lens 16.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun67()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun67\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.75 command at 150.75 degree */
	pv.prism_hor_input = 67;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 153.00 degree
 *  		  Rotate the lens 17.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun68()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun68\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.00 command at 153.00 degree */
	pv.prism_hor_input = 68;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 155.25 degree
 *  		  Rotate the lens 17.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun69()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun69\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.25 command at 155.25 degree */
	pv.prism_hor_input = 69;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 157.50 degree
 *  		  Rotate the lens 17.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun70()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun70\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.50 command at 157.50 degree */
	pv.prism_hor_input = 70;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 159.75 degree
 *  		  Rotate the lens 17.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun71()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun71\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.75 command at 159.75 degree */
	pv.prism_hor_input = 71;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 162.00 degree
 *  		  Rotate the lens 18.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun72()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun72\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.00 command at 162.00 degree */
	pv.prism_hor_input = 72;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 164.25 degree
 *  		  Rotate the lens 18.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun73()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun73\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.25 command at 164.25 degree */
	pv.prism_hor_input = 73;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 166.50 degree
 *  		  Rotate the lens 18.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun74()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun74\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.50 command at 166.50 degree */
	pv.prism_hor_input = 74;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 168.75 degree
 *  		  Rotate the lens 18.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun75()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun75\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.75 command at 168.75 degree */
	pv.prism_hor_input = 75;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 171.00 degree
 *  		  Rotate the lens 19.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun76()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun76\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.00 command at 171.00 degree */
	pv.prism_hor_input = 76;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 173.25 degree
 *  		  Rotate the lens 19.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun77()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun77\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.25 command at 173.25 degree */
	pv.prism_hor_input = 77;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 175.50 degree
 *  		  Rotate the lens 19.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun78()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun78\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.50 command at 175.50 degree */
	pv.prism_hor_input = 78;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 177.75 degree
 *  		  Rotate the lens 19.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun79()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun79\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.75 command at 177.75 degree */
	pv.prism_hor_input = 79;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates clockwise 180.00 degree
 *  		  Rotate the lens 20.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun80()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun80\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 20.0 command at 180.00 degree */
	pv.prism_hor_input = 80;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 357.75 degree
 *  		  Rotate the lens  -0.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun81()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun81\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -0.25 command at 357.75 degree */
	pv.prism_hor_input = -1;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 355.50 degree
 *  		  Rotate the lens -0.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun82()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun82\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -0.50 command at 355.50 degree */
	pv.prism_hor_input = -2;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 353.25 degree
 *  		  Rotate the lens -0.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun83()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun83\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -0.75 command at 353.25 degree */
	pv.prism_hor_input = -3;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 351.00 degree
 *  		  Rotate the lens -1.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun84()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun84\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.00 command at 351.00 degree */
	pv.prism_hor_input = -4;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 348.75 degree
 *  		  Rotate the lens -1.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun85()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun85\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.25 command at 348.75 degree */
	pv.prism_hor_input = -5;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 346.50 degree
 *  		  Rotate the lens -1.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun86()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun86\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.50 command at 346.50 degree */
	pv.prism_hor_input = -6;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 344.25 degree
 *  		  Rotate the lens .
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun87()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun87\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.75 command at 344.25 degree */
	pv.prism_hor_input = -7;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 342.00 degree
 *  		  Rotate the lens -2.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun88()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun88\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.00 command at 342.00 degree */
	pv.prism_hor_input = -8;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 339.75 degree
 *  		  Rotate the lens -2.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun89()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun89\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.25 command at 339.75 degree */
	pv.prism_hor_input = -9;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 337.50 degree
 *  		  Rotate the lens -2.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun90()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun90\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.50 command at 337.50 degree */
	pv.prism_hor_input = -10;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 335.25 degree
 *  		  Rotate the lens -2.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun91()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun91\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.75 command at 335.25 degree */
	pv.prism_hor_input = -11;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 333.00 degree
 *  		  Rotate the lens -3.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun92()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun92\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.00 command at 333.00 degree */
	pv.prism_hor_input = -12;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 330.75 degree
 *  		  Rotate the lens -3.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun93()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun93\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.25 command at 330.75 degree */
	pv.prism_hor_input = -13;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 328.50 degree
 *  		  Rotate the lens -3.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun94()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun94\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.50 command at 328.50 degree */
	pv.prism_hor_input = -14;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 326.25 degree
 *  		  Rotate the lens -3.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun95()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun95\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.75 command at 326.25 degree */
	pv.prism_hor_input = -15;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 324.00 degree
 *  		  Rotate the lens -4.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun96()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun96\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.00 command at 324.00 degree */
	pv.prism_hor_input = -16;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 321.75 degree
 *  		  Rotate the lens -4.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun97()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun97\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.25 command at 321.75 degree */
	pv.prism_hor_input = -17;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 319.50 degree
 *  		  Rotate the lens -4.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun98()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun98\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.50 command at 319.50 degree */
	pv.prism_hor_input = -18;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 317.25 degree
 *  		  Rotate the lens -4.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun99()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun99\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.75 command at 317.25 degree */
	pv.prism_hor_input = -19;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 315.00 degree
 *  		  Rotate the lens -5.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun100()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun100\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.00 command at 315.00 degree */
	pv.prism_hor_input = -20;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 312.75 degree
 *  		  Rotate the lens -5.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun101()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun101\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.25 command at 312.75 degree */
	pv.prism_hor_input = -21;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 310.50 degree
 *  		  Rotate the lens -5.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun102()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun102\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.50 command at 310.50 degree */
	pv.prism_hor_input = -22;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 308.25 degree
 *  		  Rotate the lens 13.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun103()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun103\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.75 command at 231.75 degree */
	pv.prism_hor_input = -23;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 306.00 degree
 *  		  Rotate the lens -6.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun104()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun104\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.00 command at 234.00 degree */
	pv.prism_hor_input = -24;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 303.75 degree
 *  		  Rotate the lens -6.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun105()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun105\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.25 command at 236.25 degree */
	pv.prism_hor_input = -25;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 301.50 degree
 *  		  Rotate the lens -6.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun106()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun106\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.50 command at 238.50 degree */
	pv.prism_hor_input = -26;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 299.25 degree
 *  		  Rotate the lens -6.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun107()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun107\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.75 command at 240.75 degree */
	pv.prism_hor_input = -27;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 297.00 degree
 *  		  Rotate the lens -7.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun108()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun108\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.00 command at 243.00 degree */
	pv.prism_hor_input = -28;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 294.75 degree
 *  		  Rotate the lens -7.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun109()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun109\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.25 command at 245.25 degree */
	pv.prism_hor_input = -29;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 292.50 degree
 *  		  Rotate the lens -7.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun110()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun110\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.50 command at 247.50 degree */
	pv.prism_hor_input = -30;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 290.25 degree
 *  		  Rotate the lens -7.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun111()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun111\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.75 command at 249.75 degree */
	pv.prism_hor_input = -31;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 288.00 degree
 *  		  Rotate the lens -8.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun112()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun112\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.00 command at 252.00 degree */
	pv.prism_hor_input = -32;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 285.75 degree
 *  		  Rotate the lens -8.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun113()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun113\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.25 command at 254.25 degree */
	pv.prism_hor_input = -33;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 283.50 degree
 *  		  Rotate the lens -8.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun114()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun114\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.50 command at 256.50 degree */
	pv.prism_hor_input = -34;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 281.25 degree
 *  		  Rotate the lens -8.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun115()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun115\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.75 command at 258.75 degree */
	pv.prism_hor_input = -35;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 279.00 degree
 *  		  Rotate the lens -9.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun116()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun116\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.00 command at 261.00 degree */
	pv.prism_hor_input = -36;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 276.75 degree
 *  		  Rotate the lens 13.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun117()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun117\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.25 command at 263.25 degree */
	pv.prism_hor_input = -37;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 274.50 degree
 *  		  Rotate the lens -9.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun118()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun118\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.50 command at 265.50 degree */
	pv.prism_hor_input = -38;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 272.25 degree
 *  		  Rotate the lens -9.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun119()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun119\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.75 command at 267.75 degree */
	pv.prism_hor_input = -39;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 270.00 degree
 *  		  Rotate the lens -10.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun120()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun120\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.00 command at 270.00 degree */
	pv.prism_hor_input = -40;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 267.75 degree
 *  		  Rotate the lens -10.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun121()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun121\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.25 command at 272.25 degree */
	pv.prism_hor_input = -41;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 265.50 degree
 *  		  Rotate the lens -10.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun122()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun122\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.5 command at 274.50 degree */
	pv.prism_hor_input = -42;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 263.25 degree
 *  		  Rotate the lens -10.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun123()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun123\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.75 command at 276.75 degree */
	pv.prism_hor_input = -43;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 261.00 degree
 *  		  Rotate the lens -11.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun124()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun124\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.00 command at 279.00 degree */
	pv.prism_hor_input = -44;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 258.75 degree
 *  		  Rotate the lens -11.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun125()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun125\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.25 command at 281.25 degree */
	pv.prism_hor_input = -45;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 256.50 degree
 *  		  Rotate the lens -11.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun126()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun126\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.50 command at 283.50 degree */
	pv.prism_hor_input = -46;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 254.25 degree
 *  		  Rotate the lens -11.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun127()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun127\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.75 command at 285.75 degree */
	pv.prism_hor_input = -47;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 252.00 degree
 *  		  Rotate the lens -12.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun128()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun128\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.00 command at 288.00 degree */
	pv.prism_hor_input = -48;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 249.75 degree
 *  		  Rotate the lens -12.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun129()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun129\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.25 command at 290.25 degree */
	pv.prism_hor_input = -49;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 247.50 degree
 *  		  Rotate the lens -12.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun130()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun130\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.50 command at 292.50 degree */
	pv.prism_hor_input = -50;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 245.25 degree
 *  		  Rotate the lens -12.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun131()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun131\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.75 command at 294.75  degree */
	pv.prism_hor_input = -51;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 243.00 degree
 *  		  Rotate the lens -13.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun132()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun132\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.00 command at 297.00 degree */
	pv.prism_hor_input = -52;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 240.75 degree
 *  		  Rotate the lens -13.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun133()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun133\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.25 command at 299.25 degree */
	pv.prism_hor_input = -53;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 238.50 degree
 *  		  Rotate the lens -13.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun134()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun134\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.50 command at 301.50 degree */
	pv.prism_hor_input = -54;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 236.25 degree
 *  		  Rotate the lens -13.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun135()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun135\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.75 command at 303.75 degree */
	pv.prism_hor_input = -55;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 234.00 degree
 *  		  Rotate the lens -14.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun136()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun136\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.00 command at 306.00 degree */
	pv.prism_hor_input = -56;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 231.75 degree
 *  		  Rotate the lens -14.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun137()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun137\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.25 command at 308.25 degree */
	pv.prism_hor_input = -57;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 229.50 degree
 *  		  Rotate the lens -14.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun138()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun138\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.50 command at 310.50 degree */
	pv.prism_hor_input = -58;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 227.25 degree
 *  		  Rotate the lens -14.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun139()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun139\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.75 command at 312.75 degree */
	pv.prism_hor_input = -59;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 225.00 degree
 *  		  Rotate the lens -15.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun140()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun140\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.00 command at 315.00 degree */
	pv.prism_hor_input = -60;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 222.75 degree
 *  		  Rotate the lens -15.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun141()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun141\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.25 command at 317.25 degree */
	pv.prism_hor_input = -61;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 220.50 degree
 *  		  Rotate the lens -15.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun142()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun142\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.50 command at 319.50 degree */
	pv.prism_hor_input = -62;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 218.25 degree
 *  		  Rotate the lens -15.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun143()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun143\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.75 command at 321.75 degree */
	pv.prism_hor_input = -63;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 216.00 degree
 *  		  Rotate the lens -16.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun144()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun144\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 324.00 degree */
	pv.prism_hor_input = -64;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 213.75 degree
 *  		  Rotate the lens -16.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun145()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun145\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 326.25 degree */
	pv.prism_hor_input = -65;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 211.50 degree
 *  		  Rotate the lens -16.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun146()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun146\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.50 command at 328.50 degree */
	pv.prism_hor_input = -66;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 209.25 degree
 *  		  Rotate the lens -16.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun147()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun147\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.75 command at 330.75 degree */
	pv.prism_hor_input = -67;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 207.00 degree
 *  		  Rotate the lens -17.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun148()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun148\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 333.00 degree */
	pv.prism_hor_input = -68;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 204.75 degree
 *  		  Rotate the lens -17.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun149()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun149\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 335.25 degree */
	pv.prism_hor_input = -69;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 337.50 degree
 *  		  Rotate the lens -17.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun150()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun150\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 202.50 degree */
	pv.prism_hor_input = -70;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 200.25 degree
 *  		  Rotate the lens -17.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun151()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun151\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 200.25 degree */
	pv.prism_hor_input = -71;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 198.00 degree
 *  		  Rotate the lens -18.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun152()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun152\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.00 command at 198.00 degree */
	pv.prism_hor_input = -72;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 195.75 degree
 *  		  Rotate the lens -18.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun153()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun153\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.25 command at 195.75 degree */
	pv.prism_hor_input = -73;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 193.50 degree
 *  		  Rotate the lens -18.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun154()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun154\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.50 command at 193.50 degree */
	pv.prism_hor_input = -74;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 191.25 degree
 *  		  Rotate the lens -18.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun155()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun155\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.75 command at 191.25 degree */
	pv.prism_hor_input = -75;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 189.00 degree
 *  		  Rotate the lens -19.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun156()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun156\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.00 command at 189.00 degree */
 	pv.prism_hor_input = -76;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 186.75 degree
 *  		  Rotate the lens -19.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun157()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun157\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.25 command at 186.75 degree */
	pv.prism_hor_input = -77;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 184.50 degree
 *  		  Rotate the lens -19.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun158()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun158\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.50 command at 184.50 degree */
	pv.prism_hor_input = -78;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 182.25 degree
 *  		  Rotate the lens -19.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun159()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun159\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.75 command at 182.25 degree */
	pv.prism_hor_input = -79;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 360.00 degree
 *  		  Rotate the lens -20.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun160()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun160\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 90.00 degree */
	pv.prism_hor_input = -80;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 360.00 degree
 *  		  Rotate the lens -20.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun161()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun161\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 360.00 degree */
	pv.prism_hor_input = 0;
	pv.prism_select = 2;
	prism_function(pv.prism_select);
}
/** @brief  : This function operates Anti-clockwise 360.00 degree
 *  		  Rotate the lens -20.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_hor_SubFun162()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_hor_SubFun162\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 360.00 degree */
	if(pv.ver_temp == 0)
	{
		pv.prism_hor_input = 0;
		pv.prism_ver_input = 0;
		pv.prism_select = 2;
		prism_function(pv.prism_select);
	}
}
