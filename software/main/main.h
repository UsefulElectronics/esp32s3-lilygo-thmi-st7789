/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 5, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "display/i80_controller.h"
#include "display/lvgl_ui.h"
#include "gpio/gpio_config.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "esp_err.h"
#include "esp_log.h"



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
//Board peripherals pins
#define PWR_EN_PIN 				10
#define PWR_ON_PIN 				14
#define BAT_ADC_PIN				5
#define BUTTON1_PIN				0
#define BUTTON2_PIN				21

// The pixel number in horizontal and vertical


/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* MAIN_MAIN_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
