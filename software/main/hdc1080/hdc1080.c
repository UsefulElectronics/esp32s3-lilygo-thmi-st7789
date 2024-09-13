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
#include "i2c_config.c"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static esp_timer_handle_t hdc1080_conversion_timer_h;
static bool awaiting_conversion = false;
static hdc1080_handle_t hHdc1080 = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS- --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

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
  uint8_t temp_buff[2] = {0};
  uint16_t cfg_s = 0;

  // CAPTURE THE SETTINGS TO THE FILE GLOBAL SCOPE
  memcpy(temp_buff, hdc_configuration, sizeof(hdc1080_config_t));
  // GET MANUFACTURER ID AND ENSURE A MATCH
  esp_err_t err_ck = check_hdc1080_error(read_hdc100_data(HDC1080_MANUFACTURER_ID_REG, hdc_buff, 2));
  if(err_ck != ESP_OK){ return err_ck; }
  if((unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]) != HDC1080_MANUFACTURER_ID){
    // NOT A TI CHIP
    ESP_LOGE("HDC1080", "EXPECTED TI ID 0x%04X BUT GOT 0x%04X", HDC1080_MANUFACTURER_ID, (unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]));
    return HDC1080_ERR_ID;
  }
  // GET THE DEVICE ID AND MAKE SURE IT'S AN HDC1080
  err_ck = check_hdc1080_error(read_hdc100_data(HDC1080_DEVICE_ID_REG, hdc_buff, 2));
  if(err_ck != ESP_OK){ return err_ck; }
  if((unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]) != HDC1080_DEVICE_ID){
    // NOT AND HDC1080
    ESP_LOGE("HDC1080", "EXPECTED DEVICE ID 0x%04X BUT GOT 0x%04X", HDC1080_DEVICE_ID, (unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]));
    return HDC1080_ERR_ID;  
  }
  // GET THE CURRENT CONFIGURATION AND IF IT DOESN'T MATCH, UPDATE IT
  err_ck = check_hdc1080_error(read_hdc100_data(HDC1080_CONFIG_REG, hdc_buff, 2));
  if(err_ck != ESP_OK){ return err_ck; }
  ESP_LOGD("HDC1080", "CURRENT CONFIGURATION 0x%04X", (unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]));
  if((unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]) != cfg_s){
    ESP_LOGD("HDC1080", "UPDATING CONFIGURATION FROM 0x%04X TO 0x%04X", (unsigned short)((hdc_buff[0] << 8) | hdc_buff[1]), cfg_s);
    hdc_buff[0] = hdc_cfg.config_register;
    hdc_buff[1] = 0;
    err_ck = check_hdc1080_error(write_hdc100_data(HDC1080_CONFIG_REG, hdc_buff, 2));
    if(err_ck != ESP_OK){ return err_ck; }
  }
  /* HDC1080 REQUIRES A SHORT DELAY TO PERFORM CONVERSION
   * BEFORE SENSOR DATA CAN BE READ. THE TIMER BELOW IS USED
   * WHEN TEMP READINGS ARE REQUESTED */
  const esp_timer_create_args_t hdc1080_conversion_timer_args = {
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
esp_err_t hdc1080_get_configuration(hdc1080_config_t * hdc_cfg){
  if(awaiting_conversion){ return HDC1080_CONVERTING; }
  unsigned char hdc_buff[2] = {0};
  esp_err_t err_ck = check_hdc1080_error(read_hdc100_data(HDC1080_CONFIG_REG, hdc_buff, 2));
  if(err_ck != ESP_OK){ return err_ck; }
  hdc_cfg->config_register = hdc_buff[0];
  return err_ck;
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
static esp_err_t read_hdc100_data(unsigned char i2c_register, unsigned char * read_buff, size_t read_len){
  /* CHANGE TO THE CORRECT REGISTER BEFORE THE READ */
  i2c_cmd_handle_t cmdlnk = i2c_cmd_link_create();
  check_hdc1080_error(i2c_master_start(cmdlnk));
  check_hdc1080_error(i2c_master_write_byte(cmdlnk, (hdc1080_set.i2c_address << 1) | I2C_MASTER_WRITE, true));
  check_hdc1080_error(i2c_master_write_byte(cmdlnk, i2c_register, true));
  check_hdc1080_error(i2c_master_stop(cmdlnk));
  esp_err_t err_ck = check_hdc1080_error(i2c_master_cmd_begin(hdc1080_set.i2c_port_number, cmdlnk, hdc1080_set.timeout_length));
  i2c_cmd_link_delete(cmdlnk);
  if(err_ck != ESP_OK){ return err_ck; }
  /* BEGIN THE READ */
  cmdlnk = i2c_cmd_link_create();
  check_hdc1080_error(i2c_master_start(cmdlnk));
  check_hdc1080_error(i2c_master_write_byte(cmdlnk, (hdc1080_set.i2c_address << 1) | I2C_MASTER_READ, true));
  check_hdc1080_error(i2c_master_read(cmdlnk, read_buff, read_len, I2C_MASTER_LAST_NACK));
  check_hdc1080_error(i2c_master_stop(cmdlnk));
  err_ck = check_hdc1080_error(i2c_master_cmd_begin(hdc1080_set.i2c_port_number, cmdlnk, hdc1080_set.timeout_length));
  i2c_cmd_link_delete(cmdlnk);
  return err_ck;
}

/********* (c) Copyright 2012-2024; Bilgi Elektronik A.S. *****END OF FILE****/
