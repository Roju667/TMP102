/*
 * tmp102.c
 *
 *  Created on: Sep 1, 2021
 *      Author: pawel
 */

#include "main.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "usart.h"
#include "tmp102.h"
#include "utilities.h"


uint8_t BMP_280Read8(TMP102_t *tmp102,uint8_t reg)
{
	uint8_t value;

	// address has to be shifted one place left beacue hal requires left alligned 7bit address
	HAL_I2C_Mem_Read(tmp102->I2CHandle,((tmp102->DeviceAdress)<<1),reg,1,&value,1,TMP102_I2C_TIMEOUT);

	return value;
}

uint16_t BMP280_Read16(TMP102_t *tmp102, uint8_t reg)
{
	uint8_t value[2];

	// if register value is over 3 then error
	if (reg > 3)
	{
		tmp102->ErrorCode = TMP102_ERR_WRONGREGISTERDEFINED;
		return 0;
	}

	HAL_I2C_Mem_Read(tmp102->I2CHandle,((tmp102->DeviceAdress)<<1),reg,1,value,2,TMP102_I2C_TIMEOUT);

	// write to 16 bits , two 8 bits registers
	// 0000 0000 0000 0000 , first we write buf[0] which has 8 significant bits as X << 4 XXXX XXXX
	// 0000 0000 XXXX XXXX // << 4
	// 0000 XXXX XXXX 0000 // then we push buf[1] which has only 4 significant bits (YYYY 0000) >> 4
	// 0000 XXXX XXXX YYYY // combined
	if (reg !=TMP102_REG_CONFIG)
	{
	return (value[0] << 4) | (value[1] >> 4);
	}
	else
	{
	// use union structure for config register
	return (value[0]) | (value[1] << 8);
	}
}

void Write16(TMP102_t *tmp102,uint8_t reg,uint16_t value)
{
	uint8_t buf[2];

	// if register value is over 3 then error
	if (reg > 3)
	{
		tmp102->ErrorCode = TMP102_ERR_WRONGREGISTERDEFINED;
		return;
	}

	// define bit structure for temp and config
	if(reg != TMP102_REG_CONFIG)
	{
	buf[0] = value >> 4;
	buf[1] = value << 4;
	}
	else
	{
	buf[0] = value ;
	buf[1] = value >> 8;
	}

	HAL_I2C_Mem_Write(tmp102->I2CHandle,((tmp102->DeviceAdress) << 1),reg,1,buf,2,TMP102_I2C_TIMEOUT);
	tmp102->ErrorCode = TMP102_ERR_NOERROR;
}

float TMP102GetTemp(TMP102_t *tmp102)
{
	// define variables
	int16_t val;
	float temp_c;

	// read temp data from register
	val = (int16_t)BMP280_Read16(tmp102,TMP102_REG_TEMP);

	// Convert to 2's complement, since temperature can be negative
	if (val > 0x7FF) {
		val |= 0xF000;
		}

	// Convert to float temperature value (Celsius)
	temp_c = (float)(val * 0.0625);

	return temp_c;
}

void TMP102GetConfiguration(TMP102_t *tmp102)
{
	// read uint16 config register value and convert it to bitfield
	configConverter tempConfig;
	tempConfig.i = BMP280_Read16(tmp102,TMP102_REG_CONFIG);
	tmp102->Configuration = tempConfig.conf;
}

uint8_t TMP102WriteConfig(TMP102_t *tmp102,TMP102writeConfig command,uint16_t value)
{
	// read raw config value
	uint16_t config;
	uint16_t mask;
	config = BMP280_Read16(tmp102,TMP102_REG_CONFIG);

	// STRUCTURE :
	// MSB [CR1][CR0][AL][EM][0][0][0][0][OS][R1][R0][F1][F0][POL][TM][SD] LSB
	//     [7]  [6]  [5] [4] [3][2][1][0] [7] [6] [5] [4] [3] [2] [1] [0]

	// select command
	switch(command){

	case TMP102_WRITE_SHUTDOWN:
		// check if the value is not too big for the register
		if (value > 1)
			return TMP102_ERR_WRONGCONFIG;

		mask = 0xFFFE;
		// clear shutdown bit
		config &= mask;
		// change shutdown bit
		config |= (value << 0);

	break;

	case TMP102_WRITE_THERMOSTATMODE:

		if (value > 1)
			return TMP102_ERR_WRONGCONFIG;

		mask = 0xFFFD;
		// clear TM mode bit
		config &= mask;
		// change TM mode bit
		config |= (value << 1);
	break;

	case TMP102_WRITE_POLARITY:

		if (value > 1)
			return TMP102_ERR_WRONGCONFIG;

		mask = 0xFFFB;
		// clear polarity
		config &= mask;
		// change polarity
		config |= (value << 2);

	break;

	case TMP102_WRITE_FALUTQUEUE:

		if (value > 3)
			return TMP102_ERR_WRONGCONFIG;

		mask = 0xFFE7;
		// clear fault queue
		config &= mask;
		// change fault queue
		config |= (value << 3);

	break;

	case TMP102_WRITE_EXTENDEDMODE:

		if (value > 1)
			return TMP102_ERR_WRONGCONFIG;

		mask = 0xEFFF;
		// clear extended mode
		config &= mask;
		// change extended mode
		config |= (value << 12);
	break;

	case TMP102_WRITE_CONV_RATE:

		if (value > 3)
			return TMP102_ERR_WRONGCONFIG;

		mask = 0x3FFF;
		// clear conversion rate
		config &= mask;
		// change conversion rate
		config |= (value << 15);
	break;
	}

	Write16(tmp102,TMP102_REG_CONFIG,config);

	TMP102GetConfiguration(tmp102);
	return TMP102_ERR_NOERROR;
}

void TMP102GetMinMaxTemp(TMP102_t *tmp102)
{
	// define variables
	int16_t val_max,val_min;
	float temp_max,temp_min;

	// read temp data from register
	val_max = (int16_t)BMP280_Read16(tmp102,TMP102_REG_MAXTEMP);
	val_min = (int16_t)BMP280_Read16(tmp102,TMP102_REG_MINTEMP);

	// Convert to 2's complement, since temperature can be negative
	if (val_max > 0x7FF) {
		val_max |= 0xF000;
		}

	if (val_min > 0x7FF) {
		val_min |= 0xF000;
		}

	// Convert to float temperature value (Celsius)
	temp_max = (float)(val_max * 0.0625);
	temp_min = (float)(val_min * 0.0625);

	tmp102->MaxTemperature = temp_max;
	tmp102->MinTemperatrue = temp_min;
}

uint8_t TMP102WriteMinMaxTemp(TMP102_t *tmp102, float MinTemp, float MaxTemp)
{

	// value send to fucntion is too high or too low
	// limits : -128 - +128 C
	if(fabs(MinTemp) > 128 || fabs(MaxTemp) > 128)
	{
		return TMP102_ERR_TEMPOUTOFLIMITS;
	}

	uint16_t val_max,val_min;

	// count from float to digital min temperature
	if (MinTemp > 0)
	{
		val_min = (uint16_t)MinTemp/0.0625;
	}
	else
	{
		val_min = (uint16_t)fabs(MinTemp)/0.0625;
		val_min = ~val_min;
		val_min +=1;
	}

	// write to register
	Write16(tmp102,TMP102_REG_MINTEMP ,val_min);

	if(tmp102->MaxTemperature != MaxTemp)
	{
		return TMP102_ERR_WRITEUNSUCCESSFUL;
	}


	// same for max
	if (MaxTemp > 0)
	{
		val_max = (uint16_t)MaxTemp/0.0625;
	}
	else
	{
		val_max = (uint16_t)(fabs(MaxTemp)/0.0625);
		val_max = ~val_max;
		val_max +=1;
	}

	// write to register
	Write16(tmp102,TMP102_REG_MAXTEMP ,val_max);

	TMP102GetMinMaxTemp(tmp102);

	if(tmp102->MinTemperatrue != MinTemp)
	{
		return TMP102_ERR_WRITEUNSUCCESSFUL;
	}

	return TMP102_ERR_NOERROR;
}

void TMP102Init(TMP102_t *tmp102, I2C_HandleTypeDef *initI2CHandle,
		uint8_t initDeviceAdress) {

	tmp102->I2CHandle = initI2CHandle;
	tmp102->DeviceAdress = initDeviceAdress;

	TMP102GetConfiguration(tmp102);
	TMP102GetMinMaxTemp(tmp102);
	TMP102WriteMinMaxTemp(tmp102,24,25);
	TMP102WriteConfig(tmp102,TMP102_WRITE_CONV_RATE,TMP102_CONV_RATE_025Hz);
	TMP102WriteConfig(tmp102,TMP102_WRITE_SHUTDOWN,TMP102_MODE_CONTINUOS);
}

