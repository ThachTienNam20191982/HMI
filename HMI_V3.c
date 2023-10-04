/*
 * HMI_V3.c
 *
 *  Created on: Jun 11, 2023
 *      Author: PC
 */

#include "HMI_V3.h"

/* @brift This function use to transmit current data to HMI
 * @param huart pointer to a UART_HandleTypeDef structure that contains the configuration information for UART module
 * @param HMI variable
 * @param current data in float
 */
void HMI_Transmit_V3(UART_HandleTypeDef* huart, uint8_t var, float data)
{
	uint8_t END_BYTE = 0xFF;
	int x = data*10;
	uint8_t str[]="x.val=0000";
	switch(var)
	{
	case HMI_VOLTAGE:
		str[0]='v';
	break;
	case HMI_CURRENT:
		str[0]='i';
	break;
	case HMI_TEMP:
		str[0]='t';
	break;
	}
	for(int i=9; i>5; i--)
	{
		str[i] = x%10 + 48;
		x = x/10;
	}
	HAL_UART_Transmit(huart, str, 10, 10);
	HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
	HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
	HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
}

/* @brift This function use to transmit current data to HMI
 * @param huart pointer to a UART_HandleTypeDef structure that contains the configuration information for UART module
 * @param HMI status
 */
void HMI_Status_V3(UART_HandleTypeDef* huart, uint8_t status)
{
	uint8_t END_BYTE = 0xFF;

	//uint8_t str1[]="state.txt=\"Connected\"";
	uint8_t str1[]={0x73, 0x74, 0x61, 0x74, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x3D, 0x22, 0x43, 0x6F, 0x6E, 0x6E, 0x65, 0x63, 0x74, 0x65, 0x64, 0x22};

	//uint8_t str2[]="bug.val=1";
	uint8_t str2[]={0x62, 0x75, 0x67, 0x2E, 0x76, 0x61, 0x6C, 0x3D, 0x31};

	//uint8_t str3[]="bug.val=2";
	uint8_t str3[]={0x62, 0x75, 0x67, 0x2E, 0x76, 0x61, 0x6C, 0x3D ,0x32};

	//uint8_t str4[]="bug.val=0";
	uint8_t str4[]={0x62, 0x75, 0x67, 0x2E, 0x76, 0x61, 0x6C, 0x3D ,0x30};

	switch(status)
	{
		case HMI_CONNECT:
		//	uint8_t str[]={0x73, 0x74, 0x61, 0x74, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x3D, 0x22, 0x43, 0x6F, 0x6E, 0x6E, 0x65, 0x63, 0x74, 0x65, 0x64, 0x22};
			HAL_UART_Transmit(huart, str1, sizeof(str1), 10);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
		break;
		case HMI_WARNING:
			HAL_UART_Transmit(huart, str2, sizeof(str2), 10);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
		break;
		case HMI_ERROR:
			HAL_UART_Transmit(huart, str3, sizeof(str3), 10);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
		break;
		case HMI_STOP:
			HAL_UART_Transmit(huart, str4, sizeof(str4), 10);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
			HAL_UART_Transmit(huart, &END_BYTE, 1, 1);
		break;

	}
}

/* @brift This function use to filter data from hmi
 * @param HMI_data is struct that include all data
 * @param data from HMI
 */
void HMI_Fillter(HMI_Data* data, uint8_t str[8])
{
	int check=0;
	for(int i=0;i<8;i++)
	{
		if(str[i]!=0x31) check++;
	}

	if(check) data->state=HMI_CHARGING_ON;
	else data->state=HMI_CHARGING_OFF;

	data->plug = str[0];
	data->mode = str[1];
	switch(str[1])
	{
	case '0':
		data->vout[0]=0x32;
		data->vout[1]=0x30;
		data->vout[2]=0x30;

		data->iout[0]=0x30;
		data->iout[1]=0x32;
		data->iout[2]=0x30;
	break;
	case '1':
		data->vout[0]=0x32;
		data->vout[1]=0x30;
		data->vout[2]=0x30;

		data->iout[0]=0x30;
		data->iout[1]=0x32;
		data->iout[2]=0x30;
	break;
	case '2':
		data->time[0]=str[2];
		data->time[1]=str[3];
		data->time[2]=str[4];
	break;
	case '3':
		data->vout[0]=str[2];
		data->vout[1]=str[3];
		data->vout[2]=str[4];

		data->iout[0]=str[5];
		data->iout[1]=str[6];
		data->iout[2]=str[7];
	break;
	}
}

/* @brift This function use to convert uint8_t to float
 * @param data in uint8_t*
 * @param x is the multiplier
 */
float uint8_to_float_V3(uint8_t* data, float x)
{
	uint16_t a;
	a = (data[0]&0xFF)|((data[1]&0xFF)<<8);
	return a*x;
}


/* @brift This function use to convert uint16_t to float
 * @param data in uint16_t
 * @param x is the multiplier
 */
float uint16_to_float_V3(uint16_t data, float x)
{
	return data*x;
}

/* @brift This function use to convert data to uint_16
 * @param data in uint8_t
 */
uint16_t HMI_to_uint16(uint8_t* str) {
    int result = 0;
    for (int i=0; i<3; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            result = result * 10 + (str[i] - '0');
        }
        else
        {
            return 0;
        }
    }
    return (uint16_t)result;
}


