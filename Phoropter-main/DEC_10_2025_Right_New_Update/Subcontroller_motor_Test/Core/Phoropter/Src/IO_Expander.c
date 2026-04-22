/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../Inc/common.h"
#include "stm32h7xx_hal.h"

//#include "../Phoropter/Inc/common.h"
#define IO_EXP_ADDRESS = 0x40
#define IO_EXP_ADDRESS1 = 0x48
extern private_var pv;

HAL_StatusTypeDef pca9535_read(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t *pData, uint16_t len)
{
	HAL_StatusTypeDef status;

	/* Write the IO Expander IO Register */
	status = HAL_I2C_Master_Transmit(hi2c, 0x40, &addr, 1, 1000);

	if (HAL_OK != status)
		return status;

	/* Read the data */
	status = HAL_I2C_Master_Receive(hi2c, 0x40, pData, len, 1000);

	return status;
}

HAL_StatusTypeDef pca9535_write(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t len)
{

	HAL_StatusTypeDef status;

	status = HAL_I2C_Master_Transmit(hi2c, 0x40, pData, len, 1000);

	return status;
}

/** @brief Initialize the IO Expander
 *
 *  This function intialize the IO Expander software Resource
 *
 *  @param  None
 *
 *
 *  @return 0 : SUCCESS
 *          1 : FAILURE
 */
int8_t Init_IOExpander(I2C_HandleTypeDef *hi2c)
{

	HAL_StatusTypeDef status1;
#if 0
	HAL_StatusTypeDef status2;
	int16_t            Values = 0;
    int32_t            z1test = 0;
    int8_t             tdata = 0x00;
#endif
    int8_t             tdata1[6];
	while(1)
	{

		//if (pv.dc_int1 == 1)
		{
			status1 = pca9535_read(hi2c, 0x00, &tdata1, 2);
			pv.dc_int1 = 0;

		}

#if 0
		status1 = pca9535_read(hi2c, 0x06, &tdata1, 2);
		z1test++;
		memset(&tdata1, 0x00, sizeof(tdata1));
		tdata1[0] = 0x06;
		tdata1[1] = 0xAB;
		status2 = pca9535_write(hi2c, &tdata1, 2);
		z1test++;
		memset(&tdata1, 0x00, sizeof(tdata1));
#endif
#if 0
		memset(&tdata1[0], 0x00, sizeof(tdata1));
		status1 = HAL_I2C_Master_Transmit(hi2c, 0x40, &tdata, 1, 1000);
		z1test++;
		status2 = HAL_I2C_Master_Receive(hi2c, 0x40, &tdata1[0], 2, 1000);
		z1test++;



		status1 = HAL_I2C_Mem_Write_IT(hi2c,  (uint16_t)0x40, 0x02, 1, NULL, 0);

		z1test++;
		status2 = HAL_I2C_Mem_Read_IT(hi2c,  (uint16_t)0x40, (uint16_t) 0x00, 0, (unsigned char*)&Values, 2);
		z1test++;
#endif
	}
	return status1;
}

