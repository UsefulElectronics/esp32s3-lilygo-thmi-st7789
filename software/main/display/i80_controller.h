/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  st7789.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 5, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_DISPLAY_I80_CONTROLLER_H_
#define MAIN_DISPLAY_I80_CONTROLLER_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "stdbool.h"
/* MACROS --------------------------------------------------------------------*/
// LCD data pins
#define PIN_NUM_DATA0          48
#define PIN_NUM_DATA1          47
#define PIN_NUM_DATA2          39
#define PIN_NUM_DATA3          40
#define PIN_NUM_DATA4          41
#define PIN_NUM_DATA5          42
#define PIN_NUM_DATA6          45
#define PIN_NUM_DATA7          46
//LCD control pins
#define PIN_NUM_CS             6
#define PIN_NUM_DC             7
#define PIN_NUM_RST            -1
#define PIN_NUM_BK_LIGHT       38
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void i80_controller_init(void (*i80_gpio_set_level)(uint8_t gpio_num, uint8_t level));


#endif /* MAIN_DISPLAY_I80_CONTROLLER_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
