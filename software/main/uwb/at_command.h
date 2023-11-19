/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  at_command.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 16, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef AT_COMMAND_H_
#define AT_COMMAND_H_


/* INCLUDES ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"
/* MACROS --------------------------------------------------------------------*/
#define MAX_PARAM_COUNT 		5


#define AT								"AT+"
#define COMMAND_PARAMETERS_BEGINNING	"="
#define COMMAND_SEPARATOR				","
#define COMMAND_HEADER					'+'
#define COMMAND_TERMINATOR				"\r\n"


#define NUM_COMMAND_PARAM 		5
#define MAX_PARAM_LENGTH 		50
#define MAX_COMMAND_LENGTH 		100
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
uint8_t at_command_form(char* command, char parameter_pointer[][MAX_PARAM_LENGTH], uint8_t parameter_count, char* command_string);

uint8_t at_command_parser(char* command, char parameter_pointer[][MAX_PARAM_LENGTH], uint8_t* parameter_count, char* command_string);


#endif /* MAIN_UWB_AT_COMMAND_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
