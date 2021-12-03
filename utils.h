/*
 * utils.h
 *
 *  Created on: 24 cze 2021
 *      Author: ROJEK
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

void UartLogBT (char *Msg);
void UartLogPC (char *Msg);
void I2CScan (I2C_HandleTypeDef* i2chandle);

#endif /* INC_UTILS_H_ */
