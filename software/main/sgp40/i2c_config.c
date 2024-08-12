/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  i2c_config.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Apr 30, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "i2c_config.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "i2c";

i2c_master_bus_config_t i2c_bus_config = {0};

i2c_master_bus_handle_t bus_handle ={0};

i2c_master_dev_handle_t device_handle[I2C_MASTER_SLAVE_COUNT] = {0};
i2c_device_config_t i2c_dev_conf[I2C_MASTER_SLAVE_COUNT] = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static uint8_t i2c_master_new_device_check (uint8_t slave_address);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
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
esp_err_t i2c_register_write_byte(uint8_t i2c_address, uint8_t reg_addr, uint16_t data)
{
    int ret = 0;
    uint8_t write_buf[3] = {reg_addr, data & 0xFF, data >> 8};

    //ret = i2c_master_write_to_device(I2C_MASTER_NUM, i2c_address, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}
/**
 * @brief   Write number of length bytes over I2C bus
 *
 * @param   i2c_address :   I2C slave address
 *
 * @param   data        :   Pointer to the data to write
 *
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_master_sequential_write(uint8_t i2c_address, uint8_t* data, uint16_t length)
{
	uint8_t device_index = i2c_master_new_device_check(i2c_address);
	
	int ret = i2c_master_transmit(device_handle[device_index], data, length, I2C_TOOL_TIMEOUT_VALUE_MS);

    ESP_LOGI(TAG, "error code: %d",ret);

    return ret;
}
/**
 * @brief   Read data with len length over I2C bus from a memory address
 * 
 * @param   i2c_address :   I2C slave address
 * 
 * @param   reg_addr    :   address to write data to
 * 
 * @param   data        :   pointer to copy data to
 * 
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_register_read_memory(uint8_t i2c_address, uint8_t reg_addr, uint8_t *data, size_t len)
{
    //return i2c_master_write_read_device(I2C_MASTER_NUM, i2c_address, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    return 0;
}
/**
 * @brief   Read data with len length over I2C bus
 *
 * @param   i2c_address :   I2C slave address
 *
 * @param   data        :   pointer to copy data to
 *
 * @return  esp_err_t   :   Error status
 */
esp_err_t i2c_master_sequential_read(uint8_t i2c_address, uint8_t *data, size_t len)
{
	uint8_t device_index = i2c_master_new_device_check(i2c_address);
	
	return i2c_master_receive(device_handle[device_index], data, len, I2C_TOOL_TIMEOUT_VALUE_MS);
	
	//return i2c_master_read_from_device(I2C_MASTER_NUM, i2c_address, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}
/**
 * @brief i2c master initialization
 */
esp_err_t i2c_master_init(void)
{
	i2c_master_bus_config_t i2c_bus_config = 
	{
        .clk_source 					= I2C_CLK_SRC_DEFAULT,
        .i2c_port 						= I2C_MASTER_NUM,
        .scl_io_num 					= I2C_MASTER_SCL_IO,
        .sda_io_num 					= I2C_MASTER_SDA_IO,
        .flags.enable_internal_pullup 	= true,
        .glitch_ignore_cnt 				= 7,

    };
    
    //i2c_master_bus_handle_t bus_handle;
    

    //ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));
    
    //i2c_master_bus_add_device(bus_handle, i2c_dev_conf, &bus_handle);
	
	return i2c_new_master_bus(&i2c_bus_config, &bus_handle);
	
	/*
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = 
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

//    i2c_set_timeout(i2c_master_port, 0x0000001F);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);*/
}
/**
 * @brief i2c master de-initialization
 */
esp_err_t i2c_master_deinit (void)
{
	//return i2c_driver_delete(I2C_MASTER_NUM);
	return 0;
}

static uint8_t i2c_master_new_device_check (uint8_t slave_address)
{
	uint8_t i = 0;
	
	for (; i < I2C_MASTER_SLAVE_COUNT; ++i) 
	{
		if(i2c_dev_conf[i].device_address == 0)
		{
			i2c_dev_conf[i].scl_speed_hz   = I2C_MASTER_FREQ_HZ;
        	i2c_dev_conf[i].device_address = slave_address;
        
        	i2c_master_bus_add_device(bus_handle, &i2c_dev_conf[i], &device_handle[i]);
        	break;
		}
		else if(slave_address == i2c_dev_conf[i].device_address)
		{
			break;
		}
	}
	
	return i;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
