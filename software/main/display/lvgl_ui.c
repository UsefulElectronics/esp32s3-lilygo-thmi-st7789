/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  lvgl_ui.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 10, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "lvgl_ui.h"
#include "misc/lv_anim.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
const uint32_t lvgl_gauge_color_array[COLOR_GAUGE_SECTION_COUNT][COLOR_GAUGE_LAYER_COUNT] =
{
	{COLOR_AIR_QUALITY_GOOD, 		COLOR_BG_AIR_QUALITY_GOOD},
	{COLOR_AIR_QUALITY_FAIR, 		COLOR_BG_AIR_QUALITY_FAIR},
	{COLOR_AIR_QUALITY_MODERATE, 	COLOR_BG_AIR_QUALITY_MODERATE},
	{COLOR_AIR_QUALITY_POOR, 		COLOR_BG_AIR_QUALITY_POOR},
	{COLOR_AIR_QUALITY_UNHEALTHY,	 COLOR_BG_AIR_QUALITY_UNHEALTHY},
	{COLOR_AIR_QUALITY_SEVERE,		COLOR_BG_AIR_QUALITY_SEVERE},
};

const char lvgl_gauge_comment_string[COLOR_GAUGE_SECTION_COUNT] [STRING_GAUGE_COMMENT_MAX_CHAR_COUNT] =
{
	STRING_AIR_QUALITY_GOOD,
	STRING_AIR_QUALITY_FAIR,
	STRING_AIR_QUALITY_MODERATE,
	STRING_AIR_QUALITY_POOR,
	STRING_AIR_QUALITY_UNHEALTHY,
	STRING_AIR_QUALITY_SEVERE,
};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <math.h>
#include "lvgl.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef PI
#define PI  (3.14159f)
#endif

// LVGL image declare
LV_IMG_DECLARE(ui_img_gauge_full_png)
LV_IMG_DECLARE(ui_img_gauge_empty_png)
LV_IMG_DECLARE(ui_img_ue_logo_png)
LV_IMG_DECLARE(ui_img_status_png)

LV_IMG_DECLARE(ui_img_525640412)
LV_IMG_DECLARE(ui_img_1062568172)
LV_IMG_DECLARE(ui_img_1282837856)
LV_IMG_DECLARE(ui_img_air_quality_gauge_png)
LV_IMG_DECLARE(ui_img_gauge_pointer_png)
//LV_IMG_DECLARE(esp_text)
LV_IMG_DECLARE(ui_img_useful_electronics_png)
typedef struct {
    lv_obj_t *scr;
    int count_val;
} my_timer_context_t;

static my_timer_context_t my_tim_ctx;
static lv_obj_t * btn;
static lv_obj_t *arc[3];
static lv_obj_t *img_logo;
static lv_obj_t *img_text = NULL;
static lv_color_t arc_color[] = 
{
//    LV_COLOR_MAKE(232, 87, 116),
	LV_COLOR_MAKE(22, 242, 255),
    LV_COLOR_MAKE(255, 255, 255),
    LV_COLOR_MAKE(90, 202, 228),
};
 lv_obj_t * ui_Screen1;
 lv_obj_t * ui_Arc1;
 lv_obj_t * ui_Panel1;
 lv_obj_t * ui_Panel2;
 lv_obj_t * ui_Label1;
 lv_obj_t * ui_Label2;
 lv_obj_t * ui_Chart1;
 lv_obj_t * ui_Panel8;
 lv_obj_t * ui_Image1;
 lv_obj_t * ui_Label5;
 
//screen 3 objects
lv_obj_t * ui_Panel4;
lv_obj_t * ui_Panel7;
lv_obj_t * ui_Chart3;
lv_obj_t * ui_Panel6;
lv_obj_t * ui_Chart2;
lv_obj_t * ui_Panel3;
lv_obj_t * ui_Label3;
lv_obj_t * ui_Image3;
lv_obj_t * ui_Image4;
lv_obj_t * ui_Label4;
lv_obj_t * ui_Panel5;
lv_obj_t * ui_Panel10;
lv_obj_t * ui_Image5;
lv_obj_t * ui_Image2;
lv_obj_t * ui_Panel9;
lv_obj_t * ui_Label6;

lv_obj_t *display;
lv_obj_t *tv1;
lv_obj_t *tv2;
lv_obj_t *tv3;

lv_coord_t ui_Chart1_series_1_array[CHART_DATA_COUNT_LIMIT] = { 0, 10, 20, 40, 80, 80, 40, 210, 10, 0, 4, 1, 250, 490 };
lv_chart_series_t * ui_Chart1_series_1 = {0};
lv_chart_series_t * ui_Chart2_series_1 = {0};
lv_chart_series_t * ui_Chart3_series_1 = {0};

lv_anim_t PropertyAnimation_0 = {0};

typedef struct
{
	int16_t temperature;
	int16_t humidity;
	int16_t air_quality;
	int16_t reserved;
}lvgl_sensor_data_t;

lvgl_sensor_data_t hSensor = {0};

void tv2_screen_init(void);
void tv3_screen_init(void);
static void lvgl_shrink_Animation(void);
static void lvgl_extend_Animation(void);

static void lvgl_show_Animation(void);
static void lvgl_hide_Animation(void);

static void lvgl_addvoc_Animation();
static void lvgl_removevoc_Animation();
static void lvgl_indicator_Animation();

static int lvgl_convert_angle2voc(int meter_angle);
static int lvgl_convert_voc2angle(int voc_data); 

static void _ui_anim_callback_set_height(lv_anim_t * a, int32_t v);
static void _ui_anim_callback_set_opacity(lv_anim_t * a, int32_t v);
static int32_t _ui_anim_callback_get_opacity(lv_anim_t * a);
static void _ui_anim_callback_set_image_angle(lv_anim_t * a, int32_t v);
static void _ui_anim_callback_free_user_data(lv_anim_t * a);
static void _ui_anim_callback_delete_animation(lv_anim_t * a);
static void lvgl_voc_gauge_angle(int32_t voc_value);

static void anim_timer_cb(lv_timer_t *timer)
{
    my_timer_context_t *timer_ctx = (my_timer_context_t *) timer->user_data;
    int count = timer_ctx->count_val;
    lv_obj_t *scr = timer_ctx->scr;

    // Play arc animation
    if (count < 90) {
        lv_coord_t arc_start = count > 0 ? (1 - cosf(count / 180.0f * PI)) * 270 : 0;
        lv_coord_t arc_len = (sinf(count / 180.0f * PI) + 1) * 135;

        for (size_t i = 0; i < sizeof(arc) / sizeof(arc[0]); i++) {
            lv_arc_set_bg_angles(arc[i], arc_start, arc_len);
            lv_arc_set_rotation(arc[i], (count + 120 * (i + 1)) % 360);
        }
    }

    // Delete arcs when animation finished
    if (count == 90) {
        for (size_t i = 0; i < sizeof(arc) / sizeof(arc[0]); i++) {
            lv_obj_del(arc[i]);
        }

        // Create new image and make it transparent
        img_text = lv_img_create(scr);
        lv_img_set_src(img_text, &ui_img_useful_electronics_png);
        lv_obj_set_style_img_opa(img_text, 0, 0);
    }

    // Move images when arc animation finished
    if ((count >= 100) && (count <= 180)) {
        lv_coord_t offset = (sinf((count - 140) * 2.25f / 90.0f) + 1) * 20.0f;
        lv_obj_align(img_logo, LV_ALIGN_CENTER, 0, -offset);
        lv_obj_align(img_text, LV_ALIGN_CENTER, 0,  offset);
        lv_obj_set_style_img_opa(img_text, offset / 40.0f * 255, 0);
    }

    // Delete timer when all animation finished
    if ((count += 5) == 220) {
        lv_timer_del(timer);
        //switch screen
        lv_obj_set_tile_id(display, 0, SCREEN_AIR_QUALITY_ID, LV_ANIM_ON);

        // Enable button
//        lv_obj_clear_state(btn, LV_STATE_DISABLED);
    } else {
        timer_ctx->count_val = count;
    }
}

static void start_animation(lv_obj_t *scr)
{
    // Align image
    lv_obj_center(img_logo);

    // Create arcs
    for (size_t i = 0; i < sizeof(arc) / sizeof(arc[0]); i++) {
        arc[i] = lv_arc_create(scr);

        // Set arc caption
        lv_obj_set_size(arc[i], 220 - 30 * i, 220 - 30 * i);
        lv_arc_set_bg_angles(arc[i], 120 * i, 10 + 120 * i);
        lv_arc_set_value(arc[i], 0);

        // Set arc style
        lv_obj_remove_style(arc[i], NULL, LV_PART_KNOB);
        lv_obj_set_style_arc_width(arc[i], 10, 0);
        lv_obj_set_style_arc_color(arc[i], arc_color[i], 0);

        // Make arc center
        lv_obj_center(arc[i]);
    }

    if (img_text) {
        lv_obj_del(img_text);
        img_text = NULL;
    }

    // Create timer for animation
    my_tim_ctx.count_val = -90;
    my_tim_ctx.scr = scr;
    lv_timer_create(anim_timer_cb, 50, &my_tim_ctx);

    // Disable button
//    lv_obj_add_state(btn, LV_STATE_DISABLED);
}

static void btn_cb(lv_event_t * e)
{
    lv_obj_t * scr = lv_event_get_user_data(e);
    start_animation(scr);
}

void tv2_screen_init(void)
{

    lv_obj_clear_flag(tv2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags


    ui_Panel2 = lv_obj_create(tv2);
    lv_obj_set_width(ui_Panel2, 240);
    lv_obj_set_height(ui_Panel2, 320);
    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel2, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel2, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Panel2, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Panel2, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Arc1 = lv_arc_create(tv2);
    lv_obj_set_width(ui_Arc1, 227);
    lv_obj_set_height(ui_Arc1, 231);
    lv_obj_set_x(ui_Arc1, 0);
    lv_obj_set_y(ui_Arc1, -40);
    lv_obj_set_align(ui_Arc1, LV_ALIGN_CENTER);
    lv_arc_set_range(ui_Arc1, 0, 500);
    lv_arc_set_value(ui_Arc1, 0);
    lv_obj_set_style_bg_img_src(ui_Arc1, &ui_img_gauge_empty_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_Arc1, lv_color_hex(0x4040FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_Arc1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_img_src(ui_Arc1, &ui_img_gauge_full_png, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_Arc1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_Arc1, 100, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_Arc1, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_img_src(ui_Arc1, &ui_img_gauge_full_png, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Arc1, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Arc1, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_Panel1 = lv_obj_create(tv2);
    lv_obj_set_width(ui_Panel1, 120);
    lv_obj_set_height(ui_Panel1, 120);
    lv_obj_set_x(ui_Panel1, 0);
    lv_obj_set_y(ui_Panel1, -41);
    lv_obj_set_align(ui_Panel1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel1, 512, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel1, lv_color_hex(0xC9C9C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label1 = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, " ");
    lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(tv2);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, 0);
    lv_obj_set_y(ui_Label2, -8);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "WAIT");
    
    ui_Chart1 = lv_chart_create(tv2);
    lv_obj_set_width(ui_Chart1, 240);
    lv_obj_set_height(ui_Chart1, 114);
    lv_obj_set_x(ui_Chart1, 0);
    lv_obj_set_y(ui_Chart1, 102);
    lv_obj_set_align(ui_Chart1, LV_ALIGN_CENTER);
    lv_chart_set_type(ui_Chart1, LV_CHART_TYPE_BAR);
    lv_chart_set_point_count(ui_Chart1, 15);
    lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 500);
    lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_X, 0, 0, 50, 0, false, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 0, 0, false, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, 0, 0, 0, 0, false, 25);
    ui_Chart1_series_1 = lv_chart_add_series(ui_Chart1, lv_color_hex(0x209D4A),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
                                                                 
    lv_chart_set_ext_y_array(ui_Chart1, ui_Chart1_series_1, ui_Chart1_series_1_array);
    lv_obj_set_style_bg_color(ui_Chart1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chart1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Chart1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Chart1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Chart1, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Chart1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_Chart1, lv_color_hex(0x4040FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_Chart1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    
    ui_Panel8 = lv_obj_create(tv2);
    lv_obj_set_width(ui_Panel8, 167);
    lv_obj_set_height(ui_Panel8, 65);
    lv_obj_set_x(ui_Panel8, 0);
    lv_obj_set_y(ui_Panel8, 46);
    lv_obj_set_align(ui_Panel8, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_Panel8, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image1 = lv_img_create(ui_Panel8);
    lv_img_set_src(ui_Image1, &ui_img_1062568172);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 256
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 256
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_angle(ui_Image1, 1);
    lv_img_set_zoom(ui_Image1, 160);
    lv_obj_set_style_outline_color(ui_Image1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Image1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Image1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Image1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Image1, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui_Image1, lv_color_hex(0x939393), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_Image1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5 = lv_label_create(ui_Panel8);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label5, 1);
    lv_obj_set_y(ui_Label5, -4);
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "12°C 45%");
    lv_obj_set_style_text_color(ui_Label5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void tv3_screen_init(void)
{
	lv_obj_clear_flag(tv3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel4 = lv_obj_create(tv3);
    lv_obj_set_width(ui_Panel4, 239);
    lv_obj_set_height(ui_Panel4, 320);
    lv_obj_set_align(ui_Panel4, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel4, lv_color_hex(0x31B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Panel4, lv_color_hex(0x000C24), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel4, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel4, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Panel4, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel7 = lv_obj_create(ui_Panel4);
    lv_obj_set_width(ui_Panel7, 240);
    lv_obj_set_height(ui_Panel7, 92);
    lv_obj_set_x(ui_Panel7, -1);
    lv_obj_set_y(ui_Panel7, 12);
    lv_obj_set_align(ui_Panel7, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chart3 = lv_chart_create(ui_Panel7);
    lv_obj_set_width(ui_Chart3, 215);
    lv_obj_set_height(ui_Chart3, 87);
    lv_obj_set_x(ui_Chart3, 7);
    lv_obj_set_y(ui_Chart3, 4);
    lv_obj_set_align(ui_Chart3, LV_ALIGN_CENTER);
    lv_chart_set_type(ui_Chart3, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ui_Chart3, 28);
    lv_chart_set_range(ui_Chart3, LV_CHART_AXIS_PRIMARY_Y, 10, 20);
    lv_chart_set_range(ui_Chart3, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
    lv_chart_set_div_line_count(ui_Chart3, 5, 7);
    lv_chart_set_axis_tick(ui_Chart3, LV_CHART_AXIS_PRIMARY_X, 0, 0, 5, 2, false, 50);
    lv_chart_set_axis_tick(ui_Chart3, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart3, LV_CHART_AXIS_SECONDARY_Y, 0, 0, 0, 0, false, 25);
    ui_Chart3_series_1 = lv_chart_add_series(ui_Chart3, lv_color_hex(0x31B2FF),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_Chart3_series_1_array[] = { 15, 15, 16, 16, 15, 15, 15, 14, 14, 14, 14, 15, 15, 14, 14, 13, 13, 13, 13, 12, 12, 12, 11, 12, 13, 14, 15, 15 };
    lv_chart_set_ext_y_array(ui_Chart3, ui_Chart3_series_1, ui_Chart3_series_1_array);
    lv_obj_set_style_radius(ui_Chart3, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Chart3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chart3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Chart3, lv_color_hex(0x31B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Chart3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_Chart3, lv_color_hex(0x000C20), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_Chart3, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui_Chart3, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_line_width(ui_Chart3, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);

    lv_obj_set_style_size(ui_Chart3, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(ui_Chart3, lv_color_hex(0x31B2FF), LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chart3, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Chart3, &lv_font_montserrat_10, LV_PART_TICKS | LV_STATE_DEFAULT);

    ui_Panel6 = lv_obj_create(ui_Panel4);
    lv_obj_set_width(ui_Panel6, 240);
    lv_obj_set_height(ui_Panel6, 92);
    lv_obj_set_x(ui_Panel6, 0);
    lv_obj_set_y(ui_Panel6, 107);
    lv_obj_set_align(ui_Panel6, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Chart2 = lv_chart_create(ui_Panel6);
    lv_obj_set_width(ui_Chart2, 215);
    lv_obj_set_height(ui_Chart2, 87);
    lv_obj_set_x(ui_Chart2, 8);
    lv_obj_set_y(ui_Chart2, 2);
    lv_obj_set_align(ui_Chart2, LV_ALIGN_CENTER);
    lv_chart_set_type(ui_Chart2, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ui_Chart2, 28);
    lv_chart_set_range(ui_Chart2, LV_CHART_AXIS_PRIMARY_Y, 10, 20);
    lv_chart_set_range(ui_Chart2, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
    lv_chart_set_div_line_count(ui_Chart2, 5, 7);
    lv_chart_set_axis_tick(ui_Chart2, LV_CHART_AXIS_PRIMARY_X, 0, 0, 5, 2, false, 50);
    lv_chart_set_axis_tick(ui_Chart2, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart2, LV_CHART_AXIS_SECONDARY_Y, 0, 0, 0, 0, false, 25);
    ui_Chart2_series_1 = lv_chart_add_series(ui_Chart2, lv_color_hex(0x31B2FF),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_Chart2_series_1_array[] = { 15, 15, 16, 16, 15, 15, 15, 14, 14, 14, 14, 15, 15, 14, 14, 13, 13, 13, 13, 12, 12, 12, 11, 12, 13, 14, 15, 15 };
    lv_chart_set_ext_y_array(ui_Chart2, ui_Chart2_series_1, ui_Chart2_series_1_array);
    lv_obj_set_style_radius(ui_Chart2, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Chart2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Chart2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Chart2, lv_color_hex(0x31B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Chart2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui_Chart2, lv_color_hex(0x000C20), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui_Chart2, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui_Chart2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_line_width(ui_Chart2, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);

    lv_obj_set_style_size(ui_Chart2, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(ui_Chart2, lv_color_hex(0x31B2FF), LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Chart2, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Chart2, &lv_font_montserrat_10, LV_PART_TICKS | LV_STATE_DEFAULT);

    ui_Panel3 = lv_obj_create(ui_Panel4);
    lv_obj_set_width(ui_Panel3, 240);
    lv_obj_set_height(ui_Panel3, 120);
    lv_obj_set_x(ui_Panel3, 0);
    lv_obj_set_y(ui_Panel3, -100);
    lv_obj_set_align(ui_Panel3, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel3, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel3, lv_color_hex(0x30B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Panel3, lv_color_hex(0x31B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Panel3, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label3 = lv_label_create(ui_Panel3);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, -38);
    lv_obj_set_y(ui_Label3, 10);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "27");
    lv_obj_set_style_text_font(ui_Label3, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image3 = lv_img_create(ui_Panel3);
    lv_img_set_src(ui_Image3, &ui_img_1282837856);
    lv_obj_set_width(ui_Image3, LV_SIZE_CONTENT);   /// 64
    lv_obj_set_height(ui_Image3, LV_SIZE_CONTENT);    /// 64
    lv_obj_set_x(ui_Image3, 92);
    lv_obj_set_y(ui_Image3, 10);
    lv_obj_set_align(ui_Image3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image4 = lv_img_create(ui_Panel3);
    lv_img_set_src(ui_Image4, &ui_img_525640412);
    lv_obj_set_width(ui_Image4, LV_SIZE_CONTENT);   /// 64
    lv_obj_set_height(ui_Image4, LV_SIZE_CONTENT);    /// 64
    lv_obj_set_x(ui_Image4, -98);
    lv_obj_set_y(ui_Image4, 10);
    lv_obj_set_align(ui_Image4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label4 = lv_label_create(ui_Panel3);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label4, 36);
    lv_obj_set_y(ui_Label4, 10);
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "56");
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel5 = lv_obj_create(ui_Panel3);
    lv_obj_set_width(ui_Panel5, 6);
    lv_obj_set_height(ui_Panel5, 97);
    lv_obj_set_align(ui_Panel5, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    ui_Panel10 = lv_obj_create(ui_Panel3);
    lv_obj_set_width(ui_Panel10, 236);
    lv_obj_set_height(ui_Panel10, 96);
    lv_obj_set_x(ui_Panel10, 0);
    lv_obj_set_y(ui_Panel10, 97);
    lv_obj_set_align(ui_Panel10, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel10, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel10, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image5 = lv_img_create(ui_Panel10);
    lv_img_set_src(ui_Image5, &ui_img_air_quality_gauge_png);
    lv_obj_set_width(ui_Image5, LV_SIZE_CONTENT);   /// 339
    lv_obj_set_height(ui_Image5, LV_SIZE_CONTENT);    /// 271
    lv_obj_set_align(ui_Image5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image5, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image5, 150);

    ui_Image2 = lv_img_create(ui_Panel10);
    lv_img_set_src(ui_Image2, &ui_img_gauge_pointer_png);
    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);   /// 48
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);    /// 84
    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_Image2, 23, 60);
    lv_img_set_angle(ui_Image2, 1100);
    lv_img_set_zoom(ui_Image2, 300);
    //lv_obj_set_style_blend_mode(ui_Image2, LV_BLEND_MODE_SUBTRACTIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_Image2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel9 = lv_obj_create(ui_Panel10);
    lv_obj_set_width(ui_Panel9, 60);
    lv_obj_set_height(ui_Panel9, 60);
    lv_obj_set_x(ui_Panel9, 0);
    lv_obj_set_y(ui_Panel9, 16);
    lv_obj_set_align(ui_Panel9, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel9, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel9, lv_color_hex(0x31B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel9, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label6 = lv_label_create(ui_Panel9);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, "500");
    lv_obj_set_style_text_color(ui_Label6, lv_color_hex(0x31B2FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label6, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

	lvgl_removevoc_Animation();
}

void lvgl_distance_set(uint16_t distance)
{
	char temp_distance_string[10] = {0};

	sprintf(temp_distance_string, "%d", distance);

	lv_label_set_text(ui_Label1, temp_distance_string);
}

void lvgl_packet_count_set(uint32_t packet_count)
{
	char temp_distance_string[10] = {0};

	sprintf(temp_distance_string, "%d", (uint16_t)packet_count);

	lv_label_set_text(ui_Label2, temp_distance_string);
}

void lvgl_voc_index_update(uint32_t voc_index)
{
	uint8_t gauge_section = voc_index / COLOR_GAUGE_SSECTION_RANGE;
	
	char voc_index_string[5] = {0};
	
	itoa(voc_index, voc_index_string, 10);
	
	hSensor.air_quality = voc_index;
	
	//If the timer animation is busy do not update the gauge value to prevent animation interruption
	if(lv_anim_get_timer()->paused)
	{
		lvgl_voc_gauge_angle(hSensor.air_quality);
	}
	//
	
	lv_obj_set_style_bg_color(ui_Panel1, 								//round panel color 
	 							lv_color_hex(lvgl_gauge_color_array[gauge_section][LVGL_GAUGE_COLOR_FRONT]),
	  							LV_PART_MAIN | LV_STATE_DEFAULT);
	  							
	lv_obj_set_style_bg_grad_color(ui_Panel2,								//background gradient color 
							lv_color_hex(lvgl_gauge_color_array[gauge_section][LVGL_GAUGE_COLOR_BACK]), 
							LV_PART_MAIN | LV_STATE_DEFAULT);
							
	lv_label_set_text(ui_Label1, voc_index_string);
	  							
	lv_label_set_text(ui_Label2, lvgl_gauge_comment_string[gauge_section]);
				  							
	lv_arc_set_value(ui_Arc1, voc_index);
	
	lvgl_voc_index_chart_insert(voc_index);
}

void lvgl_temperature_humidity_update(float temperature, float humidity)
{
	char sensor_data_string[20] = {0};
	
	hSensor.temperature = (int16_t) temperature;
	
	hSensor.humidity 	= (uint16_t) humidity;
	
	itoa((int8_t)temperature, sensor_data_string, 10);
	
	lv_label_set_text(ui_Label3, sensor_data_string);
	
	itoa((uint8_t)humidity, sensor_data_string, 10);
	
	lv_label_set_text(ui_Label4, sensor_data_string);
	
	sprintf(sensor_data_string, "%2d°C %2d%%",(int16_t)temperature, (uint16_t)humidity);
	
	lv_label_set_text(ui_Label5, sensor_data_string);
	
	lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_1, (uint32_t) temperature);
	
	 lv_chart_set_range(ui_Chart2, LV_CHART_AXIS_PRIMARY_Y, (uint32_t) temperature - 5, (uint32_t) temperature + 5);
	
	lv_chart_set_next_value(ui_Chart3, ui_Chart3_series_1, (uint32_t) humidity);
	
	 lv_chart_set_range(ui_Chart3, LV_CHART_AXIS_PRIMARY_Y, (uint32_t) humidity - 5, (uint32_t) humidity + 5);
}

void lvgl_voc_index_chart_insert(uint32_t voc_index)
{
	uint8_t gauge_section = voc_index / COLOR_GAUGE_SSECTION_RANGE;
	
	//shift the other values to the left
	memcpy(ui_Chart1_series_1_array, ui_Chart1_series_1_array + 1, (CHART_DATA_COUNT_LIMIT - 1)*sizeof(lv_coord_t));
	//Insert new value 
	ui_Chart1_series_1_array[CHART_DATA_COUNT_LIMIT - 1] = (uint16_t) voc_index;
	//Update chart
	lv_chart_set_series_color(ui_Chart1, ui_Chart1_series_1, lv_color_hex(lvgl_gauge_color_array[gauge_section][LVGL_GAUGE_COLOR_FRONT]));
                                                              
	lv_chart_set_ext_y_array(ui_Chart1, ui_Chart1_series_1, ui_Chart1_series_1_array);
}

void lvgl_communication_status(bool communication_status)
{
	if(communication_status)
	{
		lv_obj_add_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
		lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);
	}

}

void lvgl_screen_navigate(lvgl_nav_e action)
{
	static uint8_t current_tv_id = SCREEN_AIR_QUALITY_ID;
	
	switch (action) 
	{
		case LVGL_MENU_NEXT:
			++current_tv_id;
			
			if(SCREEN_TOTAL == current_tv_id)
			{
				current_tv_id = SCREEN_AIR_QUALITY_ID;
			}
			
			lv_obj_set_tile_id(display, 0, current_tv_id, LV_ANIM_ON);
			break;
		case LVGL_MENU_ENTER:
			//animation in 
			if(SCREEN_TEMPERATURE_HUMIDITY_ID == current_tv_id)
			{
				lvgl_extend_Animation();
				lvgl_addvoc_Animation();
				lvgl_indicator_Animation();
			}
			else if(SCREEN_AIR_QUALITY_ID == current_tv_id)
			{
				lvgl_show_Animation();
			}

			
			break;
		case LVGL_MENU_EXIT:
			//animaton out
			if(SCREEN_TEMPERATURE_HUMIDITY_ID == current_tv_id)
			{
				lvgl_removevoc_Animation();
				lvgl_shrink_Animation();
			}
			else if(SCREEN_AIR_QUALITY_ID == current_tv_id)
			{
				lvgl_hide_Animation();
			}
			

			
			break;
		case LVGL_MENU_BACK:
			--current_tv_id;
			
			if(SCREEN_INTRO_ID == current_tv_id)
			{
				current_tv_id = SCREEN_TEMPERATURE_HUMIDITY_ID;
			}
			
			lv_obj_set_tile_id(display, 0, current_tv_id, LV_ANIM_ON);
		
			break;

	}
}

///////////////////// ANIMATIONS ////////////////////
static void lvgl_extend_Animation(void)
{
	lv_obj_t * TargetObject = ui_Panel3;
	int delay = 0;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_height);
    lv_anim_set_values(&PropertyAnimation_0, 120, 290);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in_out);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
static void lvgl_shrink_Animation(void)
{
	lv_obj_t * TargetObject = ui_Panel3;
	int delay = 1000;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_height);
    lv_anim_set_values(&PropertyAnimation_0, 290, 120);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in_out);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
static void lvgl_hide_Animation(void)
{
	lv_obj_t * TargetObject = ui_Panel8;
	int delay = 0;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 255, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
static void lvgl_show_Animation(void)
{
	lv_obj_t * TargetObject = ui_Panel8;
	int delay = 0;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
static void lvgl_addvoc_Animation()
{
	lv_obj_t * TargetObject = ui_Panel10;
	int delay = 0;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 1000);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
static void lvgl_removevoc_Animation()
{
	lv_obj_t * TargetObject = ui_Panel10;
	int delay = 0;
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 255, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
static void lvgl_indicator_Animation()
{
	lv_obj_t * TargetObject = ui_Image2;
	int delay = 500;
	int32_t end_angle = lvgl_convert_voc2angle(hSensor.air_quality);;

    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;

    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 3000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_0, 1100, end_angle);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
    lv_anim_set_deleted_cb(&PropertyAnimation_0,  _ui_anim_callback_delete_animation);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}


void lvgl_demo_ui(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);

    lv_theme_t * theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);

	//Initialize 2 tiles that act as pages
    display = lv_tileview_create(scr);
	lv_obj_align(display, LV_ALIGN_TOP_RIGHT, 0, 0);
	tv1 = lv_tileview_add_tile(display, 0, SCREEN_INTRO_ID, LV_DIR_HOR);
	tv2 = lv_tileview_add_tile(display, 0, SCREEN_AIR_QUALITY_ID, LV_DIR_HOR);
	tv3 = lv_tileview_add_tile(display, 0, SCREEN_TEMPERATURE_HUMIDITY_ID, LV_DIR_HOR);
	tv2_screen_init();
	tv3_screen_init();
    // Create image

	lv_obj_set_tile_id(display, 0, 0, LV_ANIM_ON);
    img_logo = lv_img_create(tv1);
    lv_img_set_src(img_logo, &ui_img_ue_logo_png);


    start_animation(tv1);
}


static void _ui_anim_callback_free_user_data(lv_anim_t * a)
{
    lv_mem_free(a->user_data);
    a->user_data = NULL;
}
static void _ui_anim_callback_set_height(lv_anim_t * a, int32_t v)
{

    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_height(usr->target, v);

}

static void _ui_anim_callback_set_opacity(lv_anim_t * a, int32_t v)
{

    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_obj_set_style_opa(usr->target, v, 0);

}

static int32_t _ui_anim_callback_get_opacity(lv_anim_t * a)
{

    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    return lv_obj_get_style_opa(usr->target, 0);

}

static void _ui_anim_callback_set_image_angle(lv_anim_t * a, int32_t v)
{
	char voc_text[4] = {0};
	
	int temp_voc = 0;
    ui_anim_user_data_t * usr = (ui_anim_user_data_t *)a->user_data;
    lv_img_set_angle(usr->target, v);
    
    temp_voc = lvgl_convert_angle2voc(v);
    
    sprintf(voc_text, "%d", temp_voc);
    
    lv_label_set_text(ui_Label6, voc_text);
}

static void _ui_anim_callback_delete_animation(lv_anim_t * a)
{
	memset(a, 0, sizeof(lv_anim_t));
}

static void lvgl_voc_gauge_angle(int32_t voc_value)
{
	char voc_text[4] = {0};
	
	int temp_voc = voc_value;
	
	int temp_angle = lvgl_convert_voc2angle(voc_value);;
	
	lv_img_set_angle(ui_Image2, temp_angle);
    
    sprintf(voc_text, "%d", temp_voc);
    
    lv_label_set_text(ui_Label6, voc_text);
}

static int lvgl_convert_angle2voc(int meter_angle) 
{
    // Define the range limits
    int voc_min = 0;
    int voc_max = 500;
    int angle_min = -1100;
    int angle_max = 1100;

    // Convert the meter angle to VOC data using reverse linear mapping
    int voc_data = (int)(((float)(float)(meter_angle - angle_min) / (float)(angle_max - angle_min)) * (voc_max - voc_min) + voc_min);

    return voc_data;
} 

static int lvgl_convert_voc2angle(int voc_data) 
{
    // Define the range limits
    int voc_min = 0;
    int voc_max = 500;
    int angle_min = -1100;
    int angle_max = 1100;

    // Convert the VOC data to meter angle using linear mapping
    int meter_angle = (int)((float)((float)(voc_data - voc_min) / (float)(voc_max - voc_min)) * (angle_max - angle_min) + angle_min);

    return meter_angle;
} 
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
