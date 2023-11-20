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

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void system_send_to_queue(void *tx_buffer, uint8_t command_length);

static void system_uwb_callback(uint8_t* rx_data, uint8_t packetId);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{

	gpio_config_output(PIN_NUM_BK_LIGHT);
	gpio_config_output(PWR_ON_PIN);
	gpio_config_output(PWR_EN_PIN);

	uart_config();
//


	i80_controller_init((void*)gpio_set_level);

	ryuw122_init(system_send_to_queue, system_uwb_callback, RYUW122_ANCHOR);

    ESP_LOGI(TAG, "Display LVGL animation");

//    xTaskCreatePinnedToCore(uart_event_task, "uart event", 10000, NULL, 4, NULL, 0);

    xTaskCreatePinnedToCore(uart_transmission_task, "USART TX handling task", 10000, NULL, 4, NULL, 0);

//    xTaskCreatePinnedToCore(uart_reception_task, "USART RX handling task", 10000, NULL, 4, NULL, 0);



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

static void system_uwb_callback(uint8_t* rx_data, uint8_t packetId)
{

//   uint8_t packetHandler[GPIO_CONTROL_PACKET_SIZE] = {0};
//
//   switch (packetId)
//   {
//      case RYLR993_TEMPERATURE:
//         systemParam.temperature = rylr993_read_temperature();
//         break;
//      case RYLR993_PIN_CONTROL:
//         memcpy(packetHandler, rx_data, GPIO_CONTROL_PACKET_SIZE);
//
//         gpio_pin_control(rx_data[RLYR993_GPIO_NUM_POS],
//                           rx_data[RLYR993_GPIO_STATE_POS]);
//         break;
//
//      default:
//         break;
//   }
}

static void uart_reception_task(void *param)
{
   uartHandler_t uartHandler = {0};

   uint8_t movementType = 0;

   int16_t detectedDistance = 0;

//   system_packet system_buffer = {0};

//   system_queue 		= xQueueCreate(10, sizeof(system_packet));
   for(;;)
   {
      //Waiting for UART packet to get received.
      if(xQueueReceive(uartRxStore_queue, (void * )&uartHandler, portMAX_DELAY))
      {
//    	  detectedDistance = hlk_ld1125h_parse_packet(hUart.uart_rxBuffer,(uint8_t*) &movementType);
//
//    	  if(-1 != detectedDistance)
//    	  {
//    		//   ESP_LOGI(main, "dis = %d, move type %d", detectedDistance, movementType);
//
//    		  system_buffer.data[0] = detectedDistance;
//    		  system_buffer.data[1] = movementType;
//
//    		  system_buffer.packet_size = 2;
//
//    		  xQueueSendToBack(system_queue, &system_buffer, portMAX_DELAY);
//    	  }
      }
   }
}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
