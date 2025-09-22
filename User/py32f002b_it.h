/**
  ******************************************************************************
  * @file    py32f002b_it.h
  * @author  MCU Application Team
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PY32F002B_IT_H
#define __PY32F002B_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Segment defines------------------------------------------------------------*/
#define _A  0x40     //; bitmask for segment A , etc ..
#define _B  0x20
#define _C  0x10
#define _D  0x08
#define _E  0x04
#define _F  0x02
#define _G  0x01
#define _DP 0x80

// Display Pinout Defines...

#define Display_P1	1<<2	// Pin 1 connected to PB2
#define Display_P2	1<<3	// Pin 2 connected to PB3
#define Display_P3	1<<4	// Pin 3 connected to PB4
#define Display_P4	1<<1	// Pin 4 connected to PB1
#define Display_P5	1<<0	// Pin 5 connected to PB0
#define Display_P6	1<<0	// Pin 6 connected to PA0

#define Beeper		LL_GPIO_PIN_1	// PA1

#define Relay_Out		LL_GPIO_PIN_6 //PB6

#define The10seconds 20000

#define TheOneHour	360*The10seconds

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

extern __IO int32_t Tick_Timer, Action_Timer_On, Action_Delay_On, Action_Timer_Off, Action_Delay_Off, Timeout, Multiplex, Tick_Flag, Roll_Timer, Roll_Flag, Flashing_Flag, Beeper_Flag;
extern __IO int32_t Analog_Read, Analog_Flag, ADC_Ready, D1Flash, D2Flash, D3Flash, System_Ready;
extern __IO uint32_t Turn_On_Temp, Turn_Off_Temp; 
extern __IO unsigned char Disp_Chars[3];
extern __IO uint32_t Pressed_BSet, Pressed_BMinus, Pressed_BPlus, Cnt_BSet, Cnt_BMinus, Cnt_BPlus, Anykey;
extern __IO uint32_t Alarm_Flag, Calibration_Flag;

extern __IO __attribute__((aligned (4))) struct Params2 {
	uint32_t Function:2;
	uint32_t Temp_Roll:1;
	uint32_t Alarm1:1;
	uint32_t Alarm2:1;
	uint32_t fahrenheit:1;
	uint32_t Messages:1;
	uint32_t Delay_Type:1;
	uint32_t Delay_Ton:1;
	uint32_t Delay_Toff:1;
	uint32_t Timer_Enable:1;
	uint32_t Sensor_State:2;
	uint32_t Language:1;
	uint32_t Sound:1;
	uint32_t Fixed_On:1;
	uint32_t Fixed_Off:1;
	uint32_t :15;
	uint32_t Target;
	uint32_t Hysteresis;
	uint32_t Temp_Offset;
	uint32_t Delay_On;
	uint32_t Delay_Off;
	uint32_t Max_TempK100;
	uint32_t Min_TempK100;
	uint32_t Alarm1_TempK100;
	uint32_t Alarm2_TempK100;
	uint32_t Filter_Depth;
	int32_t  ADC_Offset;
	uint32_t TM_Days:12;
	uint32_t TM_Hours:5;
	uint32_t TM_Minutes:6;
	uint32_t TM_Seconds:6;
	uint32_t Bits_Extended:3;
	uint32_t Checksum;
} W1209PY;

extern __IO __attribute__((aligned (4))) struct Params {
	uint32_t R_Pullup;
	uint32_t Value;
	int32_t Log_Value;
	uint32_t R0;
	int32_t Log_R0;
	uint32_t R1;
	int32_t Log_R1;
	uint32_t T0K;
	uint32_t T1K;
	int64_t Beta;
	int64_t Beta_T0K;
	uint32_t Temp;
	uint32_t R_Wires;
} NTC;

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __PY32F002B_IT_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
