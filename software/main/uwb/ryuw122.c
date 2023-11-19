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
/* PRIVATE STRUCTRES ---------------------------------------------------------*/
typedef struct
{
    void                	(*commandSend)     (void *tx_buffer, uint8_t command_length);
    void               	 	(*receiveCallback) (void* rx_packet, packet_id_e packet_id);
    communication_mode_e    networkStatus;
    module_mdoe_e			operation_mode;

}ryuw122_handler;
/* VARIABLES -----------------------------------------------------------------*/
ryuw122_handler hRyuw122 = {0};

char parameter_buffer[NUM_COMMAND_PARAM][MAX_PARAM_LENGTH] = {0};
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void ryuw122_init(void* port_send, void* uwb_callback, bool mode)
{
	hRyuw122.commandSend 		= port_send;

	hRyuw122.receiveCallback 	= uwb_callback;

	hRyuw122.operation_mode		= mode;
}

module_mdoe_e ryuw122_get_mode(void)
{
	return hRyuw122.operation_mode;
}

void ryuw122_set_mode(module_mdoe_e mode)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 hRyuw122.operation_mode = mode;

	 sprintf(parameter_buffer[0],"%d", hRyuw122.operation_mode);

	 command_length = at_command_form(MODE, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);
}

void ryuw122_set_network_id(void)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;


	 sprintf(parameter_buffer[0],"%s", UWB_NETWORK);

	 command_length = at_command_form(NETWORKID, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);
}

void ryuw122_set_address(void)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 sprintf(parameter_buffer[0],"%s", UWB_ANCH_ADDRESS);

	 command_length = at_command_form(ADDRESS, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);
}

void ryuw122_set_password(void)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 sprintf(parameter_buffer[0],"%s", UWB_PASSWORD);

	 command_length = at_command_form(CPIN, parameter_buffer, 1, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);
}

void ryuw122_anchor_send(uint8_t* tx_buffer, uint8_t data_size)
{
	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	uint8_t command_length = 0;

	 sprintf(parameter_buffer[0],"%s", UWB_TAG_ADDRESS);

	 memcpy(parameter_buffer[2], tx_buffer, data_size);

	 sprintf(parameter_buffer[1],"%d", data_size);

	 command_length = at_command_form(ANCHOR_SEND, parameter_buffer, 3, temp_command_string);

	 hRyuw122.commandSend(temp_command_string, command_length);
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



/**
 * @brief   Separator the packet received from the LoRaWAN module
 *
 * @param   packet      :   received packet data content
 *
 * @param   packet_size :   received packet size in bytes
 *
 * @return  bool        :   true if packet is valid
 */
bool ryuw122_packet_separator(uint8_t* packet, uint8_t packet_size)
{
    bool validPacket = false;
//
//
//
//    uint8_t packetCounter = 0;
//
//    uint8_t charCounter = 0;
//
//    const uint8_t dontWait = 0;
//
//    char *pToken = strtok((char *)packet, "\r\n");
//
//    while(pToken != NULL)
//    {
//        strcpy(packetHolder[packetCounter], pToken);
//
//        ++packetCounter;
//
//        rylr993_packet_parser((uint8_t*)pToken);
//
//        pToken = strtok(NULL, "\r\n");
//
//        // ESP_LOGI(TAG, "%s", packetHolder[packetCounter]);
//    }
//
//    if(RYLR993_JOINED == rylr993_joined_check())
//    {
//        if(xQueueReceive(rlyr993_packet_queue, (void * )&module_data, dontWait))
//        {
//            hRlyr993.commandSend(&module_data);
//
//            hRlyr993.networkStatus = RYLR993_BUSY;
//        }
//    }
//
//
    return validPacket;
}

//static bool ryuw122_packet_parser(uint8_t* packet)
//{
//    bool validPacket = false;
//
//    int unused = 0;
//
//    const uint8_t packetBase = 0;
//
//    char rxPacket[MAX_PACKET_SIZE] = {0};
//
//    int rxPacketSize = 0;
//
//    //Offset buffer if it starts with space character
//    if(packet[packetBase] == SPACE)
//    {
//        ++packet;
//    }
//    //Module report packet check
//    if(!strncmp((char*)packet ,MSG_REPORT ,strlen(MSG_REPORT)))
//    {
//
//        //report header ignore
//        packet = packet + strlen(MSG_REPORT) ;
//
//        // ESP_LOGI(TAG, "%s", packet);
//
//        if(isdigit(packet[packetBase]))
//        {
//            sscanf((char*)packet, "%d:%d:%s", &unused, &rxPacketSize, rxPacket);
//
//
//            memcpy(rxPacket, rylr993_hex2raw(rxPacket, (uint8_t) rxPacketSize), rxPacketSize);
//            //First byte is the pin number and the next one is the pin state
//            hRlyr993.receiveCallback((uint8_t*) rxPacket, RYLR993_PIN_CONTROL);
//        }
//        if(!strncmp((char*)packet ,PARAM_REPORT ,strlen(PARAM_REPORT)))
//        {
//            sscanf((char*)packet, "RX_1, DR %d, RSSI %d, SNR %d", &hRlyr993.dr, &hRlyr993.rssi, &hRlyr993.snr);
//
//            validPacket = true;
//
//            hRlyr993.networkStatus = RYLR993_JOINED;
//
//        }
//        else if(!strncmp((char*)packet ,JOIN_REPORT ,strlen(JOIN_REPORT)))
//        {
//            validPacket = true;
//
//            hRlyr993.networkStatus = RYLR993_JOINED;
//
//        }
//    }
//    else if(isdigit(packet[packetBase]))
//    {
//        validPacket = true;
//
//        hRlyr993.temperature = atoi((char*)packet);
//
//        hRlyr993.receiveCallback(NULL, RYLR993_TEMPERATURE);
//
//        ESP_LOGI(TAG, "temperature is %d", (int)hRlyr993.temperature);
//    }
//    return validPacket;
//}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
