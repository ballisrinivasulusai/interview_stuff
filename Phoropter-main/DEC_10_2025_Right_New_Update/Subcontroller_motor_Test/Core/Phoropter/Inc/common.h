/*
 * common.h
 *
 *  Created on:
 *      Author:
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "process_cmd.h"
#include <math.h>
#include "../Inc/FLASH_SECTOR_H7.h"

/**** Predefine variables *****/
#define FULL_ROTATION    360
#define INIT_FREQ        2500
#define disc_acc_per     10
#define axis_acc_per     5

#define disc_accval_1     60
#define disc_accval_10    50
#define disc_accval_40    40
#define disc_accval_102   36
#define disc_accval_110   35
#define disc_accval_120	  33
#define disc_accval_154   26
#define disc_accval_160   24
#define disc_accval_200   16

#define FAILURE 0x00
#define SUCCESS 0x01

#define MOTOR_HIGH		  1
#define MOTOR_LOW		  2
#define FALSE 0
#define TRUE  1

/*!
 * Receive buffer size
 */
#define RX_BUFFER_SIZE     200

/*!
 * Carriage return ASCII value
 */
#define CARRIAGE_RETURN             0x0D

/*!
 * New Line ASCII value
 */
#define NEW_LINE                    0x0A

/*!
 * Flash Configuration address
 */
#define FLASH_CONF_ADDR                      0x08100000

/*!
 * Buffer size
 */
#define BUFFER1_SIZE     25
#define BUFFER2_SIZE     100

/*!
 * Buffer size
 */
#define FLASH_MAGIC_NUMBER  0x12345679

/**** Function Declarations ****/
void HAL_UART4_RxCpltCallback(UART_HandleTypeDef *huart);
void Disc1_homing_position(void);
void Disc2_homing_position(void);
void Disc3_homing_position(void);
void Disc4_homing_position(void);
void Disc5_homing_position(void);
void Disc6_homing_position(void);
void Axis_motor_homing(void);
void PD_motor(void);
void Prism1_motor(void);
void Prism2_motor(void);
void DC_motor_In(void);
void DC_motor_Out(void);


void Disc_motorspeed_cal(void);
void Axis_motorspeed_cal(void);
void PD_motorspeed_cal(void);
void Prism_motorspeed_cal(void);

uint8_t Disc_Motor_Rotation(double input,uint8_t disc_input);
void rotate_disc1(double input);
void rotate_disc2(double input);
void rotate_disc3(double input);
void rotate_disc4(double input);
void rotate_disc5(double input);
void rotate_disc6(double input);
void Axis_motor_rotation(int16_t  input);
void Axis_motor_rotation_calibration(int16_t input);
void PD_motor_rotation(char *input);
void check_disc_status();
void PD1_motor(int8_t PD1_input);
void Prism1_Motor_Rotation(int16_t input);
void Prism2_Motor_Rotation(int16_t input);
void prism_motor_rotation(int16_t  input);
void axis_xcyl(int16_t input);
void Prism_XY(int16_t h_input,int16_t v_input);
void Prism_Polar(int16_t input);
void prism_function(uint8_t input);


uint8_t sph_cmd(uint8_t *cbuff, uint16_t len);
uint8_t cyl_cmd(uint8_t *cbuff, uint16_t len);
uint8_t axis_cmd(uint8_t *cbuff, uint16_t len);
uint8_t PD_cmd(uint8_t *cbuff, uint16_t len);
uint8_t spcl_cmd(uint8_t *cbuff, uint16_t len);
uint8_t clear_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Disc_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Share_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Save_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Prism_hor_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Prism_ver_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Prism_polar_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Prism_polar2_cmd(uint8_t *cbuff, uint16_t len);
uint8_t Dc_cmd(uint8_t *cbuff, uint16_t len);
uint8_t xcyl_cmd(uint8_t *cbuff, uint16_t len);


void dump_packet(uint8_t *buf, uint16_t size);

/**** Variable Declarations ****/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim7;
extern double user_input;
extern uint8_t ack_data[];
extern uint8_t motor_error_flag;

extern uint8_t motor_flag;
extern uint8_t error_flag;
extern uint8_t motor_error;
extern uint8_t disc_error;
extern void Init_PV();


/*!
 *
 * Motor Error Status
 *
 */
typedef enum{
	MOTOR_SUCCESS = 0,
	MOTOR1_ERROR,
	MOTOR2_ERROR,
	MOTOR3_ERROR,
	MOTOR4_ERROR,
	MOTOR5_ERROR,
	MOTOR_BUSY,
	MOTOR_IDLE,
	MOTOR_STATUS_RUNNING,
	MOTOR_STATUS_STOP,
	MOTOR1_RUNNING,
	MOTOR2_RUNNING,
	MOTOR3_RUNNING,
	MOTOR4_RUNNING,
	MOTOR5_RUNNING,
	MOTOR6_RUNNING,
	MOTOR7_RUNNING,
	STATUS_TIMER_RUNNING,
	STATUS_TIMER_STOP,
	MOTOR_ERROR,
}motor_err;


/*!
 *
 * Menu List
 *
 */
typedef struct __menulist{
  uint8_t cmd;
  uint8_t    (*cb)(uint8_t *cbuff, uint16_t len);
}menulist;
/*!
 *
 * Holds the Disc pin details
 */
typedef struct __discpin__
{
	GPIO_TypeDef *senport;
	uint16_t     senpin;
    GPIO_TypeDef *enport;
    uint16_t     enpin;
    GPIO_TypeDef *detport;
    uint16_t     detpin;
    GPIO_TypeDef *stepinport;
    uint16_t     stepinpin;
}dpin;

/*!
 *
 * Sub Cmd List
 *
 */
typedef struct __submenulist{
  void    (*cb)();
  void    (*rescb)();
}submenulist;



/*!
 *
 * Disc rotation
 *
 */
typedef struct discRotation
{
	double rcv_buff;
	double temp_buff;
	double input_val;
	uint8_t rcv_input;
}discrot;

/*!
 *
 * Private Variables
 *
 */
typedef struct __private_var
{
    UART_HandleTypeDef *huart4;         /* Main Board uart               */

	uint8_t   motor_error_flag;
	uint8_t   rx_buf[RX_BUFFER_SIZE];
	uint8_t   temp_buf[RX_BUFFER_SIZE];
    uint8_t   tcmd_rx_buf[RX_BUFFER_SIZE];
	uint8_t   g_rvd_data_size;
	uint8_t   g_isrvd_cmd;
	uint8_t   motor_flag;
	uint8_t   error_flag;
	uint8_t   PD_Homing;
	uint8_t   ack_data[BUFFER2_SIZE];
    uint8_t   disc1_status;
    uint8_t   disc2_status;
    uint8_t   disc3_status;
    uint8_t   disc4_status;
    uint8_t   disc5_status;
    uint8_t   disc6_status;
    uint8_t   axis_status;
    uint8_t   prism1_status;
    uint8_t   prism2_status;
    uint8_t   pd_status;
    uint8_t   dc_status;
    uint8_t   disc1_check;
    uint8_t   disc2_check;
    uint8_t   disc3_check;
    uint8_t   disc4_check;
    uint8_t   disc5_check;
    uint8_t   disc6_check;
    uint8_t   share_disc;
    uint8_t   timer_flag;
	uint8_t   disc_pwm;
	uint8_t   tim3_motor;
	uint8_t   axis_motor;
	uint8_t   count;
	uint8_t   acc_per;
	uint8_t   PD_flag;
	uint8_t   pd_flag1;
	uint8_t   dc_int1;
	uint8_t   dc_int2;
	uint8_t   acc_decrease_val;
	uint8_t   dec_increase_val;
	uint8_t   duty_cycle;
	uint8_t   sensor_hit;
	uint8_t   motor_status;
	uint8_t   homing;
	uint8_t   flag_360;
    uint8_t   ioexp_int;
	uint8_t   motor_error;
	uint8_t   disc_error;
    uint8_t   axis_calib;
    uint8_t   aixs_xcyl_temp;
    uint8_t   prism_select;
    uint8_t   prism_y;
    uint8_t   prism_x;
    uint8_t   BB_on;
    uint8_t   prism;
    uint8_t   polar_func;

    int8_t    PD1_rotate_input;
    int8_t    PD1_current_val;
    int8_t    PD1_temp_old;

    uint16_t  axis_temp;
	uint16_t  Acc;
	uint16_t  Dec;
	uint16_t  Acc_Val;
	uint16_t  axis_user_input;
	uint16_t  write_ptr;
	uint16_t  read_ptr;
    uint16_t  disc_ack;
    uint16_t  cmd_status;

    int16_t   axis_rotate_input;
    int16_t   axis_xcyl;
    int16_t   prism_input;
    int16_t   prism_ver_input;
    int16_t   prism_hor_input;
    int16_t   prism_polar_temp;
    int16_t   prism_polar_input;
    int16_t   ver_temp;
    int16_t   hor_temp;

	int32_t   cal_disc[12];

	uint32_t  step_data;
	uint32_t  step;
	uint32_t  time_counter;

	uint32_t  dec_speed;
	uint32_t  value;
	uint32_t  temp;

    double    user_input;
	double    cyl_rotate_input;
	double    spcl_rotate_input;
	double    rotate_input;
	double    rotate1_input;
	double    rotate2_input;
	double    rotate3_input;
	double    x_temp;
	double    y_temp;

	I2C_HandleTypeDef *hi2c1;
    I2C_HandleTypeDef *hi2c;

    discrot D1;
	discrot D2;
	discrot D3;
	discrot D4;
	discrot D5;
	discrot D6;
	discrot axis;

}private_var;

typedef struct flash_variable
{
	uint32_t  cal_disc1[10];
}Flash_var;

#endif /* SRC_COMMON_H_ */
