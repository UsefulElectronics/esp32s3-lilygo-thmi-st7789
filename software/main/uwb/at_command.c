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
static const char *TAG = "at";
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief Forms an AT command string with parameters.
 *
 * This function constructs an AT command string with parameters based on the provided command and parameters.
 *
 * @param[in] 	command The base AT command.
 * @param[in] 	parameter_pointer An array of strings containing the parameters.
 * @param[in] 	parameter_count The number of parameters in the array.
 * @param[out] 	command_string The output buffer to store the constructed AT command string.
 *
 * @return 		Length of the generated AT command string (excluding null terminator), or zero if no parameters or an error occurred.
 				Or 0 if the parameter count is zero or exceeds MAX_PARAM_COUNT.

 */
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
		strcat(temp_command_string, COMMAND_SEPARATOR);

		strcat(temp_command_string, parameter_pointer[i]);
	}

	strcat(temp_command_string, COMMAND_TERMINATOR);

	memcpy(command_string, temp_command_string, strlen(temp_command_string));

	return  strlen(temp_command_string);
}

/**
 * @brief This function parses the provided AT command string to extract the command and its associated parameters.
 *
 *		  This function validates AT command response from the '+' header character.
 *
 * @param[out] command The extracted AT command.
 * @param[out] parameter_pointer An array to store the extracted parameters.
 * @param[out] parameter_count Pointer to the count of extracted parameters. This can ex: +OK
 * @param[in]  command_string The input AT command string to be parsed.
 *
 * @return Boolean value indicating the validity of the parsed AT command and parameters.
 * @retval true if the parsing process is successful and the AT command is valid.
 * @retval false if the AT command string format is invalid or parsing fails.
 */
uint8_t at_command_parser(char* command, char parameter_pointer[][MAX_PARAM_LENGTH], uint8_t* parameter_count, char* command_string)
{
    bool validPacket = false;


    uint8_t command_parameter_counter = 0;

    char *pToken;

    if(strchr(command_string, COMMAND_HEADER) == NULL)
    {
    	return validPacket;
    }

    validPacket = true;


    char* parameter_check = strchr(command_string, '=');

    //The received packet has at least one parameter
	if(parameter_check != NULL)
	{

		pToken = strtok((char *)command_string, COMMAND_PARAMETERS_BEGINNING);

		strcpy(command, pToken);

	    pToken = strtok((char *)NULL, COMMAND_SEPARATOR);

	    strcpy(parameter_pointer[command_parameter_counter], pToken);

	    ++command_parameter_counter;

	    pToken = strtok((char *)NULL, COMMAND_SEPARATOR);
	    //there are more parameters to extract
	    if(NULL == pToken)
	    {

	    	pToken = strtok((char *)parameter_pointer[command_parameter_counter], COMMAND_TERMINATOR);

		    strcpy(parameter_pointer[command_parameter_counter], pToken);

		    ++command_parameter_counter;

		    pToken = NULL;
	    }

	    while(pToken != NULL)
	    {
	        strcpy(parameter_pointer[command_parameter_counter], pToken);

	        ++command_parameter_counter;

	        pToken = strtok(NULL, COMMAND_SEPARATOR);
	    }

	}
	//Handle the packets that have no parameter
	else
	{
	    pToken = strtok((char *)command_string, COMMAND_TERMINATOR);

	    strcpy(command, pToken);
	}


    *parameter_count = command_parameter_counter;

    return validPacket;
}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
