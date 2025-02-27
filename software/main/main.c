/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 5, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "main.h"
#include "display/lvgl_ui.h"
#include "esp_freertos_hooks.h"
#include "hal/gpio_types.h"
#include "hdc1080/hdc1080.h"
#include "misc/lv_timer.h"
#include "rom/gpio.h"
#include "sgp40/driver_sgp40.h"
#include "sgp40/driver_sgp40_algorithm.h"
#include "gpio/button.h"
#include "gpio/esp_interrupt.h"

#include "wireless/wifi_connect.h"
#include "wireless/mqtt.h"
#include <string.h>

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";

sgp40_handle_t hSpg40 = {0};
sgp40_gas_index_algorithm_t hVoc = {0};
hdc1080_config_t hdc_settings = {0};

TaskHandle_t hMain_eventTask			= NULL;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/


/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void system_send_to_queue(void *tx_buffer, uint8_t command_length);

static void system_uwb_callback(void* rx_data, uint8_t packetId);

static uint32_t main_get_systick(void);

static void main_down_button_handler(void);

static void main_up_button_handler(void);

static void uart_reception_task(void *param);

static void anchor_periodic_send_task(void *param);

static void wirless_init_task(void* param);

static void mqtt_msg_pars_task(void* param);

static void mqtt_msg_send_task(void* param);

static void air_quality_sensor_task(void *param);

static void manager_task(void *param);

static void event_handle_task(void *param);

static void lvgl_time_task(void*param);
// VOC function array 

static void main_mqtt_msg_strings();

static void main_mqtt_msg_ppm();

static void main_mqtt_msg_humidity();

static void main_mqtt_msg_temperature();

static void main_mqtt_msg_active();

static void main_mqtt_msg_voc();

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{
	
	uint8_t task_return = 0;
	gpio_config_ext_interrupt(BUTTON1_PIN, GPIO_INTR_NEGEDGE, gpio_isr_handle);
	gpio_config_ext_interrupt(BUTTON2_PIN, GPIO_INTR_POSEDGE, gpio_isr_handle);
	
	button_init(main_get_systick, gpio_get_level);
	button_add(GPIO_ID_PIN(BUTTON1_PIN), 1, 2000,  main_up_button_handler);
	button_add(GPIO_ID_PIN(BUTTON2_PIN), 0, 2000,  main_down_button_handler);
	
	gpio_config_output(PIN_NUM_BK_LIGHT);
	gpio_config_output(PWR_ON_PIN);
	gpio_config_output(PWR_EN_PIN);
	

	sgp40_init(&hSpg40);
	
	sgp40_algorithm_init_with_sampling_interval(&hVoc, SGP40_ALGORITHM_TYPE_VOC, 3);  
	
	hdc1080_driver_init((hdc1080_handle_t*) &hSpg40);
	
	hdc_settings.bits.heater 								= HDC1080_HEATER_DISABLED;
	hdc_settings.bits.humidity_measurement_resolution 		= HDC1080_HUMIDITY_RESOLUTION_14BIT;
	hdc_settings.bits.temperature_measurement_resolution 	= HDC1080_TEMPERATURE_RESOLUTION_14BIT;
	hdc_settings.bits.mode_of_acquisition 					= HDC1080_ACQUISITION_HUMIDITY_AND_TEMPERATURE;
	
	hdc1080_configure(&hdc_settings);
	
	i80_controller_init((void*)gpio_set_level);

    ESP_LOGW(TAG, "Display LVGL animation");
    
	task_return = xTaskCreatePinnedToCore(wirless_init_task, "WiFi_init", 10000, NULL, 4, NULL, 0);
	
	ESP_LOGW(TAG, "WiFi_init %d", task_return);
	
	task_return = xTaskCreatePinnedToCore(lvgl_time_task, "lvgl_time_task", 10000, NULL, 4, NULL, 1);
	
	ESP_LOGW(TAG, "lvgl_time_task%d", task_return);
	
	task_return = xTaskCreatePinnedToCore(air_quality_sensor_task, "air_quality", 10000, NULL, 4, NULL, 1);
	
	ESP_LOGW(TAG, "air quality %d", task_return);
	
	task_return = xTaskCreatePinnedToCore(event_handle_task, "lvgl_time_task", 10000/2, NULL, 4, &hMain_eventTask, 1);
	
	ESP_LOGW(TAG, "lvgl_time_task %d", task_return);
	
	task_return = xTaskCreatePinnedToCore(manager_task, "managers_task", 10000/2, NULL, 4, NULL, 1);

    ESP_LOGW(TAG, "managers_task %d", task_return);
	//Wait for WiFi and MQTT broker connection to be established.

 	vTaskDelay(pdMS_TO_TICKS(15000));
		
	task_return = xTaskCreatePinnedToCore(mqtt_msg_pars_task, "MQTT parser", 10000/2, NULL, 4, NULL, 0);
	
	ESP_LOGW(TAG, "MQTT parser %d", task_return);
	
	task_return = xTaskCreatePinnedToCore(mqtt_msg_send_task, "MQTT sender", 10000/2, NULL, 4, NULL, 0);
		
	ESP_LOGI(TAG, "MQTT sender %d", task_return);
	
	//vTaskDelete(NULL);

    while (1)
    {
//        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
//        vTaskDelay(pdMS_TO_TICKS(10));
//        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
//        lv_timer_handler();
    }
}

void vApplicationIdleHook(void) 
{
    int core_id = xPortGetCoreID(); // Get the current core ID

    // Core-specific behavior
    if (core_id == 0) 
    {
		// Core 0 handls RF related tasks
    } 
    else if (core_id == 1)
    {

    }
       // printf("Core 1: Performing background tasks...\n");

}

static void system_send_to_queue(void *tx_buffer, uint8_t command_length)
{
	uartHandler_t temp_buffer = {0};

	memcpy(temp_buffer.uart_txBuffer, tx_buffer, command_length);

	ESP_LOGI(TAG, "command :%s, size: %d", temp_buffer.uart_txBuffer, command_length);

	temp_buffer.uart_txPacketSize = command_length;

   xQueueSendToBack(uartTx_queue, (void*) &temp_buffer, portMAX_DELAY);
}

static void system_uwb_callback(void* rx_data, uint8_t packetId)
{
	static uint32_t packet_count = 0;

	uint16_t temp_distance = 0;

	switch (packetId)
	{
		case RYUW122_ANCHOR_DISTANCE:
			memcpy(&temp_distance, rx_data, 2);

			lvgl_communication_status(false);

			lvgl_distance_set(temp_distance);

			packet_count++;

			lvgl_packet_count_set(packet_count);

			break;

		case RYUW122_ANCHOR_RX:

			break;

		case RYUW122_TAG_RX:

			break;

		default:
			break;
	}


}

static void uart_reception_task(void *param)
{
   uartHandler_t uartHandler = {0};

   uint8_t movementType = 0;

   int16_t detectedDistance = 0;


   for(;;)
   {
      //Waiting for UART packet to get received.
      if(xQueueReceive(uartRxStore_queue, (void * )&uartHandler, portMAX_DELAY))
      {
    	  ryuw122_packet_separator((char*) uartHandler.uart_rxBuffer, uartHandler.uart_rxPacketSize);

      }
   }
}

static void anchor_periodic_send_task(void *param)
{
	static uint32_t return_idle_timer = 0;

	return_idle_timer =  SYS_TICK();

	const char* test_string = "test";
	for(;;)
	{

		if(ryuw122_anchor_send(test_string, 4) == false)
		{

			if((SYS_TICK() - return_idle_timer) >= 1000)
			{
				return_idle_timer = SYS_TICK();

				ryuw122_return_idle();

				lvgl_communication_status(true);
			}
		}
		vTaskDelay(200/portTICK_PERIOD_MS);
	}
}


static void air_quality_sensor_task(void *param)
{
	
   static bool calibration_completed = false;
   uint16_t sraw_voc = 0;
   
   int32_t index_voc = 0;
   
   TickType_t xLastWakeTime;
   TickType_t task_period = 3000;
	
   uint8_t task_counter = 0; 
   
   uint16_t humidity_reg = 0;
   uint16_t temperature_reg = 0;
   
   


   for(;;)
   {

	   //sgp40_get_measure_raw_without_compensation(&hSpg40, &sraw_voc);
	   
	   hdc1080_conversion_request();
	   
	   sgp40_humidity_convert_to_register(&hSpg40, hdc1080_sensor_read()->humidity, &humidity_reg);
	   
	   sgp40_temperature_convert_to_register(&hSpg40, hdc1080_sensor_read()->temperature, &temperature_reg);
	   
	 //  sgp40_get_measure_raw(&hSpg40, humidity_reg,
       //                       temperature_reg, &sraw_voc);
	   
	   
	   
	   if(0 == sgp40_get_measure_raw(&hSpg40, humidity_reg,
               temperature_reg, &sraw_voc))
	   {
		   sgp40_algorithm_process(&hVoc, (int)sraw_voc, (int*)&index_voc);
	   }
	   
	   
	   if(task_counter >= 15)
	   {
		   sgp40_turn_heater_off(&hSpg40);
		   
		   sgp40_soft_reset(&hSpg40);
		   
		   task_counter = 0;
		   
		   calibration_completed = true;
	   }
	   
	   if(calibration_completed)
	   {
		   lvgl_voc_index_update(index_voc);
	   }
	   
	   lvgl_temperature_humidity_update(hdc1080_sensor_read()->temperature, hdc1080_sensor_read()->humidity);
	   
	   ESP_LOGI(TAG, "voc raw :%d", (int)sraw_voc);
	   
	   ESP_LOGI(TAG, "voc index :%d", (int)index_voc);
	   
	   ESP_LOGI(TAG, "temperature :%f degree", hdc1080_sensor_read()->temperature);
	   ESP_LOGI(TAG, "humidity :%f %%", hdc1080_sensor_read()->humidity);
	   
	  
	   
	   ++task_counter;

	   vTaskDelayUntil( &xLastWakeTime, task_period/portTICK_PERIOD_MS );
//	   vTaskDelay(10000/portTICK_PERIOD_MS);
   }
}

static void manager_task(void *param)
{

   TickType_t xLastWakeTime;
   TickType_t task_period = 50;
   
   for(;;)
   {
		button_manager();
		
		lvgl_wifi_state_set(wifi_is_connected());

	    vTaskDelayUntil( &xLastWakeTime, task_period/portTICK_PERIOD_MS );
   }
}

static void event_handle_task(void* param)
{
	while(1)
	{
		//Note: CallbackID is cleared immediately after executing this task
		if(xTaskNotifyWait(0, ULONG_MAX, &interrupt_id, portMAX_DELAY ))
		{
			switch (interrupt_id)
			{
			case BUTTON1_PIN:
				button_debounce(BUTTON1_PIN);
				break;
				
			case BUTTON2_PIN:
				button_debounce(BUTTON2_PIN);
				break;
				
			
			default:
				break;
			}
		}
	}
	
}

void lvgl_time_task(void* param)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{

        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
		//WiFi connection check
		//ui_set_wifi_switch_state(wifi_is_connected());

        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10) );

	}
}

static void wirless_init_task(void* param)
{
	wifi_connect();

	mqtt_app_start();

	vTaskDelete(NULL);
}

static void mqtt_msg_pars_task(void* param)
{
	mqtt_buffer_t mqttBuffer = {0};

	const char publishRequest = 1;

	while(1)
	{
		if(xQueueReceive(mqttSubscribe_queue, (void *)&mqttBuffer, portMAX_DELAY))
		{
			switch (mqttBuffer.eventType)
			{
				case MQTT_BROKER_CONNECT:

					//active state publish
					mqtt_publish(MQTT_AIR_QUALITY_SETACTIVE, &publishRequest, 0);
					
					lvgl_mqtt_state_set(true);
					
					break;
				case MQTT_BROKER_DISCONNECT:
				
					lvgl_mqtt_state_set(false);

					break;
				case MQTT_TOPIC_DATA_RX:


					break;
				default:
					break;
			}
		}
	}
}

static void mqtt_msg_send_task(void* param)
{
   	TickType_t xLastWakeTime;
   	TickType_t task_period = 9000;		//10s
   
   	uint8_t message_id = 0;
   	   	
   	void (*mqtt_message_sequence[])(void) = 
   	{
		main_mqtt_msg_strings,
		main_mqtt_msg_humidity,
		main_mqtt_msg_temperature,
		main_mqtt_msg_active,
		main_mqtt_msg_voc
	};
	
	while(1)
	{
		vTaskDelayUntil( &xLastWakeTime, task_period/portTICK_PERIOD_MS );
		
		ESP_LOGW(TAG, "mqtt publish %d", message_id);
		
		mqtt_message_sequence[message_id]();
		
		++message_id;
		
		message_id = message_id % 5;
			
	}
}
static uint32_t main_get_systick(void)
{
	return SYS_TICK();
}

static void main_up_button_handler(void)
{
	
	button_state_t button_state = button_state_get(GPIO_ID_PIN(BUTTON1_PIN));
	if(button_state == BUTTON_CLICKED) 
	{
		ESP_LOGW(TAG, "Button1 pressed");
		lvgl_screen_navigate(LVGL_MENU_NEXT);
	}
	else if (button_state == BUTTON_LONG_PRESSED)
	{
		ESP_LOGW(TAG, "Button1 long pressed");
		lvgl_screen_navigate(LVGL_MENU_ENTER);
	}
}

static void main_down_button_handler(void)
{

	button_state_t button_state = button_state_get(GPIO_ID_PIN(BUTTON2_PIN));
	if(button_state == BUTTON_CLICKED) 
	{
		ESP_LOGW(TAG, "Button2 pressed");
		lvgl_screen_navigate(LVGL_MENU_BACK);
	}
	else if (button_state == BUTTON_LONG_PRESSED)
	{
		ESP_LOGW(TAG, "Button2 long pressed");
		lvgl_screen_navigate(LVGL_MENU_EXIT);
	}
}


static void main_mqtt_msg_strings()
{
	char temp_string[50] = {0}; 

    char temp_air_quality_value_string[5][15] = 
    {
    MQTT_AIR_QUALITY_STRING_EXCELLENT,
    MQTT_AIR_QUALITY_STRING_FAIR,
    MQTT_AIR_QUALITY_STRING_INFERIOR,
    MQTT_AIR_QUALITY_STRING_POOR,
    MQTT_AIR_QUALITY_STRING_UNKNOWN,
	};
	
	sprintf(temp_string, "%s", temp_air_quality_value_string[lvgl_get_ui_sensor_data()->air_quality_gauge_segment]); 
	
	ESP_LOGW(TAG, "%s", temp_string);

	mqtt_publish(MQTT_AIR_QUALITY_SETSTRING, temp_string, strlen(temp_string));
}
static void main_mqtt_msg_ppm()
{
//	char temp_string[50] = {0}; 
//	
//	mqtt_publish(MQTT_AIR_QUALITY_SETPPM, temp_string, strlen(temp_string));
}
static void main_mqtt_msg_humidity()
{
	char temp_string[50] = {0}; 
	
	sprintf(temp_string, "%d", lvgl_get_ui_sensor_data()->humidity); 
	
	ESP_LOGW(TAG, "%s", temp_string);
	
	mqtt_publish(MQTT_AIR_QUALITY_SETHUMIDITY, temp_string, strlen(temp_string));
}
static void main_mqtt_msg_temperature()
{
	char temp_string[50] = {0}; 
	
	sprintf(temp_string, "%d", lvgl_get_ui_sensor_data()->temperature); 
	
	ESP_LOGW(TAG, "%s", temp_string);
	
	mqtt_publish(MQTT_AIR_QUALITY_SETTEMPRATURE, temp_string, strlen(temp_string));
}
static void main_mqtt_msg_active()
{
	char temp_string[50] = {0}; 
	
	temp_string[0] = '1';
	
	ESP_LOGW(TAG, "%s", temp_string);
	
	mqtt_publish(MQTT_AIR_QUALITY_SETACTIVE, temp_string, strlen(temp_string));
}
static void main_mqtt_msg_voc()
{
	char temp_string[50] = {0}; 
		
	sprintf(temp_string, "%d",lvgl_get_ui_sensor_data()->air_quality); 
	
	ESP_LOGW(TAG, "%s", temp_string);
	
	mqtt_publish(MQTT_AIR_QUALITY_SETVOC, temp_string, strlen(temp_string));
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
