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
static const char *TAG = "uart";
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
uint8_t at_command_form(char* command, char parameter_pointer[][MAX_PARAM_LENGTH], uint8_t parameter_count, char* command_string)
{
	if(0 == parameter_count && MAX_PARAM_COUNT < parameter_count)
	{
		return false;
	}

	char temp_command_string[MAX_COMMAND_LENGTH] = {0};

	sprintf(temp_command_string, "%s%s=%s"
			,AT
			,command
			,parameter_pointer[0]);

	for(uint8_t i = 1; i < parameter_count; ++i)
	{
		strcat(temp_command_string, ",");
		strcat(temp_command_string, parameter_pointer[i]);
	}

	strcat(temp_command_string, COMMAND_TERMINATOR);

	memcpy(command_string, temp_command_string, strlen(temp_command_string));

	return  strlen(temp_command_string);
}


uint8_t at_command_parser(char* command, char parameter_pointer[][MAX_PARAM_LENGTH], uint8_t* parameter_count, char* command_string)
{
    bool validPacket = false;

    uint8_t command_parameter_counter = 0;

    if(command_string[0] == COMMAND_HEADER)
    {
    	return validPacket;
    }

    validPacket = true;


    uint8_t command_counter = 0;

	char *pToken = strtok((char *)command_string, COMMAND_PARAMETERS_BEGINNING);

	strcpy(command, pToken);

    pToken = strtok((char *)NULL, COMMAND_SEPARATOR);

    strcpy(parameter_pointer[command_parameter_counter], pToken);

    ++command_parameter_counter;

    while(pToken != NULL)
    {

        pToken = strtok(NULL, COMMAND_SEPARATOR);

        strcpy(parameter_pointer[command_parameter_counter], pToken);

        ESP_LOGI(TAG, "%s", parameter_pointer[command_parameter_counter]);

        ++command_parameter_counter;
    }

    *parameter_count = command_counter;

    return validPacket;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
