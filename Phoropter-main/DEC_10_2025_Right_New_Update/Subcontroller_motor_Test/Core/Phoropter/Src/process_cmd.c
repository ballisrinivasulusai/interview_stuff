/*
 * process_cmd.c
 *
 *  Created on:
 *      Author:
 */
#include "../Inc/common.h"
#include "../Inc/cmdlist.h"
#include"../Inc/stm32h7xx_hal.h"


/* Global Variable Declaration */
extern private_var pv;



/** @brief Process the command
 *
 *  This function processes the received command
 *
 *  @param  cbuff[OUT]  : Receive data buffer
 *          len         : Buffer len
 *
 *  @return none
 *
 */
void process_cmd(uint8_t *cbuff, uint16_t len)
{
	uint16_t io_data[2];
    uint8_t index;
    uint8_t Ack_Status;
    for (index=0; index < (sizeof(ml) / sizeof(menulist)); index++)
    {

        /* Compare the command value */
        if (ml[index].cmd != cbuff[0])
            continue;

        /* Call the callback function */
        if (NULL ==  ml[index].cb)
          break;

        pv.disc1_status  = MOTOR_HIGH;
        pv.disc2_status  = MOTOR_HIGH;
        pv.disc3_status  = MOTOR_HIGH;
        pv.disc4_status  = MOTOR_HIGH;
        pv.disc5_status  = MOTOR_HIGH;
        pv.disc6_status  = MOTOR_HIGH;
        pv.axis_status   = MOTOR_HIGH;
        pv.prism1_status = MOTOR_HIGH;
        pv.prism2_status = MOTOR_HIGH;
        pv.pd_status     = MOTOR_HIGH;
        pv.dc_status     = MOTOR_HIGH;

        pca9535_read(pv.hi2c1, 0x00, &io_data ,2);

        /* checking motor status */
        if( TRUE != (io_data[0] & 1))
        {
        	pv.disc1_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>1) & 1))
        {
        	pv.disc2_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>2) & 1))
        {
        	pv.disc3_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>3) & 1))
        {
        	pv.disc4_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>4) & 1))
        {
        	pv.disc5_status = MOTOR_LOW;
        }

        if(TRUE != ((io_data[0]>>5) & 1))
        {
        	pv.disc6_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>7) & 1))
        {
        	pv.axis_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>8) & 1))
        {
        	pv.pd_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>9) & 1))
        {
        	pv.prism1_status = MOTOR_LOW;
        }

        if( TRUE != ((io_data[0]>>10) & 1))
        {
        	pv.prism2_status = MOTOR_LOW;
        }


        if( TRUE != ((io_data[0]>>11) & 1))
        {
        	pv.dc_status = MOTOR_LOW;
        }


        pv.disc1_check = 1;
       	pv.disc2_check = 1;
       	pv.disc3_check = 1;
       	pv.disc4_check = 1;
       	pv.disc5_check = 1;
       	pv.disc6_check = 1;


        if(cbuff[0] != 0x06)
        {
        	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
        	{
        		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        		pv.rotate_input = 360;
        		Disc_Motor_Rotation(pv.rotate_input,6);
        	}
        }
        if((cbuff[0] != 0x04) && (cbuff[0] != 0x10) &&
        		(cbuff[0] != 0x12) && (cbuff[0] != 0x14) &&
				(cbuff[0] != 0x15)&& (cbuff[0] != 0x05))
        {
        	if(pv.BB_on == 0)
        	{
        		Dc_SubFun2();
        	}
        }

        Ack_Status = ml[index].cb(cbuff, len);

        if(FAILURE == Ack_Status)
        {
        	cbuff[len] = 0x01;
        }
        else
        {
        	cbuff[len] = 0x00;
        }

        break;
    }
    pv.disc1_status  = MOTOR_HIGH;
    pv.disc2_status  = MOTOR_HIGH;
    pv.disc3_status  = MOTOR_HIGH;
    pv.disc4_status  = MOTOR_HIGH;
    pv.disc5_status  = MOTOR_HIGH;
    pv.disc6_status  = MOTOR_HIGH;
    pv.axis_status   = MOTOR_HIGH;
    pv.prism1_status = MOTOR_HIGH;
    pv.prism2_status = MOTOR_HIGH;
    pv.pd_status     = MOTOR_HIGH;
    pv.dc_status     = MOTOR_HIGH;

}
