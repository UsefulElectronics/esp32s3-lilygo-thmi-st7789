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

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
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

    rylr993_buffer module_data = {0};

    uint8_t packetCounter = 0;

    uint8_t charCounter = 0;

    const uint8_t dontWait = 0;

    char *pToken = strtok((char *)packet, "\r\n");

    while(pToken != NULL)
    {
        strcpy(packetHolder[packetCounter], pToken);

        ++packetCounter;

        rylr993_packet_parser((uint8_t*)pToken);

        pToken = strtok(NULL, "\r\n");

        // ESP_LOGI(TAG, "%s", packetHolder[packetCounter]);
    }

    if(RYLR993_JOINED == rylr993_joined_check())
    {
        if(xQueueReceive(rlyr993_packet_queue, (void * )&module_data, dontWait))
        {
            hRlyr993.commandSend(&module_data);

            hRlyr993.networkStatus = RYLR993_BUSY;
        }
    }


    return validPacket;
}

static bool ryuw122_packet_parser(uint8_t* packet)
{
    bool validPacket = false;

    int unused = 0;

    const uint8_t packetBase = 0;

    char rxPacket[MAX_PACKET_SIZE] = {0};

    int rxPacketSize = 0;

    //Offset buffer if it starts with space character
    if(packet[packetBase] == SPACE)
    {
        ++packet;
    }
    //Module report packet check
    if(!strncmp((char*)packet ,MSG_REPORT ,strlen(MSG_REPORT)))
    {

        //report header ignore
        packet = packet + strlen(MSG_REPORT) ;

        // ESP_LOGI(TAG, "%s", packet);

        if(isdigit(packet[packetBase]))
        {
            sscanf((char*)packet, "%d:%d:%s", &unused, &rxPacketSize, rxPacket);


            memcpy(rxPacket, rylr993_hex2raw(rxPacket, (uint8_t) rxPacketSize), rxPacketSize);
            //First byte is the pin number and the next one is the pin state
            hRlyr993.receiveCallback((uint8_t*) rxPacket, RYLR993_PIN_CONTROL);
        }
        if(!strncmp((char*)packet ,PARAM_REPORT ,strlen(PARAM_REPORT)))
        {
            sscanf((char*)packet, "RX_1, DR %d, RSSI %d, SNR %d", &hRlyr993.dr, &hRlyr993.rssi, &hRlyr993.snr);

            validPacket = true;

            hRlyr993.networkStatus = RYLR993_JOINED;

        }
        else if(!strncmp((char*)packet ,JOIN_REPORT ,strlen(JOIN_REPORT)))
        {
            validPacket = true;

            hRlyr993.networkStatus = RYLR993_JOINED;

        }
    }
    else if(isdigit(packet[packetBase]))
    {
        validPacket = true;

        hRlyr993.temperature = atoi((char*)packet);

        hRlyr993.receiveCallback(NULL, RYLR993_TEMPERATURE);

        ESP_LOGI(TAG, "temperature is %d", (int)hRlyr993.temperature);
    }
    return validPacket;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
