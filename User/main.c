
/* Includes ------------------------------------------------------------------*/
#include "main.h"


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

#define Display_P1  1<<2    // Pin 1 connected to PB2
#define Display_P2  1<<3    // Pin 2 connected to PB3
#define Display_P3  1<<4    // Pin 3 connected to PB4
#define Display_P4  1<<1    // Pin 4 connected to PB1
#define Display_P5  1<<0    // Pin 5 connected to PB0
#define Display_P6  1<<0    // Pin 6 connected to PA0

#define Beeper		LL_GPIO_PIN_1	// PA1

#define Button_Plus     LL_GPIO_PIN_4 //PA4
#define Button_Minus    LL_GPIO_PIN_2 //PA2
#define Button_Set      LL_GPIO_PIN_0 //PC0   

#define Relay_Out		LL_GPIO_PIN_6 //PB6

// Segment table translation, credits to Wolfgang Buescher for the lookup table.

#define Seg_Table_Size 51
#define Msg_Size 82

// Log function Q31 fix point defines...
#define LOG_Q31_ACCURACY 31
#define LOG_Q31_INTEGER_PART 5
#define LOQ_Q31_THRESHOLD (1u << LOG_Q31_ACCURACY)
#define LOQ_Q31_Q32_HALF LOQ_Q31_THRESHOLD
#define LOQ_Q31_Q30_HALF (LOQ_Q31_Q32_HALF >> 2)
#define LOG_Q31_INVLOG2EXP 0x58B90BFCUL

#define R_PULLUP 10000
#define _100Kelvin 27315
#define The10seconds 20000


unsigned char *(*MsgStr);

const unsigned char *(MsgStrEn[Msg_Size])={(unsigned char*)"HELL0 thiS iS VW-1209-Py V10 ",		// ID  0
								  (unsigned char*)"ProGrAmMinG VW-1209-Py ",					// ID  1
								  (unsigned char*)"tArGEt tEmMPErAturE ",						// ID  2
								  (unsigned char*)" P",											// ID  3
								  (unsigned char*)"dEViCE Function ",							// ID  4
								  (unsigned char*)"Error Adc CALibrAtion ",						// ID  5
								  (unsigned char*)"HyStErESiS ",								// ID  6
								  (unsigned char*)"HiGhESt tEmMPErAturE ",						// ID  7
								  (unsigned char*)"LoVWESt tEmMPErAturE ",						// ID  8
								  (unsigned char*)"0FFSEt AdJuSt ",								// ID  9
								  (unsigned char*)"dELAy ",										// ID 10
								  (unsigned char*)"ALArmM 1 ",									// ID 11
								  (unsigned char*)"SEt SCALE ",									// ID 12
								  (unsigned char*)"FACt0ry dEFAuLt ?",							// ID 13
								  (unsigned char*)"Edit tArGEt by roLLinG ",					// ID 14
								  (unsigned char*)"CHAnGE r_PuLLuP ",							// ID 15
								  (unsigned char*)"CHAnGE r_0 ",								// ID 16
								  (unsigned char*)"CHAnGE t_0 ",								// ID 17
								  (unsigned char*)"CHAnGE bEtA ",								// ID 18
								  (unsigned char*)"CHAnGE r_1 ",								// ID 19
								  (unsigned char*)"CHAnGE t_1 ",								// ID 20
								  (unsigned char*)"SnAPSHot r_0 ? ",							// ID 21
								  (unsigned char*)"SnAPSHot r_1 ? ",							// ID 22
								  (unsigned char*)"diSPLAy roLLinG ? ",							// ID 23
								  (unsigned char*)"diSPLAy mMESSAGES ? ",						// ID 24
								  (unsigned char*)"HEAtEr ",									// ID 25
								  (unsigned char*)"CooLEr ",									// ID 26
								  (unsigned char*)"VWindoVW ",									// ID 27
								  (unsigned char*)"tArGEt = ",									// ID 28
								  (unsigned char*)" *F ",										// ID 29
								  (unsigned char*)" *C ",										// ID 30
								  (unsigned char*)" no",										// ID 31
								  (unsigned char*)"yES",										// ID 32
								  (unsigned char*)"0FF",										// ID 33
								  (unsigned char*)" 0n",										// ID 34
								  (unsigned char*)"CELSiuS ",									// ID 35
								  (unsigned char*)"FArhEnHEit ",								// ID 36
								  (unsigned char*)"r_0 = ",										// ID 37
								  (unsigned char*)"t_0 = ",										// ID 38
								  (unsigned char*)"bEtA = ",									// ID 39
								  (unsigned char*)"r_1 = ",										// ID 40
								  (unsigned char*)"t_1 = ",										// ID 41
								  (unsigned char*)"uPdAtE r_1 ? ",								// ID 42
								  (unsigned char*)"uPdAtE bEtA ? ",								// ID 43
								  (unsigned char*)"mModiFy FiLtEr dEPth ",						// ID 44
								  (unsigned char*)"mMinutES ",									// ID 45
								  (unsigned char*)"SEC0ndS ",									// ID 46
								  (unsigned char*)"dELAy on turn_on ? ",						// ID 47
								  (unsigned char*)"dELAy on turn_oFF ? ",						// ID 48
								  (unsigned char*)"ALArmM 2 ",									// ID 49
								  (unsigned char*)"AL1 too CoLd",								// ID 50
								  (unsigned char*)"AL1 too Hot", 								// ID 51
								  (unsigned char*)"AL2 too CoLd",								// ID 52
								  (unsigned char*)"AL2 too Hot",								// ID 53
								  (unsigned char*)"EnAbLE timMEr Function ? ",					// ID 54
								  (unsigned char*)"dAyS LEFt ",									// ID 55
								  (unsigned char*)"HourS LEFt ",								// ID 56
								  (unsigned char*)"mMinutES LEFt ",								// ID 57
								  (unsigned char*)"timME LEFt = ",								// ID 58
								  (unsigned char*)"dAyS ",										// ID 59
								  (unsigned char*)"HourS ",										// ID 60
								  (unsigned char*)"timMEd 0ut - dEViCE StoPPEd ",				// ID 61
								  (unsigned char*)"r_PuLLuP = ",								// ID 62	
								  (unsigned char*)"ActiVE inSidE ",								// ID 63
								  (unsigned char*)"ActiVE 0utSidE ",							// ID 64
								  (unsigned char*)"VWindoVW VWidth ",							// ID 65
								  (unsigned char*)"Adc bit-dEPth ",								// ID 66
								  (unsigned char*)"SEnSor diSconnEctEd or dAmMAGEd ",			// ID 67
								  (unsigned char*)"SEnSor ShortEd or VWorn out ",				// ID 68
								  (unsigned char*)"no VALid ProFiLE Found - dEFAuLtS L0AdEd ",	// ID 69
								  (unsigned char*)"PLEASE SELECt nEVW ProFiLE ",				// ID 70
								  (unsigned char*)"VWArninG - CorruPted ProFiLE- uSE AnyVWAy ? ",	// ID  71
								  (unsigned char*)"CHAnGE CurrEnt ProFiLE ",					// ID 72
								  (unsigned char*)"L0Ad dAtA FromM ProFiLE ? ",					// ID 73
								  (unsigned char*)"Error SAVinG ProFiLE ",						// ID 74
								  (unsigned char*)"ErASE ALL ProFiLES ? ",						// ID 75
								  (unsigned char*)"CHAnGE LAnGuAGE ",							// ID 76
								  (unsigned char*)"ENGL1SH ",									// ID 77
								  (unsigned char*)"SPAN1SH ",									// ID 78
								  (unsigned char*)"EnAbLE Sound ? ",							// ID 79
								  (unsigned char*)"VWirE rESiStAncE in 0HmMS  ",				// ID 80
								  (unsigned char*)"Adc 0FFSEt "};								// ID 81
									
const unsigned char *(MsgStrSp[Msg_Size])={(unsigned char*)"H0LA S0y VW-1209-Py V10 ",
								  (unsigned char*)"",
								  (unsigned char*)"tEmMPErAturA 0bJEtiVo ",
								  (unsigned char*)" P",
								  (unsigned char*)"Funcion dEL APArAto ",
								  (unsigned char*)"Error dE CALibrAcion Adc ",
								  (unsigned char*)"HiStErESiS ",
								  (unsigned char*)"tEmMPErAturA mMAS ALtA ",
								  (unsigned char*)"tEmMPErAturA mMAS bAJA ",
								  (unsigned char*)"AJuStE dE dESViAcion ",
								  (unsigned char*)"rEtrASo ",
								  (unsigned char*)"ALArmMA 1 ",
								  (unsigned char*)"EStAbLEcEr ESCALA ",
								  (unsigned char*)"VAL0rES dE FAbricA ? ",
								  (unsigned char*)"EditAr 0bJEtiVo rodAndo ",
								  (unsigned char*)"CAmMbiAr r_PuLLuP ",
								  (unsigned char*)"CAmMbiAr r_0 ",
								  (unsigned char*)"CAmMbiAr t_0 ",
								  (unsigned char*)"CAmMbiAr bEtA ",
								  (unsigned char*)"CAmMbiAr r_1 ",
								  (unsigned char*)"CAmMbiAr t_1 ",
								  (unsigned char*)"CAPturAr r_0 ",
								  (unsigned char*)"CAPturAr r_1 ",
								  (unsigned char*)"PAntALLA GirAtoriA ? ",
								  (unsigned char*)"mMoStrAr LoS mMEnSAGES ? ",
								  (unsigned char*)"CALEntAdor ",
								  (unsigned char*)"EnFriAdor ",
								  (unsigned char*)"VEntAnA ",
								  (unsigned char*)"0bJEtiVo = ",
								  (unsigned char*)" *F ",
								  (unsigned char*)" *C ",
								  (unsigned char*)" no",
								  (unsigned char*)" Si",
								  (unsigned char*)"0FF",
								  (unsigned char*)" 0n",
								  (unsigned char*)"CELSiuS ",
								  (unsigned char*)"FArhEnHEit ",
								  (unsigned char*)"r_0 = ",
								  (unsigned char*)"t_0 = ",
								  (unsigned char*)"bEtA = ",
								  (unsigned char*)"r_1 = ",
								  (unsigned char*)"t_1 = ",
								  (unsigned char*)"mModiFicAr r_1 ? ",
								  (unsigned char*)"mModiFicAr bEtA ? ",
								  (unsigned char*)"CAmMbiAr ProFundidAd dE FiLtro ",
								  (unsigned char*)"mMinutoS ",
								  (unsigned char*)"SEGundoS ",
								  (unsigned char*)"rEtrASo En EncEndido ? ",
								  (unsigned char*)"rEtrASo En APAGAdo ? ",
								  (unsigned char*)"ALArmMA 2 ",
								  (unsigned char*)"AL1 mMuy Frio",
								  (unsigned char*)"AL1 mMuy CALiEntE", 
								  (unsigned char*)"AL2 mMuy Frio",
								  (unsigned char*)"AL2 mMuy CALiEntE",
								  (unsigned char*)"ActiVAr Funcion tEmMPorizAdor ? ",
								  (unsigned char*)"diAS rEStAntES ",
								  (unsigned char*)"HorAS rEStAntES ",
								  (unsigned char*)"mMinutoS rEStAntES ",
								  (unsigned char*)"tiEmMPo rEStAntE = ",
								  (unsigned char*)"diAS ",
								  (unsigned char*)"HorAS ",
								  (unsigned char*)"FuErA dE tiEmMPo - APArAto PArAdo ",
								  (unsigned char*)"r_PuLLuP = ",
								  (unsigned char*)"ActiVA dEntro ",
								  (unsigned char*)"ActiVA FuErA ",
								  (unsigned char*)"AnchurA VEntAnA ",
								  (unsigned char*)"ProFundidAd bitS Adc ",
								  (unsigned char*)"SEnSor dESconEctAdo o dA@Ado ",
								  (unsigned char*)"SEnSor En corto o GAStAdo ",
								  (unsigned char*)"",
								  (unsigned char*)"",
								  (unsigned char*)"0J0 - PErFiL dA@Ado - uSAr iGuALmMEntE ? ",
								  (unsigned char*)"CAmMbiAr PErFiL ActiVo ",
								  (unsigned char*)"CArGo dAtoS dESdE PErFiL ? ",
								  (unsigned char*)"Error GuArdAndo PErFiL ",
								  (unsigned char*)"borrAr todoS LoS PErFiLES ?",
								  (unsigned char*)"CAmMbiAr idiomMA ",
								  (unsigned char*)"1NGLES ",
								  (unsigned char*)"ESPA@0L ",
								  (unsigned char*)"ActiVAr Sonido ? ",
								  (unsigned char*)"rESiStEnciA dEL CAbLE En 0HmMioS  ",
								  (unsigned char*)"AJuStE AdC "};

const unsigned char Seg_Translate[Seg_Table_Size][2]= 
                                          {{ '0' , (_A+_B+_C+_D+_E+_F   )}, // ABCDEF. = '0'    
                                           { '1' , (   _B+_C            )}, // .BC.... = '1'    
                                           { '2' , (_A+_B   +_D+_E   +_G)}, // AB.DE.G = '2'    
                                           { '3' , (_A+_B+_C+_D      +_G)}, // ABCD..G = '3'    
                                           { '4' , (   _B+_C      +_F+_G)}, // .BC..FG = '4'    
                                           { '5' , (_A   +_C+_D   +_F+_G)}, // A.CD.FG = '5'    
                                           { '6' , (_A   +_C+_D+_E+_F+_G)}, // A.CDEFG = '6'    
                                           { '7' , (_A+_B+_C            )}, // ABC.... = '7'    
                                           { '8' , (_A+_B+_C+_D+_E+_F+_G)}, // ABCDEFG = '8'    
                                           { '9' , (_A+_B+_C+_D   +_F+_G)}, // ABCD.FG = '9'    
                                           { 'A' , (_A+_B+_C   +_E+_F+_G)}, // ABC.EFG = 'A'    
                                           { 'b' , (      _C+_D+_E+_F+_G)}, // ..CDEFG = 'b'    
                                           { 'c' , (         _D+_E   +_G)}, // ...DE.G = 'c'    
                                           { 'd' , (   _B+_C+_D+_E   +_G)}, // .BCDE.G = 'd'    
                                           { 'E' , (_A      +_D+_E+_F+_G)}, // A..DEFG = 'E'    
                                           { 'F' , (_A         +_E+_F+_G)}, // A...EFG = 'F'    
                                           { 'G' , (_A   +_C+_D+_E+_F   )}, // A.CDEF. = 'G'    
                                           { 'H' , (   _B+_C   +_E+_F+_G)}, // .BC.EFG = 'H'    
                                           { 'i' , (            _E      )}, // ....E.. = 'i'    
                                           { 'P' , (_A+_B      +_E+_F+_G)}, // AB..EFG = 'P'    
                                           { 'r' , (            _E   +_G)}, // ....E.G = 'r'    
                                           { 'o' , (      _C+_D+_E   +_G)}, // ..CDE.G = 'o'    
                                           { 'Q' , (_A+_B+_C      +_F+_G)}, // ABC..FG = 'Q'    
                                           { 'u' , (      _C+_D+_E      )}, // ..CDE.. = 'u'    
                                           { 't' , (         _D+_E+_F+_G)}, // ...DEFG = 't'    
                                           { 'S' , (_A   +_C+_D   +_F+_G)}, // A.CD.FG = 'S'    
                                           { 'Z' , (_A+_B   +_D+_E   +_G)}, // AB.DE.G = 'Z'    
                                           { 'I' , (            _E+_F   )}, // ....EF. = 'I'    
                                           { 'J' , (   _B+_C+_D         )}, // .BCD..  = 'J'    
                                           { 'k' , (         _D+_E+_F+_G)}, // ...DEFG = 'k'    
                                           { 'L' , (         _D+_E+_F   )}, // ...DEF. = 'L'    
                                           { 'N' , (_A+_B+_C   +_E+_F   )}, // ABC.EF. = 'N'    
                                           { 'V' , (      _C+_D+_E+_F   )}, // ..CDEF. = 'V'    
                                           { '=' , (         _D      +_G)}, // ...D..G = '=' 
                                           { '-' , (                 +_G)}, // ......G = '-'
										   { '_' , (         _D         )}, // ...D... = '_'
                                           { 'n' , (      _C   +_E   +_G)}, // ..C.E.G = 'n'
										   { '@' , (_A   +_C   +_E   +_G)}, // ..C.E.G = 'ñ' 										   
										   { 'y' , (   _B+_C+_D   +_F+_G)}, // .BCD.FG = 'y'
										   { 'W' , (   _B+_C+_D+_E      )}, // .BCD.FG = 'W'
										   { 'M' , (_A+_B+_C+     +_F   )}, // ABC..F. = 'M'
										   { 'm' , (_A+_B+      _E+_F   )}, // ABC..F. = 'm'	
										   { 'C' , (_A+     +_D+_E+_F   )}, // ABCDEF. = 'C'	
										   { 'h' , (      _C   +_E+_F+_G)}, // .BC.EFG = 'h' 
										   { '*' , (_A+_B         +_F+_G)}, // AB...FG = 'º'
										   { 'z' , (_A+_B   +_D+_E      )}, // AB.DE.. = 'z' 
										   { '?' , (_A+_B      +_E   +_G)}, // AB.DE.. = '?' 
										  { '\'' , (               _F   )}, // AB...FG = '\''											   
                                           { ' ' , (0b00000000)},          // ....... = ' '    
                                           { '#' , (0b11111111)}};         // all segments on  


/* Shared variables ---------------------------------------------------------*/
__IO int32_t Tick_Timer=0, Action_Timer_On=0, Action_Timer_Off=0, Action_Delay_Off=0, Action_Delay_On=0, Multiplex=0, Tick_Flag=0, Roll_Timer=0, Timeout=0, Roll_Flag=0, Beeper_Flag=0, Flashing_Flag=0, D1Flash=0, D2Flash=0, D3Flash=0;

__IO int32_t Analog_Read=2048, Analog_Flag=0, ADC_Ready=0, System_Ready=0;

int32_t BK_ADC_Ready, BK_System_Ready, BK_Roll_Flag, BK_Flashing_Flag, BK_Alarm_Flag, BK_Beeper_Flag;

__IO uint32_t Turn_On_Temp=0, Turn_Off_Temp=0; 

__IO unsigned char Disp_Chars[3]={0,0,0};   //Display interface for systick interrupt managing the multiplexing GPIOs.

__IO uint32_t Pressed_BSet=0, Pressed_BMinus=0, Pressed_BPlus=0, Cnt_BSet=0, Cnt_BMinus=0, Cnt_BPlus=0, Anykey=0;

unsigned char Display[64]="   ", *Display1=Display+3, *p_aux, *p_roll=Display;   //Buffer string for display, auxiliar string pointer, 3 char window pointer...

uint32_t Profiles[5]={0x1FFF0280, 0x08005F80, 0x08005F00, 0x08005E80, 0x08005E00}, Profile_Statuses[5]={PROFILE_EMPTY,PROFILE_EMPTY,PROFILE_EMPTY,PROFILE_EMPTY,PROFILE_EMPTY};

int32_t Aux, Aux2 , BF_Edit=0, Program_Menu=0, Cleaning=0, Active_Profile=0;
__IO uint32_t Alarm_Flag=0, Calibration_Flag=0;

uint32_t Params_Buffer[32]={0x0DEADBEEF}; // 0xDEADBEEF is the header (and for preventing OTP Lock, that needs 0xXXXXAA55 for locking)

__IO __attribute__((aligned (4))) struct Params {
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
} NTC; // sizeof(NTC)=68 Bytes.

__IO __attribute__((aligned (4))) struct Params2 {
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
} W1209PY; // Sizeof(W1209PY)=56 Bytes.

 

static void System_Clock_Config(void);
static void GPIO_Config(void);
static void GPIO_Config_2(void);
static void FlashSetOptionByte(void);
static void FLASH_PageErase(uint32_t PageAddress);
static void ADCConfig(void);
static void ADCCalibrate(void);


uint32_t Calculate_CRC(struct Params *__NTC, struct Params2 *__W1209PW, int Set_CRC);

  
unsigned char Translate_Segment(unsigned char Ch){
    
    int i;
    for(i=0;i<Seg_Table_Size;i++) if(Ch==Seg_Translate[i][0]) return Seg_Translate[i][1];
    return 0;
    
}

void Put_Buff_On_Display(unsigned char *orig, unsigned int dot){
    unsigned char *dest=(unsigned char *)Disp_Chars;
    for(int i=0;i<3;i++){
        if(*orig) {
					*dest=(*orig&_DP)?_DP:0;
                    *(dest++)|=Translate_Segment((*orig)&0x7F);					
                    orig++;
                    }
                    else *(dest++)=0;
    }
    switch(dot){
        case 1:
                Disp_Chars[0]|=_DP;
                break;
        case 2: Disp_Chars[1]|=_DP;
                break;
        default:
                break;
    }
}

// Math functions...

static inline uint32_t My_CLZ(uint32_t x) {
	static uint8_t const clz_lkup[] = {
		32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
		28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U
	};
	uint32_t n;

	if (x >= (1U << 16)) {
		if (x >= (1U << 24)) {
			if (x >= (1 << 28)) {
				n = 28U;
			}
			else {
				n = 24U;
			}
		}
		else {
			if (x >= (1U << 20)) {
				n = 20U;
			}
			else {
				n = 16U;
			}
		}
	}
	else {
		if (x >= (1U << 8)) {
			if (x >= (1U << 12)) {
				n = 12U;
			}
			else {
				n = 8U;
			}
		}
		else {
			if (x >= (1U << 4)) {
				n = 4U;
			}
			else {
				n = 0U;
			}
		}
	}
	return (uint32_t)clz_lkup[x >> n] - n;
}

static uint32_t log_q31(uint32_t src){
	int32_t i;

	int32_t c = My_CLZ(src);
	int32_t normalization=0;

	uint32_t inc = LOQ_Q31_Q32_HALF >> (LOG_Q31_INTEGER_PART + 1);

	uint32_t x, y=0;

	int32_t tmp;

	x = src;
	if ((c-1) < 0)   {
			x = x >> (1-c);
		}
	else
	    {
		x = x << (c-1);
	}
	normalization = c;

	for(i = 0; i < LOG_Q31_ACCURACY ; i++)
	{
		x = ((int64_t)x*x)  >> (LOG_Q31_ACCURACY - 1);

		if (x >= LOQ_Q31_THRESHOLD)      {
				y += inc ;
				x = x >> 1;
			}
		inc = inc >> 1;
	}
	tmp = (int32_t)y - (normalization << (LOG_Q31_ACCURACY - LOG_Q31_INTEGER_PART));

	y = ((int64_t)tmp * LOG_Q31_INVLOG2EXP) >> 31;

	return(y);

}


// Function for calculating the integer "e^x" exponential
// imput in Q5.26 fixed point format, output integer.
// Credits to https://www.quinapalus.com/efunc.html

uint32_t intexp(uint32_t x){

	uint64_t y;
	int32_t c=(5-My_CLZ(x)),k,t;

	if(c<0) return 0;
	k=c;
	x<<=5-c;
	y=0x20000000;
	t=(signed)(x-0xB17217F7); if(t>=0) x=t,y<<=2;
	t=(signed)(x-0x58b90bfc); if(t>=0) x=t,y<<=1;
	t=(signed)(x-0x33e647d9); if(t>=0) x=t,y+=y>>1;
	t=(signed)(x-0x1c8ff7c7); if(t>=0) x=t,y+=y>>2;
	t=(signed)(x-0x0f1383b7); if(t>=0) x=t,y+=y>>3;
	t=(signed)(x-0x07c28c30); if(t>=0) x=t,y+=y>>4;
	t=(signed)(x-0x03f05361); if(t>=0) x=t,y+=y>>5;
	t=(signed)(x-0x01fc0a8b); if(t>=0) x=t,y+=y>>6;
	t=(signed)(x-0x00ff0153); if(t>=0) x=t,y+=y>>7;
	t=(signed)(x-0x007fc02a); if(t>=0) x=t,y+=y>>8;
	t=(signed)(x-0x003ff005); if(t>=0) x=t,y+=y>>9;
	t=(signed)(x-0x001ffc00); if(t>=0) x=t,y+=y>>10;
	t=(signed)(x-0x000fff00); if(t>=0) x=t,y+=y>>11;
	t=(signed)(x-0x0007ffc0); if(t>=0) x=t,y+=y>>12;
	t=(signed)(x-0x0003fff0); if(t>=0) x=t,y+=y>>13;
	t=(signed)(x-0x0001fffc); if(t>=0) x=t,y+=y>>14;
	y=(y*(0x80000000+x))>>31;
	if(c==4) y<<=1;
	while(c){
		y=(y*y)>>32;
		c--;
		}
	switch(k){
		case 4: return (y);
		case 3:	return (y>>8);
		case 2: return (y>>20);
		case 1: return (y>>26);
		case 0: return (y>>29);
		}
	return y;
}

void itoa(unsigned char *dest, int32_t source, int32_t Decimal_Point){
	
	int32_t sign=(source<0)?1:0, first_digit=0;
	int32_t Decades[10]={ 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
	unsigned char digit;
		
	if(sign) {
			*(dest++)='-';
			source=-source;
		}
	for(int k=0; k<10; k++){
		digit='0';
		while(source>=Decades[k]) { source-=Decades[k]; digit++; }
		if((digit>'0')||first_digit||(k==(9-Decimal_Point))) {	first_digit=1;
																if(k==(9-Decimal_Point)&&(Decimal_Point)) (*dest)=_DP;else (*dest)=0;
																(*dest++)|=digit;
															}
	}
	*dest=0;
}

int32_t str2len(const unsigned char *source){
	int32_t temp;
	for(temp=0;*source;temp++,source++);
	return(temp);	
}

int32_t atoi(unsigned char *source){

	int32_t result=0, sign=0, base=10, char_idx;

	while(((*source<'1')||(*source>'9'))&&(*source!='x')&&(*source!='X')&&(*source!=0)){ if(*source=='-') sign=1; source++;}
	if(*source=='x'||*source=='X') { base=0x10; source++; }
	for(char_idx=0;source[char_idx];char_idx++){
		if((source[char_idx]>='a')&&(source[char_idx]<='f')&&(base==0x10)){
			result<<=4;
			result+=source[char_idx]-'a'+0x0A;
		}
		if((source[char_idx]>='A')&&(source[char_idx]<='F')&&(base==0x10)){
			result<<=4;
			result+=source[char_idx]-'A'+0x0A;
		}
		if((source[char_idx]>='0')&&(source[char_idx]<='9')){
			if(base==0x10) result<<=4; else result*=base;
			result+=source[char_idx]-'0';
		}
	}
	if(sign) result=-result;
	return result;
}

void Default_Params(void){
	NTC.R_Pullup=R_PULLUP; 				  //R_Pullup is a 10k resistor soldered on board connected to NTC and Vcc, but can be replaced.
	NTC.R0=10000;  						//R_0 = 10kOhm.
	NTC.Log_R0=log_q31(NTC.R0);
	NTC.R1=1087;
	NTC.Log_R1=log_q31(NTC.R1);
	NTC.T0K=From_Celsius(2500);			 //T0*100=25ºC
	NTC.T1K=From_Celsius(8500);			 //T0*100=85ºC
	NTC.Beta=(395000LL)<<26;			 //(Beta*100=3950) in Q37.26
	NTC.Beta_T0K=(NTC.Beta)/NTC.T0K;	 // (Beta*100)/(T0*100) in Q37.26
	NTC.R_Wires=0;
	
	W1209PY.Function=0; 				// 0 means heating, 1 means cooling, 2 means window (active inside the window), 3 means window (active outside)
	W1209PY.fahrenheit=0;  				// 0 means Celsius scale.
	W1209PY.Temp_Roll=0;				// If this is one, the temperature on main display shows scrolling.
	W1209PY.Messages=1;					// Display menu messages by default
	W1209PY.Hysteresis=From_Celsius(0);				// When function is 1 or 2, this is the backlash (or forelash if negative), when function is 3 or 4, this is the window width.
	W1209PY.Temp_Offset=From_Celsius(0);
	W1209PY.Max_TempK100=From_Celsius(11000);
	W1209PY.Min_TempK100=From_Celsius(-5000);
	W1209PY.Delay_On=0;
	W1209PY.Delay_Off=0;
	W1209PY.Delay_Type=0;
	W1209PY.Delay_Ton=0;
	W1209PY.Delay_Toff=0;
	W1209PY.Alarm1=W1209PY.Alarm2=0;
	W1209PY.Alarm1_TempK100=W1209PY.Alarm2_TempK100=From_Celsius(3000);
	W1209PY.Target=From_Celsius(2500);
	W1209PY.Filter_Depth=8;
	W1209PY.Timer_Enable=0;
	W1209PY.TM_Days=W1209PY.TM_Hours=W1209PY.TM_Minutes=W1209PY.TM_Seconds=0;
	W1209PY.Bits_Extended=0;
	W1209PY.Language=ENGLISH;
	W1209PY.Sound=1;
	W1209PY.ADC_Offset=0;
	Calculate_CRC((struct Params *)&NTC,(struct Params2 *)&W1209PY, Set_Checksum);
	
}

void Edit_Value(int32_t *Val, int32_t Signed, int32_t dot){
	
	int Digit_Count=0;
	char Display_String[4]="1d0", Str_Buffer[13]="0000000000000";
	unsigned char Disp_Backup[3]={Disp_Chars[0],Disp_Chars[1],Disp_Chars[2]};
	
	if(*Val>=0) itoa((unsigned char *)Str_Buffer, *Val,0); else itoa((unsigned char *)Str_Buffer, -(*Val),0);
	
	while((Digit_Count=str2len((unsigned char *)Str_Buffer))<13){
		for(int __ind=Digit_Count;__ind;__ind--) {	Str_Buffer[__ind+1]=Str_Buffer[__ind];
													Str_Buffer[__ind]=Str_Buffer[__ind-1];
												 }
												 Str_Buffer[0]='0';
		}
	if((*Val<0)&&(Signed)) Str_Buffer[0]='-'; 
	Display_String[2]=Str_Buffer[12];
	Flashing_Flag=1;
	while(Pressed_BSet);
	Flashing_Flag=0;
	Put_Buff_On_Display((unsigned char *)Display_String,0); 
	D1Flash=1;
	Digit_Count=0;
	while((Pressed_BSet!=3)&&!Tout()){
		if (Pressed_BSet==1 ) { Pressed_BSet=0;
								if(D1Flash==1) {D1Flash=0;D3Flash=1;} else {D1Flash=1;D3Flash=0;} 
								}
		
		if (((Pressed_BPlus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BPlus>>2)))))||(Pressed_BPlus==1)){
			if(Pressed_BPlus==1) Pressed_BPlus=0;
			if(D1Flash){
				Digit_Count=(Digit_Count>=9)?0:Digit_Count+1;
				if((Digit_Count==9)&&(!Signed)) Digit_Count=0; 
				if(Digit_Count<9) Display_String[0]=Digit_Count+'1'; else Display_String[0]='-';	
			}else{
				if(Digit_Count!=9) Str_Buffer[12-Digit_Count]=(Str_Buffer[12-Digit_Count]=='9')?'0':Str_Buffer[12-Digit_Count]+1;
				else Str_Buffer[0]=(Str_Buffer[0]=='0')?'-':'0';
			}
			if(Digit_Count!=9) Display_String[2]=Str_Buffer[12-Digit_Count]; else Display_String[2]=Str_Buffer[0]=='-'?'-':' ';
			Put_Buff_On_Display((unsigned char *)Display_String,(Digit_Count!=(dot-1))?0:2); 
			Tick_Timer++;
		}
		if (((Pressed_BMinus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BMinus>>2)))))||(Pressed_BMinus==1)){
			if(Pressed_BMinus==1) Pressed_BMinus=0;
			if(D1Flash){
				Digit_Count=(Digit_Count<=0)?9:Digit_Count-1;
				if((Digit_Count==9)&&(!Signed)) Digit_Count=8;
				if(Digit_Count<9) Display_String[0]=Digit_Count+'1'; else Display_String[0]='-';	
			}else{
				if(Digit_Count!=9) Str_Buffer[12-Digit_Count]=(Str_Buffer[12-Digit_Count]<='0')?'9':Str_Buffer[12-Digit_Count]-1;
				else Str_Buffer[0]=(Str_Buffer[0]=='0')?'-':'0';
			}
			if(Digit_Count!=9) Display_String[2]=Str_Buffer[12-Digit_Count]; else Display_String[2]=Str_Buffer[0]=='-'?'-':' ';
			Put_Buff_On_Display((unsigned char *)Display_String,(Digit_Count!=(dot-1))?0:2); 
			Tick_Timer++;
		}
	
	}
	Pressed_BSet=0;
	if(!Tout()) *Val=atoi((unsigned char *)Str_Buffer);
	D1Flash=D3Flash=0;
	Disp_Chars[0]=Disp_Backup[0];
	Disp_Chars[1]=Disp_Backup[1];
	Disp_Chars[2]=Disp_Backup[2];
	return;
}

void Get_Temperature(int32_t Display_Temp)
{
	 unsigned char *p_temp;
	 Analog_Flag=0;
	 p_aux=Display1;
	 NTC.Value=ADC_to_NTC(Analog_Read);
	 NTC.Log_Value=Log_ADC_to_NTC(Analog_Read);
	 NTC.Temp=NTC.Beta/(NTC.Log_Value-NTC.Log_R0+NTC.Beta_T0K)+W1209PY.Temp_Offset-_100Kelvin;
	 if(!Display_Temp) return;
	 itoa(p_aux,(W1209PY.fahrenheit)?To_fahrenheit(NTC.Temp):To_Celsius(NTC.Temp),2);	
	 p_temp=p_aux+str2len(p_aux);
	 if(W1209PY.fahrenheit)
			{ 
			  strcpy(p_temp,MsgStr[29]);
			}else {
			  strcpy(p_temp,MsgStr[30]);
			} 
	 if((!W1209PY.Timer_Enable)||W1209PY.TM_Seconds||W1209PY.TM_Minutes||W1209PY.TM_Days||W1209PY.TM_Hours)	System_Ready=1; 
}

int32_t MsgBox(unsigned char *msg, int32_t dot, int32_t Cyclic){
	int32_t button=0;
	if(!W1209PY.Messages) return 0;
	 Anykey=0;
	 p_aux=msg;
	 strcpy(Display1,p_aux);
	 if(dot>0) Display1[str2len(Display1)-dot]|=_DP;
	 for(p_roll=Display;(*p_roll||Cyclic)&&!Anykey;p_roll++)
						{ 
							Put_Buff_On_Display(p_roll,0);
							while(!Roll_Flag&&!Anykey) if(Analog_Flag) Get_Temperature(Dont_Put_Temp_On_Buffer);
							Roll_Flag=0;
							if(!*p_roll) p_roll=Display;
						}
	 Display1[0]=Timeout=0;
	 button=(Pressed_BSet?4:0)+(Pressed_BPlus?2:0)+(Pressed_BMinus?1:0);
	 Pressed_BSet=Pressed_BMinus=Pressed_BPlus=Anykey=0;
	 return button;
}

int32_t MsgBox2(int32_t dot, int32_t Cyclic, int32_t Touts){
	
	 int32_t button=0;
	 Anykey=Timeout=0;
	 if(dot>0) Display1[str2len(Display1)-dot]|=_DP;
	 for(p_roll=Display;(*p_roll||Cyclic)&&!Anykey&&Touts;p_roll++)
						{ 
							Put_Buff_On_Display(p_roll,0);
							while(!Roll_Flag&&!Anykey) if(Analog_Flag) Get_Temperature(Dont_Put_Temp_On_Buffer);
							Roll_Flag=0;
							if(!*p_roll) p_roll=Display;
							if(Tout()){ Touts--; Timeout=0;}
						}
	 Display1[0]=Timeout=0;
	 button=(Pressed_BSet?4:0)+(Pressed_BPlus?2:0)+(Pressed_BMinus?1:0);
	 Pressed_BSet=Pressed_BMinus=Pressed_BPlus=Anykey=0;
	 return button;
}


void Display_Menu(){
				p_aux=(unsigned char *)MsgStr[3];
				if(Program_Menu<10) {
					strcpy(Display1, p_aux);
				}else {
					p_aux++;
					strcpy(Display1, p_aux);
				}
				p_aux=Display1+str2len(Display1);
				itoa(p_aux, Program_Menu,0);
				Put_Buff_On_Display(Display1,0);
}			

int32_t DisplayOptions(int32_t FirstStr, int32_t value, int32_t n_opt){
				strcpy(Display1, MsgStr[FirstStr+value]);
				if(!Display1[3]){
					Put_Buff_On_Display(Display1,0);
					while (!Anykey&&!Tout());
				}
				else {
					p_roll=Display;
					Put_Buff_On_Display(p_roll,0);
					while (!Anykey&&!Tout()) if(Roll_Flag){
						Roll_Flag=0;
						Put_Buff_On_Display(p_roll++,0);
						if(!(*p_roll)) p_roll=Display;
					}
				}
				if((Pressed_BPlus==1)) value=((++value==n_opt)?0:value);
				if((Pressed_BMinus==1)) value=((--value<0)?n_opt-1:value);
				Anykey=Pressed_BMinus=Pressed_BPlus=0;
				return value;
}


#define Edit_Resistors(MsgNumber, Value) Edit_Scalars(MsgNumber, Value, 0, 3)

void Edit_Scalars(int32_t MsgNumber, int32_t *Value, int32_t sign, int32_t dot){
	int32_t Aux;
	
	strcpy(Display1,MsgStr[MsgNumber]);
	p_aux=Display1+str2len(Display1);
	itoa(p_aux,*Value,dot);
	Aux=MsgBox2(0, 1, 1);
	while((Aux==1||Aux==2)&&!Tout()){
			Edit_Value((int32_t *)Value, sign, dot);
			strcpy(Display1,MsgStr[MsgNumber]);
			p_aux=Display1+str2len(Display1);
			itoa(p_aux,*Value,dot);
			Aux=MsgBox2(0, 1, 1);
	}
}

void Edit_Temperatures(int32_t MsgNumber, int32_t *Value){
	
	int32_t Result, Aux;
	
	Aux=(W1209PY.fahrenheit)?To_fahrenheit(*Value):To_Celsius(*Value);
	p_aux=(unsigned char *)MsgStr[MsgNumber];
	strcpy(Display1,p_aux);
	p_aux=Display1+str2len(Display1);
	itoa(p_aux,Aux,2);
	p_aux=Display1+str2len(Display1);
	if(W1209PY.fahrenheit){ 
							  strcpy(p_aux,MsgStr[29]);
						  }else {
							  strcpy(p_aux,MsgStr[30]);
						  }
	Result=MsgBox2(0, 1, 1);
	while((Result==1||Result==2)&&!Tout()){
		Edit_Value((int32_t *)&Aux, 1, 2);
		Aux=(W1209PY.fahrenheit)?From_fahrenheit(Aux):From_Celsius(Aux);
		Aux=(Aux>0)?Aux:0;
		Aux=(W1209PY.fahrenheit)?To_fahrenheit(Aux):To_Celsius(Aux);
		strcpy(Display1,MsgStr[MsgNumber]);
		p_aux=Display1+str2len(Display1);
		itoa(p_aux,Aux,2);
		p_aux=Display1+str2len(Display1);
		if(W1209PY.fahrenheit){
								strcpy(p_aux,MsgStr[29]); 
							  }else {
							    strcpy(p_aux,MsgStr[30]);
							  }
		Result=MsgBox2(0, 1, 1);
	}
	*Value=(W1209PY.fahrenheit)?From_fahrenheit(Aux):From_Celsius(Aux);
}

void Update_Beta(){
	NTC.Beta=(int64_t)(NTC.Log_R1-NTC.Log_R0)*(int64_t)(NTC.T0K*NTC.T1K)/((int32_t)(NTC.T0K-NTC.T1K));
	NTC.Beta_T0K=(NTC.Beta)/NTC.T0K;
}

void Edit_Temperatures_Simple(int32_t *Value, int32_t Delta){
	int32_t Aux;
	
	Aux=(W1209PY.fahrenheit)?To_fahrenheit(*Value):To_Celsius(*Value);
	p_aux=Display1;
	itoa(p_aux,Aux,2);
	Put_Buff_On_Display(Display1,0);
	Timeout=0;
	while(!Pressed_BSet&&!Tout()){					
		if(Pressed_BMinus&&Pressed_BPlus){
				Aux=0;
				itoa(p_aux,Aux,2);
				Put_Buff_On_Display(Display1,0);
				Pressed_BMinus=Pressed_BPlus=0;
		}
		if (((Pressed_BPlus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BPlus>>2)))))||(Pressed_BPlus==1))
			{
				if(Pressed_BPlus==1) Pressed_BPlus=0;
				Aux+=Delta;
				itoa(p_aux,Aux,2);
				Put_Buff_On_Display(Display1,0);
				Tick_Timer++;
			} 
									
		if (((Pressed_BMinus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BMinus>>2)))))||(Pressed_BMinus==1))
			{
				if(Pressed_BMinus==1) Pressed_BMinus=0;
				Aux-=Delta;
				itoa(p_aux,Aux,2);
				Put_Buff_On_Display(Display1,0);
				Tick_Timer++;
			}
		if(Aux>99900) Aux=99900;
		if(Aux<(W1209PY.fahrenheit?-46957:-_100Kelvin)) Aux=(W1209PY.fahrenheit?-46957:-_100Kelvin);
	}
	*Value=(W1209PY.fahrenheit)?From_fahrenheit(Aux):From_Celsius(Aux);
	Pressed_BSet=0;	
}


void Align_Right(unsigned char *Str){
	int32_t Digit_Count;
	while((Digit_Count=str2len(Str))<3){
		for(int __ind=Digit_Count;__ind;__ind--) {	Str[__ind+1]=Str[__ind]; Str[__ind]=Str[__ind-1]; }
		Str[0]=' ';
		}
}
void Edit_Value_Simple(int32_t *Value, int32_t Highest, int32_t Lowest, int32_t dot){
	
	int32_t Aux=*Value;
	
	p_aux=Display1;
	itoa(p_aux,Aux,dot);
	Align_Right(Display1);
	Put_Buff_On_Display(Display1,0);
	
	for (Timeout=0;!Pressed_BSet&&!Tout();){					
		if(Pressed_BMinus&&Pressed_BPlus){
				Aux=0;
				itoa(p_aux,Aux,dot);
				Align_Right(Display1);
				Put_Buff_On_Display(Display1,0);
				Pressed_BMinus=Pressed_BPlus=0;
		}
		if (((Pressed_BPlus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BPlus>>2)))))||(Pressed_BPlus==1))
			{
				if(Pressed_BPlus==1) Pressed_BPlus=0;
				Aux=((++Aux)>Highest)?Highest:Aux;
				itoa(p_aux,Aux,dot);
				Align_Right(Display1);
				Put_Buff_On_Display(Display1,0);
				Tick_Timer++;
			} 
									
		if (((Pressed_BMinus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BMinus>>2)))))||(Pressed_BMinus==1))
			{
				if(Pressed_BMinus==1) Pressed_BMinus=0;
				Aux=((--Aux)<Lowest)?Lowest:Aux;
				itoa(p_aux,Aux,dot);
				Align_Right(Display1);
				Put_Buff_On_Display(Display1,0);
				Tick_Timer++;
			}
	}
	*Value=Aux;
	Pressed_BSet=0;	
}
	
	
void Debug_Display(int32_t value){
			itoa(Display1,value,0);
			Put_Buff_On_Display(Display1,0);
			while(!Pressed_BSet);
			Pressed_BSet=0;
}
void Prepare_Boundaries(){
	int32_t Aux=(int32_t)W1209PY.Hysteresis-_100Kelvin;
	if(W1209PY.Function!=1) {
		if(Aux>=0){ Turn_On_Temp=(int32_t)W1209PY.Target-Aux; Turn_Off_Temp=W1209PY.Target; } else { Turn_On_Temp=W1209PY.Target; Turn_Off_Temp=(int32_t)W1209PY.Target-Aux; }
		
	} else if(W1209PY.Function==1){ 
		if(Aux>=0){ Turn_Off_Temp=(int32_t)W1209PY.Target-Aux; Turn_On_Temp=W1209PY.Target; } else { Turn_Off_Temp=W1209PY.Target; Turn_On_Temp=(int32_t)W1209PY.Target-Aux; }
	}
	Action_Delay_On=W1209PY.Delay_On*2000;
	Action_Delay_Off=W1209PY.Delay_Off*2000;
}

uint32_t Calculate_CRC(struct Params *__NTC, struct Params2 *__W1209PY, int Set_CRC)
{
	uint32_t *p=(uint32_t *)__NTC;
	int idx;
	
	LL_CRC_ResetCRCCalculationUnit(CRC);
	for(idx=0;idx<(sizeof(struct Params)>>2);idx++) LL_CRC_FeedData32(CRC,p[idx]);
	for(idx=0, p=(uint32_t *)__W1209PY;idx<((sizeof(struct Params2)>>2)-1);idx++) LL_CRC_FeedData32(CRC,p[idx]);
	if(Set_CRC) { __W1209PY->Checksum=LL_CRC_ReadData32(CRC); return 0;}
	else return LL_CRC_ReadData32(CRC);
}

uint32_t Are_Settings_OK(uint32_t *p){
	uint32_t My_Checksum;
	struct Params *__NTC=(struct Params *)(p+1);
	struct Params2 *__W1209PY=(struct Params2 *)(p+1+(sizeof(struct Params)>>2));
	if((*p)==0xFFFFFFFF) return PROFILE_EMPTY;
	My_Checksum=Calculate_CRC(__NTC,__W1209PY,Get_Checksum);
	if(My_Checksum==__W1209PY->Checksum){ 
			if((*p)==0xDEADBEEF) return PROFILE_ACTIVE;
			if((*p)==0xDEAD0000) return PROFILE_INACTIVE;
	}
	return PROFILE_BAD;	
}

uint32_t Lookfor_Profiles(){
	uint32_t idx, status=0;
	
	for(idx=0;idx<5;idx++) if((Profile_Statuses[idx]=Are_Settings_OK((uint32_t *)Profiles[idx]))==PROFILE_ACTIVE) {
		//context_address=Profiles[idx];
		Active_Profile=idx;
		status=1;
	}
	return status;
}

static uint32_t FLASH_Program_Page(uint32_t Address, uint32_t * DataAddress)
{

  uint8_t index=0;
  uint32_t dest = Address;
  uint32_t * src = DataAddress;
  uint32_t primask_bit;
  SET_BIT(FLASH->CR, FLASH_CR_EOPIE);
  SET_BIT(FLASH->CR, FLASH_CR_PG);
  /* Enter critical section */
  primask_bit = __get_PRIMASK();
  __disable_irq();
  /* 32 words*/
  while(index<32U)
  {
    *(uint32_t *)dest = *src;
    src += 1U;
    dest += 4U;
    index++;
    if(index==31)
    {
      SET_BIT(FLASH->CR, FLASH_CR_PGSTRT);
    }
  }
  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
  while((FLASH->SR)&FLASH_SR_BSY) {MsDelay(1);}
  if(!((FLASH->SR)&(FLASH_SR_EOP))) return ERROR;
  CLEAR_BIT(FLASH->SR, FLASH_SR_EOP);
  CLEAR_BIT(FLASH->CR, FLASH_CR_EOPIE);
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
  return SUCCESS;
}

#define HSI_24M_EPPARA0	(*((uint32_t *)0x1FFF011C))
#define HSI_24M_EPPARA1	(*((uint32_t *)0x1FFF0120))
#define HSI_24M_EPPARA2	(*((uint32_t *)0x1FFF0124))
#define HSI_24M_EPPARA3	(*((uint32_t *)0x1FFF0128))
#define HSI_24M_EPPARA4	(*((uint32_t *)0x1FFF012C))

void Flash_Config(){
	
	__IO uint32_t F_TS0=(HSI_24M_EPPARA0)&0x000001FF;
	__IO uint32_t F_TS3=(HSI_24M_EPPARA0>>9)&0x000001FF;
	__IO uint32_t F_TS1=(HSI_24M_EPPARA0>>18)&0x000003FF;
	__IO uint32_t F_TS2P=(HSI_24M_EPPARA1)&0x000001FF;
	__IO uint32_t F_TPS3=(HSI_24M_EPPARA1>>16)&0x00000FFF;
	__IO uint32_t F_PERTPE=(HSI_24M_EPPARA2)&0x0003FFFF;
	__IO uint32_t F_SMERTPE=(HSI_24M_EPPARA3)&0x0003FFFF;
	__IO uint32_t F_PRGTPE=(HSI_24M_EPPARA4)&0x0000FFFF;
	__IO uint32_t F_PRETPE=(HSI_24M_EPPARA4>>16)&0x0003FFF;
	
	if(FLASH->TS0!=F_TS0) FLASH->TS0=F_TS0;
	if(FLASH->TS3!=F_TS3) FLASH->TS3=F_TS3;
	if(FLASH->TS1!=F_TS1) FLASH->TS1=F_TS1;
	if(FLASH->TS2P!=F_TS2P) FLASH->TS2P=F_TS2P;
	if(FLASH->TPS3!=F_TPS3) FLASH->TPS3=F_TPS3;
	if(FLASH->PERTPE!=F_PERTPE) FLASH->PERTPE=F_PERTPE;
	if(FLASH->SMERTPE!=F_SMERTPE) FLASH->SMERTPE=F_SMERTPE;
	if(FLASH->PRGTPE!=F_PRGTPE) FLASH->PRGTPE=F_PRGTPE;
	if(FLASH->PRETPE!=F_PRETPE) FLASH->PRETPE=F_PRETPE;
}

void Unselect_Profile(uint32_t Profile_Address){
	
	uint32_t idx;
	uint32_t Profile_Mask[32];
	
	if(*((uint32_t *)Profile_Address)==0xFFFFFFFF) return;
	for(idx=1, Profile_Mask[0]=0xdead0000; idx<32 ; idx++) Profile_Mask[idx]=0xFFFFFFFF; 
	LL_FLASH_Unlock();
	if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) == SUCCESS) 	if(FLASH_Program_Page(Profile_Address, Profile_Mask)== ERROR) MsgBox((unsigned char *)MsgStr[74],0,1);
	FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
	LL_FLASH_Lock();	
}

#define Select_Profile(a) context_address=a

void Save_Context(uint32_t Profile_Address){
	
	uint32_t idx, *p=(uint32_t *)&NTC, *sp=(uint32_t *)Profile_Address;
	uint32_t Params_Buffer[32];
	while((FLASH->SR)&FLASH_SR_BSY);
	
	LL_FLASH_Unlock();
	for(idx=0;idx<32;idx++) Params_Buffer[idx]=0;
	for(idx=0;idx<(sizeof(struct Params)>>2);idx++) Params_Buffer[idx+1]=p[idx];
	for(idx=0, p=(uint32_t *)&W1209PY;idx<(sizeof(struct Params2)>>2);idx++) Params_Buffer[idx+1+(sizeof(struct Params)>>2)]=p[idx];
	
	Calculate_CRC((struct Params *)(&Params_Buffer[1]),(struct Params2 *)&Params_Buffer[1+(sizeof(struct Params)>>2)],Set_Checksum);
	Params_Buffer[0]=0xdeadbeef;

	if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) == SUCCESS)   FLASH_PageErase(Profile_Address); 
	for(idx=0;(idx<32)&&(sp[idx]==0xFFFFFFFF);idx++);
	if(idx<32) {
				MsgBox((unsigned char *)MsgStr[74],0,1);
				LL_FLASH_Lock();
				return;
	}
	
	if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) == SUCCESS)	if(FLASH_Program_Page(Profile_Address,Params_Buffer)== ERROR) MsgBox((unsigned char *)MsgStr[74],0,1);
	FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
	LL_FLASH_Lock();	
}

void Load_Context(){
	uint32_t idx, *p=(uint32_t *)&NTC, *Stored_Params=(uint32_t *)Profiles[Active_Profile];
	for(idx=0;idx<(sizeof(struct Params)>>2);idx++) p[idx]=Stored_Params[idx+1];
	for(idx=0, p=(uint32_t *)&W1209PY;idx<(sizeof(struct Params2)>>2);idx++) p[idx]=Stored_Params[idx+1+(sizeof(struct Params)>>2)];
	Calculate_CRC((struct Params *)&NTC,(struct Params2 *)&W1209PY,Set_Checksum);
	MsgStr=W1209PY.Language?(unsigned char **)MsgStrSp:(unsigned char **)MsgStrEn;
}

void Erase_Profiles()
{
	LL_FLASH_Unlock();
	for(uint32_t idx=0;idx<5;idx++) if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) == SUCCESS)   FLASH_PageErase(Profiles[idx]);
	FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
	LL_FLASH_Lock();
	Active_Profile=1;
	for(uint32_t idx=0;idx<5;idx++) Profile_Statuses[idx]=3;
}

uint32_t Are_Settings_Changed(){
	
	uint32_t idx, *Stored_Params=(uint32_t *)Profiles[Active_Profile];
	struct Params *__NTC=(struct Params *)(Params_Buffer+1);
	struct Params2 *__W1209PY=(struct Params2 *)(Params_Buffer+1+(sizeof(struct Params)>>2));
	
	for(idx=1;idx<32;idx++) Params_Buffer[idx]=Stored_Params[idx];
	if(__NTC->R_Pullup!=NTC.R_Pullup) return 1;
	if(__NTC->R0!=NTC.R0) return 1;
	if(__NTC->R1!=NTC.R1) return 1;
	if(__NTC->Log_R0!=NTC.Log_R0) return 1;
	if(__NTC->Log_R1!=NTC.Log_R1) return 1;
	if(__NTC->T0K!=NTC.T0K) return 1;
	if(__NTC->T1K!=NTC.T1K) return 1;
	if(__NTC->Beta!=NTC.Beta) return 1;
	if(__NTC->Beta_T0K!=NTC.Beta_T0K) return 1;
	if(__NTC->R_Wires!=NTC.R_Wires) return 1;

	
	if(__W1209PY->Function!=W1209PY.Function) return 1;
	if(__W1209PY->Temp_Roll!=W1209PY.Temp_Roll) return 1;
	if(__W1209PY->Alarm1!=W1209PY.Alarm1) return 1;
	if(__W1209PY->Alarm2!=W1209PY.Alarm2) return 1;
	if(__W1209PY->Alarm1_TempK100!=W1209PY.Alarm1_TempK100) return 1;
	if(__W1209PY->Alarm2_TempK100!=W1209PY.Alarm2_TempK100) return 1;
	if(__W1209PY->fahrenheit!=W1209PY.fahrenheit) return 1;
	if(__W1209PY->Messages!=W1209PY.Messages) return 1;
	if(__W1209PY->Delay_Type!=W1209PY.Delay_Type) return 1;
	if(__W1209PY->Delay_Toff!=W1209PY.Delay_Toff) return 1;
	if(__W1209PY->Delay_Ton!=W1209PY.Delay_Ton) return 1;
	if(__W1209PY->Timer_Enable!=W1209PY.Timer_Enable) return 1;
	if(__W1209PY->Target!=W1209PY.Target) return 1;
	if(__W1209PY->Hysteresis!=W1209PY.Hysteresis) return 1;
	if(__W1209PY->Temp_Offset!=W1209PY.Temp_Offset) return 1;
	if(__W1209PY->Delay_On!=W1209PY.Delay_On) return 1;
	if(__W1209PY->Delay_Off!=W1209PY.Delay_Off) return 1;
	if(__W1209PY->Max_TempK100!=W1209PY.Max_TempK100) return 1;
	if(__W1209PY->Min_TempK100!=W1209PY.Min_TempK100) return 1;
	if(__W1209PY->Alarm1_TempK100!=W1209PY.Alarm1_TempK100) return 1;
	if(__W1209PY->Alarm2_TempK100!=W1209PY.Alarm2_TempK100) return 1;
	if(__W1209PY->Filter_Depth!=W1209PY.Filter_Depth) return 1;
	if(__W1209PY->Bits_Extended!=W1209PY.Bits_Extended) return 1;
	if(__W1209PY->Language!=W1209PY.Language) return 1;
	if(__W1209PY->Sound!=W1209PY.Sound) return 1;
	if(__W1209PY->ADC_Offset!=W1209PY.ADC_Offset) return 1;
	return 0;
}


int main(void)
{
  Default_Params(); 
  Prepare_Boundaries();
  Flash_Config();
  MsgStr=(unsigned char **)MsgStrEn;
  /* Configure HSI as Systemclock source */
  System_Clock_Config();
  
  // check if PC0 is SWD
  if (READ_BIT(FLASH->OPTR, OB_USER_SWD_NRST_MODE) != OB_SWD_PC0_GPIO_PB6)
  {
	// Write option byte 
    FlashSetOptionByte();
	// Reset system for normal boot 
	NVIC_SystemReset();
  }
  
  GPIO_Config(); // Setup Display and button + GPIO pins. 
  
  // Here the MCU checks for entering programming mode, that is, to not disable SWD/SWC 
  if(!LL_GPIO_IsInputPinSet(GPIOA, Button_Plus)) {
		Flashing_Flag=1;
		p_aux=(unsigned char*)(MsgStr[1]);
		strcpy(Display1,p_aux);
		p_roll=Display;
		Put_Buff_On_Display(p_roll,0);
		while (1)
		 {
			if(Roll_Flag&&Display1[3]!=0){
			Roll_Flag=0;
			Put_Buff_On_Display(p_roll++,0);
			if(!(*p_roll)) p_roll=Display;
			}
		 }
  }
  /* Continue the program flow */
  GPIO_Config_2();
  
  ADCConfig();
  
  MsDelay(10);
  
  ADCCalibrate();
  
  MsDelay(10);
  
  LL_ADC_Enable(ADC1);
  
  MsDelay(1);
    
  if(Lookfor_Profiles()) Load_Context(); else {
      Default_Params(); 
	  MsgBox((unsigned char *)MsgStr[69],0,0);		  
	do{
	  Aux2=0;
	  Aux=1;
	  MsgBox((unsigned char *)MsgStr[70],0,0);		  	  
	  Edit_Value_Simple(&Aux,5,1,0);
	  if(Profile_Statuses[Aux-1]) {
		  Aux2=1;
		}else {
			MsgBox((unsigned char *)MsgStr[71],0,0);		  	  
			while((Pressed_BSet!=1)&&!Tout()) Aux2=DisplayOptions(31,Aux2,2);
			Pressed_BSet=Timeout=0;
		}
	}while(!Aux2);
	Active_Profile=Aux-1;
	Profile_Statuses[Active_Profile]=PROFILE_ACTIVE;
	Save_Context(Profiles[Active_Profile]);
  }
  MsgStr=W1209PY.Language?(unsigned char **)MsgStrSp:(unsigned char **)MsgStrEn;
  
  // for(int indx=0;indx<5;indx++) Debug_Display(Profile_Statuses[indx]);
  
  MsgBox((unsigned char*)MsgStr[0],3,0);
  
  Prepare_Boundaries();
  
  ADC_Ready=1;
  p_roll=Display;
  while(!Analog_Flag);
  Get_Temperature(Put_Temp_On_Buffer);
  
  while (1)
   {
    if(Roll_Flag){
					if(W1209PY.Sensor_State) for(p_roll=Display;W1209PY.Sensor_State;){
						strcpy(Display1, MsgStr[66+W1209PY.Sensor_State]);
						Put_Buff_On_Display(p_roll++,0);
						if(!(*p_roll)) p_roll=Display;
						if(System_Ready){
								System_Ready=0;
								LL_GPIO_ResetOutputPin(GPIOB, Relay_Out);
								p_roll=Display;
								Flashing_Flag=1;
							}
						for(Roll_Flag=0;!Roll_Flag;);
						Cleaning=1;
						}
					
					Roll_Flag=0;
					if(W1209PY.Timer_Enable&&Tout()){
						Timeout=0;
						if(W1209PY.TM_Seconds||W1209PY.TM_Minutes||W1209PY.TM_Days||W1209PY.TM_Hours) {
							
							strcpy(Display1,MsgStr[58]);		
							
							if(W1209PY.TM_Days){								
								itoa(Display1+str2len(Display1), W1209PY.TM_Days,0);
								p_aux=Display1+str2len(Display1);
								strcpy(p_aux," ");
								p_aux=Display1+str2len(Display1);
								strcpy(p_aux,MsgStr[59]);
								p_aux=Display1+str2len(Display1)-2;
								if(W1209PY.TM_Days==1) {strcpy(p_aux," ")};
							}
							if(W1209PY.TM_Days||W1209PY.TM_Hours){
								itoa(Display1+str2len(Display1), W1209PY.TM_Hours,0);
								p_aux=Display1+str2len(Display1);
								strcpy(p_aux," ");
								p_aux=Display1+str2len(Display1);
								strcpy(p_aux,MsgStr[60]);
								p_aux=Display1+str2len(Display1)-2;
								if(W1209PY.TM_Hours==1) {strcpy(p_aux," ")};
							}
							if(W1209PY.TM_Days||W1209PY.TM_Hours||W1209PY.TM_Minutes){
								itoa(Display1+str2len(Display1), W1209PY.TM_Minutes,0);
								p_aux=Display1+str2len(Display1);
								strcpy(p_aux," ");
								p_aux=Display1+str2len(Display1);
								strcpy(p_aux,MsgStr[45]);
								p_aux=Display1+str2len(Display1)-2;
								if(W1209PY.TM_Minutes==1) {strcpy(p_aux," ")};
							}
							itoa(Display1+str2len(Display1), W1209PY.TM_Seconds,0);
							p_aux=Display1+str2len(Display1);
							strcpy(p_aux," ");
							p_aux=Display1+str2len(Display1);
							strcpy(p_aux,MsgStr[46]);	
							p_aux=Display1+str2len(Display1)-2;
							if(W1209PY.TM_Seconds==1) {strcpy(p_aux," ")};
							
							MsgBox2(0,0, 2);
						} else MsgBox((unsigned char*)MsgStr[61],0,0);
						Timeout=0;
						if(W1209PY.Temp_Roll) {p_roll=Display; Cleaning=1;}
					}
					
					if(Cleaning){Cleaning=0; Get_Temperature(Put_Temp_On_Buffer);}
					
					if(W1209PY.Temp_Roll||Alarm_Flag) Put_Buff_On_Display(p_roll++,0); else Put_Buff_On_Display(Display1,0);
					
					if(((((NTC.Temp<W1209PY.Alarm1_TempK100)&&(W1209PY.Alarm1_TempK100>=Max(Turn_Off_Temp,Turn_On_Temp)))|| \
					    ((NTC.Temp>W1209PY.Alarm1_TempK100)&&(W1209PY.Alarm1_TempK100<=Min(Turn_Off_Temp,Turn_On_Temp))))||(!W1209PY.Alarm1))&& \
					   ((((NTC.Temp<W1209PY.Alarm2_TempK100)&&(W1209PY.Alarm2_TempK100>=Max(Turn_Off_Temp,Turn_On_Temp)))|| \
					    ((NTC.Temp>W1209PY.Alarm2_TempK100)&&(W1209PY.Alarm2_TempK100<=Min(Turn_Off_Temp,Turn_On_Temp))))||(!W1209PY.Alarm2))) {
						Alarm_Flag=0;
						if(((!W1209PY.Timer_Enable)||W1209PY.TM_Seconds||W1209PY.TM_Minutes||W1209PY.TM_Days||W1209PY.TM_Hours)&&(!W1209PY.Sensor_State))	System_Ready=1;
						Flashing_Flag=0;
						if((Analog_Flag)&&!(W1209PY.Temp_Roll&&*p_roll)) Get_Temperature(Put_Temp_On_Buffer); 
							else if(Analog_Flag) Get_Temperature(Dont_Put_Temp_On_Buffer);								
					} else { 
						
						if(!Alarm_Flag) {
								System_Ready=0;
								LL_GPIO_ResetOutputPin(GPIOB, Relay_Out);
								p_roll=Display;
								Flashing_Flag=1;
						}
						if(Analog_Flag) Get_Temperature(Dont_Put_Temp_On_Buffer);
						
						Alarm_Flag=0;
						if(W1209PY.Alarm1){
							if((NTC.Temp>=W1209PY.Alarm1_TempK100)&&(W1209PY.Alarm1_TempK100>=Max(Turn_Off_Temp,Turn_On_Temp)))  Alarm_Flag|=1;
							if((NTC.Temp<=W1209PY.Alarm1_TempK100)&&(W1209PY.Alarm1_TempK100<=Min(Turn_Off_Temp,Turn_On_Temp))) Alarm_Flag|=4;
						}
						if(W1209PY.Alarm2){
							if((NTC.Temp>=W1209PY.Alarm2_TempK100)&&(W1209PY.Alarm2_TempK100>=Max(Turn_Off_Temp,Turn_On_Temp))) Alarm_Flag|=2;
							if((NTC.Temp<=W1209PY.Alarm2_TempK100)&&(W1209PY.Alarm2_TempK100<=Min(Turn_Off_Temp,Turn_On_Temp))) Alarm_Flag|=8;
						}
						switch(Alarm_Flag){
							case 1:
									{strcpy(Display1, MsgStr[51])}
									break;
							case 2: 
									{strcpy(Display1, MsgStr[53])}
									break;
							case 3:
									if(W1209PY.Alarm2_TempK100>=W1209PY.Alarm1_TempK100) { strcpy(Display1, MsgStr[53]) } else { strcpy(Display1, MsgStr[51]) }
									break;
							case 4: 
									{strcpy(Display1, MsgStr[50])}
									break;
							case 8:
									{strcpy(Display1, MsgStr[52])}
									break;
							case 12:
									if(W1209PY.Alarm2_TempK100<=W1209PY.Alarm1_TempK100) { strcpy(Display1, MsgStr[52]) } else { strcpy(Display1, MsgStr[50]) }
									break;
							default:
									Alarm_Flag=0;
									break;
						}
						if(Alarm_Flag) Display1[1]|=_DP;
						if(Alarm_Flag&&!Beeper_Flag) Beeper_Flag=1;  else if(!Alarm_Flag&&Beeper_Flag)
							{
								Beeper_Flag=0;
								LL_GPIO_ResetOutputPin(GPIOA, Beeper);								
							}
					}
		
					if(!(*p_roll)) p_roll=Display;
				}

	if(Pressed_BSet==3) {
							Pressed_BSet=0;
							Flashing_Flag=0;
							Display_Menu();
							while((Pressed_BSet!=3)&&!Tout()){
								if (((Pressed_BPlus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BPlus>>2)))))||(Pressed_BPlus==1))
									{
										if(Pressed_BPlus==1) Pressed_BPlus=0;
										Program_Menu+=(Program_Menu==30)?0:1;
										Display_Menu();
										Tick_Timer++;
									} 
									
								if (((Pressed_BMinus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BMinus>>2)))))||(Pressed_BMinus==1))
									{
										if(Pressed_BMinus==1) Pressed_BMinus=0;
										Program_Menu-=(Program_Menu==0)?0:1;
										Display_Menu();
										Tick_Timer++;
									}
								if(Pressed_BSet==1){
									Pressed_BSet=0;
									switch(Program_Menu){
												case 0:
														MsgBox((unsigned char *)MsgStr[4],0,0);	
															Aux=(W1209PY.Function==3);
															Aux2=Aux?2:W1209PY.Function;
														while((Pressed_BSet!=1)&&!Tout()) {														
															Aux2=DisplayOptions(25,Aux2,3);
															W1209PY.Function=Aux2|((Aux2==2)?Aux:0);
														}
														Pressed_BSet=Timeout=0;
														if(W1209PY.Function>=2){
															while((Pressed_BSet!=1)&&!Tout()){
																Aux=DisplayOptions(63, Aux, 2);
																W1209PY.Function=(W1209PY.Function&2)|Aux;	
															}													
														}
														break;
												case 1:
														if(W1209PY.Function<2) MsgBox((unsigned char *)MsgStr[6],0,0); else MsgBox((unsigned char *)MsgStr[65],0,0);
														Aux=W1209PY.Hysteresis-(W1209PY.fahrenheit?1778:0);
														Edit_Temperatures_Simple((int32_t*)&Aux, 1);
														W1209PY.Hysteresis=Aux+(W1209PY.fahrenheit?1777:0);
														break;
												case 2:
														MsgBox((unsigned char *)MsgStr[7],0,0);
														Edit_Temperatures_Simple((int32_t*)&W1209PY.Max_TempK100, 10);
														break;
												case 3:
														MsgBox((unsigned char *)MsgStr[8],0,0);
														Edit_Temperatures_Simple((int32_t*)&W1209PY.Min_TempK100, 10);
														break;
												case 4:
														MsgBox((unsigned char *)MsgStr[9],0,0);
														Aux=W1209PY.Temp_Offset-(W1209PY.fahrenheit?1778:0);
														Edit_Temperatures_Simple((int32_t*)&Aux, 1);
														W1209PY.Temp_Offset=Aux+(W1209PY.fahrenheit?1777:0);
														break;
												case 5:
														MsgBox((unsigned char *)MsgStr[10],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Delay_Type=DisplayOptions(45,W1209PY.Delay_Type,2);
														break;
												case 6:
														MsgBox((unsigned char *)MsgStr[11],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Alarm1=DisplayOptions(33,W1209PY.Alarm1,2);
														Pressed_BSet=Timeout=0;
														if(W1209PY.Alarm1) Edit_Temperatures_Simple((int32_t*)&W1209PY.Alarm1_TempK100, 10);
														break;
												case 7:
														MsgBox((unsigned char *)MsgStr[49],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Alarm2=DisplayOptions(33,W1209PY.Alarm2,2);
														Pressed_BSet=Timeout=0;
														if(W1209PY.Alarm2) Edit_Temperatures_Simple((int32_t*)&W1209PY.Alarm2_TempK100, 10);
														break;
												case 8:
														MsgBox((unsigned char *)MsgStr[12],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.fahrenheit=DisplayOptions(35,W1209PY.fahrenheit,2);
														break;
												case 9:
														MsgBox((unsigned char *)MsgStr[13],0,0);
														Aux=0;
														while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
														Pressed_BSet=Timeout=0;
														if(Aux==1) {
															Default_Params();
															MsgBox((unsigned char *)MsgStr[75],0,0);
															Aux=0;
															while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
															Pressed_BSet=Timeout=0;
															if(Aux==1) Erase_Profiles(); 
															MsgStr=W1209PY.Language?(unsigned char **)MsgStrSp:(unsigned char **)MsgStrEn;
														}
														break;
												case 10:
														Aux=W1209PY.Timer_Enable;
														W1209PY.Timer_Enable=0;
														MsgBox((unsigned char *)MsgStr[54],0,0);
														while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
														Pressed_BSet=Timeout=0;
														if(Aux){
															MsgBox((unsigned char *)MsgStr[55],0,0);
															BF_Edit=(int32_t)W1209PY.TM_Days;
															Edit_Value_Simple((int32_t*)&BF_Edit, 999, 0, 0);
															W1209PY.TM_Days=BF_Edit;
															Pressed_BSet=Timeout=0;
															MsgBox((unsigned char *)MsgStr[56],0,0);
															BF_Edit=(int32_t)W1209PY.TM_Hours;
															Edit_Value_Simple((int32_t*)&BF_Edit, 23, 0, 0);
															W1209PY.TM_Hours=BF_Edit;
															Pressed_BSet=Timeout=0;
															MsgBox((unsigned char *)MsgStr[57],0,0);
															BF_Edit=(int32_t)W1209PY.TM_Minutes;
															Edit_Value_Simple((int32_t*)&BF_Edit, 59, 0, 0);
															W1209PY.TM_Minutes=BF_Edit;
															
															W1209PY.TM_Seconds=0;
														}
														System_Ready=1;
														W1209PY.Timer_Enable=Aux;
														break;
												case 11:
														MsgBox((unsigned char *)MsgStr[14],0,0);
														Edit_Temperatures(28,(int32_t*)&W1209PY.Target);
														break;
												case 12:
														MsgBox((unsigned char *)MsgStr[15],0,0);
														Edit_Resistors(62,(int32_t *)&NTC.R_Pullup);
														break;
												case 13:
														MsgBox((unsigned char *)MsgStr[16],0,0);
														Edit_Resistors(37,(int32_t *)&NTC.R0);
														NTC.Log_R0=log_q31(NTC.R0);														
														break;
												case 14:
														MsgBox((unsigned char *)MsgStr[17],0,0);
														Edit_Temperatures(38,(int32_t*)&NTC.T0K);
														NTC.Beta_T0K=(NTC.Beta)/NTC.T0K;
														break;
												case 15:
														MsgBox((unsigned char *)MsgStr[18],0,0);
														Aux=(NTC.Beta>>26);
														Edit_Scalars(39,(int32_t *)&Aux,1,2);
														NTC.Beta=(int64_t)Aux<<26;
														NTC.Beta_T0K=(NTC.Beta)/NTC.T0K;
														MsgBox((unsigned char *)MsgStr[42],0,0);
														Aux=0;
														while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
														if(Aux){
															int64_t Beta_T1K=(NTC.Beta/NTC.T1K);
															NTC.Log_R1=NTC.Log_R0+(Beta_T1K-NTC.Beta_T0K);
															NTC.R1=intexp(NTC.Log_R1+0x55F3439C);
														}
														break;
												case 16:
														MsgBox((unsigned char *)MsgStr[19],0,0);
														Edit_Resistors(40,(int32_t *)&NTC.R1);
														NTC.Log_R1=log_q31(NTC.R1);		
														MsgBox((unsigned char *)MsgStr[43],0,0);
														Aux=0;
														while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
														if(Aux) Update_Beta();
														break;
												case 17:
														MsgBox((unsigned char *)MsgStr[20],0,0);
														Edit_Temperatures(41,(int32_t*)&NTC.T1K);
														MsgBox((unsigned char *)MsgStr[43],0,0);
														Aux=0;
														while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
														if(Aux) Update_Beta();
														break;
												case 18:
														MsgBox((unsigned char *)MsgStr[21],0,0);
														Aux=0;
														while((Pressed_BSet!=1)) { Aux=DisplayOptions(31,Aux,2); Timeout=0;}
														if(Aux){
															NTC.R0=ADC_to_NTC(Analog_Read);
															NTC.Log_R0=Log_ADC_to_NTC(Analog_Read);
														}
														break;
												case 19:
														MsgBox((unsigned char *)MsgStr[22],0,0);
														Aux=0;
														while((Pressed_BSet!=1)) { Aux=DisplayOptions(31,Aux,2); Timeout=0;}
														if(Aux){
															NTC.R1=ADC_to_NTC(Analog_Read);
															NTC.Log_R1=Log_ADC_to_NTC(Analog_Read);
														}
														break;
												case 20:
														MsgBox((unsigned char *)MsgStr[80],0,0);
														Edit_Value_Simple((int32_t*)&NTC.R_Wires, 999, 0, 0);
														break;
												case 21:
														MsgBox((unsigned char *)MsgStr[47],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Delay_Ton=DisplayOptions(31,W1209PY.Delay_Ton,2);
														Pressed_BSet=Timeout=0;
														if(W1209PY.Delay_Ton){
															if(!W1209PY.Delay_Type) W1209PY.Delay_On/=60;														
															Edit_Value_Simple((int32_t*)&W1209PY.Delay_On, 999, 0, 0);
															if(!W1209PY.Delay_Type) W1209PY.Delay_On*=60;
														}
														break;
												case 22:
														MsgBox((unsigned char *)MsgStr[48],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Delay_Toff=DisplayOptions(31,W1209PY.Delay_Toff,2);
														Pressed_BSet=Timeout=0;
														if(W1209PY.Delay_Toff){
															if(!W1209PY.Delay_Type) W1209PY.Delay_Off/=60;														
															Edit_Value_Simple((int32_t*)&W1209PY.Delay_Off, 999, 0, 0);
															if(!W1209PY.Delay_Type) W1209PY.Delay_Off*=60;
														}
														break;
												case 23:
														MsgBox((unsigned char *)MsgStr[44],0,0);
														Edit_Value_Simple((int32_t*)&W1209PY.Filter_Depth, 19-W1209PY.Bits_Extended, 0, 0);
														Filter_Init(LL_ADC_REG_ReadConversionData12(ADC1), W1209PY.Filter_Depth+W1209PY.Bits_Extended);
														break;
												case 24:
														Aux=W1209PY.Bits_Extended+12;
														MsgBox((unsigned char *)MsgStr[66],0,0);
														Edit_Value_Simple((int32_t*)&Aux, 18, 12, 0);
														W1209PY.Bits_Extended=Aux-12;
														if((W1209PY.Filter_Depth+W1209PY.Bits_Extended)>19) W1209PY.Filter_Depth=19-W1209PY.Bits_Extended;
														Filter_Init(LL_ADC_REG_ReadConversionData12(ADC1), W1209PY.Filter_Depth+W1209PY.Bits_Extended);
														break;
												case 25:
														MsgBox((unsigned char *)MsgStr[81],0,0);
														Edit_Value_Simple((int32_t*)&W1209PY.ADC_Offset, 512, -512, 0);
														break;
												case 26:
														MsgBox((unsigned char *)MsgStr[23],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Temp_Roll=DisplayOptions(31,W1209PY.Temp_Roll,2);
														break;
												case 27:
														MsgBox((unsigned char *)MsgStr[24],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Messages=DisplayOptions(31,W1209PY.Messages,2);
														break;
												case 28:
														MsgBox((unsigned char *)MsgStr[79],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Sound=DisplayOptions(31,W1209PY.Sound,2);
														break;
												case 29:
														MsgBox((unsigned char *)MsgStr[72],0,0);
														Aux=Active_Profile+1;	
														do{
															Aux2=0;																 
															Edit_Value_Simple(&Aux,5,1,0);
															if(Profile_Statuses[Aux-1]) Aux2=1; else {
																	MsgBox((unsigned char *)MsgStr[71],0,0);		  	  
																	while((Pressed_BSet!=1)&&!Tout()) Aux2=DisplayOptions(31,Aux2,2);
																	Pressed_BSet=Timeout=0;
																	}
															}while(!Aux2);
														if(Active_Profile!=Aux-1){
															Unselect_Profile(Profiles[Active_Profile]);
															Profile_Statuses[Active_Profile]=PROFILE_INACTIVE;
															Active_Profile=Aux-1;
															if(Profile_Statuses[Active_Profile]==PROFILE_INACTIVE){
																Aux=1;
																MsgBox((unsigned char *)MsgStr[73],0,0);
																while((Pressed_BSet!=1)&&!Tout()) Aux=DisplayOptions(31,Aux,2);
																Pressed_BSet=Timeout=0;
																if(Aux) Load_Context();
															}
															Profile_Statuses[Active_Profile]=PROFILE_ACTIVE;
															Calculate_CRC((struct Params *)&NTC,(struct Params2 *)&W1209PY, Set_Checksum);
															Save_Context(Profiles[Active_Profile]);
														}
														break;
												case 30:
														MsgBox((unsigned char *)MsgStr[76],0,0);
														while((Pressed_BSet!=1)&&!Tout()) W1209PY.Language=DisplayOptions(77,W1209PY.Language,2);
														Pressed_BSet=Timeout=0;
														MsgStr=W1209PY.Language?(unsigned char **)MsgStrSp:(unsigned char **)MsgStrEn;
														break;
												
												default:
														break;									
									}
									Pressed_BSet=Timeout=0;
									Display_Menu();
									Prepare_Boundaries();
								}
							}
							if(W1209PY.Temp_Roll) {p_roll=Display; Cleaning=1;}
							Alarm_Flag=0;
							Pressed_BSet=0;
							Calculate_CRC((struct Params *)&NTC,(struct Params2 *)&W1209PY, Set_Checksum);
							if(Are_Settings_Changed()) Save_Context(Profiles[Active_Profile]);
						}	
		
	if(Pressed_BPlus==1||Pressed_BMinus==1){
		if(W1209PY.TM_Days||W1209PY.TM_Hours||W1209PY.TM_Minutes||W1209PY.TM_Seconds) Timeout=The10seconds; 
			else if(W1209PY.Timer_Enable) {W1209PY.Timer_Enable=0; Save_Context(Profiles[Active_Profile]);}
	}
	
	if((Pressed_BPlus==3)||(Pressed_BMinus==3)||W1209PY.Fixed_Off||W1209PY.Fixed_On){
		if(Pressed_BMinus) W1209PY.Fixed_Off=1; else if(Pressed_BPlus) W1209PY.Fixed_On=1;
		Pressed_BPlus=Pressed_BMinus=0;
		BK_ADC_Ready=ADC_Ready; BK_System_Ready=System_Ready; BK_Roll_Flag=Roll_Flag; BK_Flashing_Flag=Flashing_Flag; BK_Alarm_Flag=Alarm_Flag; BK_Beeper_Flag=Beeper_Flag;
		Save_Context(Profiles[Active_Profile]);	
		ADC_Ready=System_Ready=Roll_Flag=Alarm_Flag=Beeper_Flag=W1209PY.Timer_Enable=W1209PY.Sound=0;
		Aux=LL_GPIO_IsOutputPinSet(GPIOB, Relay_Out);
		Flashing_Flag=1;
		if(W1209PY.Fixed_Off) {
			LL_GPIO_ResetOutputPin(GPIOB, Relay_Out);
			p_aux=(unsigned char *)MsgStr[33];
		}
		if(W1209PY.Fixed_On)  {
			LL_GPIO_SetOutputPin(GPIOB, Relay_Out);
			p_aux=(unsigned char *)MsgStr[34];
		}
		Put_Buff_On_Display(p_aux,0);
		Anykey=0;
		while(!Anykey);
		ADC_Ready=BK_ADC_Ready; System_Ready=BK_System_Ready; Roll_Flag=BK_Roll_Flag; Flashing_Flag=BK_Flashing_Flag; Alarm_Flag=BK_Alarm_Flag; Beeper_Flag=BK_Beeper_Flag;
		Load_Context(Profiles[Active_Profile]);
		Anykey=Pressed_BSet=Pressed_BMinus=Pressed_BPlus=W1209PY.Fixed_Off=W1209PY.Fixed_On=0;
		Save_Context(Profiles[Active_Profile]);	
		LL_GPIO_SetOutputPin(GPIOB, Relay_Out<<(16*(1-Aux)));
	}
	
	if(Pressed_BSet==1) {
							MsgBox((unsigned char *)MsgStr[2],0,0);
							Pressed_BSet=0;
							Flashing_Flag=1;
							Aux=(W1209PY.fahrenheit)?To_fahrenheit(W1209PY.Target):To_Celsius(W1209PY.Target);
							p_aux=Display1;
							itoa(p_aux,Aux,2);
							Put_Buff_On_Display(Display1,0);
							Timeout=0;
							while(!Pressed_BSet&&!Tout()){					
							
								if (((Pressed_BPlus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BPlus>>2)))))||(Pressed_BPlus==1))
									{
										Flashing_Flag=0;
										if(Pressed_BPlus==1) Pressed_BPlus=0;
										Aux+=10;
										if(Aux>((W1209PY.fahrenheit)?To_fahrenheit(W1209PY.Max_TempK100):To_Celsius(W1209PY.Max_TempK100))) 
										   Aux=(W1209PY.fahrenheit)?To_fahrenheit(W1209PY.Max_TempK100):To_Celsius(W1209PY.Max_TempK100);
										itoa(p_aux,Aux,2);
										Put_Buff_On_Display(Display1,0);
										Tick_Timer++;
									} 
									
								if (((Pressed_BMinus>1)&&(!(Tick_Timer&(0x3FF>>(Pressed_BMinus>>2)))))||(Pressed_BMinus==1))
									{
										Flashing_Flag=0;
										if(Pressed_BMinus==1) Pressed_BMinus=0;
										Aux-=10;
										if(Aux<((W1209PY.fahrenheit)?To_fahrenheit(W1209PY.Min_TempK100):To_Celsius(W1209PY.Min_TempK100))) 
										   Aux=(W1209PY.fahrenheit)?To_fahrenheit(W1209PY.Min_TempK100):To_Celsius(W1209PY.Min_TempK100);
										itoa(p_aux,Aux,2);
										Put_Buff_On_Display(Display1,0);
										Tick_Timer++;
									}
								if(!Pressed_BMinus&&!Pressed_BPlus) Flashing_Flag=1;
								if(Aux>99900) Aux=99900;
								if(Aux<(W1209PY.fahrenheit?-46957:-_100Kelvin)) Aux=(W1209PY.fahrenheit?-46957:-_100Kelvin);
							}
							W1209PY.Target=(W1209PY.fahrenheit)?From_fahrenheit(Aux):From_Celsius(Aux);
							Flashing_Flag=Pressed_BSet=0;	
							if(W1209PY.Temp_Roll) {p_roll=Display; Cleaning=1;}	
							Alarm_Flag=0;
							Prepare_Boundaries();		
							Calculate_CRC((struct Params *)&NTC,(struct Params2 *)&W1209PY, Set_Checksum);
							if(Are_Settings_Changed()) Save_Context(Profiles[Active_Profile]);						
						}
					
						

	if(Calibration_Flag){
			Calibration_Flag=0;
			LL_ADC_Disable(ADC1);
			ADCCalibrate();
			LL_ADC_Enable(ADC1);
	   }	
   }
 
}


static void System_Clock_Config(void)
{
  /* Enable HSI */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Set AHB divider: HCLK = SYSCLK */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* HSISYS used as SYSCLK clock source  */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  /* Set APB1 divider */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  SysTick_Config(12000); // Tick every 0.5ms and enable interrupt

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(24000000);
  
  SET_BIT(RCC->AHBENR, RCC_AHBENR_CRCEN); //Enable CRC peripheral
}

/**
  * @brief  GPIO configuration program
  * @param  None
  * @retval None
  */

static void GPIO_Config_2(void){
	
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    
  LL_GPIO_SetPinSpeed(GPIOA, Button_Minus, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOA, Button_Minus, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinMode(GPIOA, Button_Minus, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinSpeed(GPIOC, Button_Set, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOC, Button_Set, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinMode(GPIOC, Button_Set, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinOutputType(GPIOB, Relay_Out, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, Relay_Out, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOB, Relay_Out, LL_GPIO_PULL_NO);
  LL_GPIO_ResetOutputPin(GPIOB, Relay_Out);
  LL_GPIO_SetPinMode(GPIOB, Relay_Out, LL_GPIO_MODE_OUTPUT);
    
}

static void GPIO_Config(void)
{
  /* Enable GPIO clocks */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  LL_GPIO_SetPinOutputType(GPIOA, Display_P6, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, Display_P6, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOA, Display_P6, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOA, Display_P6, LL_GPIO_MODE_INPUT);
  
  
  LL_GPIO_SetPinSpeed(GPIOA, Button_Plus, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOA, Button_Plus, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinMode(GPIOA, Button_Plus, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinOutputType(GPIOA, Beeper, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, Beeper, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOA, Beeper, LL_GPIO_PULL_NO);
  LL_GPIO_ResetOutputPin(GPIOA, Beeper);
  LL_GPIO_SetPinMode(GPIOA, Beeper, LL_GPIO_MODE_OUTPUT);
  
  
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  
  LL_GPIO_SetPinOutputType(GPIOB, Display_P1, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, Display_P1, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOB, Display_P1, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOB, Display_P1, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinOutputType(GPIOB, Display_P2, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, Display_P2, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOB, Display_P2, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOB, Display_P2, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinOutputType(GPIOB, Display_P3, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, Display_P3, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOB, Display_P3, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOB, Display_P3, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinOutputType(GPIOB, Display_P4, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, Display_P4, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOB, Display_P4, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOB, Display_P4, LL_GPIO_MODE_INPUT);
  
  LL_GPIO_SetPinOutputType(GPIOB, Display_P5, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, Display_P5, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOB, Display_P5, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinMode(GPIOB, Display_P5, LL_GPIO_MODE_INPUT);
  
}

static void FLASH_PageErase(uint32_t PageAddress)
{

  /* Clean the error context */
  pFlash.ErrorCode = LL_FLASH_ERROR_NONE;

  SET_BIT(FLASH->CR, FLASH_CR_PER);
  *(__IO uint32_t *)(PageAddress) = 0xFF;
  while (__LL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0x00U) {MsDelay(1)};
  FLASH->SR = FLASH_FLAG_SR_CLEAR;

}

static void FlashSetOptionByte(void)
{
  FLASH_OBProgramInitTypeDef OBInitCfg;

  LL_FLASH_Unlock();
  LL_FLASH_OB_Unlock();

  OBInitCfg.OptionType = OPTIONBYTE_USER;

  /* 
   * USERType is a combination of
      OB_USER_BOR_EN, OB_USER_BOR_LEV, OB_USER_IWDG_SW, OB_USER_SWD_NRST_MODE OB_USER_IWDG_STOP
  */
  OBInitCfg.USERType = OB_USER_BOR_EN | OB_USER_BOR_LEV | OB_USER_IWDG_SW | OB_USER_SWD_NRST_MODE | OB_USER_IWDG_STOP;
  /*
   * USERConfig is a combination of 
   *  FLASH_OB_USER_BOR_ENABLE: OB_BOR_DISABLE, OB_BOR_ENABLE
   *  FLASH_OB_USER_BOR_LEVEL:  OB_BOR_LEVEL_1p7_1p8, OB_BOR_LEVEL_1p9_2p0, OB_BOR_LEVEL_2p1_2p2, ... OB_BOR_LEVEL_3p1_3p2
   *  FLASH_OB_USER_IWDG_SW:    OB_IWDG_SW, OB_IWDG_HW 
   *  FLASH_OB_USER_IWDG_STOP:  OB_IWDG_STOP_FREEZE, OB_IWDG_STOP_ACTIVE
   *  FLASH_OB_USER_SWD_NRST:   OB_SWD_PB6_NRST_PC0, OB_SWD_PB6_GPIO_PC0, OB_SWD_PC0_GPIO_PB6
   * Default value
   *  OB_BOR_DISABLE
   *  OB_BOR_LEVEL_3p1_3p2
   *  OB_IWDG_SW
   *  OB_IWDG_STOP_ACTIVE
   *  OB_SWD_PB6_NRST_PC0
  */
  OBInitCfg.USERConfig = OB_BOR_DISABLE | OB_BOR_LEVEL_3p1_3p2 | OB_IWDG_SW | OB_IWDG_STOP_ACTIVE | OB_SWD_PC0_GPIO_PB6;
  LL_FLASH_OBProgram(&OBInitCfg);

  LL_FLASH_Lock();
  LL_FLASH_OB_Lock();
  /* Reload option bytes */
  LL_FLASH_OB_Launch();
}

static void ADCConfig(void)
{

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);							 // Enable ADC1 clock
  
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG);				 // Configure PA6 (ADC_IN3) pin in analog input mode 

  LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV16); 							 // Set ADC clock to pclk/8 
  LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B); 							 // Set ADC resolution to 12 bit 
  LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);  						 // ADC conversion data alignment: right aligned
  LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE); 							 // No low power mode
  LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_239CYCLES_5);   // Sampling time 239.5 ADC clock cycles (80us)
  LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE); 					 // ADC regular group conversion, software triggered
  LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE); 					 // Set ADC conversion mode to single mode
  LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN); 				 // ADC in case of overrun: data overwritten 
  LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);   		 // Disable ADC regular group sequencer discontinuous mode  
  LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_3); 						 // Set PA6:ADC_IN3 channel
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE ); //No Vrefint or TB conversion.

  LL_ADC_EnableIT_EOC(ADC1);     // Enable End of conversion interrupt

  NVIC_SetPriority(ADC_COMP_IRQn, 0);
  NVIC_EnableIRQ(ADC_COMP_IRQn);
}

static void ADCCalibrate(void)
{
  uint32_t Timeout = 2; //2ms of timeout

  if (LL_ADC_IsEnabled(ADC1) == 0)  // Make sure the ADC is already disabled.
  {
    LL_ADC_StartCalibration(ADC1); // Start calibration.

    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0){
		if(Timeout-- == 0)
        {
			p_aux=(unsigned char*)(MsgStr[5]);
			strcpy(Display,p_aux);
			p_roll=Display;
			Put_Buff_On_Display(p_aux,0);
			while (1)
			{
				if(Roll_Flag&&Display[3]!=0){
					Roll_Flag=0;
					Put_Buff_On_Display(p_roll++,0);
					if(!(*p_roll)) p_roll=Display;
				}
			}
        }
     
    MsDelay(1);
	}
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file：Pointer to the source file name
  * @param  line：assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add His own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
