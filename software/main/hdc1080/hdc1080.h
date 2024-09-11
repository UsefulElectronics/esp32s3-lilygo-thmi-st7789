/**
  ******************************************************************************
  *		(c) Copyright 2012-2024; Bilgi Elektronik A.S.
  *
  *	All rights reserved.  Protected by international copyright laws.
  *	Knowledge of the source code may not be used to write a similar
  *	product.  This file may only be used in accordance with a license
  *	and should not be redistributed in any way.
  *
  *****************************************************************************/

/**
 ******************************************************************************
 * @file    :  hdc1080.h
 * @author  :  WARD
 * @version :  v.1.0
 * @date    :  Sep 11, 2024
 * @brief   :
 *
 ******************************************************************************/

#ifndef MAIN_HDC1080_H_
#define MAIN_HDC1080_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <esp_timer.h>

/* MACROS --------------------------------------------------------------------*/
#define HDC1080_TEMPERATURE_REG     0x00    /* TEMPERATURE MEASUREMENT OUTPUT */
#define HDC1080_HUMIDITY_REG        0x01    /* RELATIVE HUMIDITY MEASUREMENT OUTPUT */
#define HDC1080_CONFIG_REG          0x02    /* HDC1080 CONFIGURATION DATA */
#define HDC1080_SERIALID2_REG       0xFB    /* FIRST 2 BYTES OF SERIAL ID */
#define HDC1080_SERIALID1_REG       0xFC    /* MID 2 BYTES OF THE SERIAL ID */
#define HDC1080_SERIALID0_REG       0xFD    /* LAST BYTE BIT OF THE SERIAL ID */
#define HDC1080_MANUFACTURER_ID_REG 0xFE    /* ID OF TEXAS INSTRUMENTS */
#define HDC1080_DEVICE_ID_REG       0xFF    /* REGISTER OF THE DEVICE ID */
#define HDC1080_DEVICE_ID           0x1050  /* HDC1080 UNIQUE ID */
#define HDC1080_MANUFACTURER_ID     0x5449  /* TI MANUFACTURER ID */
#define HDC1080_I2C_ADDRESS         0x40    /* I2C ADDRESS OF THE HDC1080 */

#define HDC1080_ACQUISITION_HUMIDITY_AND_TEMPERATURE  0x01
#define HDC1080_ACQUISITION_HUMIDITY_OR_TEMPERATURE   0x00
#define HDC1080_TEMPERATURE_RESOLUTION_11BIT          0x01
#define HDC1080_TEMPERATURE_RESOLUTION_14BIT          0x00
#define HDC1080_HUMIDITY_RESOLUTION_8BIT              0x02
#define HDC1080_HUMIDITY_RESOLUTION_11BIT             0x01
#define HDC1080_HUMIDITY_RESOLUTION_14BIT             0x00

#define HDC1080_HEATER_ENABLED      0x01
#define HDC1080_HEATER_DISABLED     0x00
#define HDC1080_BATTERY_STATUS_OK   0x00
#define HDC1080_BATTERY_STATUS_LOW  0x01
#define HDC1080_ERR_ID              0xFF
#define HDC1080_CONVERTING          0xFE
#define HDC1080_CONVERSION_WAIT_PERIOD   (500000) /* CONVERSION WAIT PERIOD */

/* CONVERT CELSIUS TO FAHRENHEIT */
#define CEL2FAH(CELSIUS) ((1.8 * CELSIUS) + 32)
/* CALCULATE DEWPOINT USING TEMPERATURE AND HUMIDITY */
#define DEWPOINT(CELSIUS, RH) (CELSIUS - (14.55 + 0.114 * CELSIUS) * \
         (1 - (0.01 * RH)) - pow(((2.5 + 0.007 * CELSIUS) * \
         (1 - (0.01 * RH))),3) - (15.9 + 0.117 * CELSIUS) * \
         pow((1 - (0.01 * RH)), 14))
/* CALCULATE AIR SATURATION VAPOR PRESSURE IN PASCALS */
#define SVP(CELSIUS) (610.78 * pow(2.71828, (CELSIUS / (CELSIUS+237.3) * 17.2694)))
/* AIR VAPOR PRESSURE DEFICIT IN kPa */
#define VPD(SVP, RH) ((SVP *(1 - RH/100))/1000)
/* CONVERT PASCALS TO kPa */
#define PAS2KPA(PASCALS) (PASCALS / 1000)
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* THE CONFIG REGISTER IS 16 BITS LONG BUT THE FIRST 8 
 * BITS ARE RESERVED, SO HERE JUST PACK IN THE LAST 8 BITS */
typedef union 
{
	uint16_t config_register;
	struct 
	{
		uint16_t reserved:                          8;
		uint16_t humidity_measurement_resolution:   2;
		uint16_t temperature_measurement_resolution:1;
		uint16_t battery_status:                    1;
		uint16_t mode_of_acquisition:               1;
		uint16_t heater:                            1;
		uint16_t reserved_bit:                      1;
		uint16_t software_reset:                    1;
	} bits;
} hdc1080_config_t;

typedef struct
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t inited;                                                            /**< inited flag */
} hdc1080_handle_t;

typedef struct HDC1080_SENSOR_READINGS 
{
	float humidity;
	float temperature;
} hdc1080_sensor_readings_t;
/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* MAIN_HDC1080_H_ */

/********* (c) Copyright 2012-2024; Bilgi Elektronik A.S. *****END OF FILE****/
