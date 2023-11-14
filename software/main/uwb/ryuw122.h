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

/* MACROS --------------------------------------------------------------------*/
#define AT						"AT+"

//Configuration commands
#define NETWORKID               "NETWORKID"
#define ADDRESS                 "ADDRESS"		//Set the ADDRESS ID of module.									AT+NETWORKID=<NETWORK ID> 8 BYTES ASCII
#define CPIN                  	"CPIN"			//Encryption password											Set the AES128 password of the network A 32 characters long AES password
#define TAGD                  	"TAGD"			//Set the parameters of TAG RF duty cycle						AT+NETWORKID=<NETWORK ID> 8 BYTES ASCII
#define CAL                  	"CAL"			//Distance Calibration											AT+CAL=<DISTANCE CALIBRATION> From -100 to +10
//communication commands
#define ANCHOR_SEND             "ANCHOR_SEND"	//Send data to the appointed address							AT+ ANCHOR_SEND =<TAG Address>,<Payload Length>,<Dat
#define TAG_SEND                "TAG_SEND"		//Send data to the module and wait for the anchor to re			AT+TAG_SEND=<Payload Length>,<Data
#define ANCHOR_RCV              "+ANCHOR_RCV"	//Show the received data of ANCHOR actively.					+ANCHOR_RCV=<TAG Address>,< PAYLOAD LENGTH>,<TAG DATA>,<DISTANCE>
#define TAG_RCV               	"+TAG_RCV"		//Show the received data of TAG actively.						+TAG_RCV=< PAYLOAD LENGTH>,<DATA>
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/



#endif /* MAIN_UWB_RYUW122_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
