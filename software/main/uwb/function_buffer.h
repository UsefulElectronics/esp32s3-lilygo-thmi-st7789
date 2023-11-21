/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  function_buffer.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 22, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef FUNCTION_BUFFER_H_
#define FUNCTION_BUFFER_H_


/* INCLUDES ------------------------------------------------------------------*/
#define MAX_PRIMITIVE_BUFFER_SIZE 10
/* MACROS --------------------------------------------------------------------*/

/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef void (*primitive)(void);

typedef struct {
	primitive primitives[MAX_PRIMITIVE_BUFFER_SIZE];
    int head;
    int tail;
} primitive_buffer_t;
/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief Initializes the head and tail pointers of the primitive buffer structure.
 *
 * @param[out] buffer Pointer to the primitive buffer structure to be initialized.
 */
void primitive_buffer_init(primitive_buffer_t* buffer);
/**
 * @brief Executes a primitive function stored in the primitive buffer.
 *
 * @param[in,out] buffer Pointer to the primitive buffer structure.
 */
void primitive_execute(primitive_buffer_t* buffer);
/**
 * @brief Pushes a primitive function pointer into the primitive buffer.
 *
 * @param[in] function Pointer to the primitive function to be pushed into the buffer.
 *
 * @return Boolean value indicating the success of the push operation.
 */
bool primitive_push(primitive_buffer_t* buffer, primitive function);



#endif /* FUNCTION_BUFFER_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
