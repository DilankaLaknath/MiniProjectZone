#ifndef __Buttondriver_H
#define __Buttondriver_H

#include <stddef.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "GPIO_HAL.h"

/*Enumerators*/
typedef enum {pressed, released} btn_state;
	
/*Function pointers*/
typedef void (*ButtonCallback_t)(portx buttonPort, uint8_t buttonPin, btn_state buttonState);


/*Functions*/

/**
 * @brief Function to initiate button.
 *
 * Use this function to set the enable interrupts and set the button to receive inputs.
 *
 * @param btn_port - Port of the button.
 *       
 * @param btn_pin - Pin of the button.
 *
 * @return NO_ERROR        - no error in the input parameters.
 *
 * @return PORT_ERROR      - port name wrong
 *
 * @return PIN_ERROR	   - pin number wrong
 *
 * @return PIN_NOT_SUPPORT -The relavant port pin not supported for interrupt
 */
uint32_t btn_init(portx btn_port, uint8_t btn_pin);


/**
 * @brief Function to make the connection to the callback function of the button.
 *
 * @param callback - the function to be called back
 *
 * @return NO_ERROR         - no error in the input parameters.
 *
 * @return CALLBACK_MISSING - callback function is missing in the input parameters.
 */
uint32_t btn_set_event_callback(ButtonCallback_t callback);


/**
 * @brief Function to execute the callback function of the button.
 *
 * @param int_num - The interrupt pin number that was triggered.
 *
 * @return None
 */
void btn_interrupt(uint8_t int_num);

#endif