/**
  ******************************************************************************
  * @file    main.h
  * @author  MCU Application Team
  * @brief   Header for main.c file.
  *          This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f002b_ll_rcc.h"
#include "py32f002b_ll_bus.h"
#include "py32f002b_ll_system.h"
#include "py32f002b_ll_cortex.h"
#include "py32f002b_ll_utils.h"
#include "py32f002b_ll_pwr.h"
#include "py32f002b_ll_gpio.h"
#include "py32f002b_ll_flash.h"
#include "py32f002b_ll_adc.h"
#include "py32f002b_ll_crc.h"


#if defined(USE_FULL_ASSERT)
#include "py32_assert.h"
#endif /* USE_FULL_ASSERT */

#define LOG_Q31_INVLOG2EXP 0x58B90BFCUL
#define LOG_Q30_INVLOG2EXP 0x55F3439CUL

#define strcpy(a,b);	for(int _ind=0;b[_ind];_ind++) {a[_ind]=b[_ind];a[_ind+1]=0;}
				
#define Wait_For_Tick();  for(Tick_Flag=0;!Tick_Flag;);
#define MsDelay(a); 	for(int32_t _ind=Tick_Timer;Tick_Timer<=(_ind+(a<<1)+1););

#define ADC_to_NTC(a)  (uint32_t)((NTC.R_Pullup*a)/((4096U<<(uint32_t)W1209PY.Bits_Extended)-a)-NTC.R_Wires)
//#define Log_ADC_to_NTC(AR) (log_q31(AR)+log_q31(NTC.R_Pullup)-log_q31((4096U<<(uint32_t)W1209PY.Bits_Extended)-AR))
#define Log_ADC_to_NTC(AR) (log_q31(AR*NTC.R_Pullup-NTC.R_Wires*((4096U<<(uint32_t)W1209PY.Bits_Extended)-AR))-LOG_Q30_INVLOG2EXP-log_q31((4096U<<(uint32_t)W1209PY.Bits_Extended)-AR))
//#define Log_ADC_to_NTC(AR) (log_q31(ADC_to_NTC(AR)))
#define Tout() (Timeout>=The10seconds)

#define To_Celsius(a) (int32_t)(a-_100Kelvin)

#define To_fahrenheit(a) (((int32_t)18*To_Celsius(a)/10)+3200)

#define From_Celsius(a) (a+_100Kelvin)

#define From_fahrenheit(a) From_Celsius(((a-3200)*10)/18)

#define Max(a,b)	((a>=b)?a:b)
#define Min(a,b)	((a<=b)?a:b)

#define Put_Temp_On_Buffer 1
#define Dont_Put_Temp_On_Buffer 0

#define Set_Checksum	1
#define Get_Checksum	0

#define PROFILE_EMPTY	 3
#define PROFILE_INACTIVE 2
#define PROFILE_ACTIVE	 1
#define PROFILE_BAD	0

#define ENGLISH 0
#define SPANISH 1


void Filter_Init(uint32_t Raw_Result, uint32_t Filter_Length);


#ifdef __cplusplus

}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
