/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  at_command.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 16, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "at_command.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
uint8_t at_command_form(char* command, char* parameter_pointer, uint8_t parameter_count, char* command_string)
{
	if(0 == parameter_count && MAX_PARAM_COUNT < parameter_count)
	{
		return false;
	}

	char temp_command_string = {0};

	sprintf(temp_command_string, "%s%s="
			,AT
			,command);

	for(uint8_t i = 0; i < parameter_count; ++i)
	{
		strcat(temp_command_string, parameter_pointer[i]);
	}

	strcat(temp_command_string, TERMINATOR);


}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
