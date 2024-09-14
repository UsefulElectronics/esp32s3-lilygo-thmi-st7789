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
 * @file    :  hdc1080.c
 * @author  :  WARD
 * @version :  v.1.0
 * @date    :  Sep 11, 2024
 * @brief   :
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "hdc1080.h"
#include <stdint.h>
#include "sgp40/i2c_config.h"
#include <string.h>
#include <esp_timer.h>
/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static esp_timer_handle_t hdc1080_conversion_timer_h;
static bool awaiting_conversion = false;
static hdc1080_handle_t hHdc1080 = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS- --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static esp_err_t check_hdc1080_error(esp_err_t hdc_err);
static esp_err_t read_hdc100_data(uint8_t i2c_register, uint8_t * read_buff, size_t read_len);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
esp_err_t hdc1080_driver_init(hdc1080_config_t* hdc_cfg)
{
	esp_err_t err_ck = ESP_OK;
	
	memcpy(&hHdc1080, hdc_cfg, sizeof(hdc1080_config_t));
	
	if(hHdc1080.iic_read_cmd 	== NULL ||
		hHdc1080.debug_print 	== NULL ||
		hHdc1080.delay_ms 	 	== NULL ||
		hHdc1080.iic_deinit  	== NULL ||
		hHdc1080.iic_init    	== NULL ||
		hHdc1080.iic_write_cmd 	== NULL )
		{
			err_ck = ESP_FAIL;
		}
	hHdc1080.inited = true;
	
	return err_ck;
}
esp_err_t hdc1080_configure(hdc1080_config_t* hdc_configuration)
{
  if(awaiting_conversion){ return HDC1080_CONVERTING; }
  uint8_t temp_buff[3] = {0};
  uint16_t cfg_s = 0;

  // CAPTURE THE SETTINGS TO THE FILE GLOBAL SCOPE

  temp_buff[0] = HDC1080_CONFIG_REG;
  memcpy(temp_buff + HDC1080_REG_SIZE, hdc_configuration, sizeof(hdc1080_config_t));
  esp_err_t err_ck = i2c_master_sequential_write(HDC1080_I2C_ADDRESS, temp_buff, HDC1080_REG_SIZE + sizeof(hdc1080_config_t));
  
  check_hdc1080_error(err_ck);

  /* HDC1080 REQUIRES A SHORT DELAY TO PERFORM CONVERSION
   * BEFORE SENSOR DATA CAN BE READ. THE TIMER BELOW IS USED
   * WHEN TEMP READINGS ARE REQUESTED */
  const esp_timer_create_args_t hdc1080_conversion_timer_args = 
  {
    .callback = &hdc1080_conversion_completed,
    .name = "hdc1080_conversion_timer"
  };
  /* CREATE THE TEMPERATURE TRIGGER TIMER */
  err_ck = esp_timer_create(&hdc1080_conversion_timer_args, &hdc1080_conversion_timer_h);
  return err_ck;
}

/* ----------------------------------------------------------------------
 * @name esp_err_t hdc1080_get_configuration(hdc1080_config_t * hdc_cfg)
 * ----------------------------------------------------------------------
 * @brief Write to the i2c bus
 * @param hdc_cfg -> pointer to an hdc1080_config_t to fill
 * @return ESP_OK on success* 
 */
esp_err_t hdc1080_get_configuration(hdc1080_config_t * hdc_cfg)
{
  if(awaiting_conversion){ return HDC1080_CONVERTING; }
  unsigned char hdc_buff[2] = {0};
  esp_err_t err_ck = check_hdc1080_error(read_hdc100_data(HDC1080_CONFIG_REG, hdc_buff, 2));
  if(err_ck != ESP_OK){ return err_ck; }
  hdc_cfg->config_register = hdc_buff[0];
  return err_ck;
}

/* -------------------------------------------------------------
 * @name void hdc1080_conversion_completed(void* arg)
 * -------------------------------------------------------------
 * @brief callback from the conversion timer. Gets the current
 * sensor readings and callsback to the original request set
 * during configuration
 */
static void hdc1080_conversion_completed(void* arg)
{
  hdc1080_sensor_readings_t sens_readings = {0};
  unsigned char read_buff[4];
  // READ IN THE DATA
  esp_err_t err_ck = check_hdc1080_error(read_hdc100_data(HDC1080_CONFIG_REG, hdc_buff, 2))
  
  check_hdc1080_error(i2c_master_read_from_device(hdc1080_set.i2c_port_number, hdc1080_set.i2c_address, (unsigned char *)read_buff, sizeof(read_buff), hdc1080_set.timeout_length));
  if(err_ck == ESP_OK){
    // IF NO ERROR OCCURED THEN DO THE FLOAT CONVERSION 
    // OTHERWISE 0 WILL BE RETURNED FOR BOTH VALUES TO SIGNAL AND ISSUE
    sens_readings.temperature = ((((float)((read_buff[0] << 8) | read_buff[1])/65536) * 165) - 40);   /* pow(2, 16) ==  65536 */
    sens_readings.humidity = (((float)((read_buff[2] << 8) | read_buff[3])/65536) * 100);
  }
  awaiting_conversion = false;  // MARK THE FINISHED STATE
  hdc1080_set.callback(sens_readings);  // RUN THE CONFIGURED CALLBACK
}

/* --------------------------------------------------------------------------------------------------
 * @name static esp_err_t read_hdc100_data(unsigned char i2c_register, unsigned char * read_buff, size_t read_len)
 * --------------------------------------------------------------------------------------------------
 * @brief Read from the i2c bus
 * @param i2c_register -> register to read from
 * @param read_buff -> pointer to the buffer where the data will be stored
 * @param read_len -> length of the read
 * @return ESP_OK on success* 
 */
static esp_err_t read_hdc100_data(uint8_t i2c_register, uint8_t * read_buff, size_t read_len)
{
  /* CHANGE TO THE CORRECT REGISTER BEFORE THE READ */
  
  esp_err_t err_ck = i2c_master_sequential_write(HDC1080_I2C_ADDRESS, &i2c_register, HDC1080_REG_SIZE);
  
  if(ESP_OK == err_ck)
  {
	  err_ck = i2c_master_sequential_read(HDC1080_I2C_ADDRESS, read_buff, read_len);
  }

  return err_ck;
}

/* --------------------------------------------------------------
 * @name static esp_err_t check_hdc1080_error(esp_err_t hdc_err)
 * --------------------------------------------------------------
 * @brief Check for esp errors and print them
 * @param hdc_err -> The returned error from the check
 * @return ESP_OK on success, original error on fail
 * @note Any special error handling can be put in here
 */
static esp_err_t check_hdc1080_error(esp_err_t hdc_err)
{
  if(hdc_err == ESP_OK){ return ESP_OK; }
  ESP_LOGE("HDC1080", "ERROR HAS OCCURED: %s", esp_err_to_name(hdc_err));
  return hdc_err;
}
/********* (c) Copyright 2012-2024; Bilgi Elektronik A.S. *****END OF FILE****/
