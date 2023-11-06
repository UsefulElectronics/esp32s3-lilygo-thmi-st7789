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

/* MACROS --------------------------------------------------------------------*/


#define LCD_PIXEL_CLOCK_HZ     (10 * 1000 * 1000)

#define LCD_BK_LIGHT_ON_LEVEL  1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
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
#define LCD_H_RES              	240
#define LCD_V_RES              	320
// Bit number used to represent command and parameter

#define LCD_CMD_BITS          	8
#define LCD_PARAM_BITS         	8
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* MAIN_MAIN_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
