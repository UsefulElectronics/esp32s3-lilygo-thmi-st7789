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

/**
 * @brief The function assigns the provided function pointers for sending commands and handling callbacks to the UWB module's internal structure.
 *          It sets the initial operation mode, initializes a buffer for primitive buffer to evoke functions later on, and configures the module's address, network ID, and password.
 *          Additionally, it initializes the module's operation using the provided mode and specific primitive operations.
 *
 * @param port_send A pointer to the function used for sending commands to the module usually UART TX.
 *
 * @param uwb_callback A pointer to the callback function for handling received data or events

 * @param mode The initial operation mode for the UWB module (RYUW122_TAG or RYUW122_ANCHOR.
 *
 */
void ryuw122_init(void* port_send, void* uwb_callback, bool mode);

/**
 * @brief read the module operation mode whether it is anchor or tag
 *
 * @param RYUW122_TAG or RYUW122_ANCHOR
 */
module_mdoe_e ryuw122_get_mode(void);
/**
 * @brief reads the busy state of the module
 *
 * @param true if the module is busy
 */
bool ryuw122_get_state(void);
/**
 * @brief Sets the operation mode of the UWB module either Anchor or TAG device.
 * 		  this function will forms AT command and sends it over UART to RYUW122 module.
 *
 * @param RYUW122_TAG or RYUW122_ANCHOR
 */
void ryuw122_set_mode(module_mdoe_e mode);
/**
 * @brief The function prepares a command string to send data from an anchor node using the UWB module.
 *          It populates parameters with the tag address, data size, and the data buffer.
 *          If the module is not busy and is initialized, it constructs and sends the command to transmit data.
 *
 *
 * @param tx_buffer A pointer to the buffer containing the data to be transmitted.
 *
 * @param data_size The size of the data to be transmitted in bytes.
 *
 * @return bool Returns true if the data transmission is initiated successfully, otherwise false.
 */
bool ryuw122_anchor_send(uint8_t* tx_buffer, uint8_t data_size);
/**
 * @brief This function sets the device address for the UWB module using a predefined value (UWB_ANCH_ADDRESS).
 * 		  This address is important when anchor device is sending data to the tag
 * 		  device. It will also forms AT command and sends it over UART to RYUW122 module.
 */
void ryuw122_set_address();
/**
 * @brief This function sets the network ID for the UWB module using a predefined value (UWB_NETWORK).
 * 		  and will forms AT command and sends it over UART to RYUW122 module.
 */
void ryuw122_set_network_id(void);
/**
 * @brief This function sets the device encryption password for the UWB module using a predefined value (UWB_PASSWORD).
 * 		  both anchor and tag must be configured with the same password.
 * 		  It will also forms AT command and sends it over UART to RYUW122 module.
 */
void ryuw122_set_password(void);
/**
 * @brief Separates and processes received packets from the UWB module.
 * 		  The function uses an AT command parser to identify and process different types of received packets.
 *        It checks the packet header and performs specific actions based on the header type.
 *        If the header matches predefined headers (e.g., UWB_OK, UWB_ERROR, ANCHOR_RCV, TAG_RCV),
 *        it executes corresponding actions or callbacks.
 *
 * @param packet A pointer to the received packet.
 *
 * @param packet_size The size of the received packet in bytes.
 *
 * @return bool Returns true if a valid packet is processed, otherwise false.
 */
bool ryuw122_packet_separator(char* packet, uint8_t packet_size);

/**
 * @brief This function converts the input distance string to a uint16_t distance value and returns it.
 *
 * @return uint16_t The distance value in uint16 form.
 */
uint16_t  ryuw122_get_distance_from_string(char* distance_string);
/**
 * @brief make device ready used as busy state timeout
 *
 */
void ryuw122_return_idle(void);

#endif /* MAIN_UWB_RYUW122_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
