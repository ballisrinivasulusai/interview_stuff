################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Phoropter/Src/Common.c \
../Core/Phoropter/Src/Dc_cmd.c \
../Core/Phoropter/Src/Disc_cal.c \
../Core/Phoropter/Src/IO_Expander.c \
../Core/Phoropter/Src/PD_cmd.c \
../Core/Phoropter/Src/Prism_hor_cmd.c \
../Core/Phoropter/Src/Prism_polar2_cmd.c \
../Core/Phoropter/Src/Prism_polar_cmd.c \
../Core/Phoropter/Src/Prism_ver_cmd.c \
../Core/Phoropter/Src/Save_Flash_Values.c \
../Core/Phoropter/Src/Share_disc_homing.c \
../Core/Phoropter/Src/Spcl_cmd.c \
../Core/Phoropter/Src/axis_cmd.c \
../Core/Phoropter/Src/clear.c \
../Core/Phoropter/Src/cyl_cmd.c \
../Core/Phoropter/Src/process_cmd.c \
../Core/Phoropter/Src/spr_cmd.c \
../Core/Phoropter/Src/xcyl.c 

OBJS += \
./Core/Phoropter/Src/Common.o \
./Core/Phoropter/Src/Dc_cmd.o \
./Core/Phoropter/Src/Disc_cal.o \
./Core/Phoropter/Src/IO_Expander.o \
./Core/Phoropter/Src/PD_cmd.o \
./Core/Phoropter/Src/Prism_hor_cmd.o \
./Core/Phoropter/Src/Prism_polar2_cmd.o \
./Core/Phoropter/Src/Prism_polar_cmd.o \
./Core/Phoropter/Src/Prism_ver_cmd.o \
./Core/Phoropter/Src/Save_Flash_Values.o \
./Core/Phoropter/Src/Share_disc_homing.o \
./Core/Phoropter/Src/Spcl_cmd.o \
./Core/Phoropter/Src/axis_cmd.o \
./Core/Phoropter/Src/clear.o \
./Core/Phoropter/Src/cyl_cmd.o \
./Core/Phoropter/Src/process_cmd.o \
./Core/Phoropter/Src/spr_cmd.o \
./Core/Phoropter/Src/xcyl.o 

C_DEPS += \
./Core/Phoropter/Src/Common.d \
./Core/Phoropter/Src/Dc_cmd.d \
./Core/Phoropter/Src/Disc_cal.d \
./Core/Phoropter/Src/IO_Expander.d \
./Core/Phoropter/Src/PD_cmd.d \
./Core/Phoropter/Src/Prism_hor_cmd.d \
./Core/Phoropter/Src/Prism_polar2_cmd.d \
./Core/Phoropter/Src/Prism_polar_cmd.d \
./Core/Phoropter/Src/Prism_ver_cmd.d \
./Core/Phoropter/Src/Save_Flash_Values.d \
./Core/Phoropter/Src/Share_disc_homing.d \
./Core/Phoropter/Src/Spcl_cmd.d \
./Core/Phoropter/Src/axis_cmd.d \
./Core/Phoropter/Src/clear.d \
./Core/Phoropter/Src/cyl_cmd.d \
./Core/Phoropter/Src/process_cmd.d \
./Core/Phoropter/Src/spr_cmd.d \
./Core/Phoropter/Src/xcyl.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Phoropter/Src/%.o Core/Phoropter/Src/%.su Core/Phoropter/Src/%.cyclo: ../Core/Phoropter/Src/%.c Core/Phoropter/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H742xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Phoropter-2f-Src

clean-Core-2f-Phoropter-2f-Src:
	-$(RM) ./Core/Phoropter/Src/Common.cyclo ./Core/Phoropter/Src/Common.d ./Core/Phoropter/Src/Common.o ./Core/Phoropter/Src/Common.su ./Core/Phoropter/Src/Dc_cmd.cyclo ./Core/Phoropter/Src/Dc_cmd.d ./Core/Phoropter/Src/Dc_cmd.o ./Core/Phoropter/Src/Dc_cmd.su ./Core/Phoropter/Src/Disc_cal.cyclo ./Core/Phoropter/Src/Disc_cal.d ./Core/Phoropter/Src/Disc_cal.o ./Core/Phoropter/Src/Disc_cal.su ./Core/Phoropter/Src/IO_Expander.cyclo ./Core/Phoropter/Src/IO_Expander.d ./Core/Phoropter/Src/IO_Expander.o ./Core/Phoropter/Src/IO_Expander.su ./Core/Phoropter/Src/PD_cmd.cyclo ./Core/Phoropter/Src/PD_cmd.d ./Core/Phoropter/Src/PD_cmd.o ./Core/Phoropter/Src/PD_cmd.su ./Core/Phoropter/Src/Prism_hor_cmd.cyclo ./Core/Phoropter/Src/Prism_hor_cmd.d ./Core/Phoropter/Src/Prism_hor_cmd.o ./Core/Phoropter/Src/Prism_hor_cmd.su ./Core/Phoropter/Src/Prism_polar2_cmd.cyclo ./Core/Phoropter/Src/Prism_polar2_cmd.d ./Core/Phoropter/Src/Prism_polar2_cmd.o ./Core/Phoropter/Src/Prism_polar2_cmd.su ./Core/Phoropter/Src/Prism_polar_cmd.cyclo ./Core/Phoropter/Src/Prism_polar_cmd.d ./Core/Phoropter/Src/Prism_polar_cmd.o ./Core/Phoropter/Src/Prism_polar_cmd.su ./Core/Phoropter/Src/Prism_ver_cmd.cyclo ./Core/Phoropter/Src/Prism_ver_cmd.d ./Core/Phoropter/Src/Prism_ver_cmd.o ./Core/Phoropter/Src/Prism_ver_cmd.su ./Core/Phoropter/Src/Save_Flash_Values.cyclo ./Core/Phoropter/Src/Save_Flash_Values.d ./Core/Phoropter/Src/Save_Flash_Values.o ./Core/Phoropter/Src/Save_Flash_Values.su ./Core/Phoropter/Src/Share_disc_homing.cyclo ./Core/Phoropter/Src/Share_disc_homing.d ./Core/Phoropter/Src/Share_disc_homing.o ./Core/Phoropter/Src/Share_disc_homing.su ./Core/Phoropter/Src/Spcl_cmd.cyclo ./Core/Phoropter/Src/Spcl_cmd.d ./Core/Phoropter/Src/Spcl_cmd.o ./Core/Phoropter/Src/Spcl_cmd.su ./Core/Phoropter/Src/axis_cmd.cyclo ./Core/Phoropter/Src/axis_cmd.d ./Core/Phoropter/Src/axis_cmd.o ./Core/Phoropter/Src/axis_cmd.su ./Core/Phoropter/Src/clear.cyclo ./Core/Phoropter/Src/clear.d ./Core/Phoropter/Src/clear.o ./Core/Phoropter/Src/clear.su ./Core/Phoropter/Src/cyl_cmd.cyclo ./Core/Phoropter/Src/cyl_cmd.d ./Core/Phoropter/Src/cyl_cmd.o ./Core/Phoropter/Src/cyl_cmd.su ./Core/Phoropter/Src/process_cmd.cyclo ./Core/Phoropter/Src/process_cmd.d ./Core/Phoropter/Src/process_cmd.o ./Core/Phoropter/Src/process_cmd.su ./Core/Phoropter/Src/spr_cmd.cyclo ./Core/Phoropter/Src/spr_cmd.d ./Core/Phoropter/Src/spr_cmd.o ./Core/Phoropter/Src/spr_cmd.su ./Core/Phoropter/Src/xcyl.cyclo ./Core/Phoropter/Src/xcyl.d ./Core/Phoropter/Src/xcyl.o ./Core/Phoropter/Src/xcyl.su

.PHONY: clean-Core-2f-Phoropter-2f-Src

