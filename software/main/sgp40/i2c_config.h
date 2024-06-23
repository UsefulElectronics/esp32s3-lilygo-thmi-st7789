/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  i2c_config..h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Apr 28, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef I2C_H_
#define I2C_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "stdbool.h"
/* MACROS --------------------------------------------------------------------*/
#define I2C_MASTER_SCL_IO           GPIO_NUM_5      		/*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           GPIO_NUM_6     			/*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0               /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000                  /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                       /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                       /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief i2c master initialization
 */
esp_err_t i2c_master_init(void);
/**
 * @brief i2c master de-initialization
 */
esp_err_t i2c_master_deinit (void);
/**
 * @brief   Read data with len length over I2C bus
 * 
 * @param   i2c_address :   I2C slave address
 * 
 * @param   reg_addr    :   address to write data to
 * 
 * @param   data        :   Data to write 
 * 
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_register_read(uint8_t i2c_address, uint8_t reg_addr, uint8_t *data, size_t len);
/**
 * @brief   Read data with len length over I2C bus
 *
 * @param   i2c_address :   I2C slave address
 *
 * @param   data        :   pointer to copy data to
 *
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_master_sequential_read(uint8_t i2c_address, uint8_t *data, size_t len);
/**
 * @brief   Write 2 bytes over I2C bus
 * 
 * @param   i2c_address :   I2C slave address
 * 
 * @param   reg_addr    :   address to write data to
 * 
 * @param   data        :   Data to write 
 * 
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_register_write_byte(uint8_t i2c_address, uint8_t reg_addr, uint16_t data);
/**
 * @brief   Write number of length bytes over I2C bus
 *
 * @param   i2c_address :   I2C slave address
 *
 * @param   data        :   Pointer to the data to write
 *
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_master_sequential_write(uint8_t i2c_address, uint8_t* data, uint16_t length);

#endif /* I2C_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
