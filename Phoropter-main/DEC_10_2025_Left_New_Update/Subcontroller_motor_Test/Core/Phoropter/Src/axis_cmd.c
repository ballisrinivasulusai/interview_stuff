/*
 * process_cmd.c
 *
 *  Created on:
 *      Author:
 */

/*** Header File ***/
#include "../Inc/common.h"
#include "../Inc/axis_cmd.h"

/* Global variable Declaration */
extern private_var pv;

/** @brief  : Process the command.
 *           This function processes the received command.
 *
 *  @param  : cbuff[OUT]: Receive data buffer
 *            len       : Buffer len
 *
 *  @return : none
 *
 */
uint8_t axis_cmd(uint8_t *cbuff, uint16_t len)
{
	/* Validate the max array size */
	if (!( (cbuff[1] >= 0) &&
			(cbuff[1] <= (sizeof(axis_func) / sizeof(submenulist)))))
	{
		return 0;
	}

	/* Call the callback function */
    if (NULL == axis_func[cbuff[1]].cb)
		return 0;
    if (MOTOR1_RUNNING == pv.cmd_status)
    {
    	return 0;
    }

    pv.cmd_status = MOTOR1_RUNNING;

    /* Call the appropriate callback function  */
    axis_func[cbuff[1]].cb(cbuff,len);

    pv.cmd_status = MOTOR_STATUS_STOP;


    return 1;
}

/** @brief  :This function call the axis motor rotation
 *           function with "1" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun1()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun1\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 1 degree */
	pv.axis_rotate_input = 1;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "2" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun2()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun2\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 2 degree */
	pv.axis_rotate_input = 2;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "3" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun3()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun3\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 3 degree */
	pv.axis_rotate_input = 3;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "4" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun4()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun4\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 4 degree */
	pv.axis_rotate_input = 4;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "5" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun5()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun5\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 5 degree */
	pv.axis_rotate_input = 5;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "6" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun6()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun6\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 6 degree */
	pv.axis_rotate_input = 6;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "7" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun7()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun7\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 7 degree */
	pv.axis_rotate_input = 7;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "8" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun8()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun8\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 8 degree */
	pv.axis_rotate_input = 8;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "9" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun9()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun9\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 9 degree */
	pv.axis_rotate_input = 9;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "10" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun10()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun10\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 10 degree */
	pv.axis_rotate_input = 10;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  :This function call the axis motor rotation
 *           function with "11" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun11()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun11\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 11 degree */
	pv.axis_rotate_input = 11;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "12" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun12()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun12\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 12 degree */
	pv.axis_rotate_input = 12;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "13" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun13()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun13\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 13 degree */
	pv.axis_rotate_input = 13;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "14" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun14()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun14\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 14 degree */
	pv.axis_rotate_input = 14;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "15" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun15()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun15\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 15 degree */
	pv.axis_rotate_input = 15;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "16" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun16()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun16\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 16 degree */
	pv.axis_rotate_input = 16;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "17" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun17()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun17\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 17 degree */
	pv.axis_rotate_input = 17;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "18" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun18()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun18\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 18 degree */
	pv.axis_rotate_input = 18;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "19" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun19()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun19\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 19 degree */
	pv.axis_rotate_input = 19;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "20" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun20()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun20\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 20 degree */
	pv.axis_rotate_input = 20;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  :This function call the axis motor rotation
 *           function with "21" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun21()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun21\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 21 degree */
	pv.axis_rotate_input = 21;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "22" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun22()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun22\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 22 degree */
	pv.axis_rotate_input = 22;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "23" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun23()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun23\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 23 degree */
	pv.axis_rotate_input = 23;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "24" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun24()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun24\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 24 degree */
	pv.axis_rotate_input = 24;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "25" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun25()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun25\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 25 degree */
	pv.axis_rotate_input = 25;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "26" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun26()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun26\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 26 degree */
	pv.axis_rotate_input = 26;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "27" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun27()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun27\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 27 degree */
	pv.axis_rotate_input = 27;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "28" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun28()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun28\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 28 degree */
	pv.axis_rotate_input = 28;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "29" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun29()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun29\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 29 degree */
	pv.axis_rotate_input = 29;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "30" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun30()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun30\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 30 degree */
	pv.axis_rotate_input = 30;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  :This function call the axis motor rotation
 *           function with "31" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun31()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun31\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 31 degree */
	pv.axis_rotate_input = 31;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "32" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun32()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun32\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 32 degree */
	pv.axis_rotate_input = 32;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "33" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun33()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun33\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 33 degree */
	pv.axis_rotate_input = 33;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "34" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun34()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun34\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 34 degree */
	pv.axis_rotate_input = 34;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "35" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun35()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun35\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 35 degree */
	pv.axis_rotate_input = 35;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "36" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun36()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun36\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 36 degree */
	pv.axis_rotate_input = 36;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "37" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun37()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun37\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 37 degree */
	pv.axis_rotate_input = 37;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "38" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun38()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun38\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 38 degree */
	pv.axis_rotate_input = 38;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "39" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun39()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun39\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 39 degree */
	pv.axis_rotate_input = 39;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "40" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun40()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun40\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 40 degree */
	pv.axis_rotate_input = 40;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "41" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun41()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun41\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 41 degree */
	pv.axis_rotate_input = 41;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "42" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun42()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun42\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 42 degree */
	pv.axis_rotate_input = 42;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "43" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun43()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun43\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 43 degree */
	pv.axis_rotate_input = 43;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "44" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun44()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun44\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 44 degree */
	pv.axis_rotate_input = 44;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "45" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun45()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun45\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 45 degree */
	pv.axis_rotate_input = 45;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "46" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun46()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun46\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 46 degree */
	pv.axis_rotate_input = 46;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "47" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun47()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun47\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 47 degree */
	pv.axis_rotate_input = 47;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "48" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun48()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun48\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 48 degree */
	pv.axis_rotate_input = 48;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "49" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun49()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun49\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 49 degree */
	pv.axis_rotate_input = 49;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "50" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun50()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun50\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 50 degree */
	pv.axis_rotate_input = 50;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "51" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun51()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun51\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 51 degree */
	pv.axis_rotate_input = 51;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "52" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun52()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun52\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 52 degree */
	pv.axis_rotate_input = 52;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "53" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun53()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun53\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 53 degree */
	pv.axis_rotate_input = 53;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "54" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun54()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun54\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 54 degree */
	pv.axis_rotate_input = 54;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "55" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun55()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun55\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 55 degree */
	pv.axis_rotate_input = 55;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "56" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun56()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun56\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 56 degree */
	pv.axis_rotate_input = 56;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "57" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun57()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun57\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 57 degree */
	pv.axis_rotate_input = 57;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "58" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun58()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun58\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 58 degree */
	pv.axis_rotate_input = 58;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "59" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun59()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun59\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 59 degree */
	pv.axis_rotate_input = 59;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "60" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun60()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun60\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 60 degree */
	pv.axis_rotate_input = 60;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "61" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun61()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun61\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 61 degree */
	pv.axis_rotate_input = 61;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "62" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun62()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun62\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 62 degree */
	pv.axis_rotate_input = 62;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "63" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun63()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun63\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 63 degree */
	pv.axis_rotate_input = 63;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "64" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun64()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun64\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 64 degree */
	pv.axis_rotate_input = 64;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "65" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun65()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun65\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 65 degree */
	pv.axis_rotate_input = 65;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "66" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun66()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun66\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 66 degree */
	pv.axis_rotate_input = 66;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "67" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun67()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun67\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 67 degree */
	pv.axis_rotate_input = 67;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "68" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun68()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun68\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 68 degree */
	pv.axis_rotate_input = 68;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "69" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun69()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun69\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 69 degree */
	pv.axis_rotate_input = 69;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "70" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun70()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun70\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 70 degree */
	pv.axis_rotate_input = 70;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "71" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun71()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun71\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 71 degree */
	pv.axis_rotate_input = 71;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "72" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun72()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun72\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 72 degree */
	pv.axis_rotate_input = 72;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "73" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun73()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun73\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 73 degree */
	pv.axis_rotate_input = 73;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "74" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun74()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun74\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 74 degree */
	pv.axis_rotate_input = 74;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "75" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun75()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun75\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 75 degree */
	pv.axis_rotate_input = 75;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "76" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun76()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun76\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 76 degree */
	pv.axis_rotate_input = 76;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "77" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun77()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun77\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 77 degree */
	pv.axis_rotate_input = 77;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "78" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun78()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun78\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 78 degree */
	pv.axis_rotate_input = 78;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "79" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun79()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun79\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 79 degree */
	pv.axis_rotate_input = 79;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "80" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun80()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun80\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 80 degree */
	pv.axis_rotate_input = 80;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "81" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun81()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun81\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 81 degree */
	pv.axis_rotate_input = 81;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "82" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun82()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun82\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 82 degree */
	pv.axis_rotate_input = 82;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "83" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun83()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun83\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 83 degree */
	pv.axis_rotate_input = 83;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "84" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun84()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun84\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 84 degree */
	pv.axis_rotate_input = 84;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "85" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun85()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun85\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 85 degree */
	pv.axis_rotate_input = 85;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "86" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun86()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun86\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 86 degree */
	pv.axis_rotate_input = 86;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "87" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun87()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun87\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 87 degree */
	pv.axis_rotate_input = 87;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "88" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun88()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun88\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 88 degree */
	pv.axis_rotate_input = 88;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "89" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun89()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun89\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 89 degree */
	pv.axis_rotate_input = 89;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "90" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun90()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun90\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 90 degree */
	pv.axis_rotate_input = 90;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "91" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun91()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun91\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 99 degree */
	pv.axis_rotate_input = 91;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "92" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun92()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun92\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 92 degree */
	pv.axis_rotate_input = 92;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "93" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun93()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun93\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 93 degree */
	pv.axis_rotate_input = 93;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "94" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun94()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun94\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 94 degree */
	pv.axis_rotate_input = 94;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "95" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun95()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun95\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 95 degree */
	pv.axis_rotate_input = 95;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "96" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun96()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun96\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 96 degree */
	pv.axis_rotate_input = 96;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "97" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun97()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun97\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 97 degree */
	pv.axis_rotate_input = 97;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "98" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun98()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun98\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 98 degree */
	pv.axis_rotate_input = 98;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "99" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun99()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun99\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 99 degree */
	pv.axis_rotate_input = 99;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "100" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun100()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun100\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 100 degree */
	pv.axis_rotate_input = 100;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "101" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun101()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun101\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 101 degree */
	pv.axis_rotate_input = 101;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "102" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun102()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun102\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 102 degree */
	pv.axis_rotate_input = 102;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "103" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun103()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun103\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 103 degree */
	pv.axis_rotate_input = 103;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "104" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun104()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun104\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 104 degree */
	pv.axis_rotate_input = 104;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "105" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun105()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun105\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 105 degree */
	pv.axis_rotate_input = 105;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "106" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun106()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun106\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 106 degree */
	pv.axis_rotate_input = 106;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "107" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun107()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun107\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 107 degree */
	pv.axis_rotate_input = 107;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "108" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun108()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun108\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 108 degree */
	pv.axis_rotate_input = 108;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "109" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun109()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun109\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 109 degree */
	pv.axis_rotate_input = 109;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "110" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun110()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun110\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 110 degree */
	pv.axis_rotate_input = 110;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "111" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun111()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun111\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 111 degree */
	pv.axis_rotate_input = 111;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "112" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun112()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun112\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 112 degree */
	pv.axis_rotate_input = 112;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "113" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun113()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun113\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 113 degree */
	pv.axis_rotate_input = 113;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "114" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun114()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun114\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 114 degree */
	pv.axis_rotate_input = 114;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "115" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun115()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun115\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 115 degree */
	pv.axis_rotate_input = 115;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "116" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun116()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun116\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 116 degree */
	pv.axis_rotate_input = 116;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "117" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun117()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun117\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 117 degree */
	pv.axis_rotate_input = 117;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "118" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun118()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun118\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 118 degree */
	pv.axis_rotate_input = 118;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "119" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun119()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun119\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 119 degree */
	pv.axis_rotate_input = 119;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "120" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun120()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun120\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 120 degree */
	pv.axis_rotate_input = 120;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "121" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun121()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun121\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 121 degree */
	pv.axis_rotate_input = 121;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "122" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun122()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun122\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 122 degree */
	pv.axis_rotate_input = 122;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "123" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun123()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun123\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 123 degree */
	pv.axis_rotate_input = 123;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "124" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun124()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun124\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 124 degree */
	pv.axis_rotate_input = 124;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "125" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun125()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun125\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 125 degree */
	pv.axis_rotate_input = 125;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "126" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun126()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun126\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 126 degree */
	pv.axis_rotate_input = 126;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "127" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun127()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun127\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 127 degree */
	pv.axis_rotate_input = 127;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "128" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun128()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun128\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 128 degree */
	pv.axis_rotate_input = 128;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "129" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun129()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun129\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 129 degree */
	pv.axis_rotate_input = 129;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "130" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun130()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun130\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 130 degree */
	pv.axis_rotate_input = 130;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "131" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun131()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun131\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 131 degree */
	pv.axis_rotate_input = 131;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "132" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun132()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun132\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 132 degree */
	pv.axis_rotate_input = 132;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "133" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun133()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun133\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 133 degree */
	pv.axis_rotate_input = 133;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "134" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun134()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun134\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 134 degree */
	pv.axis_rotate_input = 134;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "135" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun135()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun135\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 135 degree */
	pv.axis_rotate_input = 135;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "136" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun136()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun136\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 136 degree */
	pv.axis_rotate_input = 136;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "137" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun137()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun137\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 137 degree */
	pv.axis_rotate_input = 137;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "138" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun138()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun138\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 138 degree */
	pv.axis_rotate_input = 138;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "139" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun139()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun139\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 139 degree */
	pv.axis_rotate_input = 139;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "140" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun140()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun140\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 140 degree */
	pv.axis_rotate_input = 140;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "141" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun141()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun141\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 141 degree */
	pv.axis_rotate_input = 141;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "142" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun142()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun142\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 142 degree */
	pv.axis_rotate_input = 142;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "143" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun143()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun143\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 143 degree */
	pv.axis_rotate_input = 143;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "144" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun144()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun144\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 144 degree */
	pv.axis_rotate_input = 144;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "145" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun145()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun145\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 145 degree */
	pv.axis_rotate_input = 145;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "146" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun146()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun146\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 146 degree */
	pv.axis_rotate_input = 146;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "147" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun147()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun147\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 147 degree */
	pv.axis_rotate_input = 147;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "148" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun148()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun148\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 148 degree */
	pv.axis_rotate_input = 148;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "149" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun149()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun149\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 149 degree */
	pv.axis_rotate_input = 149;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "150" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun150()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun150\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 150 degree */
	pv.axis_rotate_input = 150;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "1515" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun151()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun151\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 151 degree */
	pv.axis_rotate_input = 151;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "152" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun152()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun152\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 152 degree */
	pv.axis_rotate_input = 152;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "153" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun153()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun153\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 153 degree */
	pv.axis_rotate_input = 153;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "154" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun154()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun154\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 154 degree */
	pv.axis_rotate_input = 154;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "155" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun155()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun155\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 155 degree */
	pv.axis_rotate_input = 155;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "156" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun156()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun156\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 156 degree */
	pv.axis_rotate_input = 156;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "157" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun157()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun157\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 157 degree */
	pv.axis_rotate_input = 157;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "158" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun158()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun158\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 158 degree */
	pv.axis_rotate_input = 158;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "159" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun159()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun159\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 159 degree */
	pv.axis_rotate_input = 159;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "160" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun160()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun160\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 160 degree */
	pv.axis_rotate_input = 160;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "161" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun161()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun161\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 161 degree */
	pv.axis_rotate_input = 161;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "162" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun162()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun162\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 162 degree */
	pv.axis_rotate_input = 162;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "163" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun163()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun163\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 163 degree */
	pv.axis_rotate_input = 163;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "164" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun164()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun164\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 164 degree */
	pv.axis_rotate_input = 164;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "165" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun165()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun165\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 165 degree */
	pv.axis_rotate_input = 165;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "166" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun166()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun166\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 166 degree */
	pv.axis_rotate_input = 166;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "167" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun167()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun167\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 167 degree */
	pv.axis_rotate_input = 167;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "168" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun168()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun168\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 168 degree */
	pv.axis_rotate_input = 168;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "169" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun169()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun169\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 169 degree */
	pv.axis_rotate_input = 169;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "170" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun170()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun170\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 170 degree */
	pv.axis_rotate_input = 170;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  :This function call the axis motor rotation
 *           function with "171" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun171()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun171\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 171 degree */
	pv.axis_rotate_input = 171;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "172" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun172()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun172\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 172 degree */
	pv.axis_rotate_input = 172;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief This function call the axis motor rotation
 *         function with "173" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun173()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun173\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 173 degree */
	pv.axis_rotate_input = 173;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief This function call the axis motor rotation
 *         function with "174" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun174()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun174\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 174 degree */
	pv.axis_rotate_input = 174;
	Axis_motor_rotation(pv.axis_rotate_input);

}

/** @brief  : This function call the axis motor rotation
 *            function with "175" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun175()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun175\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 175 degree */
	pv.axis_rotate_input = 175;
	Axis_motor_rotation(pv.axis_rotate_input);
}



/** @brief  : This function call the axis motor rotation
 *            function with "176" degree.
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun176()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun176\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 176 degree */
	pv.axis_rotate_input = 176;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "177" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun177()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun177\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 177 degree */
	pv.axis_rotate_input = 177;
	Axis_motor_rotation(pv.axis_rotate_input);
}

/** @brief  : This function call the axis motor rotation
 *            function with "178" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun178()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun178\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 178 degree */
	pv.axis_rotate_input = 178;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "179" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun179()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun179\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 179 degree */
	pv.axis_rotate_input = 179;
	Axis_motor_rotation(pv.axis_rotate_input);


}

/** @brief  : This function call the axis motor rotation
 *            function with "180" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun180()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun180\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor 180 degree */
	pv.axis_rotate_input = 180;
	Axis_motor_rotation(pv.axis_rotate_input);

}
/** @brief  : This function call the axis motor rotation
 *            function with "181" degree.
 *
 *  @param  : none
 *
 *  @return : none
 *
 */
void axis_SubFun181()
{
#ifdef __CMD_DEBUG__
	/* Send the Ack to module */
	uint8_t scmd[]="Axis_SubFun181\r\n";
	dump_packet(scmd, strlen((const char *)scmd));
#endif
	/* Rotate the Axis motor to 0 degree */
	pv.axis_rotate_input = 0;
	Axis_motor_rotation(pv.axis_rotate_input);

}
