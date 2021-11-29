/*
 * utilities.c
 *
 *  Created on: Sep 3, 2021
 *      Author: pawel
 */

#include "main.h"
#include "utilities.h"
#include "stdio.h"
#include "usart.h"


void I2CScan (I2C_HandleTypeDef* i2chandle)
{

 	HAL_StatusTypeDef result;
  	uint8_t i;
	char Msg[64];
	uint16_t Len;

	Len = sprintf(Msg,"Scanning i2c bus\r\n Devices found :\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) Msg, Len, 1000);

  	for (i=0; i<127; i++)
  	{
  	  /*
  	   * the HAL wants a left aligned i2c address
  	   * &hi2c1 is the handle
  	   * (uint16_t)(i<<1) is the i2c address left aligned
  	   * retries 2
  	   * timeout 2
  	   */

  	  result = HAL_I2C_IsDeviceReady(i2chandle, (uint16_t)(i<<1), 2, 2);
  	  if (result != HAL_OK) // HAL_ERROR or HAL_BUSY or HAL_TIMEOUT
  	  {
  		  //
  	  }
  	  if (result == HAL_OK)
  	  {
  		Len = sprintf(Msg,"Device found! Adress : 0x%X\r\n", i);
  		HAL_UART_Transmit(&huart2, (uint8_t*) Msg, Len, 1000);
  	  }
  	}

  	Len = sprintf(Msg,"Scan finished\r\n");
  	HAL_UART_Transmit(&huart2, (uint8_t*) Msg, Len, 1000);
}
