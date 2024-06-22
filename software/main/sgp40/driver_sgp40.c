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
 * @file      driver_sgp40.c
 * @brief     driver sgp40 source file
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

#include "driver_sgp40.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion SGP40"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.70f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.60f                    /**< chip max supply voltage */
#define MAX_CURRENT               4.00f                    /**< chip max current */
#define TEMPERATURE_MIN           -20.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           55.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip address definition
 */
#define SGP40_ADDRESS             (0x59 << 1)              /**< chip iic address */

/**
 * @brief chip command definition
 */
#define SGP40_COMMAND_MEASURE_RAW                 0x260FU        /**< measure raw command */
#define SGP40_COMMAND_EXECUTE_SELF_TEST           0x280EU        /**< execute self test command */
#define SGP40_COMMAND_TURN_HEATER_OFF             0x3615U        /**< turn heater off command */
#define SGP40_COMMAND_GET_SERIAL_ID               0x3682U        /**< get serial id command */
#define SGP40_COMMAND_SOFT_RESET                  0x0006U        /**< soft reset command */

/**
 * @brief crc8 definition
 */
#define SGP40_CRC8_POLYNOMIAL        0x31
#define SGP40_CRC8_INIT              0xFF

/**
 * @brief      read bytes with param
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[in]  reg is the iic register address
 * @param[in]  *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @param[out] *output points to an output buffer
 * @param[in]  output_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_sgp40_iic_read_with_param(sgp40_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len,
                                           uint16_t delay_ms, uint8_t *output, uint16_t output_len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                            /* check length */
    {
        return 1;                                                                  /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                          /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                         /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                                /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                                      /* copy write data */
    }
    
    if (handle->iic_write_cmd(SGP40_ADDRESS, (uint8_t *)buf, len + 2) != 0)        /* write iic command */
    {
        return 1;                                                                  /* write command */
    }
    handle->delay_ms(delay_ms);                                                    /* delay ms */
    if (handle->iic_read_cmd(SGP40_ADDRESS, output, output_len) != 0)              /* read data */
    {
        return 1;                                                                  /* write command */
    }
    else
    {
        return 0;                                                                  /* success return 0 */
    }
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to an sgp40 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_sgp40_iic_read(sgp40_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[2];
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                     /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                   /* set reg MSB */
    buf[1] = (uint8_t)(reg & 0xFF);                                          /* set reg LSB */
    if (handle->iic_write_cmd(SGP40_ADDRESS, (uint8_t *)buf, 2) != 0)        /* write command */
    {   
        return 1;                                                            /* return error */
    }
    handle->delay_ms(delay_ms);                                              /* delay ms */
    if (handle->iic_read_cmd(SGP40_ADDRESS, data, len) != 0)                 /* read data */
    {
        return 1;                                                            /* write command */
    }
    else
    {
        return 0;                                                            /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an sgp40 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_sgp40_iic_write(sgp40_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                            /* check length */
    {
        return 1;                                                                  /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                          /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                         /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                                /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                                      /* copy write data */
    }
    
    if (handle->iic_write_cmd(SGP40_ADDRESS, (uint8_t *)buf, len + 2) != 0)        /* write iic command */
    {
        return 1;                                                                  /* write command */
    }
    else
    {
        return 0;                                                                  /* success return 0 */
    }
}

/**
 * @brief     generate the crc
 * @param[in] *data points to a data buffer
 * @param[in] count is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_sgp40_generate_crc(uint8_t* data, uint8_t count) 
{
    uint8_t current_byte;
    uint8_t crc = SGP40_CRC8_INIT;
    uint8_t crc_bit;

    for (current_byte = 0; current_byte < count; ++current_byte)        /* calculate crc */
    {
        crc ^= (data[current_byte]);                                    /* xor data */
        for (crc_bit = 8; crc_bit > 0; --crc_bit)                       /* 8 bit */
        {
            if ((crc & 0x80) != 0)                                      /* if 7th bit is 1 */
            {
                crc = (crc << 1) ^ SGP40_CRC8_POLYNOMIAL;               /* xor */
            }
            else
            {
                crc = crc << 1;                                         /* left shift 1 */
            }
        }
    }
    
    return crc;                                                         /* return crc */
}

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
uint8_t sgp40_humidity_convert_to_register(sgp40_handle_t *handle, float rh, uint16_t *reg)
{
    if (handle == NULL)                               /* check handle */
    {
        return 2;                                     /* return error */
    }
    if (handle->inited != 1)                          /* check handle initialization */
    {
        return 3;                                     /* return error */
    }
    
    *reg = (uint16_t)(rh / 100.0f * 65535.0f);        /* convert the humidity */
    
    return 0;                                         /* success return 0 */
}

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
uint8_t sgp40_temperature_convert_to_register(sgp40_handle_t *handle, float temp, uint16_t *reg)
{
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    *reg = (uint16_t)((temp + 45.0f) / 175.0f * 65535.0f);        /* convert the humidity */
    
    return 0;                                                     /* success return 0 */
}

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
                              uint16_t raw_temperature, uint16_t *sraw_voc)
{
    uint8_t res;
    uint8_t input[6];
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                               /* clear the buffer */
    input[0] = (raw_humidity >> 8) & 0xFF;                                                             /* index 0 */
    input[1] = (raw_humidity >> 0) & 0xFF;                                                             /* index 1 */
    input[2] = a_sgp40_generate_crc(&input[0], 2);                                                     /* index 2 */
    input[3] = (raw_temperature >> 8) & 0xFF;                                                          /* index 3 */
    input[4] = (raw_temperature >> 0) & 0xFF;                                                          /* index 4 */
    input[5] = a_sgp40_generate_crc(&input[3], 2);                                                     /* index 5 */
    res = a_sgp40_iic_read_with_param(handle, SGP40_COMMAND_MEASURE_RAW, input, 6, 50, buf, 3);        /* read measure raw */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("sgp40: read measure raw failed.\n");                                      /* read measure failed */
       
        return 1;                                                                                      /* return error */
    }
    if (buf[2] != a_sgp40_generate_crc((uint8_t *)buf, 2))                                             /* check 1st crc */
    {
        handle->debug_print("sgp40: sraw voc crc check error.\n");                                     /* sraw voc crc check error */
       
        return 1;                                                                                      /* return error */
    }
    *sraw_voc = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                          /* get raw voc data */
    
    return 0;                                                                                          /* success return 0 */
}

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
uint8_t sgp40_get_measure_raw_without_compensation(sgp40_handle_t *handle, uint16_t *sraw_voc)
{
    uint8_t res;
    uint8_t input[6];
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                /* check handle */
    {
        return 2;                                                                                      /* return error */
    }
    if (handle->inited != 1)                                                                           /* check handle initialization */
    {
        return 3;                                                                                      /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                               /* clear the buffer */
    input[0] = 0x80;                                                                                   /* index 0 */
    input[1] = 0x00;                                                                                   /* index 1 */
    input[2] = 0xA2;                                                                                   /* index 2 */
    input[3] = 0x66;                                                                                   /* index 3 */
    input[4] = 0x66;                                                                                   /* index 4 */
    input[5] = 0x93;                                                                                   /* index 5 */
    res = a_sgp40_iic_read_with_param(handle, SGP40_COMMAND_MEASURE_RAW, input, 6, 50, buf, 3);        /* read measure raw */
    if (res != 0)                                                                                      /* check result */
    {
        handle->debug_print("sgp40: read measure raw failed.\n");                                      /* read measure failed */
       
        return 1;                                                                                      /* return error */
    }
    if (buf[2] != a_sgp40_generate_crc((uint8_t *)buf, 2))                                             /* check 1st crc */
    {
        handle->debug_print("sgp40: sraw voc crc check error.\n");                                     /* sraw voc crc check error */
       
        return 1;                                                                                      /* return error */
    }
    *sraw_voc = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                          /* get raw voc data */
    
    return 0;                                                                                          /* success return 0 */
}

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
uint8_t sgp40_get_measure_test(sgp40_handle_t *handle, uint16_t *result)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 3);                                                         /* clear the buffer */
    res = a_sgp40_iic_read(handle, SGP40_COMMAND_EXECUTE_SELF_TEST, (uint8_t *)buf, 3, 320);     /* read measure test */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("sgp40: read measure test failed.\n");                               /* read measure test failed */
       
        return 1;                                                                                /* return error */
    }
    if (buf[2] != a_sgp40_generate_crc((uint8_t *)buf, 2))                                       /* check crc */
    {
        handle->debug_print("sgp40: measure test check error.\n");                               /* measure test check error */
       
        return 1;                                                                                /* return error */
    }
    *result = (uint16_t)(((uint16_t)buf[0]) << 8 | buf[1]);                                      /* combine data */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t sgp40_soft_reset(sgp40_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }

    reg = 0x06;                                                          /* soft reset command */
    res = handle->iic_write_cmd(0x00, (uint8_t *)&reg, 1);               /* write reset config */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("sgp40: write soft reset failed.\n");        /* write soft reset failed */
       
        return 1;                                                        /* return error */
    }
    handle->delay_ms(5);                                                 /* delay 5ms */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t sgp40_turn_heater_off(sgp40_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }

    res = a_sgp40_iic_write(handle, SGP40_COMMAND_TURN_HEATER_OFF, NULL, 0);        /* write turn heater off command */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("sgp40: write turn heater off failed.\n");              /* write turn heater off failed */
       
        return 1;                                                                   /* return error */
    }
    handle->delay_ms(1);                                                            /* delay 1 ms */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t sgp40_get_serial_id(sgp40_handle_t *handle, uint16_t id[3])
{
    uint8_t res;
    uint8_t buf[9];
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    memset(buf, 0, sizeof(uint8_t) * 9);                                                      /* clear the buffer */
    res = a_sgp40_iic_read(handle, SGP40_COMMAND_GET_SERIAL_ID, (uint8_t *)buf, 9, 1);        /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("sgp40: read serial id failed.\n");                               /* read serial id failed */
       
        return 1;                                                                             /* return error */
    }
    if (buf[2] != a_sgp40_generate_crc((uint8_t *)&buf[0], 2))                                /* check 1st crc */
    {
        handle->debug_print("sgp40: crc 1 check failed.\n");                                  /* crc 1 check failed */
       
        return 1;                                                                             /* return error */
    }
    if (buf[5] != a_sgp40_generate_crc((uint8_t *)&buf[3], 2))                                /* check 2nd crc */
    {
        handle->debug_print("sgp40: crc 2 check failed.\n");                                  /* crc 2 check failed */
       
        return 1;                                                                             /* return error */
    }    
    if (buf[8] != a_sgp40_generate_crc((uint8_t *)&buf[6], 2))                                /* check 3rd crc */
    {
        handle->debug_print("sgp40: crc 3 check failed.\n");                                  /* crc 3 check failed */
       
        return 1;                                                                             /* return error */
    }
    id[0] = (uint16_t)((((uint16_t)buf[0]) << 8) | buf[1]);                                   /* set id 0 */
    id[1] = (uint16_t)((((uint16_t)buf[3]) << 8) | buf[4]);                                   /* set id 1 */
    id[2] = (uint16_t)((((uint16_t)buf[6]) << 8) | buf[7]);                                   /* set id 2 */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t sgp40_init(sgp40_handle_t *handle)
{ 
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("sgp40: iic_init is null.\n");                   /* iic_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("sgp40: iic_deinit is null.\n");                 /* iic_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                       /* check iic_write_cmd */
    {
        handle->debug_print("sgp40: iic_write_cmd is null.\n");              /* iic_write_cmd is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                        /* check iic_read_cmd */
    {
        handle->debug_print("sgp40: iic_read_cmd is null.\n");               /* iic_read_cmd is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("sgp40: delay_ms is null.\n");                   /* delay_ms is null */
    
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("sgp40: iic init failed.\n");                    /* iic init failed */
    
        return 3;                                                            /* return error */
    }
    handle->inited = 1;                                                      /* flag finish initialization */
  
    return 0;                                                                /* success return 0 */
}

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
uint8_t sgp40_deinit(sgp40_handle_t *handle)
{
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }    
    
    if (sgp40_turn_heater_off(handle) != 0)                             /* turn heater off */
    {
        handle->debug_print("sgp40: turn heater off failed.\n");        /* turn heater off failed */
    
        return 4;                                                       /* return error */
    }
    if (handle->iic_deinit() != 0)                                      /* iic deinit */
    {
        handle->debug_print("sgp40: iic close failed.\n");              /* iic close failed */
    
        return 3;                                                       /* return error */
    }
    handle->inited = 0;                                                 /* flag close initialization */
  
    return 0;                                                           /* success return 0 */
}

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
uint8_t sgp40_set_reg(sgp40_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
  
    return a_sgp40_iic_write(handle, reg, buf, len);          /* write data */
}

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
uint8_t sgp40_get_reg(sgp40_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
  
    return a_sgp40_iic_read(handle, reg, buf, len, 320);      /* read data */
}

/**
 * @brief      get chip information
 * @param[out] *info points to an sgp40 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sgp40_info(sgp40_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(sgp40_info_t));                          /* initialize sgp40 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
