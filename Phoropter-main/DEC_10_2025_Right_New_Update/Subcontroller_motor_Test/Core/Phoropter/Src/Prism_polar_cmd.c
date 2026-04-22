/*
 * prism1_cmd.c
 *
 *  Created on:
 *      Author:
 */

/*** Header File ***/
#include "../Inc/common.h"
#include "../Inc/Prism_polar_cmd.h"

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
uint8_t Prism_polar_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_cmd\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Prism_polar_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Prism_polar_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    Prism_polar_func[cbuff[1]].cb(cbuff, len);

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
void Prism_polar_SubFun1()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 0.25 command at 2.25 degree */
	pv.prism_polar_input = 1;
	pv.prism_select = 4;
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

void Prism_polar_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 0.50 command at 4.50 degree */
	pv.prism_polar_input = 2;
	pv.prism_select = 4;
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
void Prism_polar_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 0.75 command at 6.75 degree */
	pv.prism_polar_input = 3;
	pv.prism_select = 4;
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

void Prism_polar_SubFun4()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.00 command at 9.00 degree */
	pv.prism_polar_input = 4;
	pv.prism_select = 4;
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
void Prism_polar_SubFun5()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.25 command at 11.25 degree */
	pv.prism_polar_input = 5;
	pv.prism_select = 4;
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
void Prism_polar_SubFun6()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.50 command at 13.50 degree */
	pv.prism_polar_input = 6;
	pv.prism_select = 4;
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
void Prism_polar_SubFun7()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 1.75 command at 15.75 degree */
	pv.prism_polar_input = 7;
	pv.prism_select = 4;
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
void Prism_polar_SubFun8()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.00 command at 18.00 degree */
	pv.prism_polar_input = 8;
	pv.prism_select = 4;
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
void Prism_polar_SubFun9()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.25 command at 20.25 degree */
	pv.prism_polar_input = 9;
	pv.prism_select = 4;
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
void Prism_polar_SubFun10()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.50 command at 22.50 degree */
	pv.prism_polar_input = 10;
	pv.prism_select = 4;
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
void Prism_polar_SubFun11()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 2.75 command at 24.75 degree */
	pv.prism_polar_input = 11;
	pv.prism_select = 4;
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

void Prism_polar_SubFun12()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.00 command at 27.00 degree */
	pv.prism_polar_input = 12;
	pv.prism_select = 4;
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
void Prism_polar_SubFun13()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.25 command at 29.25 degree */
	pv.prism_polar_input = 13;
	pv.prism_select = 4;
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
void Prism_polar_SubFun14()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.50 command at 31.50 degree */
	pv.prism_polar_input = 14;
	pv.prism_select = 4;
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
void Prism_polar_SubFun15()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 3.75 command at 33.75 degree */
	pv.prism_polar_input = 15;
	pv.prism_select = 4;
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
void Prism_polar_SubFun16()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.00 command at 36.00 degree */
	pv.prism_polar_input = 16;
	pv.prism_select = 4;
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
void Prism_polar_SubFun17()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.25 command at 38.25 degree */
	pv.prism_polar_input = 17;
	pv.prism_select = 4;
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
void Prism_polar_SubFun18()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.50 command at 40.50 degree */
	pv.prism_polar_input = 18;
	pv.prism_select = 4;
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
void Prism_polar_SubFun19()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 4.75 command at 42.75 degree */
	pv.prism_polar_input = 19;
	pv.prism_select = 4;
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
void Prism_polar_SubFun20()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.00 command at 45.00 degree */
	pv.prism_polar_input = 20;
	pv.prism_select = 4;
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

void Prism_polar_SubFun21()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.25 command at 47.25 degree */
	pv.prism_polar_input = 21;
	pv.prism_select = 4;
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

void Prism_polar_SubFun22()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.50 command at 49.50 degree */
	pv.prism_polar_input = 22;
	pv.prism_select = 4;
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
void Prism_polar_SubFun23()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 5.75 command at 51.75 degree */
	pv.prism_polar_input = 23;
	pv.prism_select = 4;
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
void Prism_polar_SubFun24()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.00 command at 54.00 degree */
	pv.prism_polar_input = 24;
	pv.prism_select = 4;
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
void Prism_polar_SubFun25()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.25 command at 56.25 degree */
	pv.prism_polar_input = 25;
	pv.prism_select = 4;
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
void Prism_polar_SubFun26()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.50 command at 58.50 degree */
	pv.prism_polar_input = 26;
	pv.prism_select = 4;
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
void Prism_polar_SubFun27()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 6.75 command at 60.75 degree */
	pv.prism_polar_input = 27;
	pv.prism_select = 4;
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
void Prism_polar_SubFun28()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.00 command at 63.00 degree */
	pv.prism_polar_input = 28;
	pv.prism_select = 4;
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
void Prism_polar_SubFun29()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.25 command at 65.25 degree */
	pv.prism_polar_input = 29;
	pv.prism_select = 4;
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
void Prism_polar_SubFun30()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.50 command at 67.50 degree */
	pv.prism_polar_input = 30;
	pv.prism_select = 4;
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
void Prism_polar_SubFun31()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 7.75 command at 69.75 degree */
	pv.prism_polar_input = 31;
	pv.prism_select = 4;
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

void Prism_polar_SubFun32()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.00 command at 72.00 degree */
	pv.prism_polar_input = 32;
	pv.prism_select = 4;
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

void Prism_polar_SubFun33()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.25 command at 74.25 degree */
	pv.prism_polar_input = 33;
	pv.prism_select = 4;
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
void Prism_polar_SubFun34()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.50 command at 76.50 degree */
	pv.prism_polar_input = 34;
	pv.prism_select = 4;
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

void Prism_polar_SubFun35()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 8.75 command at 78.75 degree */
	pv.prism_polar_input = 35;
	pv.prism_select = 4;
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

void Prism_polar_SubFun36()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.00 command at 81.00 degree */
	pv.prism_polar_input = 36;
	pv.prism_select = 4;
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

void Prism_polar_SubFun37()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.25 command at 83.25 degree */
	pv.prism_polar_input = 37;
	pv.prism_select = 4;
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

void Prism_polar_SubFun38()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.50 command at 85.50 degree */
	pv.prism_polar_input = 38;
	pv.prism_select = 4;
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

void Prism_polar_SubFun39()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 9.75 command at 87.75 degree */
	pv.prism_polar_input = 39;
	pv.prism_select = 4;
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

void Prism_polar_SubFun40()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.00 command at 90.00 degree */
	pv.prism_polar_input = 40;
	pv.prism_select = 4;
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

void Prism_polar_SubFun41()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.25 command at 92.25 degree */
	pv.prism_polar_input = 41;
	pv.prism_select = 4;
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

void Prism_polar_SubFun42()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.50 command at 94.50 degree */
	pv.prism_polar_input = 42;
	pv.prism_select = 4;
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

void Prism_polar_SubFun43()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 10.75 command at 96.75 degree */
	pv.prism_polar_input = 43;
	pv.prism_select = 4;
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

void Prism_polar_SubFun44()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.00 command at 99.00 degree */
	pv.prism_polar_input = 44;
	pv.prism_select = 4;
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

void Prism_polar_SubFun45()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.25 command at 101.25 degree */
	pv.prism_polar_input = 45;
	pv.prism_select = 4;
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

void Prism_polar_SubFun46()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.50 command at 103.50 degree */
	pv.prism_polar_input = 46;
	pv.prism_select = 4;
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

void Prism_polar_SubFun47()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 11.75 command at 105.75 degree */
	pv.prism_polar_input = 47;
	pv.prism_select = 4;
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

void Prism_polar_SubFun48()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.00 command at 108.00 degree */
	pv.prism_polar_input = 48;
	pv.prism_select = 4;
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

void Prism_polar_SubFun49()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.25 command at 110.25 degree */
	pv.prism_polar_input = 49;
	pv.prism_select = 4;
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

void Prism_polar_SubFun50()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.50 command at 112.50 degree */
	pv.prism_polar_input = 50;
	pv.prism_select = 4;
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

void Prism_polar_SubFun51()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 12.75 command at 114.75 degree */
	pv.prism_polar_input = 51;
	pv.prism_select = 4;
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

void Prism_polar_SubFun52()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun52\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.00 command at 117.00 degree */
	pv.prism_polar_input = 52;
	pv.prism_select = 4;
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

void Prism_polar_SubFun53()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.25 command at 119.25 degree */
	pv.prism_polar_input = 53;
	pv.prism_select = 4;
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

void Prism_polar_SubFun54()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.50 command at 121.50 degree */
	pv.prism_polar_input = 54;
	pv.prism_select = 4;
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

void Prism_polar_SubFun55()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 13.75 command at 123.75 degree */
	pv.prism_polar_input = 55;
	pv.prism_select = 4;
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

void Prism_polar_SubFun56()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.00 command at 126.00 degree */
	pv.prism_polar_input = 56;
	pv.prism_select = 4;
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

void Prism_polar_SubFun57()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.25 command at 128.25 degree */
	pv.prism_polar_input = 57;
	pv.prism_select = 4;
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

void Prism_polar_SubFun58()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.50 command at 130.50 degree */
	pv.prism_polar_input = 58;
	pv.prism_select = 4;
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

void Prism_polar_SubFun59()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 14.75 command at 132.75 degree */
	pv.prism_polar_input = 59;
	pv.prism_select = 4;
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

void Prism_polar_SubFun60()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.00 command at 135.00 degree */
	pv.prism_polar_input = 60;
	pv.prism_select = 4;
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

void Prism_polar_SubFun61()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.25 command at 137.25 degree */
	pv.prism_polar_input = 61;
	pv.prism_select = 4;
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

void Prism_polar_SubFun62()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.50 command at 139.50 degree */
	pv.prism_polar_input = 62;
	pv.prism_select = 4;
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

void Prism_polar_SubFun63()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun63\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 15.75 command at 141.75 degree */
	pv.prism_polar_input = 63;
	pv.prism_select = 4;
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

void Prism_polar_SubFun64()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun64\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.00 command at 144.00 degree */
	pv.prism_polar_input = 64;
	pv.prism_select = 4;
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

void Prism_polar_SubFun65()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun65\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.25 command at 146.25 degree */
	pv.prism_polar_input = 65;
	pv.prism_select = 4;
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

void Prism_polar_SubFun66()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun66\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.50 command at 148.50 degree */
	pv.prism_polar_input = 66;
	pv.prism_select = 4;
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

void Prism_polar_SubFun67()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun67\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 16.75 command at 150.75 degree */
	pv.prism_polar_input = 67;
	pv.prism_select = 4;
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

void Prism_polar_SubFun68()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun68\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.00 command at 153.00 degree */
	pv.prism_polar_input = 68;
	pv.prism_select = 4;
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

void Prism_polar_SubFun69()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun69\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.25 command at 155.25 degree */
	pv.prism_polar_input = 69;
	pv.prism_select = 4;
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

void Prism_polar_SubFun70()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun70\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.50 command at 157.50 degree */
	pv.prism_polar_input = 70;
	pv.prism_select = 4;
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

void Prism_polar_SubFun71()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun71\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 17.75 command at 159.75 degree */
	pv.prism_polar_input = 71;
	pv.prism_select = 4;
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

void Prism_polar_SubFun72()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun72\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.00 command at 162.00 degree */
	pv.prism_polar_input = 72;
	pv.prism_select = 4;
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

void Prism_polar_SubFun73()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun73\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.25 command at 164.25 degree */
	pv.prism_polar_input = 73;
	pv.prism_select = 4;
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

void Prism_polar_SubFun74()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun74\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.50 command at 166.50 degree */
	pv.prism_polar_input = 74;
	pv.prism_select = 4;
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

void Prism_polar_SubFun75()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun75\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 18.75 command at 168.75 degree */
	pv.prism_polar_input = 75;
	pv.prism_select = 4;
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

void Prism_polar_SubFun76()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun76\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.00 command at 171.00 degree */
	pv.prism_polar_input = 76;
	pv.prism_select = 4;
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

void Prism_polar_SubFun77()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun77\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.25 command at 173.25 degree */
	pv.prism_polar_input = 77;
	pv.prism_select = 4;
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

void Prism_polar_SubFun78()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun78\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.50 command at 175.50 degree */
	pv.prism_polar_input = 78;
	pv.prism_select = 4;
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

void Prism_polar_SubFun79()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun79\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 19.75 command at 177.75 degree */
	pv.prism_polar_input = 79;
	pv.prism_select = 4;
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

void Prism_polar_SubFun80()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun80\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens 20.0 command at 180.00 degree */
	pv.prism_polar_input = 80;
	pv.prism_select = 4;
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

void Prism_polar_SubFun81()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun81\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -0.25 command at 357.75 degree */
	pv.prism_polar_input = 81;
	pv.prism_select = 4;
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

void Prism_polar_SubFun82()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun82\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -0.50 command at 355.50 degree */
	pv.prism_polar_input = 82;
	pv.prism_select = 4;
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

void Prism_polar_SubFun83()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun83\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -0.75 command at 353.25 degree */
	pv.prism_polar_input = 83;
	pv.prism_select = 4;
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

void Prism_polar_SubFun84()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun84\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.00 command at 351.00 degree */
	pv.prism_polar_input = 84;
	pv.prism_select = 4;
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

void Prism_polar_SubFun85()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun85\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.25 command at 348.75 degree */
	pv.prism_polar_input = 85;
	pv.prism_select = 4;
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

void Prism_polar_SubFun86()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun86\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.50 command at 346.50 degree */
	pv.prism_polar_input = 86;
	pv.prism_select = 4;
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

void Prism_polar_SubFun87()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun87\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -1.75 command at 344.25 degree */
	pv.prism_polar_input = 87;
	pv.prism_select = 4;
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

void Prism_polar_SubFun88()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun88\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.00 command at 342.00 degree */
	pv.prism_polar_input = 88;
	pv.prism_select = 4;
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

void Prism_polar_SubFun89()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun89\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.25 command at 339.75 degree */
	pv.prism_polar_input = 89;
	pv.prism_select = 4;
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

void Prism_polar_SubFun90()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun90\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.50 command at 337.50 degree */
	pv.prism_polar_input = 90;
	pv.prism_select = 4;
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

void Prism_polar_SubFun91()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun91\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -2.75 command at 335.25 degree */
	pv.prism_polar_input = 91;
	pv.prism_select = 4;
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

void Prism_polar_SubFun92()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun92\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.00 command at 333.00 degree */
	pv.prism_polar_input = 92;
	pv.prism_select = 4;
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

void Prism_polar_SubFun93()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun93\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.25 command at 330.75 degree */
	pv.prism_polar_input = 93;
	pv.prism_select = 4;
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

void Prism_polar_SubFun94()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun94\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.50 command at 328.50 degree */
	pv.prism_polar_input = 94;
	pv.prism_select = 4;
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

void Prism_polar_SubFun95()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun95\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -3.75 command at 326.25 degree */
	pv.prism_polar_input = 95;
	pv.prism_select = 4;
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

void Prism_polar_SubFun96()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun96\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.00 command at 324.00 degree */
	pv.prism_polar_input = 96;
	pv.prism_select = 4;
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

void Prism_polar_SubFun97()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun97\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.25 command at 321.75 degree */
	pv.prism_polar_input = 97;
	pv.prism_select = 4;
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

void Prism_polar_SubFun98()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun98\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.50 command at 319.50 degree */
	pv.prism_polar_input = 98;
	pv.prism_select = 4;
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

void Prism_polar_SubFun99()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun99\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -4.75 command at 317.25 degree */
	pv.prism_polar_input = 99;
	pv.prism_select = 4;
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

void Prism_polar_SubFun100()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun100\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.00 command at 315.00 degree */
	pv.prism_polar_input = 100;
	pv.prism_select = 4;
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

void Prism_polar_SubFun101()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun101\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.25 command at 312.75 degree */
	pv.prism_polar_input = 101;
	pv.prism_select = 4;
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

void Prism_polar_SubFun102()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun102\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.50 command at 310.50 degree */
	pv.prism_polar_input = 102;
	pv.prism_select = 4;
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

void Prism_polar_SubFun103()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun103\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.75 command at 231.75 degree */
	pv.prism_polar_input = 103;
	pv.prism_select = 4;
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

void Prism_polar_SubFun104()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun104\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.00 command at 234.00 degree */
	pv.prism_polar_input = 104;
	pv.prism_select = 4;
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

void Prism_polar_SubFun105()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun105\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.25 command at 236.25 degree */
	pv.prism_polar_input = 105;
	pv.prism_select = 4;
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

void Prism_polar_SubFun106()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun106\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.50 command at 238.50 degree */
	pv.prism_polar_input = 106;
	pv.prism_select = 4;
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

void Prism_polar_SubFun107()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun107\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.75 command at 240.75 degree */
	pv.prism_polar_input = 107;
	pv.prism_select = 4;
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

void Prism_polar_SubFun108()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun108\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.00 command at 243.00 degree */
	pv.prism_polar_input = 108;
	pv.prism_select = 4;
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

void Prism_polar_SubFun109()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun109\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.25 command at 245.25 degree */
	pv.prism_polar_input = 109;
	pv.prism_select = 4;
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

void Prism_polar_SubFun110()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun110\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.50 command at 247.50 degree */
	pv.prism_polar_input = 110;
	pv.prism_select = 4;
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

void Prism_polar_SubFun111()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun111\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.75 command at 249.75 degree */
	pv.prism_polar_input = 111;
	pv.prism_select = 4;
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

void Prism_polar_SubFun112()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun112\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.00 command at 252.00 degree */
	pv.prism_polar_input = 112;
	pv.prism_select = 4;
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

void Prism_polar_SubFun113()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun113\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.25 command at 254.25 degree */
	pv.prism_polar_input = 113;
	pv.prism_select = 4;
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

void Prism_polar_SubFun114()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun114\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.50 command at 256.50 degree */
	pv.prism_polar_input = 114;
	pv.prism_select = 4;
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

void Prism_polar_SubFun115()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun115\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.75 command at 258.75 degree */
	pv.prism_polar_input = 115;
	pv.prism_select = 4;
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

void Prism_polar_SubFun116()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun116\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.00 command at 261.00 degree */
	pv.prism_polar_input = 116;
	pv.prism_select = 4;
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

void Prism_polar_SubFun117()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun117\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.25 command at 263.25 degree */
	pv.prism_polar_input = 117;
	pv.prism_select = 4;
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

void Prism_polar_SubFun118()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun118\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.50 command at 265.50 degree */
	pv.prism_polar_input = 118;
	pv.prism_select = 4;
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

void Prism_polar_SubFun119()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun119\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.75 command at 267.75 degree */
	pv.prism_polar_input = 119;
	pv.prism_select = 4;
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

void Prism_polar_SubFun120()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun120\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.00 command at 270.00 degree */
	pv.prism_polar_input = 120;
	pv.prism_select = 4;
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

void Prism_polar_SubFun121()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun121\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.25 command at 272.25 degree */
	pv.prism_polar_input = 121;
	pv.prism_select = 4;
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

void Prism_polar_SubFun122()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun122\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.5 command at 274.50 degree */
	pv.prism_polar_input = 122;
	pv.prism_select = 4;
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

void Prism_polar_SubFun123()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun123\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.75 command at 276.75 degree */
	pv.prism_polar_input = 123;
	pv.prism_select = 4;
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

void Prism_polar_SubFun124()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun124\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.00 command at 279.00 degree */
	pv.prism_polar_input = 124;
	pv.prism_select = 4;
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

void Prism_polar_SubFun125()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun125\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.25 command at 281.25 degree */
	pv.prism_polar_input = 125;
	pv.prism_select = 4;
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

void Prism_polar_SubFun126()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun126\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.50 command at 283.50 degree */
	pv.prism_polar_input = 126;
	pv.prism_select = 4;
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

void Prism_polar_SubFun127()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun127\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.75 command at 285.75 degree */
	pv.prism_polar_input = 127;
	pv.prism_select = 4;
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

void Prism_polar_SubFun128()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun128\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.00 command at 288.00 degree */
	pv.prism_polar_input = 128;
	pv.prism_select = 4;
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

void Prism_polar_SubFun129()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun129\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.25 command at 290.25 degree */
	pv.prism_polar_input = 129;
	pv.prism_select = 4;
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

void Prism_polar_SubFun130()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun130\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.50 command at 292.50 degree */
	pv.prism_polar_input = 130;
	pv.prism_select = 4;
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

void Prism_polar_SubFun131()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun131\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.75 command at 294.75  degree */
	pv.prism_polar_input = 131;
	pv.prism_select = 4;
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

void Prism_polar_SubFun132()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun132\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.00 command at 297.00 degree */
	pv.prism_polar_input = 132;
	pv.prism_select = 4;
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

void Prism_polar_SubFun133()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun133\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.25 command at 299.25 degree */
	pv.prism_polar_input = 133;
	pv.prism_select = 4;
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

void Prism_polar_SubFun134()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun134\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.50 command at 301.50 degree */
	pv.prism_polar_input = 134;
	pv.prism_select = 4;
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

void Prism_polar_SubFun135()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun135\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -13.75 command at 303.75 degree */
	pv.prism_polar_input = 135;
	pv.prism_select = 4;
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

void Prism_polar_SubFun136()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun136\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.00 command at 306.00 degree */
	pv.prism_polar_input = 136;
	pv.prism_select = 4;
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

void Prism_polar_SubFun137()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun137\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.25 command at 308.25 degree */
	pv.prism_polar_input = 137;
	pv.prism_select = 4;
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

void Prism_polar_SubFun138()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun138\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.50 command at 310.50 degree */
	pv.prism_polar_input = 138;
	pv.prism_select = 4;
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

void Prism_polar_SubFun139()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun139\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.75 command at 312.75 degree */
	pv.prism_polar_input = 139;
	pv.prism_select = 4;
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

void Prism_polar_SubFun140()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun140\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.00 command at 315.00 degree */
	pv.prism_polar_input = 140;
	pv.prism_select = 4;
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

void Prism_polar_SubFun141()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun141\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.25 command at 317.25 degree */
	pv.prism_polar_input = 141;
	pv.prism_select = 4;
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

void Prism_polar_SubFun142()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun142\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.50 command at 319.50 degree */
	pv.prism_polar_input = 142;
	pv.prism_select = 4;
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

void Prism_polar_SubFun143()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun143\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.75 command at 321.75 degree */
	pv.prism_polar_input = 143;
	pv.prism_select = 4;
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

void Prism_polar_SubFun144()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun144\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 324.00 degree */
	pv.prism_polar_input = 144;
	pv.prism_select = 4;
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

void Prism_polar_SubFun145()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun145\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 326.25 degree */
	pv.prism_polar_input = 145;
	pv.prism_select = 4;
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

void Prism_polar_SubFun146()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun146\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.50 command at 328.50 degree */
	pv.prism_polar_input = 146;
	pv.prism_select = 4;
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

void Prism_polar_SubFun147()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun147\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.75 command at 330.75 degree */
	pv.prism_polar_input = 147;
	pv.prism_select = 4;
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

void Prism_polar_SubFun148()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun148\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 333.00 degree */
	pv.prism_polar_input = 148;
	pv.prism_select = 4;
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

void Prism_polar_SubFun149()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun149\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 335.25 degree */
	pv.prism_polar_input = 149;
	pv.prism_select = 4;
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

void Prism_polar_SubFun150()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun150\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 202.50 degree */
	pv.prism_polar_input = 150;
	pv.prism_select = 4;
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

void Prism_polar_SubFun151()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun151\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 200.25 degree */
	pv.prism_polar_input = 151;
	pv.prism_select = 4;
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

void Prism_polar_SubFun152()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun152\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.00 command at 198.00 degree */
	pv.prism_polar_input = 152;
	pv.prism_select = 4;
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

void Prism_polar_SubFun153()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun153\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.25 command at 195.75 degree */
	pv.prism_polar_input = 153;
	pv.prism_select = 4;
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

void Prism_polar_SubFun154()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun154\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.50 command at 193.50 degree */
	pv.prism_polar_input = 154;
	pv.prism_select = 4;
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

void Prism_polar_SubFun155()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun155\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.75 command at 191.25 degree */
	pv.prism_polar_input = 155;
	pv.prism_select = 4;
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

void Prism_polar_SubFun156()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun156\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.00 command at 189.00 degree */
 	pv.prism_polar_input = 156;
	pv.prism_select = 4;
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

void Prism_polar_SubFun157()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun157\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.25 command at 186.75 degree */
	pv.prism_polar_input = 157;
	pv.prism_select = 4;
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

void Prism_polar_SubFun158()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun158\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.50 command at 184.50 degree */
	pv.prism_polar_input = 158;
	pv.prism_select = 4;
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

void Prism_polar_SubFun159()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun159\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.75 command at 182.25 degree */
	pv.prism_polar_input = 159;
	pv.prism_select = 4;
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

void Prism_polar_SubFun160()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun160\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 90.00 degree */
	pv.prism_polar_input = 160;
	pv.prism_select = 4;
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
void Prism_polar_SubFun161()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun161\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.25 command at 272.25 degree */
	pv.prism_polar_input = 161;
	pv.prism_select = 4;
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

void Prism_polar_SubFun162()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun162\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.5 command at 274.50 degree */
	pv.prism_polar_input = 162;
	pv.prism_select = 4;
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

void Prism_polar_SubFun163()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun163\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.75 command at 276.75 degree */
	pv.prism_polar_input = 163;
	pv.prism_select = 4;
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

void Prism_polar_SubFun164()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun164\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.00 command at 279.00 degree */
	pv.prism_polar_input = 164;
	pv.prism_select = 4;
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

void Prism_polar_SubFun165()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun165\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.25 command at 281.25 degree */
	pv.prism_polar_input = 165;
	pv.prism_select = 4;
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

void Prism_polar_SubFun166()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun166\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.50 command at 283.50 degree */
	pv.prism_polar_input = 166;
	pv.prism_select = 4;
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

void Prism_polar_SubFun167()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun167\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.75 command at 285.75 degree */
	pv.prism_polar_input = 167;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 252.00 degree
 *  		  Rotate the lens -16.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar_SubFun168()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun168\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 288.00 degree */
	pv.prism_polar_input = 168;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 249.75 degree
 *  		  Rotate the lens -16.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar_SubFun169()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun169\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 290.25 degree */
	pv.prism_polar_input = 169;
	pv.prism_select = 4;
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

void Prism_polar_SubFun170()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun170\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.50 command at 292.50 degree */
	pv.prism_polar_input = 170;
	pv.prism_select = 4;
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

void Prism_polar_SubFun171()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun171\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.75 command at 294.75  degree */
	pv.prism_polar_input = 171;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 243.00 degree
 *  		  Rotate the lens -17.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar_SubFun172()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun172\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 297.00 degree */
	pv.prism_polar_input = 172;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 240.75 degree
 *  		  Rotate the lens -17.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar_SubFun173()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun173\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 299.25 degree */
	pv.prism_polar_input = 173;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 238.50 degree
 *  		  Rotate the lens -17.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar_SubFun174()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun174\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 301.50 degree */
	pv.prism_polar_input = 174;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 236.25 degree
 *  		  Rotate the lens -17.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar_SubFun175()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun175\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 303.75 degree */
	pv.prism_polar_input = 175;
	pv.prism_select = 4;
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

void Prism_polar_SubFun176()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun176\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.00 command at 306.00 degree */
	pv.prism_polar_input = 176;
	pv.prism_select = 4;
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

void Prism_polar_SubFun177()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun177\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.25 command at 308.25 degree */
	pv.prism_polar_input = 177;
	pv.prism_select = 4;
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

void Prism_polar_SubFun178()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun178\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.50 command at 310.50 degree */
	pv.prism_polar_input = 178;
	pv.prism_select = 4;
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

void Prism_polar_SubFun179()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun179\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.75 command at 312.75 degree */
	pv.prism_polar_input = 179;
	pv.prism_select = 4;
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

void Prism_polar_SubFun180()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar_SubFun180\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.00 command at 315.00 degree */
	pv.prism_polar_input = 180;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

