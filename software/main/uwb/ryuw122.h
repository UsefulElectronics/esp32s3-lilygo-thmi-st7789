/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  ryuw122.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 14, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_UWB_RYUW122_H_
#define MAIN_UWB_RYUW122_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>

#include "at_command.h"
/* MACROS --------------------------------------------------------------------*/
//Configuration commands
#define NETWORKID               "NETWORKID"		//Set the network ID											<NETWORK ID>= 8 BYTES ASCII (default 00000000)
#define MODE					"MODE"			//Set the wireless work mode									0 : TAG mode (Default). 1 : ANCHOR mode 2 : Sleep mode
#define ADDRESS                 "ADDRESS"		//Set the ADDRESS ID of module.									AT+NETWORKID=<NETWORK ID> 8 BYTES ASCII
#define CPIN                  	"CPIN"			//Encryption password											Set the AES128 password of the network A 32 characters long AES password
#define TAGD                  	"TAGD"			//Set the parameters of TAG RF duty cycle						AT+NETWORKID=<NETWORK ID> 8 BYTES ASCII
#define CAL                  	"CAL"			//Distance Calibration											AT+CAL=<DISTANCE CALIBRATION> From -100 to +10
//communication commands
#define ANCHOR_SEND             "ANCHOR_SEND"	//Send data to the appointed address							AT+ ANCHOR_SEND =<TAG Address>,<Payload Length>,<Dat
#define TAG_SEND                "TAG_SEND"		//Send data to the module and wait for the anchor to re			AT+TAG_SEND=<Payload Length>,<Data
#define ANCHOR_RCV              "+ANCHOR_RCV"	//Show the received data of ANCHOR actively.					+ANCHOR_RCV=<TAG Address>,< PAYLOAD LENGTH>,<TAG DATA>,<DISTANCE>
#define TAG_RCV               	"+TAG_RCV"		//Show the received data of TAG actively.						+TAG_RCV=< PAYLOAD LENGTH>,<DATA>
#define UWB_OK               	"+OK"
#define UWB_ERROR               "+ERR"

#define UWB_NETWORK            	"REYAX123"
#define UWB_ANCH_ADDRESS       	"ANC00001"
#define UWB_TAG_ADDRESS       	"TAG00001"
#define UWB_PASSWORD       		"FABC0002EEDCAA90FABC0002EEDCAA90"

#define UWB_DISTANCE_POSITION	2

/* ENUMORATIONS --------------------------------------------------------------*/
typedef enum
{
	RYUW122_TAG 		= 0,
	RYUW122_ANCHOR ,

}module_mdoe_e;

typedef enum
{
	RYUW122_OK			= 0,
	RYUW122_ANCHOR_RX,
	RYUW122_ANCHOR_DISTANCE,
	RYUW122_TAG_RX,


}packet_id_e;

typedef enum
{
	RYUW122_NO_DEVICE = 0,
	RYUW122_COMMUNICATION_ONGOING ,

}communication_mode_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void ryuw122_init(void* port_send, void* uwb_callback, bool mode);

module_mdoe_e ryuw122_get_mode(void);

bool ryuw122_get_state(void);

void ryuw122_set_mode(module_mdoe_e mode);

bool ryuw122_anchor_send(uint8_t* tx_buffer, uint8_t data_size);

void ryuw122_set_address();

void ryuw122_set_network_id(void);

void ryuw122_set_password(void);

bool ryuw122_packet_separator(char* packet, uint8_t packet_size);

uint16_t  ryuw122_get_distance_from_string(char* distance_string);

void ryuw122_return_idle(void);

#endif /* MAIN_UWB_RYUW122_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
