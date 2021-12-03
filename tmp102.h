/*
 * tmp102.h
 *
 *  Created on: Sep 1, 2021
 *      Author: pawel
 */

#ifndef INC_TMP102_H_
#define INC_TMP102_H_

// Defined by user if float operations are enabled
// Working with float is recommended
#define TMP102_USE_FLOATNUMBERS				1

/*
 * TMP102 specific defines @defines
 */
#define TMP102_RESOLUTION					0.0625
#define TMP102_MIN					0
#define TMP102_MAX					1
#define TMP102_I2C_TIMEOUT			1000

/*
 * TMP102 error @error
 */
#define TMP102_ERR_NOERROR 					0
#define TMP102_ERR_WRITEUNSUCCESSFUL		1
#define TMP102_ERR_WRONGCONFIG				2
#define TMP102_ERR_WRONGREGISTERDEFINED		3
#define TMP102_ERR_WRONGMINMAXVALUES		4
#define TMP102_ERR_TEMPOUTOFLIMITS			5

/*
 * TMP102 adresses @address
 */
#define TMP102_ADDRESS 				0x48
#define TMP102_ADRESS_ALT

/*
 * TMP102 registers @registers
 */
#define TMP102_REG_TEMP				0x00
#define TMP102_REG_CONFIG			0x01
#define TMP102_REG_MINTEMP			0x02
#define TMP102_REG_MAXTEMP			0x03

/*
 * Configurable register values @config
 */
// Modes
#define TMP102_CR_MODE_CONTINUOS	0
#define TMP102_CR_MODE_SHUTDOWN		1
// Conversion Rate
#define TMP102_CR_CONV_RATE_025Hz	0
#define TMP102_CR_CONV_RATE_1Hz		1
#define TMP102_CR_CONV_RATE_4Hz		2
#define TMP102_CR_CONV_RATE_8Hz		3
// Hysteresis
#define TMP102_CR_FALUTQUEUE_1F		0
#define TMP102_CR_FALUTQUEUE_2F		1
#define TMP102_CR_FALUTQUEUE_4F		2
#define TMP102_CR_FALUTQUEUE_6F		3
// Activate one shot conversion
#define TMP102_CR_ONESHOT			1
// Extended mode
#define TMP102_CR_EXTENDED_ON
#define TMP102_CR_EXTENDED_OFF
// Polarity
#define TMP102_CR_POLARITY_LOW		0
#define TMP102_CR_POLARITY_HIGH		1
// Thermostat mode
#define TMP102_CR_THERMOSTAT_DEFAULT 	0
#define TMP102_CR_THERMOSTAT_IT			1

/*
 * Register offset @offset
 */
#define TMP102_CR_OFFSET_SD			0
#define TMP102_CR_OFFSET_TM			1
#define TMP102_CR_OFFSET_POL		2
#define TMP102_CR_OFFSET_FQ			3
#define TMP102_CR_OFFSET_R			5
#define TMP102_CR_OFFSET_OS			7
#define TMP102_CR_OFFSET_EM			12
#define TMP102_CR_OFFSET_AL			13
#define TMP102_CR_OFFSET_CR			14

/*
 * Configuration register structure
 */
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

/*
 * Union to make uint16 -> bitfield conversion
 */
typedef union
{
	TMP102config_t conf;
	uint16_t i;
}configConverter;

/*
 * Write commands @commands
 */
typedef enum
{
	TMP102_WRITE_SHUTDOWN = 0,
	TMP102_WRITE_THERMOSTATMODE,
	TMP102_WRITE_POLARITY,
	TMP102_WRITE_FALUTQUEUE,
	TMP102_WRITE_EXTENDEDMODE,
	TMP102_WRITE_CONV_RATE

}TMP102writeConfig;

/*
 * TMP102 structure variable
 */
typedef struct
{
	I2C_HandleTypeDef* 	I2CHandle; // pointer to i2c line
	uint8_t     	DeviceAdress;  // device addres
#if (TMP102_USE_FLOATNUMBERS == 1)
	float			MaxTemperature;  // min temp
	float			MinTemperature;	 // max temp
#else
	int8_t			MaxTemperatureIntegerPart;  // min temp integer part
	uint8_t			MaxTemperatureDecimalPart;  // min temp decimal part
	int8_t			MinTemperatureIntegerPart;	// max temp integer part
	uint8_t			MinTemperatureDecimalPart;	// max temp decimal
#endif
	TMP102config_t	Configuration;   // configuration
	uint8_t			ErrorCode;

}TMP102_t;


/*
 * TMP102 functions
 */
void TMP102Init(TMP102_t *DeviceStruct, I2C_HandleTypeDef *initI2CHandle,uint8_t initDeviceAddress);
#if (TMP102_USE_FLOATNUMBERS == 1)
float TMP102GetTempFloat(TMP102_t *tmp102);
uint8_t TMP102WriteMinMaxTempFloat(TMP102_t *tmp102, float temp, uint8_t MinOrMax);
#else
uint8_t TMP102WriteMinMaxTempInt(TMP102_t *tmp102, int8_t IntegerPart, uint8_t DecimalPart, uint8_t MinOrMax);
#endif
void TMP102GetTempInt(TMP102_t *tmp102,uint8_t* value);
void TMP102GetConfiguration(TMP102_t *tmp102);
void TMP102GetMinMaxTemp(TMP102_t *tmp102);



#endif /* INC_TMP102_H_ */
