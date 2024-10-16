/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sgp40_interface_template.c
 * @brief     driver sgp40 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/08/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_sgp40_interface.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOSConfig.h"
#include "esp_log.h"

static const char *TAG = "sgp40";
/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t sgp40_interface_iic_init(void)
{
    return i2c_master_init();
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t sgp40_interface_iic_deinit(void)
{
    return i2c_master_deinit();
}

/**
 * @brief     interface iic bus write command
 * @param[in] addr is the iic device write address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t sgp40_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    return i2c_master_sequential_write(addr, buf, len);
}

/**
 * @brief      interface iic bus read command
 * @param[in]  addr is the iic device write address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp40_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    return i2c_master_sequential_read(addr, buf, len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void sgp40_interface_delay_ms(uint32_t ms)
{
	
	for(uint32_t i = 0; i < ms * ms; ++i)
	{
		for(uint32_t j = 0; j < ms * ms; ++j)
		{
			asm("NOP");
		}
	}
	
//	SYS_TICK()
//	vTaskDelay(ms / 10);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void sgp40_interface_debug_print(const char *const fmt, ...)
{
    char str[256];

    va_list args;

    memset((char *)str, 0, sizeof(char) * 256);
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);

    ESP_LOGI(TAG, "%s",str);
}
