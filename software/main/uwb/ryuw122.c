/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  ryuw122.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 14, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "ryuw122.h"
#include "at_command.h"
#include "function_buffer.h"
#include <ctype.h>
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct
{
    void                	(*commandSend)     (void *tx_buffer, uint8_t command_length);
    void               	 	(*receiveCallback) (void* rx_packet, packet_id_e packet_id);
    communication_mode_e    networkStatus;
    module_mdoe_e			operation_mode;
    bool					busy;
    bool					initialized;



}ryuw122_handler;
/* VARIABLES -----------------------------------------------------------------*/
ryuw122_handler hRyuw122 = {0};

primitive_buffer_t premetive_buffer = {0};

char parameter_buffer[NUM_COMMAND_PARAM][MAX_PARAM_LENGTH] = {0};

static const char *TAG = "uwb";
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void ryuw122_init(void* port_send, void* uwb_callback, bool mode)
{
	hRyuw122.commandSend 		= port_send;

	hRyuw122.receiveCallback 	= uwb_callback;

	hRyuw122.operation_mode		= mode;

	primitive_buffer_init(&premetive_buffer);

	ryuw122_set_mode(hRyuw122.operation_mode);

	primitive_push(&premetive_buffer, ryuw122_set_address);

	primitive_push(&premetive_buffer, ryuw122_set_network_id);

	primitive_push(&premetive_buffer, ryuw122_set_password);
}

module_mdoe_e ryuw122_get_mode(void)
{
	return hRyuw122.operation_mode;
}

bool ryuw122_get_state(void)
{
	return hRyuw122.busy;
}

void ryuw122_set_mode(module_mdoe_e mode)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 hRyuw122.operation_mode = mode;

	 sprintf(parameter_buffer[0],"%d", hRyuw122.operation_mode);

	 command_length = at_command_form(MODE, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);

	 hRyuw122.busy = true;
}
/**
 * @brief This function sets the network ID for the UWB module using a predefined value (UWB_NETWORK).
 *
 */
void ryuw122_set_network_id(void)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;


	 sprintf(parameter_buffer[0],"%s", UWB_NETWORK);

	 command_length = at_command_form(NETWORKID, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);

	 hRyuw122.busy = true;
}

void ryuw122_set_address(void)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 sprintf(parameter_buffer[0],"%s", UWB_ANCH_ADDRESS);

	 command_length = at_command_form(ADDRESS, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);

	 hRyuw122.busy = true;
}

void ryuw122_set_password(void)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 sprintf(parameter_buffer[0],"%s", UWB_PASSWORD);

	 command_length = at_command_form(CPIN, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);

	 hRyuw122.initialized = true;

	 hRyuw122.busy = true;
}

bool ryuw122_anchor_send(uint8_t* tx_buffer, uint8_t data_size)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	bool retval = false;

	if(hRyuw122.busy == false && hRyuw122.initialized == true)
	{
		 sprintf(parameter_buffer[0],"%s", UWB_TAG_ADDRESS);

		 sprintf(parameter_buffer[1],"%d", data_size);

		 memset(parameter_buffer[2], 0, MAX_PARAM_LENGTH);

		 memcpy(parameter_buffer[2], tx_buffer, data_size);

		 command_length = at_command_form(ANCHOR_SEND, parameter_buffer, 3, temp_command_string);

		 hRyuw122.commandSend(temp_command_string, command_length);

		 retval = true;

		 hRyuw122.busy = true;
	}

	return retval;
}

void ryuw122_tag_send(module_mdoe_e mode)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 hRyuw122.operation_mode = mode;

	 sprintf(parameter_buffer[0],"%d", hRyuw122.operation_mode);

	 command_length = at_command_form(TAG_SEND, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);
}




bool ryuw122_packet_separator(char* packet, uint8_t packet_size)
{
    bool validPacket = false;

    uint8_t parameter_count = 0;

    uint16_t detected_distance = 0;

    char temp_command_header[MAX_PARAM_LENGTH] = {0};
    // Parse the received packet and check for a valid packet
    validPacket =  at_command_parser(temp_command_header, parameter_buffer, &parameter_count, packet);


    ESP_LOGI(TAG, "%s", temp_command_header);

    if (strcmp(temp_command_header, UWB_OK) == 0)
    {

    	 // Execute primitive and update module state if successful
    	if(false != primitive_execute(&premetive_buffer))
    	{
    		hRyuw122.busy = false;
    	}

    }
    else if (strcmp(temp_command_header, UWB_ERROR) == 0)
    {
    	 // Execute primitive and update module state if successful
    	if(false != primitive_execute(&premetive_buffer))
    	{
    		hRyuw122.busy = false;
    	}
    }

    else if (strcmp(temp_command_header, ANCHOR_RCV) == 0)
    {
    	// Update module state, extract distance, and invoke receiveCallback
    	hRyuw122.busy = false;

    	detected_distance = ryuw122_get_distance_from_string(parameter_buffer[parameter_count - 1]);

    	hRyuw122.receiveCallback (&detected_distance, RYUW122_ANCHOR_DISTANCE);


    }
    else if (strcmp(temp_command_header, TAG_RCV) == 0)
    {
    	// Perform actions related to TAG_RCV (commented code)

    }
    else
    {

    }
    return validPacket;
}

void ryuw122_return_idle(void)
{
	hRyuw122.busy = false;
}

uint16_t  ryuw122_get_distance_from_string(char* distance_string)
{
	return strtol(distance_string, NULL, 10);

}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
