/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  function_buffer.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 22, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "function_buffer.h"
#include <stdbool.h>
/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief Initializes the head and tail pointers of the primitive buffer structure.
 *
 * @param[out] buffer Pointer to the primitive buffer structure to be initialized.
 */
void primitive_buffer_init(primitive_buffer_t* buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
}
/**
 * @brief Pushes a primitive function pointer into the primitive buffer.
 *
 * @param[in] function Pointer to the primitive function to be pushed into the buffer.
 *
 * @return Boolean value indicating the success of the push operation.
 */
bool primitive_push(primitive_buffer_t* buffer, primitive function)
{
    uint8_t next = (buffer->head + 1) % MAX_BUFFER_SIZE;
    if (next != buffer->tail)
    {
        buffer->primitives[buffer->head] = function;

        buffer->head = next;
        // Successfully pushed
        return true;
    }
    // Buffer full, push failed
    return false;
}
/**
 * @brief Executes a primitive function stored in the primitive buffer.
 *
 * @param[in,out] buffer Pointer to the primitive buffer structure.
 */
void primitive_execute(primitive_buffer_t* buffer)
{
	//Check if the buffer contains any function
    if (buffer->tail != buffer->head)
    {
    	primitive function = buffer->primitives[buffer->tail];
        buffer->tail = (buffer->tail + 1) % MAX_BUFFER_SIZE;
        // Execute the function
        function();
    }
}

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
