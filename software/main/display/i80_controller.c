/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  st7789.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 5, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "i80_controller.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct
{
    void                		(*i80_gpio_contol)     	(uint8_t gpio_num, uint8_t level);
    esp_lcd_i80_bus_handle_t 	i80_bus;
    esp_lcd_panel_handle_t 		panel_handle;
    void                (*receiveCallback) (void* rx_packet, packet_id_e packet_id);
    module_status_e     networkStatus;
    int32_t             temperature;
    uint8_t             timer;
    int  rssi;
    int  snr;
    int  dr;

}i80_handler;
/* VARIABLES -----------------------------------------------------------------*/
 esp_lcd_panel_handle_t panel_handle = NULL;


 i80_handler hI80 = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
 void i80_controller_bus_init(void);

 void i80_controller_io_init(void);


/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void i80_controller_bus_init(void)
{

    esp_lcd_i80_bus_config_t bus_config =
    {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .dc_gpio_num = PIN_NUM_DC,
        .wr_gpio_num = PIN_NUM_PCLK,
        .data_gpio_nums =
        {
            PIN_NUM_DATA0,
            PIN_NUM_DATA1,
            PIN_NUM_DATA2,
            PIN_NUM_DATA3,
            PIN_NUM_DATA4,
            PIN_NUM_DATA5,
            PIN_NUM_DATA6,
            PIN_NUM_DATA7,
        },
        .bus_width = CONFIG_EXAMPLE_LCD_I80_BUS_WIDTH,
        .max_transfer_bytes = LCD_H_RES * LCD_V_RES  * sizeof(uint16_t),
    };

    esp_lcd_new_i80_bus(&bus_config, &hI80.i80_bus);
}

void i80_controller_io_init(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i80_config_t io_config =
    {
        .cs_gpio_num 			= EXAMPLE_PIN_NUM_CS,
        .pclk_hz 				= EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth 		= 10,
        .on_color_trans_done 	= example_notify_lvgl_flush_ready,
        .user_ctx 				= &disp_drv,
        .lcd_cmd_bits 			= EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits 		= EXAMPLE_LCD_PARAM_BITS,
        .dc_levels =
        {
            .dc_idle_level 		= 0,
            .dc_cmd_level 		= 0,
            .dc_dummy_level 	= 0,
            .dc_data_level 		= 1,
        },
        .flags =
        {
            .swap_color_bytes = !LV_COLOR_16_SWAP, // Swap can be done in LvGL (default) or DMA
        },

    };

    esp_lcd_new_panel_io_i80(hI80.i80_bus, &io_config, &io_handle);

	esp_lcd_panel_dev_config_t panel_config =
	{
		.reset_gpio_num = EXAMPLE_PIN_NUM_RST,
		.rgb_endian = LCD_RGB_ENDIAN_RGB,
		.color_space = ESP_LCD_COLOR_SPACE_RGB,
		.bits_per_pixel = 16,
	};

	esp_lcd_new_panel_st7789(io_handle, &panel_config, &hI80.panel_handle);

	esp_lcd_panel_reset(hI80.panel_handle);
	esp_lcd_panel_init(hI80.panel_handle);

    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    esp_lcd_panel_disp_on_off(hI80.panel_handle, true);

    ESP_LOGI(TAG, "Turn on LCD backlight");

    //TODO create MCU layer and then create function to pass GPIO control fucntions

    i80_gpio_set_level();
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
}

void i80_controller_lvgl_init(void)
{
    lv_init();
    // alloc draw buffers used by LVGL
    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    lv_color_t *buf1 = NULL;
    lv_color_t *buf2 = NULL;

    buf1 = heap_caps_malloc(LCD_H_RES * LCD_V_RES * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    assert(buf1);

    buf2 = heap_caps_malloc(LCD_H_RES * LCD_V_RES * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    assert(buf2);
    ESP_LOGI(TAG, "buf1@%p, buf2@%p", buf1, buf2);
    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES);

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_H_RES;
    disp_drv.ver_res = LCD_V_RES;
    disp_drv.flush_cb = example_lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    disp_drv.full_refresh = 1;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args =
    {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));
}

void i80_controller_mcu_init(void (*i80_gpio_set_level)(uint8_t gpio_num, uint8_t level))
{
	hI80.i80_gpio_contol = i80_gpio_set_level;
}
void i80_controller_init(void)
{

}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
