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
 * @file      driver_sgp40.h
 * @brief     driver sgp40 header file
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

#ifndef DRIVER_SGP40_H
#define DRIVER_SGP40_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sgp40_driver sgp40 driver function
 * @brief    sgp40 driver modules
 * @{
 */

/**
 * @addtogroup sgp40_base_driver
 * @{
 */

/**
 * @brief sgp40 handle structure definition
 */
typedef struct sgp40_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t inited;                                                            /**< inited flag */
} sgp40_handle_t;

/**
 * @brief sgp40 information structure definition
 */
typedef struct sgp40_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} sgp40_info_t;

/**
 * @}
 */

/**
 * @defgroup sgp40_link_driver sgp40 link driver function
 * @brief    sgp40 link driver modules
 * @ingroup  sgp40_driver
 * @{
 */

/**
 * @brief     initialize sgp40_handle_t structure
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] STRUCTURE is sgp40_handle_t
 * @note      none
 */
#define DRIVER_SGP40_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_SGP40_LINK_IIC_INIT(HANDLE, FUC)             (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SGP40_LINK_IIC_DEINIT(HANDLE, FUC)           (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] FUC points to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_SGP40_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)    (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] FUC points to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_SGP40_LINK_IIC_READ_COMMAND(HANDLE, FUC)     (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_SGP40_LINK_DELAY_MS(HANDLE, FUC)             (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an sgp40 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_SGP40_LINK_DEBUG_PRINT(HANDLE, FUC)          (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup sgp40_base_driver sgp40 base driver function
 * @brief    sgp40 base driver modules
 * @ingroup  sgp40_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to an sgp40 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sgp40_info(sgp40_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an sgp40 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t sgp40_init(sgp40_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an sgp40 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 turn heater off failed
 * @note      none
 */
uint8_t sgp40_deinit(sgp40_handle_t *handle);

/**
 * @brief      get the measure raw result
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[in]  raw_humidity is the humidity raw data
 * @param[in]  raw_temperature is the temperature raw data
 * @param[out] *sraw_voc points to a raw tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 get measure raw failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_get_measure_raw(sgp40_handle_t *handle, uint16_t raw_humidity,
                              uint16_t raw_temperature, uint16_t *sraw_voc);

/**
 * @brief      get the measure raw result without compensation
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[out] *sraw_voc points to a raw tvoc buffer
 * @return     status code
 *             - 0 success
 *             - 1 get measure raw without compensation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_get_measure_raw_without_compensation(sgp40_handle_t *handle, uint16_t *sraw_voc);

/**
 * @brief      convert the humidity to the register data
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[in]  rh is the current relative humidity
 * @param[out] *reg points to a converted register data
 * @return     status code
 *             - 0 success
 *             - 1 humidity convert to register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_humidity_convert_to_register(sgp40_handle_t *handle, float rh, uint16_t *reg);

/**
 * @brief      convert the temperature to the register data
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[in]  temp is the current temperature
 * @param[out] *reg points to a converted register data
 * @return     status code
 *             - 0 success
 *             - 1 temperature convert to register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_temperature_convert_to_register(sgp40_handle_t *handle, float temp, uint16_t *reg);

/**
 * @brief      get the chip measure test
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[out] *result points to a test result buffer
 * @return     status code
 *             - 0 success
 *             - 1 measure test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_get_measure_test(sgp40_handle_t *handle, uint16_t *result);

/**
 * @brief     soft reset the chip
 * @param[in] *handle points to an sgp40 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp40_soft_reset(sgp40_handle_t *handle);

/**
 * @brief     turn heater off
 * @param[in] *handle points to an sgp40 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 turn heater off failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp40_turn_heater_off(sgp40_handle_t *handle);

/**
 * @brief      get the chip serial id
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial id failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_get_serial_id(sgp40_handle_t *handle, uint16_t id[3]);

/**
 * @}
 */

/**
 * @defgroup sgp40_extern_driver sgp40 extern driver function
 * @brief    sgp40 extern driver modules
 * @ingroup  sgp40_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an sgp40 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sgp40_set_reg(sgp40_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sgp40_get_reg(sgp40_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
