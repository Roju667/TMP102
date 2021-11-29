/*
 * tmp102.h
 *
 *  Created on: Sep 1, 2021
 *      Author: pawel
 */

#ifndef INC_TMP102_H_
#define INC_TMP102_H_

// erorr codes

#define TMP102_ERR_NOERROR 				0
#define TMP102_ERR_WRITEUNSUCCESSFUL	1
#define TMP102_ERR_TEMPOUTOFLIMITS		2
#define TMP102_ERR_WRONGCONFIG			3
#define TMP102_ERR_WRONGREGISTERDEFINED	4

// adresses

#define TMP102_ADRESS 		0x48

// registers

#define TMP102_REG_TEMP		0x00
#define TMP102_REG_CONFIG	0x01
#define TMP102_REG_MINTEMP	0x02
#define TMP102_REG_MAXTEMP	0x03

#define TMP102_I2C_TIMEOUT	1000

// CONFIG
// modes
#define TMP102_MODE_CONTINUOS	0
#define TMP102_MODE_SHUTDOWN	1
// conversion rate
#define TMP102_CONV_RATE_025Hz	0
#define TMP102_CONV_RATE_1Hz	1
#define TMP102_CONV_RATE_4Hz	2
#define TMP102_CONV_RATE_8Hz	3
// hysteresis
#define TMP102_FALUTQUEUE_1F	0
#define TMP102_FALUTQUEUE_2F	1
#define TMP102_FALUTQUEUE_4F	2
#define TMP102_FALUTQUEUE_6F	3
// activate one shot conversion
#define TMP102_ONESHOT			1
// extended mode
#define TMP102_EXTENDED_ON
#define TMP102_EXTENDED_OFF
// polarity
#define TMP102_POLARITY_LOW		0
#define TMP102_POLARITY_HIGH		1
// thermostat mode
#define TMP102_THERMOSTAT_DEFAULT 	0
#define TMP102_THERMOSTAT_IT		1


typedef struct
{

	// STRUCTURE :
	// MSB [CR1][CR0][AL][EM][0][0][0][0][OS][R1][R0][F1][F0][POL][TM][SD] LSB
	//     [7]  [6]  [5] [4] [3][2][1][0] [7] [6] [5] [4] [3] [2] [1] [0]

	uint16_t TMP102_SD:1; 	// shutdown - if 1 then shutdown					// R/W
	uint16_t TMP102_TM:1;	// 0 - theromostat mode / 1 - IT mode				// R/W
	uint16_t TMP102_POL:1;	// polarity											// R/W
	uint16_t TMP102_FQ:2;	// fault queue -> how many faults to trigger alarm	// R/W
	uint16_t TMP102_R:2;	// resolution										// R
	uint16_t TMP102_OS:1;	// one shot											// R/W
	uint16_t TMP102_NOTUSED4:4;
	uint16_t TMP102_EM:1; 	// extended mode									// R/W
	uint16_t TMP102_AL:1;	// alert											// R
	uint16_t TMP102_CR:2; 	// conversion rate									// R/W
}TMP102config_t;

typedef union
{
	// to make uint16 -> bitfield conversion
	TMP102config_t conf;
	uint16_t i;
}configConverter;

typedef enum
{
	TMP102_WRITE_SHUTDOWN = 0,
	TMP102_WRITE_THERMOSTATMODE,
	TMP102_WRITE_POLARITY,
	TMP102_WRITE_FALUTQUEUE,
	TMP102_WRITE_EXTENDEDMODE,
	TMP102_WRITE_CONV_RATE

}TMP102writeConfig;


typedef struct
{
	I2C_HandleTypeDef* 	I2CHandle; // pointer to i2c line
	uint8_t     	DeviceAdress;  // device addres
	float			MaxTemperature;  // min temp
	float			MinTemperatrue;	 // max temp
	TMP102config_t	Configuration;   // configuration
	uint8_t			ErrorCode;

}TMP102_t;

void TMP102Init(TMP102_t *DeviceStruct, I2C_HandleTypeDef *initI2CHandle,uint8_t initDeviceAdress);

float TMP102GetTemp(TMP102_t *tmp102);
void TMP102GetConfiguration(TMP102_t *tmp102);





#endif /* INC_TMP102_H_ */
