/*
 * prism_polar2_cmd.c
 *
 *  Created on:
 *      Author:
 */

/*** Header File ***/
#include "../Inc/common.h"
#include "../Inc/Prism_polar2_cmd.h"

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
uint8_t Prism_polar2_cmd(uint8_t *cbuff, uint16_t len)
{
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_cmd\r\n";

	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(Prism_polar2_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == Prism_polar2_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
        {
        	return 0;
        }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    Prism_polar2_func[cbuff[1]].cb(cbuff, len);

    pv.cmd_status = MOTOR_STATUS_STOP;

    return 1;
}

/** @brief  : This function operates Anti-clockwise 222.75 degree
 *  		  Rotate the lens -15.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun1()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.25 command at 317.25 degree */
	pv.prism_polar_input = 181;
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

void Prism_polar2_SubFun2()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.50 command at 319.50 degree */
	pv.prism_polar_input = 182;
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

void Prism_polar2_SubFun3()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.75 command at 321.75 degree */
	pv.prism_polar_input = 183;
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

void Prism_polar2_SubFun4()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 324.00 degree */
	pv.prism_polar_input = 184;
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

void Prism_polar2_SubFun5()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 326.25 degree */
	pv.prism_polar_input = 185;
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

void Prism_polar2_SubFun6()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.50 command at 328.50 degree */
	pv.prism_polar_input = 186;
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

void Prism_polar2_SubFun7()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.75 command at 330.75 degree */
	pv.prism_polar_input = 187;
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

void Prism_polar2_SubFun8()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 333.00 degree */
	pv.prism_polar_input = 188;
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

void Prism_polar2_SubFun9()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 335.25 degree */
	pv.prism_polar_input = 189;
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

void Prism_polar2_SubFun10()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 202.50 degree */
	pv.prism_polar_input = 190;
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

void Prism_polar2_SubFun11()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun11\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 200.25 degree */
	pv.prism_polar_input = 191;
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

void Prism_polar2_SubFun12()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.00 command at 198.00 degree */
	pv.prism_polar_input = 192;
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

void Prism_polar2_SubFun13()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.25 command at 195.75 degree */
	pv.prism_polar_input = 193;
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

void Prism_polar2_SubFun14()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.50 command at 193.50 degree */
	pv.prism_polar_input = 194;
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

void Prism_polar2_SubFun15()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.75 command at 191.25 degree */
	pv.prism_polar_input = 195;
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

void Prism_polar2_SubFun16()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.00 command at 189.00 degree */
 	pv.prism_polar_input = 196;
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

void Prism_polar2_SubFun17()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.25 command at 186.75 degree */
	pv.prism_polar_input = 197;
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

void Prism_polar2_SubFun18()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.50 command at 184.50 degree */
	pv.prism_polar_input = 198;
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

void Prism_polar2_SubFun19()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.75 command at 182.25 degree */
	pv.prism_polar_input = 199;
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

void Prism_polar2_SubFun20()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 90.00 degree */
	pv.prism_polar_input = 200;
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

void Prism_polar2_SubFun21()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 360.00 degree */
	pv.prism_polar_input = 201;
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

void Prism_polar2_SubFun22()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.50 command at 320.50 degree */
	pv.prism_polar_input = 202;
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

void Prism_polar2_SubFun23()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.75 command at 231.75 degree */
	pv.prism_polar_input = 203;
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

void Prism_polar2_SubFun24()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.00 command at 234.00 degree */
	pv.prism_polar_input = 204;
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

void Prism_polar2_SubFun25()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.25 command at 236.25 degree */
	pv.prism_polar_input = 205;
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

void Prism_polar2_SubFun26()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.50 command at 238.50 degree */
	pv.prism_polar_input = 206;
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

void Prism_polar2_SubFun27()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.75 command at 240.75 degree */
	pv.prism_polar_input = 207;
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

void Prism_polar2_SubFun28()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.00 command at 243.00 degree */
	pv.prism_polar_input = 208;
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

void Prism_polar2_SubFun29()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.25 command at 245.25 degree */
	pv.prism_polar_input = 29;
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

void Prism_polar2_SubFun30()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.50 command at 247.50 degree */
	pv.prism_polar_input = 210;
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

void Prism_polar2_SubFun31()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.75 command at 249.75 degree */
	pv.prism_polar_input = 211;
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

void Prism_polar2_SubFun32()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.00 command at 252.00 degree */
	pv.prism_polar_input = 212;
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

void Prism_polar2_SubFun33()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.25 command at 254.25 degree */
	pv.prism_polar_input = 213;
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

void Prism_polar2_SubFun34()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.50 command at 256.50 degree */
	pv.prism_polar_input = 214;
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

void Prism_polar2_SubFun35()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.75 command at 258.75 degree */
	pv.prism_polar_input = 215;
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

void Prism_polar2_SubFun36()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.00 command at 261.00 degree */
	pv.prism_polar_input = 216;
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

void Prism_polar2_SubFun37()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.25 command at 263.25 degree */
	pv.prism_polar_input = 217;
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

void Prism_polar2_SubFun38()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.50 command at 265.50 degree */
	pv.prism_polar_input = 218;
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

void Prism_polar2_SubFun39()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.75 command at 267.75 degree */
	pv.prism_polar_input = 219;
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

void Prism_polar2_SubFun40()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.00 command at 270.00 degree */
	pv.prism_polar_input = 220;
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

void Prism_polar2_SubFun41()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.25 command at 272.25 degree */
	pv.prism_polar_input = 221;
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

void Prism_polar2_SubFun42()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.5 command at 274.50 degree */
	pv.prism_polar_input = 222;
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

void Prism_polar2_SubFun43()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.75 command at 276.75 degree */
	pv.prism_polar_input = 223;
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

void Prism_polar2_SubFun44()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.00 command at 279.00 degree */
	pv.prism_polar_input = 224;
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

void Prism_polar2_SubFun45()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.25 command at 281.25 degree */
	pv.prism_polar_input = 225;
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

void Prism_polar2_SubFun46()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.50 command at 283.50 degree */
	pv.prism_polar_input = 226;
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

void Prism_polar2_SubFun47()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.75 command at 285.75 degree */
	pv.prism_polar_input = 227;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 252.00 degree
 *  		  Rotate the lens -4.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun48()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -22.00 command at 288.00 degree */
	pv.prism_polar_input = 228;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 249.75 degree
 *  		  Rotate the lens -22.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun49()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -22.25 command at 290.25 degree */
	pv.prism_polar_input = 229;
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

void Prism_polar2_SubFun50()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.50 command at 292.50 degree */
	pv.prism_polar_input = 230;
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

void Prism_polar2_SubFun51()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.75 command at 294.75  degree */
	pv.prism_polar_input = 231;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 243.00 degree
 *  		  Rotate the lens -5.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun52()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun232\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.00 command at 297.00 degree */
	pv.prism_polar_input = 52;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 240.75 degree
 *  		  Rotate the lens -23.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun53()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -23.25 command at 299.25 degree */
	pv.prism_polar_input = 233;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 238.50 degree
 *  		  Rotate the lens -23.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun54()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -23.50 command at 301.50 degree */
	pv.prism_polar_input = 234;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 236.25 degree
 *  		  Rotate the lens -23.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun55()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -23.75 command at 303.75 degree */
	pv.prism_polar_input = 235;
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

void Prism_polar2_SubFun56()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.00 command at 306.00 degree */
	pv.prism_polar_input = 236;
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

void Prism_polar2_SubFun57()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.25 command at 308.25 degree */
	pv.prism_polar_input = 237;
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

void Prism_polar2_SubFun58()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.50 command at 310.50 degree */
	pv.prism_polar_input = 238;
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

void Prism_polar2_SubFun59()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.75 command at 312.75 degree */
	pv.prism_polar_input = 239;
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

void Prism_polar2_SubFun60()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.00 command at 315.00 degree */
	pv.prism_polar_input = 240;
	pv.prism_select = 1;
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

void Prism_polar2_SubFun61()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.25 command at 317.25 degree */
	pv.prism_polar_input = 244;
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

void Prism_polar2_SubFun62()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.50 command at 319.50 degree */
	pv.prism_polar_input = 242;
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

void Prism_polar2_SubFun63()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun63\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.75 command at 321.75 degree */
	pv.prism_polar_input = 243;
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

void Prism_polar2_SubFun64()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun64\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 324.00 degree */
	pv.prism_polar_input = 244;
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

void Prism_polar2_SubFun65()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun65\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 326.25 degree */
	pv.prism_polar_input = 245;
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

void Prism_polar2_SubFun66()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun66\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.50 command at 328.50 degree */
	pv.prism_polar_input = 246;
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

void Prism_polar2_SubFun67()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun67\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.75 command at 330.75 degree */
	pv.prism_polar_input = 247;
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

void Prism_polar2_SubFun68()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun68\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 333.00 degree */
	pv.prism_polar_input = 248;
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

void Prism_polar2_SubFun69()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun69\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 335.25 degree */
	pv.prism_polar_input = 249;
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

void Prism_polar2_SubFun70()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun70\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 202.50 degree */
	pv.prism_polar_input = 250;
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

void Prism_polar2_SubFun71()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun71\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 200.25 degree */
	pv.prism_polar_input = 251;
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

void Prism_polar2_SubFun72()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun72\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.00 command at 198.00 degree */
	pv.prism_polar_input = 252;
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

void Prism_polar2_SubFun73()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun73\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.25 command at 195.75 degree */
	pv.prism_polar_input = 253;
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

void Prism_polar2_SubFun74()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun74\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.50 command at 193.50 degree */
	pv.prism_polar_input = 254;
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

void Prism_polar2_SubFun75()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun75\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.75 command at 191.25 degree */
	pv.prism_polar_input = 255;
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

void Prism_polar2_SubFun76()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun76\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.00 command at 189.00 degree */
 	pv.prism_polar_input = 256;
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

void Prism_polar2_SubFun77()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun77\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.25 command at 186.75 degree */
	pv.prism_polar_input = 257;
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

void Prism_polar2_SubFun78()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun78\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.50 command at 184.50 degree */
	pv.prism_polar_input = 258;
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

void Prism_polar2_SubFun79()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun79\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.75 command at 182.25 degree */
	pv.prism_polar_input = 259;
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

void Prism_polar2_SubFun80()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun80\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 90.00 degree */
	pv.prism_polar_input = 260;
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
void Prism_polar2_SubFun81()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun81\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.25 command at 272.25 degree */
	pv.prism_polar_input = 261;
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

void Prism_polar2_SubFun82()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun82\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.5 command at 274.50 degree */
	pv.prism_polar_input = 262;
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

void Prism_polar2_SubFun83()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun83\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.75 command at 276.75 degree */
	pv.prism_polar_input = 263;
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

void Prism_polar2_SubFun84()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun84\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.00 command at 279.00 degree */
	pv.prism_polar_input = 264;
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

void Prism_polar2_SubFun85()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun85\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.25 command at 281.25 degree */
	pv.prism_polar_input = 265;
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

void Prism_polar2_SubFun86()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun86\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.50 command at 283.50 degree */
	pv.prism_polar_input = 266;
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

void Prism_polar2_SubFun87()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun87\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.75 command at 285.75 degree */
	pv.prism_polar_input = 267;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 252.00 degree
 *  		  Rotate the lens -26.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun88()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun88\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -26.00 command at 288.00 degree */
	pv.prism_polar_input = 268;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 249.75 degree
 *  		  Rotate the lens -26.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun89()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun89\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -26.25 command at 290.25 degree */
	pv.prism_polar_input = 269;
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

void Prism_polar2_SubFun90()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun90\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.50 command at 292.50 degree */
	pv.prism_polar_input = 270;
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

void Prism_polar2_SubFun91()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun91\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.75 command at 294.75  degree */
	pv.prism_polar_input = 271;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 243.00 degree
 *  		  Rotate the lens -27.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun92()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun92\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -27.00 command at 297.00 degree */
	pv.prism_polar_input = 272;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 240.75 degree
 *  		  Rotate the lens -27.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun93()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun93\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -27.25 command at 299.25 degree */
	pv.prism_polar_input = 273;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 238.50 degree
 *  		  Rotate the lens -27.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun94()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun94\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -27.50 command at 301.50 degree */
	pv.prism_polar_input = 274;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 236.25 degree
 *  		  Rotate the lens -27.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun95()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun95\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -27.75 command at 303.75 degree */
	pv.prism_polar_input = 275;
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

void Prism_polar2_SubFun96()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun96\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.00 command at 306.00 degree */
	pv.prism_polar_input = 276;
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

void Prism_polar2_SubFun97()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun97\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.25 command at 308.25 degree */
	pv.prism_polar_input = 277;
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

void Prism_polar2_SubFun98()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun98\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.50 command at 310.50 degree */
	pv.prism_polar_input = 278;
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

void Prism_polar2_SubFun99()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun99\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.75 command at 312.75 degree */
	pv.prism_polar_input = 279;
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

void Prism_polar2_SubFun100()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun100\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.00 command at 315.00 degree */
	pv.prism_polar_input = 280;
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

void Prism_polar2_SubFun101()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun101\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.25 command at 317.25 degree */
	pv.prism_polar_input = 281;
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

void Prism_polar2_SubFun102()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun102\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.50 command at 319.50 degree */
	pv.prism_polar_input = 282;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 228.25 degree
 *  		  Rotate the lens -15.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun103()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun103\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.75 command at 321.75 degree */
	pv.prism_polar_input = 283;
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

void Prism_polar2_SubFun104()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun104\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 324.00 degree */
	pv.prism_polar_input = 284;
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

void Prism_polar2_SubFun105()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun105\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 326.25 degree */
	pv.prism_polar_input = 285;
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

void Prism_polar2_SubFun106()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun106\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.50 command at 310.50 degree */
	pv.prism_polar_input = 286;
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

void Prism_polar2_SubFun107()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun107\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.75 command at 330.75 degree */
	pv.prism_polar_input = 287;
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

void Prism_polar2_SubFun108()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun108\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 333.00 degree */
	pv.prism_polar_input = 288;
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

void Prism_polar2_SubFun109()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun109\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 335.25 degree */
	pv.prism_polar_input = 289;
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

void Prism_polar2_SubFun110()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun110\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 202.50 degree */
	pv.prism_polar_input = 290;
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

void Prism_polar2_SubFun111()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun111\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 200.25 degree */
	pv.prism_polar_input = 291;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 298.00 degree
 *  		  Rotate the lens -18.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun112()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun112\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.00 command at 298.00 degree */
	pv.prism_polar_input = 292;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 295.75 degree
 *  		  Rotate the lens -18.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun113()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun113\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.25 command at 295.75 degree */
	pv.prism_polar_input = 293;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 293.50 degree
 *  		  Rotate the lens -18.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun114()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun114\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.50 command at 293.50 degree */
	pv.prism_polar_input = 294;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 291.25 degree
 *  		  Rotate the lens -18.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun115()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun115\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.75 command at 291.25 degree */
	pv.prism_polar_input = 295;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 189.00 degree
 *  		  Rotate the lens -29.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun116()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun116\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -29.00 command at 189.00 degree */
 	pv.prism_polar_input = 296;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 186.75 degree
 *  		  Rotate the lens -29.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun117()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun117\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -29.25 command at 186.75 degree */
	pv.prism_polar_input = 297;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 184.50 degree
 *  		  Rotate the lens -29.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun118()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun118\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -29.50 command at 184.50 degree */
	pv.prism_polar_input = 298;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 182.25 degree
 *  		  Rotate the lens -29.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun119()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun119\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -29.75 command at 182.25 degree */
	pv.prism_polar_input = 299;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 360.00 degree
 *  		  Rotate the lens -30.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun120()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun120\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -30.00 command at 90.00 degree */
	pv.prism_polar_input = 300;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 360.00 degree
 *  		  Rotate the lens -30.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun121()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun121\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -30.00 command at 360.00 degree */
	pv.prism_polar_input = 301;
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

void Prism_polar2_SubFun122()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun122\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.50 command at 330.50 degree */
	pv.prism_polar_input = 302;
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

void Prism_polar2_SubFun123()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun123\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -5.75 command at 231.75 degree */
	pv.prism_polar_input = 303;
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

void Prism_polar2_SubFun124()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun124\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.00 command at 234.00 degree */
	pv.prism_polar_input = 304;
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

void Prism_polar2_SubFun125()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun125\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.25 command at 236.25 degree */
	pv.prism_polar_input = 305;
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

void Prism_polar2_SubFun126()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun126\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.50 command at 238.50 degree */
	pv.prism_polar_input = 306;
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

void Prism_polar2_SubFun127()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun127\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -6.75 command at 240.75 degree */
	pv.prism_polar_input = 307;
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

void Prism_polar2_SubFun128()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun128\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.00 command at 243.00 degree */
	pv.prism_polar_input = 308;
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

void Prism_polar2_SubFun129()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun129\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.25 command at 245.25 degree */
	pv.prism_polar_input = 309;
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

void Prism_polar2_SubFun130()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun130\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.50 command at 247.50 degree */
	pv.prism_polar_input = 310;
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

void Prism_polar2_SubFun131()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun131\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -7.75 command at 249.75 degree */
	pv.prism_polar_input = 311;
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

void Prism_polar2_SubFun132()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun132\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.00 command at 252.00 degree */
	pv.prism_polar_input = 312;
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

void Prism_polar2_SubFun133()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun133\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.25 command at 254.25 degree */
	pv.prism_polar_input = 313;
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

void Prism_polar2_SubFun134()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun134\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.50 command at 256.50 degree */
	pv.prism_polar_input = 314;
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

void Prism_polar2_SubFun135()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun135\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -8.75 command at 258.75 degree */
	pv.prism_polar_input = 315;
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

void Prism_polar2_SubFun136()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun136\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.00 command at 261.00 degree */
	pv.prism_polar_input = 316;
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

void Prism_polar2_SubFun137()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun137\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.25 command at 263.25 degree */
	pv.prism_polar_input = 317;
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

void Prism_polar2_SubFun138()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun138\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.50 command at 265.50 degree */
	pv.prism_polar_input = 318;
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

void Prism_polar2_SubFun139()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun139\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -9.75 command at 267.75 degree */
	pv.prism_polar_input = 319;
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

void Prism_polar2_SubFun140()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun140\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.00 command at 270.00 degree */
	pv.prism_polar_input = 320;
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

void Prism_polar2_SubFun141()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun141\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.25 command at 272.25 degree */
	pv.prism_polar_input = 321;
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

void Prism_polar2_SubFun142()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun142\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.5 command at 274.50 degree */
	pv.prism_polar_input = 322;
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

void Prism_polar2_SubFun143()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun143\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -10.75 command at 276.75 degree */
	pv.prism_polar_input = 323;
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

void Prism_polar2_SubFun144()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun144\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.00 command at 279.00 degree */
	pv.prism_polar_input = 324;
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

void Prism_polar2_SubFun145()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun145\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.25 command at 281.25 degree */
	pv.prism_polar_input = 325;
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

void Prism_polar2_SubFun146()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun146\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.50 command at 283.50 degree */
	pv.prism_polar_input = 326;
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

void Prism_polar2_SubFun147()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun147\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -11.75 command at 285.75 degree */
	pv.prism_polar_input = 327;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 252.00 degree
 *  		  Rotate the lens -14.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun148()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun148\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -32.00 command at 288.00 degree */
	pv.prism_polar_input = 328;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 249.75 degree
 *  		  Rotate the lens -32.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun149()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun149\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -32.25 command at 290.25 degree */
	pv.prism_polar_input = 329;
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

void Prism_polar2_SubFun150()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun150\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.50 command at 292.50 degree */
	pv.prism_polar_input = 330;
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

void Prism_polar2_SubFun151()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun151\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -12.75 command at 294.75  degree */
	pv.prism_polar_input = 331;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 243.00 degree
 *  		  Rotate the lens -33.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun152()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun152\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -33.00 command at 297.00 degree */
	pv.prism_polar_input = 332;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 240.75 degree
 *  		  Rotate the lens -33.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun153()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun153\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -33.25 command at 299.25 degree */
	pv.prism_polar_input = 333;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 338.50 degree
 *  		  Rotate the lens -33.50.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun154()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun154\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -33.50 command at 301.50 degree */
	pv.prism_polar_input = 334;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 336.25 degree
 *  		  Rotate the lens -33.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun155()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun155\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -33.75 command at 303.75 degree */
	pv.prism_polar_input = 335;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 334.00 degree
 *  		  Rotate the lens -14.00.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun156()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun156\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.00 command at 306.00 degree */
	pv.prism_polar_input = 336;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 331.75 degree
 *  		  Rotate the lens -14.25.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun157()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun157\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.25 command at 308.25 degree */
	pv.prism_polar_input = 337;
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

void Prism_polar2_SubFun158()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun158\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.50 command at 310.50 degree */
	pv.prism_polar_input = 338;
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

void Prism_polar2_SubFun159()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun159\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -14.75 command at 312.75 degree */
	pv.prism_polar_input = 339;
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

void Prism_polar2_SubFun160()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun160\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.00 command at 315.00 degree */
	pv.prism_polar_input = 340;
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

void Prism_polar2_SubFun161()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun161\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.25 command at 317.25 degree */
	pv.prism_polar_input = 341;
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

void Prism_polar2_SubFun162()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun162\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.50 command at 319.50 degree */
	pv.prism_polar_input = 342;
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

void Prism_polar2_SubFun163()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun163\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -15.75 command at 321.75 degree */
	pv.prism_polar_input = 343;
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

void Prism_polar2_SubFun164()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun164\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.00 command at 334.00 degree */
	pv.prism_polar_input = 344;
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

void Prism_polar2_SubFun165()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun165\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.25 command at 326.25 degree */
	pv.prism_polar_input = 345;
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

void Prism_polar2_SubFun166()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun166\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.50 command at 328.50 degree */
	pv.prism_polar_input = 346;
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

void Prism_polar2_SubFun167()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun167\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -16.75 command at 330.75 degree */
	pv.prism_polar_input = 347;
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

void Prism_polar2_SubFun168()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun168\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.00 command at 333.00 degree */
	pv.prism_polar_input = 348;
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

void Prism_polar2_SubFun169()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun169\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.25 command at 335.25 degree */
	pv.prism_polar_input = 349;
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

void Prism_polar2_SubFun170()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun170\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.50 command at 202.50 degree */
	pv.prism_polar_input = 350;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 200.35 degree
 *  		  Rotate the lens -17.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun171()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun171\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -17.75 command at 200.35 degree */
	pv.prism_polar_input = 351;
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

void Prism_polar2_SubFun172()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun172\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.00 command at 198.00 degree */
	pv.prism_polar_input = 352;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 195.75 degree
 *  		  Rotate the lens -18.35.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun173()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun173\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.35 command at 195.75 degree */
	pv.prism_polar_input = 353;
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

void Prism_polar2_SubFun174()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun174\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.50 command at 193.50 degree */
	pv.prism_polar_input = 354;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 191.35 degree
 *  		  Rotate the lens -18.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun175()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun175\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -18.75 command at 191.35 degree */
	pv.prism_polar_input = 355;
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

void Prism_polar2_SubFun176()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun176\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.00 command at 189.00 degree */
 	pv.prism_polar_input = 356;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 186.75 degree
 *  		  Rotate the lens -19.35.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun177()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun177\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.35 command at 186.75 degree */
	pv.prism_polar_input = 357;
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

void Prism_polar2_SubFun178()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun178\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.50 command at 184.50 degree */
	pv.prism_polar_input = 358;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

/** @brief  : This function operates Anti-clockwise 182.35 degree
 *  		  Rotate the lens -19.75.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */

void Prism_polar2_SubFun179()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun179\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -19.75 command at 182.35 degree */
	pv.prism_polar_input = 359;
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

void Prism_polar2_SubFun180()
{
	/* Send the Ack to module  */
#ifdef __CMD_DEBUG__
	uint8_t scmd[]="Prism_polar2_SubFun360\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the lens -20.00 command at 90.00 degree */
	pv.prism_polar_input = 0;
	pv.prism_select = 4;
	prism_function(pv.prism_select);
}

