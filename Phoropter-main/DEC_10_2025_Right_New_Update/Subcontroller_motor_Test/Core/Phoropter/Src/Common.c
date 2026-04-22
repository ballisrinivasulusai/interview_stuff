/*
 * Common.c
 *
 *  Created on:
 *      Author:
 */
#include "../Inc/common.h"
#include"../Inc/stm32h7xx_hal.h"
extern private_var pv;
extern UART_HandleTypeDef huart4;

/* Global declaration */

/* Variable used for disc homing */
uint8_t  disc_homing = 0;


/** @brief Process the Command
 *
 *  This function processes the received main board commands.
 *
 *  @param  None
 *
 *
 *  @return  1 : SUCCESS
 *           0 : FAILURE
 *
 */

uint8_t MBCmdReceivefunc()
{
	uint16_t     read_value;
    uint16_t     cmd_size;
    uint16_t     cmd_buf_offset = 0;
    uint16_t     index;
    uint16_t     wptr;

    wptr = pv.write_ptr;

    /* Check the data is available in the readbuffer */
    /* No data is available in the read buffer */
    if (pv.read_ptr == wptr )
    	return SUCCESS;

    if (pv.read_ptr < wptr)
    {
    	read_value = (wptr - pv.read_ptr);
    	/* Copy the data into the temp buffer */
    	memset(&pv.temp_buf[0], 0x00, sizeof(pv.temp_buf));
    	memcpy(&pv.temp_buf[0], &pv.rx_buf[pv.read_ptr], read_value);
    }
    else
    {
    	/* Overflow condition */
   		read_value = (sizeof(pv.rx_buf) - pv.read_ptr);

   		/* Copy the partial data into the buffer */
    	memset(&pv.temp_buf[0], 0x00, sizeof(pv.temp_buf));

    	if ( 0 < read_value)
    		memcpy(&pv.temp_buf[0], &pv.rx_buf[pv.read_ptr], read_value);

       	/* Copy the remaining data */
       	memcpy(&pv.temp_buf[read_value], &pv.rx_buf[0], wptr);

       	read_value += wptr;

    }
    cmd_size = 0;
    for(index = 0; index < read_value; index++)
    {
    	cmd_size++;

    	if ((CARRIAGE_RETURN == pv.temp_buf[index]) &&
            (NEW_LINE == pv.temp_buf[index+1]))
        {

    		/* Over Flow condition */
    		if((pv.read_ptr + cmd_size + 1) > sizeof(pv.rx_buf))
			{
    			pv.read_ptr = ((pv.read_ptr + cmd_size + 1) - sizeof(pv.rx_buf));
			}
    		else
    		{
				/* Update the read_ptr */
    			pv.read_ptr += (cmd_size + 1);
    		}
			cmd_size = 0;
    		index++;

    		/* Reaches end of the buffer */
    		/* Roll back the pointer     */
    		if (sizeof(pv.rx_buf) == pv.read_ptr)
    		    pv.read_ptr = 0;

    		/* Received command is \r\n */
    		if((index - cmd_buf_offset) <= 2)
    		{
                cmd_buf_offset = index+1;
    			continue;
    		}


            /* Copy the data into the temp buffer */
            memcpy(&pv.tcmd_rx_buf[0], &pv.temp_buf[cmd_buf_offset],
            		((index+1) - cmd_buf_offset - 2));

            pv.tcmd_rx_buf[(index+1) - cmd_buf_offset - 2] = '\0';

            /* Call the main process command */
         	process_cmd(pv.tcmd_rx_buf, ((index+1) - cmd_buf_offset - 2));

         	cmd_buf_offset = index+1;
         	memset(&pv.tcmd_rx_buf[0], 0x00, sizeof(pv.tcmd_rx_buf));
        }
    }

    return SUCCESS;
}


uint32_t sending_ACK = 0;
void check_motor_status(void)
{
	uint16_t  ioexp_data[2];
	if (MOTOR_STATUS_STOP == pv.cmd_status)
	{
		pv.cmd_status = MOTOR_IDLE;
		sending_ACK ++;
		/* If the timer is running, cancel the timer */
		if( STATUS_TIMER_RUNNING == pv.timer_flag)
		{
			/* Cancel the timer */
			HAL_TIM_Base_Stop(&htim7);
			pv.timer_flag = STATUS_TIMER_STOP;
		}

		memset(&pv.ack_data[0], 0x00, sizeof(pv.ack_data));
		memset(&ioexp_data[0], 0x00,sizeof(ioexp_data));
        /* Check the Error flag which is updated in I2C ISR */
		if(0 == pv.share_disc)
		{
			if( 1 == pv.ioexp_int )
			{
				pca9535_read(pv.hi2c1, 0x00, &ioexp_data ,2);
				sprintf((char *)&pv.ack_data, "FAILURE/%x\r\n", ioexp_data[0]);
			}
			else if( 0 == pv.ioexp_int )
			{
				pca9535_read(pv.hi2c1, 0x00, &ioexp_data ,2);
				sprintf((char *)&pv.ack_data, "SUCCESS/%x\r\n", ioexp_data[0]);
			}
		}
		else if(1 == pv.share_disc)
		{
			sprintf((char *)&pv.ack_data, "SUCCESS/Disc1/%ld/Disc2/%ld/Disc3/%ld/Disc4/%ld/"
					"Disc5/%ld/Disc6/%ld/PD/%ld/Axis/%ld/Prism1/%ld/Prism2/%ld/0D \r\n",
					pv.cal_disc[0],pv.cal_disc[1],pv.cal_disc[2],pv.cal_disc[3],
					pv.cal_disc[4],pv.cal_disc[5],pv.cal_disc[7],pv.cal_disc[6],
					pv.cal_disc[8],pv.cal_disc[9]);
		}

		pv.ioexp_int = 0;
		pv.share_disc = 0;
		dump_packet(&pv.ack_data[0], strlen((const char *)pv.ack_data));
		pv.motor_error = MOTOR_SUCCESS;
		pv.motor_flag  = MOTOR_SUCCESS;
	}

}


/** @brief  : This function initialize the private variables
 *
 *  @param  None
 *
 *
 *  @return none
 *
 */
void Init_PV()
{
	/* Clear the private memory */
	memset(&pv, 0x00, sizeof(pv));

	pv.dec_speed = 1;
	pv.value = 1;

	pv.motor_error = MOTOR_IDLE;
	pv.disc_error  = MOTOR_SUCCESS;

	pv.huart4 = &huart4;

	/*
	 * Initialize the decrement value of Acceleration and
	 * increment value of deceleration.
	 */

	pv.acc_decrease_val = 1;
	pv.dec_increase_val = 1;

	/* Initialize the PWM Duty cycle */
	pv.duty_cycle = 2;    //50%
}

/**
 * @brief  This function call is check the homing position for Disc1
 *         and rotate the motor 40 degree forward & Reverse.
 * @param  None.
 * @retval None.
 *
 */
void Disc1_homing_position(void)
{
	/*** Sensor Hit and Forward 40 Degree ***/
	pv.user_input = 360;
	Disc_motorspeed_cal( );
	pv.homing = 1;
	pv.sensor_hit = 0;
	disc_homing = 0;
	pv.disc_pwm = 1;
	/* Set the direction pin as Anti clockwise - PA4*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	/* Set the enable pin as low - Disc1 motor start - PA9 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH3 - Step Pin PA2 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc1 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(10);

	/*** Reverse 40 Degree ***/
	pv.homing = 2;
	pv.sensor_hit = 1;
	pv.motor_status = 1;
	pv.disc_pwm = 1;
	pv.user_input = 36;
	Disc_motorspeed_cal( );
	pv.time_counter = INIT_FREQ;
	pv.dec_speed = pv.time_counter;
	/* Set the direction pin as  clockwise - PA4 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	/* Set the enable pin as low - Disc1 motor start - PA11  */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH1 - Step Pin PA5 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);
	while( pv.motor_status != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc1 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);  //Motor Stop
	HAL_Delay(10);

}

/**
 * @brief : This function call is check the homing position for Disc2
 *          and rotate the motor 40 degree forward & Reverse.
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void Disc2_homing_position(void)
{
	/*** Sensor Hit and Forward 40 Degree ***/
	pv.user_input = 360;
	Disc_motorspeed_cal( );
	pv.homing = 1;
	pv.sensor_hit = 0;
	disc_homing = 1;
	pv.disc_pwm = 2;
	/* Set the direction pin as Anti clockwise - PA7*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	/* Set the enable pin as low - Disc2 motor start - PA9 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH3 - Step Pin PA2 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc2 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_Delay(10);

	/*** Reverse 40 Degree ***/
	pv.homing = 2;
	pv.sensor_hit = 1;
	pv.motor_status = 1;
	pv.disc_pwm = 2;
	pv.user_input = 36;
	Disc_motorspeed_cal( );
	pv.time_counter = INIT_FREQ;
	pv.dec_speed = pv.time_counter;
	/* Set the direction pin as  clockwise - PA7 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	/* Set the enable pin as low - Disc2 motor start - PA11  */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH1 - Step Pin PA5 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4);
	while( pv.motor_status != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc2 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);  //Motor Stop
	HAL_Delay(10);

}
/**
 * @brief : This function call is check the homing position for Disc3
 *          and rotate the motor 40 degree forward & Reverse.
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void Disc3_homing_position(void)
{
	/*** Sensor Hit and Forward 40 Degree ***/
	pv.user_input = 360;
	Disc_motorspeed_cal( );
	pv.homing = 1;
	pv.sensor_hit = 0;
	disc_homing = 2;
	pv.disc_pwm = 3;
	/* Set the direction pin as Anti clockwise - PA8*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	/* Set the enable pin as low - Disc3 motor start - PA11 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH1 - Step Pin PA5 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc3 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(10);

	/*** Reverse 40 Degree ***/
	pv.homing = 1;
	pv.sensor_hit = 1;
	pv.motor_status = 1;
	pv.disc_pwm = 3;
	pv.user_input = 215.2;
	Disc_motorspeed_cal( );
	/* Set the direction pin as  clockwise - PA8 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	/* Set the enable pin as low - Disc3 motor start - PA11  */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH1 - Step Pin PA5 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
	while( pv.motor_status != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc3 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);  //Motor Stop
	HAL_Delay(10);

}

/**
 * @brief : This function call is check the homing position for Disc4
 *          and rotate the motor 40 degree forward & Reverse.
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void Disc4_homing_position(void)
{
    /*** Sensor Hit and Forward 40 Degree ***/
	pv.user_input = 360;
	Disc_motorspeed_cal( );
	pv.homing = 1;
	pv.sensor_hit = 0;
	disc_homing = 3;
	pv.tim3_motor = 1;
	/* Set the direction pin as  clockwise - PA12*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	/* Set the enable pin as low - Disc4 motor start */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	/* Start the PWM TIM4_CH3 - Step Pin PD14 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc4 motor stop */
  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);  //axis halling
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_Delay(10);

	/*** Reverse 40 Degree ***/
	pv.homing = 1;
	pv.sensor_hit = 1;
	pv.motor_status = 1;
	pv.user_input = 145.4;
	pv.tim3_motor = 1;
	Disc_motorspeed_cal( );
	/* Set the direction pin as Anti clockwise - PA12 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
	/* Set the enable pin as low - Disc4 motor start */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	/* Start the PWM TIM4_CH3 - Step Pin PD14 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	while( pv.motor_status != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc4 motor stop */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);  //Motor Stop
	HAL_Delay(10);
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);   //axis hal

}

/**
 * @brief : This function call is check the homing position for Disc5
 *          and rotate the motor 40 degree forward & Reverse.
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void Disc5_homing_position(void)
{
	/*** Sensor Hit and Forward 40 Degree ***/
	pv.user_input = 360.76;
	Disc_motorspeed_cal( );
	pv.homing = 1;
	pv.sensor_hit = 0;
	disc_homing = 4;
	pv.disc_pwm = 4;
	/* Set the direction pin as  clockwise - PB1*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	/* Set the enable pin as low - Disc5 motor start - PB2 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH2 - Step Pin PB3  */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc5 motor stop */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_Delay(100);

	/*** Reverse 40 Degree ***/
	pv.homing = 1;
	pv.sensor_hit = 1;
	pv.motor_status = 1;
	pv.disc_pwm = 4;
	pv.user_input = 155.2;
	Disc_motorspeed_cal( );
	/* Set the direction pin as Anti clockwise - PB1 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	/* Set the enable pin as low - Disc5 motor start - PB2  */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH2 - Step Pin PB3 */
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);
	while( pv.motor_status != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc5 motor stop */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);  //Motor Stop
	HAL_Delay(10);
}


/**
 * @brief : This function call is check the homing position for Disc6
 *          and rotate the motor 40 degree forward & Reverse.
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void Disc6_homing_position(void)
{
	/*** Sensor Hit and Forward 40 Degree ***/
	pv.user_input = 360;
	Disc_motorspeed_cal( );
	pv.homing = 1;
	pv.sensor_hit = 0;
	disc_homing = 5;
	/* Set the direction pin as clockwise - PC5*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	/* Set the enable pin as low - Disc6 motor start - PB2 */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH2 - Step Pin PB3  */
	HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc6 motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(10);

	/*** Reverse 40 Degree ***/
	pv.homing = 2;
	pv.sensor_hit = 1;
	pv.motor_status = 1;
	pv.user_input = 240;
	Disc_motorspeed_cal( );
	pv.time_counter = INIT_FREQ;
	pv.dec_speed = pv.time_counter;
	/* Set the direction pin as Anti clockwise - PC5 */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	/* Set the enable pin as low - Disc6 motor start - PB2  */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH2 - Step Pin PB3 */
	HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
	while( pv.motor_status != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Disc6 motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);  //Motor Stop
	HAL_Delay(10);

}

/**
 * @brief : This function call is check the homing position for PD motor
 *          homing this will rotate the motor forward and reverse for
 *          respectively with input mm.
 *
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void PD_motor(void)
{
#if 1
	/* Homing position - Forward direction (20 mm) */
	pv.step_data  = 600000;   // 30000 steps  for 1mm
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.pd_flag1 = 1;
	pv.acc_per  = 1;
	PD_motorspeed_cal() ;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 1)
	{
		/* Set the direction pin as clockwise  - PB4*/
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		/* Set the enable pin as low - PD Motor - PB5*/
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		/* Start the PWM TIM4_CH1 - Step Pin PB6 */
		HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
		while(pv.flag_360 != 0);
		HAL_Delay(1);
		/* Set the enable pin as high - PD Motor  */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		/* 1second delay must */
		HAL_Delay(1000);
	}
#endif
	/* Homing position - Reverse direction (12mm) */
	pv.flag_360 = 1;
	pv.step_data = pv.cal_disc[7];
	pv.acc_per = 1;
	pv.pd_flag1 = 0;
	PD_motorspeed_cal() ;
	pv.PD_flag = 0;
	/* Set the direction pin as Anti clockwise */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	/* Set the enable pin as low - PD Motor  */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	/* Start the PWM TIM4_CH1 - Step Pin PB6 */
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - PD Motor  */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(200);
}

/**
 * @brief : This function call is check the homing position for Axis motor
 *          and before hitting homing this will rotate the motor forward and
 *          reverse for particular degree(180,90,45).
 *
 * @param : None.
 *
 * @retval: None.
 *
 */

void Axis_motor_homing(void)
{
	/*** Rotate 360 degree Forward***/
	pv.axis_user_input = 360;
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.axis_motor = 1;
	pv.tim3_motor = 3;
	pv.homing = 0;
	pv.axis_calib = 0;
	Axis_motorspeed_cal();
	/* Set the direction pin as clockwise */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
	/* Set the enable pin as low - Axis motor start */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH4 - Step Pin PB11 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.sensor_hit==0);
	while(pv.Dec != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(100);

	/*** Rotate 180 degree Forward***/
	pv.axis_user_input = 180;
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.tim3_motor = 3;
	pv.homing = 0;
	Axis_motorspeed_cal();
	/* Set the direction pin as clockwise */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
	/* Set the enable pin as low - Axis motor start */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH4 - Step Pin PB11 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(200);

	/*** Rotate 180 degree Reverse***/
	pv.axis_user_input = 180;
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.tim3_motor = 3;
	pv.homing = 0;
	Axis_motorspeed_cal();
	/* Set the direction pin as Anti clockwise */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
	/* Set the enable pin as low - Axis motor start */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH4 - Step Pin PB11 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(100);

	/*** Rotate 90 degree Forward***/
	pv.axis_user_input = 90;
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.tim3_motor = 3;
	pv.homing = 0;
	Axis_motorspeed_cal();
	/* Set the direction pin as clockwise */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
	/* Set the enable pin as low - Axis motor start */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH4 - Step Pin PB11 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(100);

	/*** Rotate 90 degree Reverse***/
	pv.axis_user_input = 90;
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.tim3_motor = 3;
	pv.homing = 0;
	Axis_motorspeed_cal();
	/* Set the direction pin as Anti clockwise */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
	/* Set the enable pin as low - Axis motor start */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH4 - Step Pin PB11 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(100);

	/*** Rotate 45 degree Forward***/
	pv.axis_user_input = 45;
	pv.sensor_hit = 1;
	pv.flag_360 = 1;
	pv.tim3_motor = 3;
	pv.homing = 0;
	Axis_motorspeed_cal();
	pv.time_counter = 1800;
	/* Set the direction pin as clockwise */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
	/* Set the enable pin as low - Axis motor start */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	/* Start the PWM TIM2_CH4 - Step Pin PB11 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(100);

	/*** Sensor Hit and Forward 40 Degree ***/
	pv.axis_user_input = 360;
	pv.homing = 0;
	pv.sensor_hit = 0;
	pv.axis_motor = 1;
	Axis_motorspeed_cal();
	/* Set the direction pin as Anti-clockwise - PB1*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_Delay(2);
	/* Set the enable pin as low - Axis motor start - PB2 */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_Delay(2);
	/* Start the PWM TIM2_CH2 - Step Pin PB3  */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.sensor_hit==0);
	while(pv.flag_360 != 0);
	HAL_Delay(1);
	/* Set the enable pin as high - Axis motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(100);

	/* Calibration Process */
	if(pv.cal_disc[6] != 0)
	{
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.tim3_motor = 3;
		pv.homing = 0;
		pv.axis_calib = 1;
		Axis_motorspeed_cal();
		/* Set the enable pin as low - Axis motor start */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
		/* Start the PWM TIM2_CH4 - Step Pin PB11 */
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		while(pv.flag_360 != 0);
		HAL_Delay(1);
		/* Set the enable pin as high - Axis motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
		HAL_Delay(100);
		pv.axis_calib = 0;
	}
	pv.axis_motor = 0;

}

/**
 * @brief : This function call is check the homing position for Prism1 motor
 *          and before hitting homing this will rotate the motor forward and
 *          reverse for 360 degree.
 * @param : None.
 *
 * @retval: None.
 *
 */

void Prism1_motor(void)
{
	/***0 Forward direction of 360 degree ***/
	pv.step_data = 358500;                  //995.83
	pv.sensor_hit = 0;
	pv.flag_360 = 1;
	pv.acc_per = 3;
	pv.homing = 4;
	Prism_motorspeed_cal() ;
	pv.tim3_motor = 2;
	/* Set the direction pin as clockwise - PC3*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(2);
	/* Set the enable pin as low - Prism1 motor start - PC2 */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_Delay(2);
	/* Start the PWM TIM2_CH3 - Step Pin PA2 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
	while(pv.sensor_hit==0);
 	//while(pv.Dec != 0);
	HAL_Delay(100);
	/* Set the enable pin as high - Prism1 motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_Delay(1000);


	/* Calibration Process */
	if( 0 != pv.cal_disc[8])
	{
		if( 0 < pv.cal_disc[8])
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
			{
				/* Set the direction of the prism motor - Clock wise  */
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			pv.step_data = pv.cal_disc[8] * 995.83 ;               // 995.83 steps for 1 degree
		}
		else if( 0 > pv.cal_disc[8])
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
			{
				/* Set the direction of the prism motor - Clock wise  */
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
			}
			pv.step_data =  ((-1) * pv.cal_disc[8]) * 995.83 ;      // 995.83 steps for 1 degree
		}
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.acc_per = 3;
		pv.homing = 3;
		Prism_motorspeed_cal() ;
		pv.tim3_motor = 2;
		/* Set the enable pin as low - Prism1 motor start - PC2 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_Delay(2);
		/* Start the PWM TIM2_CH3 - Step Pin PA2 */
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
		while(pv.Dec != 0);
		HAL_Delay(10);
		/* Set the enable pin as high - Prism1 motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_Delay(100);
	}

}

/**
 * @brief  This function call is check the homing position for Prism2 motor
 *         and before hitting homing this will rotate the motor forward and
 *         reverse 360 degree.
 *
 * @param  None.
 *
 * @retval None.
 *
 */

void Prism2_motor(void)
{
	/*** Forward direction of 360 degree ***/
	pv.step_data = 358500;
	pv.sensor_hit = 0;
	pv.flag_360 = 1;
	pv.homing = 5;
	pv.acc_per = 3;
	pv.tim3_motor = 4;
	Prism_motorspeed_cal();
	/* Set the direction pin as clockwise - PC3*/
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_Delay(2);
	/* Set the enable pin as low - Prism2 motor start - PC2 */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_Delay(2);
	/* Start the PWM TIM2_CH3 - Step Pin PA2 */
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);
	while(pv.sensor_hit==0);
	//while(pv.Dec != 0);
	HAL_Delay(10);
	/* Set the enable pin as high - Prism2 motor stop */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_Delay(1000);

	/* Calibration Process */
	if( 0 != pv.cal_disc[9])
	{
		if( 0 < pv.cal_disc[9])
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
			{
				/* Set the direction of the prism motor - Clock wise  */
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
			}
			pv.step_data = pv.cal_disc[9] * 995.83 ;               // 995.83 steps for 1 degree
		}
		else if( 0 > pv.cal_disc[9])
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
			{
				/* Set the direction of the prism motor - Clock wise  */
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
			}
			pv.step_data =  ((-1) * pv.cal_disc[9]) * 995.83 ;      // 995.83 steps for 1 degree
		}
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.acc_per = 3;
		pv.homing = 3;
		Prism_motorspeed_cal() ;
		pv.tim3_motor = 4;
		/* Set the enable pin as low - Prism1 motor start - PC2 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(2);
		/* Start the PWM TIM2_CH3 - Step Pin PA2 */
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);
		while(pv.Dec != 0);
		HAL_Delay(10);
		/* Set the enable pin as high - Prism1 motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(100);

	}
}

/**
 * @brief : This function call is check the homing position for DC motor.
 *
 * @param : None.
 *
 * @retval: None.
 *
 */
void DC_motor_In(void)
{
	if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_RESET)
			&& (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == GPIO_PIN_SET))
	{
		pv.dc_int1 = 0;
		/* Forward Direction */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		//HAL_Delay(2000);
		while(pv.dc_int1 != 1);
		/* Break */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

	}
}

void DC_motor_Out(void)
{
	if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_SET)
			&& (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == GPIO_PIN_RESET))
	{
		pv.dc_int2 = 0;
		/* Forward Direction */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		//HAL_Delay(2000);
		while(pv.dc_int2 != 1);
		/* Break */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

	}
	else if(((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_RESET)
			&& (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == GPIO_PIN_SET))
			|| ((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_SET)
					&& (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) == GPIO_PIN_SET)))
	{
		pv.dc_int1 = 0;
		/* Reverse Direction Direction */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		while(pv.dc_int1 != 1);
		/* Break */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		HAL_Delay(1000);

		pv.dc_int2 = 0;
		/* Forward Direction */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		//HAL_Delay(2000);
		while(pv.dc_int2 != 1);
		/* Break */
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

	}

}

/**
 *
 * @brief  Rotate the motor as per UART input.
 * @param  input, disc_input
 * @retval None.
 *
 */
uint8_t Disc_Motor_Rotation(double input,uint8_t disc_input)
{

	if(disc_input == 1)
	{
		if(pv.disc1_status == MOTOR_HIGH)
			rotate_disc1(input);
		else
			pv.disc1_check = 0;
	}
	if(disc_input == 2)
	{
		if(pv.disc2_status == MOTOR_HIGH)
			rotate_disc2(input);
		else
			pv.disc2_check = 0;
	}
	if(disc_input == 3)
	{
		if(pv.disc3_status == MOTOR_HIGH)
			rotate_disc3(input);
		else
			pv.disc3_check = 0;
	}
	if(disc_input == 4)
	{
		if(pv.disc4_status == MOTOR_HIGH)
			rotate_disc4(input);
		else
			pv.disc4_check = 0;
	}
	if(disc_input == 5)
	{
		if(pv.disc5_status == MOTOR_HIGH)
			rotate_disc5(input);
		else
			pv.disc5_check = 0;
	}
	if(disc_input == 6)
	{
		if(pv.disc6_status == MOTOR_HIGH)
			rotate_disc6(input);
		else
			pv.disc6_check = 0;
	}

	Disc_motorspeed_cal( );


	if((pv.user_input >= 2)&&(pv.user_input <= 240))
	{
		pv.time_counter = INIT_FREQ;
		pv.dec_speed = pv.time_counter;
		pv.homing = 2;
	}

	else if(pv.user_input > 240)
	{
		pv.homing = 1;
	}

	pv.motor_status = 1;
	pv.sensor_hit = 1;

	/* Start the status timer */
	pv.timer_flag = STATUS_TIMER_STOP;
	HAL_TIM_Base_Start(&htim7);
	pv.count = 0;

	if(0 == pv.Acc)
	{
		return 0;
	}

	/* Set the enable pin as low - Start the disc motor */
	if((disc_input == 1) && (pv.disc1_status == MOTOR_HIGH))
	{
		pv.disc_pwm = 1;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);
		/* Update the initial motor status flags */
		pv.motor_error = MOTOR_BUSY;
		pv.motor_flag  = MOTOR1_RUNNING;
	}
	else if((disc_input == 2) && (pv.disc2_status == MOTOR_HIGH))
	{
		pv.disc_pwm = 2;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4);

		/* Update the initial motor status flags */
		pv.motor_error = MOTOR_BUSY;
		pv.motor_flag  = MOTOR2_RUNNING;
	}
	else if((disc_input == 3) && (pv.disc3_status == MOTOR_HIGH))
	{
		pv.disc_pwm = 3;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);

		/* Update the initial motor status flags */
		pv.motor_error = MOTOR_BUSY;
		pv.motor_flag  = MOTOR3_RUNNING;
	}
	else if((disc_input == 4) && (pv.disc4_status == MOTOR_HIGH))
	{
		pv.tim3_motor = 1;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);

		/* Update the initial motor status flags */
		pv.motor_error = MOTOR_BUSY;
		pv.motor_flag  = MOTOR4_RUNNING;
	}
	else if((disc_input == 5) && (pv.disc5_status = MOTOR_HIGH))
	{
		pv.disc_pwm = 4;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);

		/* Update the initial motor status flags */
		pv.motor_error = MOTOR_BUSY;
		pv.motor_flag  = MOTOR5_RUNNING;
	}
	else if((disc_input == 6) && (pv.disc6_status == MOTOR_HIGH))
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);

		/* Update the initial motor status flags */
		pv.motor_error = MOTOR_BUSY;
		pv.motor_flag  = MOTOR5_RUNNING;
	}

	while((pv.motor_status != 0) && (MOTOR_BUSY == pv.motor_error));
	HAL_Delay(200);

	/* Set the enable pin as high - Stop the disc motor */
	if((disc_input == 1) && (pv.disc1_status == MOTOR_HIGH))
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	else if((disc_input == 2) && (pv.disc2_status == MOTOR_HIGH))
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	else if((disc_input == 3) && (pv.disc3_status == MOTOR_HIGH))
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	else if((disc_input == 4) && (pv.disc4_status == MOTOR_HIGH))
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	else if((disc_input == 5) && (pv.disc5_status == MOTOR_HIGH))
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	else if((disc_input == 6) && (pv.disc6_status == MOTOR_HIGH))
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_Delay(100);

	/* if any one of the disc is low set the interrupt variable */
	if((pv.disc1_check == 1) && (pv.disc2_check == 1) &&
			(pv.disc3_check == 1) && (pv.disc4_check == 1) &&
			(pv.disc5_check == 1) && (pv.disc6_check == 1))
	{
		pv.ioexp_int = 0;
	}
	else
	{
		pv.ioexp_int = 1;
	}

	pv.homing = 0;
	return SUCCESS;
}

/**
 *
 * @brief  Rotate the disc1 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void rotate_disc1(double input)
{
    pv.D1.rcv_buff = input;

    pv.user_input = pv.D1.rcv_buff;

    if(pv.D1.rcv_buff > pv.D1.temp_buff)
    {
    	pv.D1.input_val = pv.D1.rcv_buff - pv.D1.temp_buff ;
    	if(pv.D1.input_val <= 160)
    	{
    		/* Anti Clockwise Direction */
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    		pv.user_input = pv.D1.input_val;
    	}
    	else if(pv.D1.input_val > 160)
    	{
    		/* Clockwise Direction */
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    		pv.user_input = (FULL_ROTATION - pv.D1.rcv_buff) + pv.D1.temp_buff ;
    	}
    	pv.D1.temp_buff = pv.D1.rcv_buff;
    }
    else
    {
    	pv.D1.input_val = pv.D1.temp_buff - pv.D1.rcv_buff ;
    	if(pv.D1.input_val <= 160)
    	{
    		/* Clockwise Direction */
       		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
       		pv.user_input = pv.D1.input_val;
    	}
    	else if(pv.D1.input_val > 160)
    	{
    		/* AntiClockwise Direction */
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    		pv.user_input = (FULL_ROTATION - pv.D1.temp_buff) + pv.D1.rcv_buff;
    	}
    	pv.D1.temp_buff = pv.D1.rcv_buff;
    }
}

/**
 *
 * @brief  Rotate the disc2 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void rotate_disc2(double input)
{
    pv.D2.rcv_buff = input;

    pv.user_input = pv.D2.rcv_buff;

    if(pv.D2.rcv_buff > pv.D2.temp_buff)
    {
    	pv.D2.input_val = pv.D2.rcv_buff - pv.D2.temp_buff ;
    	if(pv.D2.input_val <= 160)
    	{
    		/* AntiClockwise Direction */
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    		pv.user_input = pv.D2.input_val;
    	}
    	else if(pv.D2.input_val > 160)
    	{
    		/* Clockwise Direction */
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    		pv.user_input = (FULL_ROTATION - pv.D2.rcv_buff) + pv.D2.temp_buff ;
    	}
    	pv.D2.temp_buff = pv.D2.rcv_buff;
    }
    else
    {
    	pv.D2.input_val = pv.D2.temp_buff - pv.D2.rcv_buff ;
    	if(pv.D2.input_val <= 160)
    	{
    		/* Clockwise Direction */
       		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
       		pv.user_input = pv.D2.input_val;
    	}
    	else if(pv.D2.input_val > 160)
    	{
    		/* AntiClockwise Direction */
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    		pv.user_input = (FULL_ROTATION - pv.D2.temp_buff) + pv.D2.rcv_buff;
    	}
    	pv.D2.temp_buff = pv.D2.rcv_buff;
    }
}

/**
 *
 * @brief  Rotate the disc3 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void rotate_disc3(double input)
{
    pv.D3.rcv_buff = input;

    pv.user_input = pv.D3.rcv_buff;

    if(pv.D3.rcv_buff > pv.D3.temp_buff)
    {
        pv.D3.input_val = pv.D3.rcv_buff - pv.D3.temp_buff ;
        if(pv.D3.input_val <= 160)
        {
                /* AntiClockwise Direction */
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
                pv.user_input = pv.D3.input_val;
        }
        else if(pv.D3.input_val > 160)
        {
                /* Clockwise Direction */
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
                pv.user_input = (FULL_ROTATION - pv.D3.rcv_buff) + pv.D3.temp_buff ;
        }
        pv.D3.temp_buff = pv.D3.rcv_buff;
    }
    else
    {
    	pv.D3.input_val = pv.D3.temp_buff - pv.D3.rcv_buff ;
    	if(pv.D3.input_val <= 160)
    	{
    		/* Clockwise Direction */
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    		pv.user_input = pv.D3.input_val;
    	}
    	else if(pv.D3.input_val > 160)
    	{
    		/* AntiClockwise Direction */
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    		pv.user_input = (FULL_ROTATION - pv.D3.temp_buff) + pv.D3.rcv_buff;
    	}
    	pv.D3.temp_buff = pv.D3.rcv_buff;
    }
}

/**
 *
 * @brief  Rotate the disc4 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void rotate_disc4(double input)
{
    pv.D4.rcv_buff = input;

    pv.user_input = pv.D4.rcv_buff;

    if(pv.D4.rcv_buff > pv.D4.temp_buff)
    {
        pv.D4.input_val = pv.D4.rcv_buff - pv.D4.temp_buff ;
        if(pv.D4.input_val <= 160)
        {
                /* AntiClockwise Direction */
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
                pv.user_input = pv.D4.input_val;
        }
        else if(pv.D4.input_val > 160)
        {
                /* Clockwise Direction */
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
                pv.user_input = (FULL_ROTATION - pv.D4.rcv_buff) + pv.D4.temp_buff ;
        }
        pv.D4.temp_buff = pv.D4.rcv_buff;
    }
    else
    {
        pv.D4.input_val = pv.D4.temp_buff - pv.D4.rcv_buff ;
        if(pv.D4.input_val <= 160)
        {
                /* Clockwise Direction */
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
                pv.user_input = pv.D4.input_val;
        }
        else if(pv.D4.input_val > 160)
        {
                /* AntiClockwise Direction */
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
                pv.user_input = (FULL_ROTATION - pv.D4.temp_buff) + pv.D4.rcv_buff;
        }
        pv.D4.temp_buff = pv.D4.rcv_buff;
    }

    pv.user_input = round(pv.user_input*10)/10;
}

/**
 *
 * @brief  Rotate the disc5 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void rotate_disc5(double input)
{
    pv.D5.rcv_buff = input;

    pv.user_input = pv.D5.rcv_buff;

    if(pv.D5.rcv_buff > pv.D5.temp_buff)
    {
        pv.D5.input_val = pv.D5.rcv_buff - pv.D5.temp_buff ;
        if(pv.D5.input_val <= 160)
        {
                /* AntiClockwise Direction */
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
                pv.user_input = pv.D5.input_val;
        }
        else if(pv.D5.input_val > 160)
        {
                /* Clockwise Direction */
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
                pv.user_input = (FULL_ROTATION - pv.D5.rcv_buff) + pv.D5.temp_buff ;
        }
        pv.D5.temp_buff = pv.D5.rcv_buff;
    }
    else
    {
        pv.D5.input_val = pv.D5.temp_buff - pv.D5.rcv_buff ;
        if(pv.D5.input_val <= 160)
        {
                /* Clockwise Direction */
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
                pv.user_input = pv.D5.input_val;
        }
        else if(pv.D5.input_val > 160)
        {
                /* AntiClockwise Direction */
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
                pv.user_input = (FULL_ROTATION - pv.D5.temp_buff) + pv.D5.rcv_buff;
        }
        pv.D5.temp_buff = pv.D5.rcv_buff;
    }

    pv.user_input = round(pv.user_input*10)/10;

}

/**
 *
 * @brief  Rotate the disc6 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void rotate_disc6(double input)
{
    pv.D6.rcv_buff = input;

    pv.user_input = pv.D6.rcv_buff;

    if(pv.D6.rcv_buff > pv.D6.temp_buff)
    {
    	pv.D6.input_val = pv.D6.rcv_buff - pv.D6.temp_buff ;
    	if(pv.D6.input_val <= 160)
    	{
    		/* AntiClockwise Direction */
    		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    		pv.user_input = pv.D6.input_val;
    	}
    	else if(pv.D6.input_val > 160)
    	{
    		/* Clockwise Direction */
    		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    		pv.user_input = (FULL_ROTATION - pv.D6.rcv_buff) + pv.D6.temp_buff ;
    	}
    	pv.D6.temp_buff = pv.D6.rcv_buff;
    }
    else
    {
    	pv.D6.input_val = pv.D6.temp_buff - pv.D6.rcv_buff ;
    	if(pv.D6.input_val <= 160)
    	{
    		/* Clockwise Direction */
       		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
       		pv.user_input = pv.D6.input_val;
    	}
    	else if(pv.D6.input_val > 160)
    	{
    		/* AntiClockwise Direction */
        	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    		pv.user_input = (FULL_ROTATION - pv.D6.temp_buff) + pv.D6.rcv_buff;
    	}
    	pv.D6.temp_buff = pv.D6.rcv_buff;
    }
}

/**
 *
 * @brief  Rotate the Axis motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */

void Axis_motor_rotation(int16_t input)
{

	  if(pv.axis_status == MOTOR_HIGH)
	  {

		  if(input > pv.axis_temp)
		  {
			  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_SET)
			  {
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET); //Clockwise
			  }
			  pv.axis_user_input = input - pv.axis_temp;
		  }
		  else if(input < pv.axis_temp)
		  {
			  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_RESET)
			  {
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);  //Anti-Clockwise
			  }
			  pv.axis_user_input = pv.axis_temp - input;
		  }

		  Axis_motorspeed_cal();
		  pv.flag_360 = 1;
		  pv.sensor_hit = 1;
		  pv.tim3_motor = 3;
		  pv.axis_motor = 1;
		  pv.homing=0;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET); //Motor Start
		  HAL_Delay(2);
		  HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		  while(pv.flag_360 != 0);
		  HAL_Delay(10);
		  HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); //stop motor
		  HAL_Delay(200);

		  pv.axis_motor = 0;
		  pv.axis_temp  = input;
		  if(pv.aixs_xcyl_temp == 1)
			  pv.aixs_xcyl_temp = 2 ;

	  }
	  if(MOTOR_LOW == pv.axis_status)
	  {
		  pv.ioexp_int = 1;
	  }
	  else
	  {
		  pv.ioexp_int = 0;
	  }
}

/**
 *
 * @brief : Rotate the Axis motor as per UART input.
 *
 * @param : input.
 *
 * @retval: None.
 *
 */

void Axis_motor_rotation_calibration(int16_t input)
 {

	  if(pv.axis_status == MOTOR_HIGH)
	  {

		  if(input > 0)
		  {
			  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_SET)
			  {
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET); //Clockwise
			  }
			  pv.axis_user_input = 1;
		  }
		  else if(input < 0)
		  {
			  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_RESET)
			  {
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);  //Anti-Clockwise
			  }
			  pv.axis_user_input = 1;
		  }

		  Axis_motorspeed_cal();
		  pv.flag_360 = 1;
		  pv.sensor_hit = 1;
		  pv.tim3_motor = 3;
		  pv.axis_motor = 1;
		  pv.homing=0;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET); //Motor Start
		  HAL_Delay(2);
		  HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		  while(pv.flag_360 != 0);
		  HAL_Delay(10);
		  HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); //stop motor
		  HAL_Delay(200);

		  pv.axis_motor = 0;
		  pv.axis_temp  = 0;

	  }
	  if(MOTOR_LOW == pv.axis_status)
	  {
		  pv.ioexp_int = 1;
	  }
	  else
	  {
		  pv.ioexp_int = 0;
	  }
}


void axis_xcyl(int16_t input)
{
	if( (input == 22) || (input == -22) )
	{
		if( pv.aixs_xcyl_temp == 0 || pv.aixs_xcyl_temp == 2)
		{
			if(input == 22)
				input = pv.axis_xcyl + 22;
			if(input == -22)
				input = pv.axis_xcyl - 22;
			if(pv.aixs_xcyl_temp == 0)
				pv.aixs_xcyl_temp = 1;
		}
		else if( pv.aixs_xcyl_temp == 1 )
		{
			if(input == 22)
				input = pv.axis_xcyl + 45;
			if(input == -22)
				input = pv.axis_xcyl - 45;
			pv.aixs_xcyl_temp = 1;
		}
	}
	else
	{
		pv.aixs_xcyl_temp = 0 ;
	}
	if(input >= pv.axis_xcyl)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET); //Clockwise
		}
		pv.axis_user_input = input - pv.axis_xcyl;
	}
	else if(input < pv.axis_xcyl)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_RESET)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);  //Anti-Clockwise
		}
		pv.axis_user_input = pv.axis_xcyl - input;
	}
	if(pv.axis_user_input < 0)
		pv.axis_user_input = (-1) * pv.axis_user_input;
	Axis_motorspeed_cal();
	pv.flag_360 = 1;
	pv.sensor_hit = 1;
	pv.tim3_motor = 3;
	pv.axis_motor = 1;
	pv.homing=0;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET); //Motor Start
	HAL_Delay(2);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	while(pv.flag_360 != 0);
	HAL_Delay(10);
	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); //stop motor
	HAL_Delay(200);
	pv.axis_motor = 0;
	pv.axis_xcyl = input;
}
/**
 *
 * @brief  Rotate the Prism1 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */

void Prism1_Motor_Rotation(int16_t input)
{
	if(pv.prism1_status == MOTOR_HIGH)
	{
		if((pv.prism_select == 4))
		{
			if(input > 0)
			{
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
				}
				pv.step_data = input * 995.83;
			}
			else
			{
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
				}
				pv.step_data = (-1) * input * 995.83;
			}
		}

		pv.homing = 3;
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.acc_per = 1;
		pv.tim3_motor = 4;
		Prism_motorspeed_cal();
		/* Set the enable pin as low - Prism2 motor start - PC13 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_Delay(2);
		/* Start the PWM TIM3_CH2 - Step Pin PA2 */
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
		//while(sensor_hit==0);
		while(pv.Dec != 0);
		HAL_Delay(10);
		/* Set the enable pin as high - Prism2 motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_Delay(100);
	}
	if(MOTOR_LOW == pv.prism1_status)
	{
		pv.ioexp_int = 1;
	}
	else
	{
		pv.ioexp_int = 0;
	}
}

/**
 *
 * @brief  Rotate the Prism2 motor as per UART input.
 * @param  input.
 * @retval None.
 *
 */

void Prism2_Motor_Rotation(int16_t input)
{
	if(pv.prism2_status == MOTOR_HIGH)
	{
		if((pv.prism_select == 4))
		{
			if(input > 0)
			{
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
				}
				pv.step_data = input * 995.83;
			}
			else
			{
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
				}
				pv.step_data = (-1) * input * 995.83;
			}
		}
		pv.homing = 3;
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.acc_per = 1;
		pv.tim3_motor = 4;
		Prism_motorspeed_cal();
		/* Set the enable pin as low - Prism2 motor start - PC13 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(2);
		/* Start the PWM TIM3_CH4 - Step Pin PA2 */
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);
		//while(sensor_hit==0);
		while(pv.Dec != 0);
		HAL_Delay(10);
		/* Set the enable pin as high - Prism2 motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(100);

	}
	if(MOTOR_LOW == pv.prism2_status)
	{
		pv.ioexp_int = 1;
	}
	else
	{
		pv.ioexp_int = 0;
	}
}
void prism_motor_rotation(int16_t  input)
{
	if((pv.prism2_status == MOTOR_HIGH) && (pv.prism2_status == MOTOR_HIGH))
	{
		if((pv.prism_select == 4))
		{
			if(input > 0)
			{
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
				}
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
				}
				pv.step_data = input * 995.83;
			}
			else
			{
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
				}
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
				}
				pv.step_data = (-1) * input * 995.83;
			}
			pv.homing = 3;
			pv.sensor_hit = 1;
			pv.flag_360 = 1;
			pv.acc_per = 1;
			Prism_motorspeed_cal();
			/* Set the enable pin as low - Prism2 motor start - PC13 */
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
			HAL_Delay(2);
			/* Set the enable pin as low - Prism2 motor start - PC13 */
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_Delay(2);
			while(pv.Dec != 0)
			{
					pv.tim3_motor = 2;
					HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
					pv.tim3_motor = 4;
					HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);
			}
			HAL_Delay(10);
			/* Set the enable pin as high - Prism2 motor stop */
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);
			pv.tim3_motor = 0;

		}
	}
}

/**
 *
 * @brief  Prism  polar function as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void Prism_Polar(int16_t input)
{
	int16_t value;
	value = input - pv.prism_polar_temp;
	if(value > 180)
	{
		value = value - 360 ;
	}
	else if(value < -180)
	{
		value = value + 360 ;
	}
	prism_motor_rotation(value);
	pv.prism_polar_temp = input;
}

/**
 *
 * @brief  Prism  XY function as per UART input.
 * @param  input.
 * @retval None.
 *
 */
void Prism_XY(int16_t h_input,int16_t v_input)
{
#if 1
	if(pv.prism == 1)
	{
		if(pv.BB_on == 0)
		{
			if(pv.x_temp >= 0)
			{
				if(h_input < 0)
				{
					if(0 != pv.prism_polar_temp)
					{
						pv.prism_polar_input = 0;
						pv.prism_select = 4;
						prism_function(pv.prism_select);
					}
					pv.prism_polar_temp = 180;
					pv.polar_func = 1;
				}
				else if(h_input == 0)
				{
					if(0 != pv.prism_polar_temp)
					{
						pv.prism_polar_input = 0;
						pv.prism_select = 4;
						prism_function(pv.prism_select);
					}
					pv.prism_polar_temp = 0;
					pv.polar_func = 0;
				}

			}
			else if(pv.x_temp < 0)
			{
				if(h_input >= 0)
				{
					if(0 != pv.prism_polar_temp)
					{
						pv.prism_polar_input = 180;
						pv.prism_select = 4;
						prism_function(pv.prism_select);
					}
					pv.prism_polar_temp = 0;
					pv.polar_func = 0;
				}
			}
		}
	}
#endif
	double  x_input;
	double  y_input;
	double  x_degree;
	double  y_degree;
	double  x_rotate;
	double  y_rotate;
	x_input  = h_input * 0.25;
	y_input  = v_input * 0.25;
	x_degree = sqrt((x_input*x_input) + (y_input * y_input));  // formula for cartesian to polar
	if(y_input > 0)
		y_degree = atan(y_input/x_input) * (180/3.14);		   // formula for cartesian to polar
	else if(y_input == 0)
		y_degree = 0;
	else if(y_input < 0)
	{
		y_degree = atan(y_input/x_input) * (180/3.14);
		y_degree = -1 * y_degree;
	}
	if(y_degree != 0 || pv.prism_y == 0)
	{
		if(y_degree  >= (pv.y_temp))
		{
			y_rotate = y_degree - pv.y_temp;
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
			{
				/*   Set the direction of the prism motor - Clock wise	*/
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
			{
				/* Set the direction of the prism motor - Clock wise	*/
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
			}
		}
		else if(y_degree  < (pv.y_temp))
		{
			y_rotate = pv.y_temp - y_degree;
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
			{
				/*   Set the direction of the prism motor - Anti-Clock wise	*/
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
			{
				/* Set the direction of the prism motor - Clock wise	*/
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
			}
		}
		if(y_rotate < 0)
			y_rotate = (-1) * y_rotate ;
		pv.step_data = (y_rotate * 995.83);
		pv.homing = 3;
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.acc_per = 1;
		Prism_motorspeed_cal();
		/* Set the enable pin as low - Prism2 motor start - PC13 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_Delay(2);
		/* Set the enable pin as low - Prism2 motor start - PC13 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(2);
		while(pv.Dec != 0)
		{
			pv.tim3_motor = 2;
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
			pv.tim3_motor = 4;
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);
		}
		HAL_Delay(10);
		/* Set the enable pin as high - Prism2 motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(100);
	}

	if(1 != pv.prism_x)
	{
		if((h_input == 0)  && (v_input < 0))
			x_degree = -1 * x_degree;
		if(x_input < 0)
			x_degree = -1 * x_degree;
		if(pv.BB_on == 0)
		{
			if(x_degree  >= (pv.x_temp + 0.25))
			{
				x_rotate = x_degree - pv.x_temp;
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
				{
					/*   Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
				}
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
				}

			}
			else if(x_degree  <= (pv.x_temp  - 0.25))
			{
				x_rotate = pv.x_temp - x_degree;
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
				{
					/*   Set the direction of the prism motor - Anti-Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
				}
				if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
				{
					/* Set the direction of the prism motor - Clock wise	*/
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
				}
			}
		}
		if(pv.BB_on == 1)
		{
			if(pv.prism == 1)
			{
				if(x_degree  >= (pv.x_temp + 0.25))
				{
					x_rotate = x_degree - pv.x_temp;
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
					{
						/*   Set the direction of the prism motor - Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
					}
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
					{
						/* Set the direction of the prism motor - Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
					}

				}
				else if(x_degree  <= (pv.x_temp  - 0.25))
				{
					x_rotate = pv.x_temp - x_degree;
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
					{
						/*   Set the direction of the prism motor - Anti-Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
					}
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
					{
						/* Set the direction of the prism motor - Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
					}
				}
			}
			else if(pv.prism == 0)
			{
				if(x_degree  >= (pv.x_temp + 0.25))
				{
					x_rotate = x_degree - pv.x_temp;
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_RESET)
					{
						/*   Set the direction of the prism motor - Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
					}
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_RESET)
					{
						/* Set the direction of the prism motor - Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
					}

				}
				else if(x_degree  <= (pv.x_temp  - 0.25))
				{
					x_rotate = pv.x_temp - x_degree;
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) != GPIO_PIN_SET)
					{
						/*   Set the direction of the prism motor - Anti-Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
					}
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) != GPIO_PIN_SET)
					{
						/* Set the direction of the prism motor - Clock wise	*/
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
					}
				}
			}
		}
		if(x_rotate < 0)
			x_rotate = (-1) * x_rotate ;
		pv.step_data = (x_rotate * 4 * 995.83 * 1.125);
		pv.homing = 3;
		pv.sensor_hit = 1;
		pv.flag_360 = 1;
		pv.acc_per = 1;
		Prism_motorspeed_cal();
		/* Set the enable pin as low - Prism2 motor start - PC13 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		/* Set the enable pin as low - Prism2 motor start - PC13 */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(2);
		while(pv.Dec != 0)
		{
			pv.tim3_motor = 2;
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
			pv.tim3_motor = 4;
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);
		}
		HAL_Delay(10);
		/* Set the enable pin as high - Prism2 motor stop */
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(100);
	}

	pv.x_temp = x_degree;
	pv.y_temp = y_degree;
	pv.ver_temp = v_input;
	pv.hor_temp = h_input;
	if(pv.prism_x == 1)
	{
		pv.x_temp = 0;
		pv.ver_temp = 0;
	}
	if(y_degree == 0 && pv.prism_y == 1)
	{
		pv.y_temp = 90;
	}
	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);
	pv.tim3_motor = 0;

}
/**
 *
 * @brief  select the Prism function as per UART input.
 * @param  input.
 * @retval None.
 *
 */

void prism_function(uint8_t input)
{
	pv.prism = pv.cal_disc[10];
	if((input == 1) || (input == 2))
	{
		Prism_XY(pv.prism_hor_input,pv.prism_ver_input);
	}
	else if(input == 4)
	{
		Prism_Polar(pv.prism_polar_input);
	}
}
/**
 *
 * @brief  Rotate the PD motor as per UART input.
 * @param  input.
 * @retval None.
 *
 **/

void PD1_motor(int8_t PD1_input)
{
	if(pv.pd_status == MOTOR_HIGH)
	{
		pv.PD1_current_val = PD1_input;
		if(pv.PD1_current_val >  0 )
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) != GPIO_PIN_SET)
			{
				/* Set the pin as high for pd motor clockwise direction */
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
				pv.PD_flag = 0;
			}
		}
		else if(pv.PD1_current_val < 0)
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) != GPIO_PIN_RESET)
			{
				/* Set the pin as high for pd motor clockwise direction */
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
				pv.PD_flag = 0;
			}
			pv.PD1_current_val = (-1) * pv.PD1_current_val;
		}
		pv.step_data = pv.PD1_current_val * 15000;
		pv.flag_360 = 1;
		pv.pd_flag1 = 0;
		pv.acc_per = 1;
		if(pv.PD1_current_val ==  2)
		{
			pv.acc_per = 3;
		}
		if(pv.PD1_current_val ==  1)
		{
			pv.acc_per = 5;
		}
		PD_motorspeed_cal() ;
		 /* Set the enable pin as low - PD Motor  */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		/* Start the PWM TIM4_CH1 - Step Pin PB6 */
		HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
		while(pv.flag_360 != 0);
		HAL_Delay(1);
		/* Set the enable pin as high - PD Motor  */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		//pv.PD1_temp_old = PD1_input;
	}
	if(MOTOR_LOW == pv.pd_status)
	{
		pv.ioexp_int = 1;
	}
	else
	{
		pv.ioexp_int = 0;
	}
}
/**
 * @brief  Calculate the Disc motor speed.
 * @param  None.
 * @retval None.
 *
 */
void Disc_motorspeed_cal(void)
{
	  pv.step_data = (disc_rotation_1degree * pv.user_input);
      pv.Acc = ((pv.step_data*disc_acc_per)/100);

      /* This condition is only for the Disc Calibration */
      if(pv.user_input == 1)
      {
    	  pv.Acc = INIT_FREQ + 35000 ;
    	  pv.Acc_Val = pv.Acc - disc_accval_1;
    	  pv.time_counter = pv.Acc + 3000;
    	  pv.dec_speed = pv.time_counter;
    	  pv.homing = 2;

      }

      if((pv.user_input >= 11) && (pv.user_input <= 36))
    	  pv.Acc_Val = pv.Acc - disc_accval_10;

      if((pv.user_input == 36) || (pv.user_input == 40) ||
    	 ((pv.user_input >= 51) && (pv.user_input <= 53)) ||
    	 ((pv.user_input >= 60) && (pv.user_input <= 80)))

    	  pv.Acc_Val = pv.Acc - disc_accval_40;

      if((pv.user_input >= 102) && (pv.user_input <= 103))
    	  pv.Acc_Val = pv.Acc - disc_accval_102;

      if(pv.user_input == 120)
          pv.Acc_Val = pv.Acc - disc_accval_120;

      if((pv.user_input >= 110) && (pv.user_input <= 111))
    	  pv.Acc_Val = pv.Acc - disc_accval_110;

      if((pv.user_input >= 154) && (pv.user_input <= 156))
          pv.Acc_Val = pv.Acc - disc_accval_154;

      if((pv.user_input == 160)||((pv.user_input >= 175) && (pv.user_input <= 176)))
    	  pv.Acc_Val = pv.Acc - disc_accval_160;

      if((pv.user_input >= 180) && (pv.user_input <= 240))
          pv.Acc_Val = pv.Acc - disc_accval_200;

      if(pv.user_input != 1)
      {
      pv.Dec = pv.Acc;
	  pv.time_counter = (pv.Acc + Cons_Speed);
	  pv.dec_speed = pv.time_counter;
      }
}

/**
 * @brief  Calculate the Axis motor speed.
 *
 * @param  None.
 *
 * @retval None.
 *
 */
void Axis_motorspeed_cal(void)
{
	if(pv.axis_calib == 0)
	{
		pv.step_data = (Rotation_1deg_Axis * pv.axis_user_input);
		pv.Acc = ((pv.step_data*axis_acc_per)/100);
		if((pv.axis_user_input >= 1) && (pv.axis_user_input < 5))
		{
			pv.time_counter = (pv.Acc + axis_initial_1);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
		else if((pv.axis_user_input >= 5) && (pv.axis_user_input <= 10))
		{
			pv.time_counter = (pv.Acc + axis_initial_5);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
		else if((pv.axis_user_input >= 11) && (pv.axis_user_input <= 45))
		{
			pv.time_counter = (pv.Acc + axis_initial_45);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
		else
		{
			pv.time_counter = (pv.Acc + axis_cons_speed);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
	}
	else if(pv.axis_calib == 1)
	{
		if(pv.cal_disc[6] > 0)
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_SET)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET); //Clockwise
			}
			pv.axis_user_input = pv.cal_disc[6];
		}
		else if (pv.cal_disc[6] < 0)
		{
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) != GPIO_PIN_RESET)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);// Anti-Clockwise
			}
			pv.axis_user_input = ((-1) * pv.cal_disc[6]);
		}
		pv.step_data = (Rotation_1deg_Axis * pv.axis_user_input);
		pv.Acc = ((pv.step_data*axis_acc_per)/100);
		if((pv.axis_user_input==1)||(pv.axis_user_input==2))
		{
			pv.time_counter = (pv.Acc + axis_initial_1);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
		else if((pv.axis_user_input==5)||(pv.axis_user_input==10))
		{
			pv.time_counter = (pv.Acc + axis_initial_5);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
		else if((pv.axis_user_input>=11)&&(pv.axis_user_input<=45))
		{
			pv.time_counter = (pv.Acc + axis_initial_45);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
		else
		{
			pv.time_counter = (pv.Acc + axis_cons_speed);
			pv.Dec = pv.Acc;
			pv.dec_speed = pv.time_counter;
		}
	}

}


/**
 * @brief  Calculate the PD motor speed.
 * @param  None.
 * @retval None.
 *
 */
void PD_motorspeed_cal(void)
{
	pv.Acc = ((pv.step_data*pv.acc_per)/100);
	pv.Dec = pv.Acc;
	pv.time_counter = (pv.Acc + PD_cons_speed);
	pv.dec_speed = pv.time_counter;
}

/**
 * @brief  Calculate the Prism motor speed.
 * @param  None.
 * @retval None.
 *
 */

void Prism_motorspeed_cal(void)
{

		pv.Acc = ((pv.step_data*pv.acc_per)/100);
		pv.Dec = pv.Acc;
		pv.time_counter = (pv.Acc + 3000);
		pv.dec_speed = pv.time_counter;
}

/*
 * @brief  PWM callback function for control the motor speed.
 * @param  Timer Handler.
 * @retval None
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*** Timer callback function 1second timer **/
	if (htim->Instance == TIM7)
	{
		if((pv.count == 4) && (pv.timer_flag == 0))     //4 seconds
		{
			pv.timer_flag = 1;
			pv.count = 0;

			/* Update the error status flag */
			pv.motor_error = MOTOR_ERROR;
			if (MOTOR1_RUNNING == pv.motor_flag)
				pv.motor_flag = MOTOR1_ERROR;
			else if (MOTOR2_RUNNING == pv.motor_flag)
				pv.motor_flag = MOTOR2_ERROR;
			else if (MOTOR3_RUNNING == pv.motor_flag)
				pv.motor_flag = MOTOR3_ERROR;
			else if (MOTOR4_RUNNING == pv.motor_flag)
				pv.motor_flag = MOTOR4_ERROR;
			else if (MOTOR5_RUNNING == pv.motor_flag)
				pv.motor_flag = MOTOR5_ERROR;
		}
		pv.count++;
	}
	/****************************DISC1 & DISC 3 & DISC 5 *************************/
	if((htim->Instance==TIM2) && (pv.homing == 1))
	{
		if(pv.step > pv.step_data)
		{
			pv.sensor_hit = 1;
			if(	1 == pv.disc_pwm )          //Disc1
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_3);
			}
			else if( 2 == pv.disc_pwm )     //Disc2
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4);
			}
			else if( 3 == pv.disc_pwm )     //Disc3
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
			}
			else if(4 == pv.disc_pwm)       //Disc5
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2);
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;

			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;

		}
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Sensor hit and Forward 40 degree ***/
			if(((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_RESET)
				  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_RESET)
				  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_RESET)
				  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_RESET)) && (pv.sensor_hit==0))  //Sensor Hit
			{
				pv.sensor_hit=1;
				pv.step_data = pv.cal_disc[disc_homing];
				pv.Acc = 0;
				if(pv.cal_disc[disc_homing] >= 2400)
				{
					pv.Dec = (pv.cal_disc[disc_homing] * 20)/100;
				}
				else if(pv.cal_disc[disc_homing] < 2000)
				{
					pv.Dec = 1000;
				}
				else
					pv.Dec = 800;
				pv.step = 0;
			}
			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{
			if(	1 == pv.disc_pwm )          //Disc1
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_3);
			}
			else if( 2 == pv.disc_pwm )     //Disc2
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4);
			}
			else if( 3 == pv.disc_pwm )     //Disc3
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
			}
			else if(4 == pv.disc_pwm)       //Disc5
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2);
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;

			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(1 == pv.disc_pwm)           //Disc 1
		{
			TIM2->ARR = pv.time_counter;
			TIM2->CCR3= (pv.time_counter/pv.duty_cycle);
		}
		else if(2 == pv.disc_pwm)      //Disc 2
		{
			TIM2->ARR = pv.time_counter;
			TIM2->CCR4= (pv.time_counter/pv.duty_cycle);
		}
		else if(3 == pv.disc_pwm)      //Disc 3
		{
			TIM2->ARR= pv.time_counter;
			TIM2->CCR1= (pv.time_counter/pv.duty_cycle);
		}
		else if(4 == pv.disc_pwm)       //Disc5
		{
			TIM2->ARR= pv.time_counter;
			TIM2->CCR2= (pv.time_counter/pv.duty_cycle);
		}
	}

	/***** DISC1, DISC 2, DISC 3 & DISC 5-Rotate based on UART input(40,80,120,160,240) *****/

	if((htim->Instance==TIM2) && (pv.homing == 2) )
	{
		if(pv.step <= pv.step_data)
		{
			pv.step++;
			/*** Accelaration ***/
			if(pv.step <= pv.Acc_Val)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}
			else if(pv.step <= pv.Acc)
			{
				pv.temp = pv.time_counter - pv.value;
				pv.time_counter = pv.temp;
				pv.value++;
			}
			else
			{
				pv.temp = 0;
				pv.value = 1;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
				{
					if(pv.step <= ((pv.step_data-pv.Dec)+pv.Acc_Val))
					{
							pv.time_counter = pv.time_counter + pv.dec_increase_val;
					}
					else if(pv.step <= pv.step_data)
					{
						pv.temp = pv.time_counter + pv.value;
						pv.time_counter = pv.temp;
						pv.value++;

					}
				}
			}
		}
		else
		{
			if(	1 == pv.disc_pwm )           //Disc1
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_3);
			}
			else if( 2 == pv.disc_pwm )      //Disc2
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4);
			}
			else if( 3 == pv.disc_pwm )     //Disc3
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
			}
			else if(4 == pv.disc_pwm)       //Disc5
			{
				HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_2);
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;

			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;

		}
		if(1 == pv.disc_pwm)           //Disc 1
		{
			TIM2->ARR = pv.time_counter;
			TIM2->CCR3= (pv.time_counter/pv.duty_cycle);
		}
		else if(2 == pv.disc_pwm)      //Disc 2
		{
			TIM2->ARR = pv.time_counter;
			TIM2->CCR4= (pv.time_counter/pv.duty_cycle);
		}
		else if(3 == pv.disc_pwm)      //Disc 3
		{
			TIM2->ARR= pv.time_counter;
			TIM2->CCR1= (pv.time_counter/pv.duty_cycle);
		}
		else if(4 == pv.disc_pwm)      //Disc5
		{
			TIM2->ARR= pv.time_counter;
			TIM2->CCR2= (pv.time_counter/pv.duty_cycle);
		}
	}

	/**************************** DISC 4 *************************/
	if((htim->Instance==TIM3) && (pv.homing == 1))
	{
		if(pv.step > pv.step_data)
		{
			pv.sensor_hit = 1;
			if(1 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);   //Disc4
			}
			else if(2 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);  //Prism1
			}
			else if(3 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);  //Axis
			}
			else if(4 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);  //Prism2
			}

			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;

			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Sensor hit and Forward 40 degree ***/
			if(((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_RESET)
			  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_RESET)
			  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_RESET)
			  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_RESET)) && (pv.sensor_hit==0))  //Sensor Hit
			{
				pv.sensor_hit=1;
				pv.step_data = pv.cal_disc[disc_homing];
				pv.Acc = 0;
				if(pv.cal_disc[disc_homing] >= 2400)
				{
					pv.Dec = (pv.cal_disc[disc_homing] * 20)/100;
				}
				else if(pv.cal_disc[disc_homing] < 2000)
				{
					pv.Dec = 1000;
				}
				else
					pv.Dec = 800;
				pv.step = 0;
			}

			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{
			if(1 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);   //Disc4
			}
			else if(2 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);  //Prism1
			}
			else if(3 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);  //Axis
			}
			else if(4 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);  //Prism2
			}

			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;

			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;

		}
		if(1 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR1= (pv.time_counter/pv.duty_cycle);
		}
		else if(2 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR2= (pv.time_counter/pv.duty_cycle);
		}
		else if(3 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR3= (pv.time_counter/pv.duty_cycle);
		}
		else if(4 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR4= (pv.time_counter/pv.duty_cycle);
		}
	}

	/********      Prism1 and Prism2 Movement Based on Commands    *******/
	if((htim->Instance==TIM3) && (pv.homing == 3))
	{
		if(pv.step <= pv.step_data)
		{
			pv.step++;
			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/

			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{
			if(2 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);  //Prism1
			}
			if(4 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);  //Prism2
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(2 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR2= (pv.time_counter/pv.duty_cycle);
		}
		if(4 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR4= (pv.time_counter/pv.duty_cycle);
		}

	}

	/********      Prism1 Homing Based on Sensor hit    *******/
	if((htim->Instance==TIM3) && (pv.homing == 4))
	{
		if(pv.step > pv.step_data)
		{
			pv.sensor_hit = 1;
			if(2 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);  //Prism1
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;

		}
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Sensor hit and Forward 40 degree ***/
			if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET)
					&& (pv.sensor_hit == 0))  //Sensor Hit
			{
				pv.sensor_hit = 1;
				pv.Acc=0;
				pv.Dec=0;
				pv.step=0;
				pv.step_data=0;
				pv.motor_status = 0;
				/* Update the initial motor status flags */
				pv.motor_error = MOTOR_SUCCESS;
				pv.motor_flag  = MOTOR_SUCCESS;

			}
			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{
			if(2 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);  //Prism1
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(2 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR2= (pv.time_counter/pv.duty_cycle);
		}
	}

	/********      Prism2 Homing Based on Sensor hit    *******/
	if((htim->Instance==TIM3) && (pv.homing == 5))
	{
		if(pv.step > pv.step_data)
		{
			pv.sensor_hit = 1;
			if(4 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);  //Prism2
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Sensor hit and Forward 40 degree ***/
			if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
					&& (pv.sensor_hit==0))  //Sensor Hit
			{
				pv.sensor_hit = 1;
				pv.Acc=0;
				pv.Dec=0;
				pv.step=0;
				pv.step_data=0;
				pv.motor_status = 0;
				/* Update the initial motor status flags */
				pv.motor_error = MOTOR_SUCCESS;
				pv.motor_flag  = MOTOR_SUCCESS;

			}
			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{

			if(4 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);  //Prism2
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(4 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR4= (pv.time_counter/pv.duty_cycle);
		}
	}

	/***** DISC 4 - Rotate based on UART input(40,80,120,160,240) *****/

	if((htim->Instance==TIM3) && (pv.homing == 2) )
	{
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Accelaration ***/
			if(pv.step <= pv.Acc_Val)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}
			else if(pv.step <= pv.Acc)
			{
				pv.temp = pv.time_counter - pv.value;
				pv.time_counter = pv.temp;
				pv.value++;
			}
			else
			{
				pv.temp = 0;
				pv.value = 1;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
				{
					if(pv.step < ((pv.step_data-pv.Dec)+pv.Acc_Val))
					{
						pv.time_counter = pv.time_counter + pv.dec_increase_val;
					}
					else if(pv.step <= pv.step_data)
					{
						pv.temp = pv.time_counter + pv.value;
						pv.time_counter = pv.temp;
						pv.value++;

					}
				}
			}
		}
		else
		{
			if(1 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);   //Disc4
			}
			else if(2 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);  //Prism1
			}
			else if(3 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);  //Axis
			}
			else if(4 == pv.tim3_motor)
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);  //Prism2
			}
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		if(1 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR1= (pv.time_counter/pv.duty_cycle);
		}
		else if(2 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR2= (pv.time_counter/pv.duty_cycle);
		}
		else if(3 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR3= (pv.time_counter/pv.duty_cycle);
		}
		else if(4 == pv.tim3_motor)
		{
			TIM3->ARR= pv.time_counter;
			TIM3->CCR4= (pv.time_counter/pv.duty_cycle);
		}
	}

	/**************************** DISC 6 *************************/
	if((htim->Instance==TIM8) && (pv.homing == 1))
	{
		if(pv.step > pv.step_data)
		{
			pv.sensor_hit = 1;
			HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_1);
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;

		}
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Sensor hit and Forward 40 degree ***/
			if(((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_RESET)
			  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_RESET)
			  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_RESET)
			  ||(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_RESET)) && (pv.sensor_hit==0))  //Sensor Hit
			{
				pv.sensor_hit=1;
				pv.step_data = pv.cal_disc[disc_homing];
				pv.Acc = 0;
				if(pv.cal_disc[disc_homing] >= 2400)
				{
					pv.Dec = (pv.cal_disc[disc_homing] * 20)/100;
				}
				else if(pv.cal_disc[disc_homing] < 2000)
				{
					pv.Dec = 1000;
				}
				else
					pv.Dec = 800;
				pv.step = 0;
			}

			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{
			HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_1);
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		TIM8->ARR= pv.time_counter;
		TIM8->CCR1= (pv.time_counter/pv.duty_cycle);
	}

	/***** DISC 6 - Rotate based on UART input(40,80,120,160,240) *****/

	if((htim->Instance==TIM8) && (pv.homing == 2))
	{
		if(pv.step <= pv.step_data)
		{
			pv.step++;

			/*** Accelaration ***/
			if(pv.step <= pv.Acc_Val)
			{
				pv.time_counter = pv.time_counter-pv.acc_decrease_val;
			}
			else if(pv.step <= pv.Acc)
			{
				pv.temp = pv.time_counter - pv.value;
				pv.time_counter = pv.temp;
				pv.value++;
			}
			else
			{
				pv.temp = 0;
				pv.value = 1;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
				{
					if(pv.step <= ((pv.step_data-pv.Dec)+pv.Acc_Val))
					{
						pv.time_counter = pv.time_counter + pv.dec_increase_val;
					}
					else if(pv.step <= pv.step_data)
					{
						pv.temp = pv.time_counter + pv.value;
						pv.time_counter = pv.temp;
						pv.value++;

					}
				}
			}
		}
		else
		{
			HAL_TIM_PWM_Stop_IT(&htim8, TIM_CHANNEL_1);
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		TIM8->ARR= pv.time_counter;
		TIM8->CCR1= (pv.time_counter/pv.duty_cycle);
	}

	/************************Axis Motor **************************/
	if((htim->Instance==TIM3) && (pv.axis_motor == 1))
	{
		if(pv.step > pv.step_data)
		{
			pv.sensor_hit = 1;
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.flag_360 = 0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;

		}
		if(pv.step <= pv.step_data)
		{
			pv.step++;
	#if 1
			if((HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5) == GPIO_PIN_RESET)
					&& ((pv.sensor_hit==0)))
			{
				pv.sensor_hit=1;
				pv.step_data = 0;
				pv.Acc = 0;
				pv.Dec = 0;
				pv.step = 0;
			}
	#endif
			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter-pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}

		}
		else
		{
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.flag_360 = 0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		TIM3->ARR= pv.time_counter;
		TIM3->CCR3= (pv.time_counter/pv.duty_cycle);
	}

	/******************** PD motor **************************************/
	if((htim->Instance==TIM4))
	{
		if((pv.step <= pv.step_data) && (pv.PD_flag == 0))
		{
			pv.step++;
			/*** Accelaration ***/
			if(pv.step < pv.Acc)
			{
				pv.time_counter = pv.time_counter - pv.acc_decrease_val;
			}

			/*** Decelaration ***/
			if(pv.step >= (pv.step_data - pv.Dec))
			{
				if(pv.time_counter < pv.dec_speed)
					pv.time_counter = pv.time_counter + pv.dec_increase_val;
			}
		}
		else
		{
			HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_1);
			pv.Acc=0;
			pv.Dec=0;
			pv.step=0;
			pv.step_data=0;
			pv.flag_360 = 0;
			pv.motor_status = 0;
			/* Update the initial motor status flags */
			pv.motor_error = MOTOR_SUCCESS;
			pv.motor_flag  = MOTOR_SUCCESS;
		}
		TIM4->ARR= pv.time_counter;
		TIM4->CCR1= (pv.time_counter/pv.duty_cycle);
	}
}
