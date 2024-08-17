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
#include "sgp40/driver_sgp40.h"
#include "sgp40/driver_sgp40_algorithm.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";

sgp40_handle_t hSpg40 = {0};
sgp40_gas_index_algorithm_t hVoc = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/


/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void system_send_to_queue(void *tx_buffer, uint8_t command_length);

static void system_uwb_callback(void* rx_data, uint8_t packetId);

static void uart_reception_task(void *param);

static void anchor_periodic_send_task(void *param);

static void air_quality_sensor_task(void *param);

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{

	gpio_config_output(PIN_NUM_BK_LIGHT);
	gpio_config_output(PWR_ON_PIN);
	gpio_config_output(PWR_EN_PIN);

	sgp40_init(&hSpg40);
		
	sgp40_algorithm_init(&hVoc, SGP40_ALGORITHM_TYPE_VOC);

	i80_controller_init((void*)gpio_set_level);

    ESP_LOGI(TAG, "Display LVGL animation");

	xTaskCreatePinnedToCore(air_quality_sensor_task, "air quality", 10000, NULL, 4, NULL, 1);

    while (1)
    {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
    }
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
   


   for(;;)
   {

	   sgp40_get_measure_raw_without_compensation(&hSpg40, &sraw_voc);
	   
	   if(sraw_voc != 0)
	   {
		   sgp40_algorithm_process(&hVoc, (int)sraw_voc, (int*)&index_voc);
	   }
	   
	   
	   if(task_counter >= 45)
	   {
		   sgp40_turn_heater_off(&hSpg40);
		   
		   sgp40_soft_reset(&hSpg40);
		   
		   task_counter = 0;
		   
		   task_period = 8000;
		   
		   calibration_completed = true;
	   }
	   
	   if(calibration_completed)
	   {
		   lvgl_voc_index_update(index_voc);
	   }
	   
	   
	   ESP_LOGI(TAG, "voc raw :%d", (int)sraw_voc);
	   
	   ESP_LOGI(TAG, "voc index :%d", (int)index_voc);
	   
	   ++task_counter;

	   vTaskDelayUntil( &xLastWakeTime, task_period/portTICK_PERIOD_MS );
//	   vTaskDelay(10000/portTICK_PERIOD_MS);
   }
}


/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
