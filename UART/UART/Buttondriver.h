#ifndef __Buttondriver_H
#define __Buttondriver_H

#include <stddef.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/*Enumerators*/
enum portb {AB,BB,CB,DB,EB,FB,GB,HB,JB,KB,LB};

/*Function pointers*/
typedef void (*ButtonCallback_t)(char buttonPort, uint8_t buttonPin, bool buttonState);


/*Functions*/

/**
 * @brief Function to initiate button.
 *
 * Use this function to set the enable interrupts and set the button to receive inputs.
 *
 * @param btn_port - Port of the button.
 *        btn_pin - Pin of the button.
 *
 * @return Error code.
 */
uint32_t btn_init(enum portb btn_port, uint8_t btn_pin);


/**
 * @brief Function to make the connection to the callback function of the button.
 *
 * @param callback - the function to be called back
 *
 * @return Error code.
 */
uint32_t btn_set_event_callback(ButtonCallback_t callback);


/**
 * @brief Function to execute the callback function of the button.
 *
 * @param int_num - The interrupt pin number that was triggered.
 *
 * @return Error code.
 */
void btn_interrupt(uint8_t int_num);

#endif