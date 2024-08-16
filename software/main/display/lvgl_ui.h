/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  lvgl_ui.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 10, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_DISPLAY_LVGL_UI_H_
#define MAIN_DISPLAY_LVGL_UI_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "lvgl.h"
/* MACROS --------------------------------------------------------------------*/
#define COLOR_AIR_QUALITY_GOOD			0x39EA7B
#define COLOR_AIR_QUALITY_FAIR			0x209D4A
#define COLOR_AIR_QUALITY_MODERATE		0xFFDA29
#define COLOR_AIR_QUALITY_POOR 			0xFFBE00
#define COLOR_AIR_QUALITY_UNHEALTHY 	0xFF5052
#define COLOR_AIR_QUALITY_SEVERE 		0xFF3431

#define COLOR_BG_AIR_QUALITY_GOOD		0x8BF2AC
#define COLOR_BG_AIR_QUALITY_FAIR		0x7BC694
#define COLOR_BG_AIR_QUALITY_MODERATE	0xFFEA83
#define COLOR_BG_AIR_QUALITY_POOR		0xFFDA6A
#define COLOR_BG_AIR_QUALITY_UNHEALTHY	0xFF999C
#define COLOR_BG_AIR_QUALITY_SEVERE		0xFF8583

#define COLOR_AIR_QUALITY_CALIBRATION	0xC9C9C9

#define STRING_AIR_QUALITY_GOOD			"GOOD"
#define STRING_AIR_QUALITY_FAIR			"FAIR"
#define STRING_AIR_QUALITY_MODERATE		"MODERATE"
#define STRING_AIR_QUALITY_POOR 		"POOR"
#define STRING_AIR_QUALITY_UNHEALTHY 	"UNHEALTHY"
#define STRING_AIR_QUALITY_SEVERE 		"SEVERE"

#define STRING_AIR_QUALITY_CALIBRATION	"WAIT"
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void lvgl_demo_ui(lv_disp_t *disp);

void lvgl_distance_set(uint16_t distance);

void lvgl_packet_count_set(uint32_t packet_count);

void lvgl_communication_status(bool communication_status);


#endif /* MAIN_DISPLAY_LVGL_UI_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
