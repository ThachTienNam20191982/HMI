/*
 * Ebike_Charger.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Admin
 */

#ifndef EBIKE_CHARGER_H_
#define EBIKE_CHARGER_H_
#include <EVSELab_System.h>
#include <Fixed_point_math.h>
#include "DAVE.h"
#include "time.h"

//#define OPEN_LOOP_TEST

typedef enum{
	/*--------------Normal Operating Modes---------------*/
	Open_Loop = 0x00,
	Battery_Check = 0x01,
	Soft_start_Mode = 0x02,
	Hand_Shake_Mode = 0x03,
	Wait_For_CC_Mode = 0x04,
	CC_Mode_PFM = 0x05,
	CC_Mode_PWM = 0x06,
	CV_Mode_PWM = 0x07,
	CV_Mode_PFM = 0x08,
	Fully_Charged = 0x09,

	/*--------------------Operating Error Modes--------------------*/
	Reverse_Polarity 		  = 0xE0,
	Over_Primary_Current	  = 0xE1,
	Over_Output_Voltage 	  = 0xE2,
	Over_Temperature          = 0xE3,
	Over_Power 				  = 0xE4,
	Low_Input_Voltage_Protect = 0xE5,
	Derate_75_Percent_Current = 0xE6,
	Derate_50_Percent_Current = 0xE7,
	Derate_25_Percent_Current = 0xE8,
	Grid_Off				  = 0xE9

	/*--------------------Communications Error Mo des--------------------*/

} Operating_Mode;

typedef enum{
	Read_From_EEPROM,
	Read_From_CAN_BUS,
	Read_From_Defined_Constants
} Charging_Params_Mode;

/*---------Initial params-----------*/
#define V_OUT_INITIAL		5.0f  	//Volts
#define Tz  				5e-5f 	//seconds
#define DUTY_MAX			(uint32_t)(5000) //50%
#define DUTY_MIN			(uint32_t)(0)    //0%

/*---------Protection params-----------*/
#define V_OUT_HIGH_LIMIT	69.0f  	//Volts
#define V_OUT_LOW_LIMIT		45.0f  	//Volts
#define P_OUT_HIGH_LIMIT	850.0f 	//Watts
#define IBAT_HIGH_LIMIT		15.0f
#define V_GRID_OPTO_THRES	1.0f 	//Volts

/*---------Charging Profile params-----------*/
#define IREF_START			0.0f	//Amps
#define IREF_END			10.0f	//Amps
#define IBAT_FULL			1.0f	//Amps
#define VREF				67.0f 	//Volts

/*---------Soft Start Params-----------*/
#define START_FREQ 			500e3   //Hz, No load soft start frequency
#define START_DEAD_TIME		600		//ns
#define END_DEAD_TIME		350		//ns

/*---------Anti windup parameters-----------*/
#define RESONANT_FREQ  		165e3
#define Fx_lower_limit  	0.63f//lower limit of fsw is 90 kHz
#define Fx_higher_limit  	2.42f  //higher limit of fsw is 200 kHz

/*--------- Measurement circuits params-----------*/
#define VO_GAIN				0.019536f
#define IBAT_GAIN 	 		0.0172f

//Diana 1: 0.0183
//Diana 3:
#define CAN_Tx			CAN_Node_LMO_01_Config
#define CAN_Rx			CAN_Node_LMO_02_Config

#define Request_ID			0x127
#define Slave_Measured_Data_ID			0x254
#define Slave_Operation_Data_ID			0x255

#define DERATE_TEMPERATURE_THRESHOLD	(uint32_t)(85) //Degree Celsius
#define PROTECT_TEMPERATURE_THRESHOLD	(uint32_t)(93) //Degree Celsius
#define NORMAL_TEMPERATURE_THRESHOLD	(uint32_t)(70) //Degree Celsius
#define TEMPERATURE_SAMPLING_CYCLE		(uint32_t)(50000) //5s

#define MAX_RESET_TIMES		3U  /* System reset times after error */

void Check_Protection(Operating_Mode *Charging_Mode, uint16_t ADC_Vo, uint16_t ADC_Ibat, uint16_t ADC_Vgrid);
void Check_Charging_Mode(Operating_Mode *Charing_Mode, uint16_t ADC_Vbat, uint16_t ADC_Vo, uint16_t ADC_Ibat);
void Check_Derating_Mode(Operating_Mode *Charging_Mode);
void Check_Temperature(uint16_t *Temperature);

_Bool Is_Grid_Off(void);
_Bool Is_CV_Mode(uint16_t ADC_Vo);
_Bool Is_Battery_Full(void);
_Bool Is_Vo_Back_To_Initial_Value(void);

void Set_Boostrap_Capacitor_Precharge(void);
void Set_Soft_Start_Mode(void);
void Set_CC_CV_Control_Mode(Operating_Mode *Charging_Mode, uint16_t ADC_Vo, uint16_t ADC_Ibat);
void Set_Voltage_Loop(int *f_I_ref, const int f_V_ref, const int f_I_ref_end, int *f_I_vol, uint16_t ADC_Vo);
void Set_Current_Loop_PIR(Operating_Mode *Charging_Mode, int f_I_ref, uint16_t ADC_Ibat);
void Set_Current_Loop_PI(Operating_Mode *Charging_Mode, int f_I_ref);
void Set_Output_Current_Profile(Operating_Mode Charging_Mode, int *f_I_ref, const int f_I_ref_end);
void Set_LED_Indicator(Operating_Mode Charging_Mode);
void Set_Soft_Start_Open_Loop(uint16_t Vo_ref);

void Update_Switching_Frequency(uint32_t f_Fx);
void Update_To_CAN_Buffer(const CAN_NODE_LMO_t lmo_ptr, uint16_t Data_Type);
void Update_Duty(float duty);
void System_Initialize(void);

#endif /* EBIKE_CHARGER_H_ */
