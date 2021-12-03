/*
 * utils.c
 *
 *  Created on: 24 cze 2021
 *      Author: ROJEK
 */

#include "main.h"
#include "utils.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

void UartLogBT (char *Msg) {
	HAL_UART_Transmit(&huart1, (uint8_t*)Msg, strlen(Msg), 100);
}

void UartLogPC (char *Msg) {
	HAL_UART_Transmit(&huart2, (uint8_t*)Msg, strlen(Msg), 100);
}

void I2CScan (I2C_HandleTypeDef* i2chandle)
{

 	HAL_StatusTypeDef result;
  	uint8_t i;
	char Msg[64];
	uint16_t Len;

	Len = sprintf(Msg,"Scanning i2c bus...");
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
  		HAL_UART_Transmit(&huart2, (uint8_t*)".", 1, 1000);
  	  }
  	  if (result == HAL_OK)
  	  {
  		Len = sprintf(Msg,"\r\nDevice found! Address : 0x%X\r\n", i);
  		HAL_UART_Transmit(&huart2, (uint8_t*) Msg, Len, 1000);
  	  }
  	}

  	Len = sprintf(Msg,"Scan finished\r\n");
  	HAL_UART_Transmit(&huart2, (uint8_t*) Msg, Len, 1000);
}
