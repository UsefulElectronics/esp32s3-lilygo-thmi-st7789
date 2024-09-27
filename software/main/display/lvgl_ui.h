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

#define COLOR_GAUGE_SECTION_COUNT		6
#define COLOR_GAUGE_LAYER_COUNT			2
#define COLOR_GAUGE_SSECTION_RANGE		83
#define STRING_GAUGE_COMMENT_MAX_CHAR_COUNT	10

#define CHART_DATA_COUNT_LIMIT			15


/* ENUMORATIONS --------------------------------------------------------------*/
enum
{
	LVGL_GAUGE_COLOR_FRONT = 0,
	LVGL_GAUGE_COLOR_BACK = 1,
};

typedef enum
{
	LVGL_MENU_NEXT = 0,
	LVGL_MENU_BACK,
	LVGL_MENU_ENTER,
	LVGL_MENU_EXIT,
}lvgl_nav_e;

typedef enum
{
	SCREEN_INTRO_ID = 0,
	SCREEN_AIR_QUALITY_ID,
	SCREEN_TEMPERATURE_HUMIDITY_ID,
	SCREEN_TOTAL,
	SCREEN_OVERFLOW = 255,
	
}lvgl_screen_id_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
/** Describes an animation*/
typedef struct _ui_anim_user_data_t {
    lv_obj_t * target;
    lv_img_dsc_t ** imgset;
    int32_t imgset_size;
    int32_t val;
} ui_anim_user_data_t;
/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void lvgl_demo_ui(lv_disp_t *disp);

void lvgl_distance_set(uint16_t distance);

void lvgl_packet_count_set(uint32_t packet_count);

void lvgl_voc_index_update(uint32_t voc_index);

void lvgl_voc_index_chart_insert(uint32_t voc_index);

void lvgl_communication_status(bool communication_status);

void lvgl_screen_navigate(lvgl_nav_e action);


#endif /* MAIN_DISPLAY_LVGL_UI_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
