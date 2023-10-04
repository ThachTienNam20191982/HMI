/*
 * HMI_V3.h
 *
 *  Created on: Jun 11, 2023
 *      Author: PC
 */

#ifndef INC_HMI_V3_H_
#define INC_HMI_V3_H_

#include <stdio.h>
#include "main.h"
//#include "stm32f1xx_hal.h" //Change chip's library

//HMI Variable
#define HMI_VOLTAGE 0x00
#define HMI_CURRENT 0X01
#define HMI_TEMP 0x02

//HMI Status
#define HMI_CONNECT 0x10
#define HMI_WARNING 0x11
#define HMI_ERROR 0x12
#define HMI_STOP 0x13

#define HMI_CHARGING_ON 0x01
#define HMI_CHARGING_OFF 0x00

//Struct
typedef struct
{
	int state;
	uint8_t plug;
	uint8_t mode;
	uint8_t vout[3];
	uint8_t iout[3];
	uint8_t time[3];
}HMI_Data;

//HMI Function
void HMI_Transmit_V3(UART_HandleTypeDef* huart, uint8_t var, float data);
void HMI_Status_V3(UART_HandleTypeDef* huart, uint8_t status);
void HMI_Fillter(HMI_Data* data, uint8_t str[8]);

//Convert Function
float uint8_to_float_V3(uint8_t* data, float x);
float uint16_to_float_V3(uint16_t data, float x);
uint16_t int_to_uint16_V3(float x);
uint16_t HMI_to_uint16(uint8_t* str);

#endif /* INC_HMI_V3_H_ */
