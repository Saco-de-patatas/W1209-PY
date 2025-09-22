/**
  ******************************************************************************
  * @file    py32f002b_it.c
  * @author  MCU Application Team
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "py32f002b_it.h"

#define Button_Plus     LL_GPIO_PIN_4 //PA4
#define Button_Minus    LL_GPIO_PIN_2 //PA2
#define Button_Set      LL_GPIO_PIN_0 //PC0   

static int32_t Filter_state=-1, Active=-1, OneSec_Ticker=0, ADC_Cal_Time=TheOneHour-(6*The10seconds);

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers         */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
	Tick_Timer++; ADC_Cal_Time++;
	Action_Timer_On+=(Action_Timer_On<Action_Delay_On)?1:0;
	Action_Timer_Off+=(Action_Timer_Off<Action_Delay_Off)?1:0;
	if(Timeout<=The10seconds) Timeout++;
	Multiplex=((++Multiplex)==24)?0:Multiplex;
	
	if(W1209PY.Timer_Enable){
		OneSec_Ticker++;
		if(OneSec_Ticker==2000){
			OneSec_Ticker=0;
			if(W1209PY.TM_Seconds||W1209PY.TM_Minutes||W1209PY.TM_Days||W1209PY.TM_Hours) { 
				if(W1209PY.TM_Seconds) { W1209PY.TM_Seconds--; } else {
					W1209PY.TM_Seconds=59;
					if(W1209PY.TM_Minutes) {W1209PY.TM_Minutes--; }else {
						W1209PY.TM_Minutes=59;
						if(W1209PY.TM_Hours) {W1209PY.TM_Hours--;} else{
							W1209PY.TM_Hours=23;
							if(W1209PY.TM_Days) W1209PY.TM_Days--;
						}
					}
				}
			} else {
				System_Ready=0;
				if(LL_GPIO_IsOutputPinSet(GPIOB, Relay_Out)) LL_GPIO_ResetOutputPin(GPIOB, Relay_Out);
				if(!Beeper_Flag) Beeper_Flag=1;
			}
		}
	} else if(Beeper_Flag&&(!Alarm_Flag)) Beeper_Flag=0;
	
	Tick_Flag=1;
	if((++Roll_Timer)==500){
		Roll_Timer=0;
		Roll_Flag=1;
	}
	
	if(Beeper_Flag&&W1209PY.Sound)	LL_GPIO_SetOutputPin(GPIOA,Beeper<<(((Tick_Timer&1)&&(Roll_Timer>250))?0:16)); else if(LL_GPIO_IsOutputPinSet(GPIOA,Beeper)) LL_GPIO_ResetOutputPin(GPIOA,Beeper);

	LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_INPUT);
	
	if(!Flashing_Flag||(Roll_Timer>=250)){

      if(!D1Flash||(Roll_Timer>=250)) switch(Multiplex){
		case 0: 
				if(Disp_Chars[0]&_DP){
				LL_GPIO_SetOutputPin(GPIOB, Display_P2| (Display_P1<<16)); //Pin1 (-) Pin2 (+);	
				LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_OUTPUT); 
				LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT); //activate output for Dot1 segment.	
				}					
				break;
		case 2:
				if(Disp_Chars[0]&_F){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P2<<16) | Display_P3); //Pin2 (-) Pin3 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT); //activate output for F1 segment.
				}	
		  		break;
		case 3:
				if(Disp_Chars[0]&_G){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P2<<16) | Display_P4); //Pin2 (-) Pin4 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT);	//activate output for G1 segment.
				}
				break;
		case 4:
				if(Disp_Chars[0]&_C){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P2<<16) | Display_P5); //Pin2 (-) Pin5 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT); //activate output for C1 segment.
				}
				break;
		case 19:
				if(Disp_Chars[0]&_D){
					LL_GPIO_SetOutputPin(GPIOB, Display_P2); //Pin2 (+)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6<<16); //Pin6 (-)
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for D1 segment.
				}
				break;
		case 5:
				if(Disp_Chars[0]&_A){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P3<<16) | Display_P2); //Pin3 (-) Pin2 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT); //activate output for A1 segment.
				}
				break;	
		case 9:
				if(Disp_Chars[0]&_B){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P4<<16) | Display_P2); //Pin4 (-) Pin2 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT); //activate output for B1 segment.
				}
				break;
		case 14:
				if(Disp_Chars[0]&_E){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P5<<16) | Display_P2); //Pin5 (-) Pin2 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_OUTPUT); //activate output for E1 segment.
				}
				break;
		default:
				break;
	  }
	  if(!D2Flash||(Roll_Timer>=250)) switch(Multiplex){
		case 1:
				if(Disp_Chars[1]&_DP){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P1<<16) | Display_P3); //Pin1 (-) Pin3 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT); //activate output for Dot1 segment.
				}	
		  		break;
		case 6:
				if(Disp_Chars[1]&_F){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P3<<16) | Display_P4); //Pin3 (-) Pin4 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT); //activate output for F2 segment.
				}
				break;
		case 7:
				if(Disp_Chars[1]&_G){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P3<<16) | Display_P5); //Pin3 (-) Pin5 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT); //activate output for G2 segment.
				}
				break;
		case 8:
				if(Disp_Chars[1]&_E){
					LL_GPIO_SetOutputPin(GPIOB, Display_P3<<16); //Pin3 (-)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6); //Pin6 (+)
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for E2 segment.
				}
				break;
		case 10:
				if(Disp_Chars[1]&_D){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P4<<16) | Display_P3); //Pin4 (-) Pin3 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT); //activate output for D2 segment.
				}
				break;
		case 11:
				if(Disp_Chars[1]&_A){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P4<<16) | Display_P5); //Pin4 (-) Pin5 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT); //activate output for A2 segment.
				}
				break;
		case 15:
				if(Disp_Chars[1]&_B){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P5<<16) | Display_P3); //Pin5 (-) Pin3 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT); //activate output for B2 segment.
				}
				break;
		case 16:
				if(Disp_Chars[1]&_C){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P5<<16) | Display_P4); //Pin5 (-) Pin4 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT); //activate output for C2 segment.
				}
				break;
		default:
				break;
	  }
	  if(!D3Flash||(Roll_Timer>=250)) switch(Multiplex){
		case 12:
				if(Disp_Chars[2]&_D){
					LL_GPIO_SetOutputPin(GPIOB, Display_P4<<16); //Pin4 (-)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6); //Pin6 (+)
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for D3 segment.
				}
				break;
		case 13:
				if(Disp_Chars[2]&_G){
					LL_GPIO_SetOutputPin(GPIOB, (Display_P5<<16) | Display_P1); //Pin5 (-) Pin1 (+);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_OUTPUT); //activate output for G3 segment.
				}
				break;
		case 17:
				if(Disp_Chars[2]&_F){
					LL_GPIO_SetOutputPin(GPIOB, Display_P5<<16); //Pin5 (-)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6); //Pin6 (+)
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for F3 segment.
				}
				break;
		case 18:
				if(Disp_Chars[2]&_A){
					LL_GPIO_SetOutputPin(GPIOB, Display_P1); //Pin1 (+)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6<<16); //Pin6 (-)
					LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for A3 segment.
				}
				break;
		case 20:
				if(Disp_Chars[2]&_B){
					LL_GPIO_SetOutputPin(GPIOB, Display_P3); //Pin3 (+)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6<<16); //Pin6 (-)
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for B3 segment.
				}
				break;
		case 21:
				if(Disp_Chars[2]&_E){
					LL_GPIO_SetOutputPin(GPIOB, Display_P4); //Pin4 (+)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6<<16); //Pin6 (-)
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for E3 segment.
				}
				break;
		case 22:
				if(Disp_Chars[2]&_C){
					LL_GPIO_SetOutputPin(GPIOB, Display_P5); //Pin5 (+)
					LL_GPIO_SetOutputPin(GPIOA, Display_P6<<16); //Pin6 (-)
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_OUTPUT);
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_OUTPUT); //activate output for C3 segment.
				}
				break;
	
		default:
				break;
	  }
	  if(Multiplex==23){
					LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_INPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_INPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_INPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_INPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_INPUT);
					LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_INPUT);

				//Here do analog and switches...
				
				//Process Button_Plus/Minus/Set pushing time, debounce delay (50ms mininum key press for capture).
				//Increments every 3 seconds for long press. Release before 3 seconds for short press.
				
				if(!LL_GPIO_IsInputPinSet(GPIOA, Button_Plus)) {
					Timeout=0;
					Cnt_BPlus++;
					if((Cnt_BPlus&0x3F)==0x3F) Pressed_BPlus=((Cnt_BPlus+1)>>6)+1;
				} else {
						if((Cnt_BPlus>=1)&&(Cnt_BPlus<0x3F)) Pressed_BPlus=1; else Pressed_BPlus=0;
						Cnt_BPlus=0;
						}
					
				if(!LL_GPIO_IsInputPinSet(GPIOA, Button_Minus)) {
					Timeout=0;
					Cnt_BMinus++;
					if((Cnt_BMinus&0x3F)==0x3F) Pressed_BMinus=((Cnt_BMinus+1)>>6)+1;
				} else {
					   if((Cnt_BMinus>=1)&&(Cnt_BMinus<0x3F)) Pressed_BMinus=1; else Pressed_BMinus=0;
					   Cnt_BMinus=0;
					   }

				if(!LL_GPIO_IsInputPinSet(GPIOC, Button_Set)) {
					Timeout=0;
					Cnt_BSet++;
					if((Cnt_BSet&0x3F)==0x3F) Pressed_BSet=((Cnt_BSet+1)>>6)+1;
				} else {
					   if((Cnt_BSet>=1)&&(Cnt_BSet<0x3F)) Pressed_BSet=1; else Pressed_BSet=0;
					   Cnt_BSet=0;
					   }
					   
			    Anykey=(Pressed_BMinus|Pressed_BPlus|Pressed_BSet);
				
				if(ADC_Ready&&(!Calibration_Flag)){
					if(ADC_Cal_Time<TheOneHour) LL_ADC_REG_StartConversion(ADC1);
					else {
						Calibration_Flag=1;
						ADC_Cal_Time=0;
					}
				}
			}
		if(System_Ready){
					if(W1209PY.Function<2){
						if((((NTC.Temp<Turn_On_Temp)^((W1209PY.Function==1)))||(NTC.Temp==Turn_On_Temp))&&((Active==0)||(Active==-1)))
								{
									Active=1; 
									Action_Timer_On=W1209PY.Delay_Ton?0:Action_Delay_On; 
								}
						if((((NTC.Temp>Turn_Off_Temp)^((W1209PY.Function==1)))||(NTC.Temp==Turn_Off_Temp))&&((Active==1)||(Active==-1)))
								{  
									Active=0;
									Action_Timer_Off=W1209PY.Delay_Toff?0:Action_Delay_Off;
								}
					} else {
						if(((((NTC.Temp>Turn_On_Temp)&&(NTC.Temp>Turn_Off_Temp))||((NTC.Temp<Turn_On_Temp)&&(NTC.Temp<Turn_Off_Temp)))^(W1209PY.Function!=3))&&((Active==0)||(Active==-1)))
								{
									Active=1; 
									Action_Timer_On=W1209PY.Delay_Ton?0:Action_Delay_On; 
								}
						if(((((NTC.Temp>Turn_On_Temp)&&(NTC.Temp>Turn_Off_Temp))||((NTC.Temp<Turn_On_Temp)&&(NTC.Temp<Turn_Off_Temp)))^(W1209PY.Function==3))&&((Active==1)||(Active==-1)))
								{  
									Active=0;
									Action_Timer_Off=W1209PY.Delay_Toff?0:Action_Delay_Off;
								}
					}
					if(Action_Timer_On>=Action_Delay_On) 	if((Active==1)&&(!LL_GPIO_IsOutputPinSet(GPIOB, Relay_Out))) LL_GPIO_SetOutputPin(GPIOB, Relay_Out);
					if(Action_Timer_Off>=Action_Delay_Off)	if((Active==0)&&(LL_GPIO_IsOutputPinSet(GPIOB, Relay_Out))) LL_GPIO_ResetOutputPin(GPIOB, Relay_Out);
													
		}
	}
}

void Filter_Init(uint32_t Raw_Result, uint32_t Filter_Length){
	Filter_state=Raw_Result<<(Filter_Length);	
}

uint32_t Filter(uint32_t Raw_Result, uint32_t Filter_Length){
	if(Filter_state==-1) Filter_Init(Raw_Result,Filter_Length+W1209PY.Bits_Extended);
	W1209PY.Sensor_State=(Raw_Result>4047)?1:(Raw_Result<48)?2:0;
	if(!W1209PY.Sensor_State) Filter_state+=(Raw_Result-(Filter_state>>(Filter_Length+W1209PY.Bits_Extended)));
	return (uint32_t)(Filter_state>>Filter_Length);
}

void ADC_COMP_IRQHandler(void){
	
if(LL_ADC_IsActiveFlag_EOC(ADC1) != 0)
  {
    LL_ADC_ClearFlag_EOC(ADC1);
    Analog_Read = Filter(LL_ADC_REG_ReadConversionData12(ADC1)+W1209PY.ADC_Offset, W1209PY.Filter_Depth);
	Analog_Flag = 1;
  }
	
}


/******************************************************************************/
/* PY32F002B Peripheral Interrupt Handlers                                     */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file.                                          */
/******************************************************************************/

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
