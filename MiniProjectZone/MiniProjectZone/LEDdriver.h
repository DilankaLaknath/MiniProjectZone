#ifndef __LEDdriver_H
#define __LEDdriver_H

#include <avr/io.h>
#include <util/delay.h>
#include "GPIO_HAL.h"


/*Functions*/

/**
 * @brief Function to initiate LED.
 *
 * Use this function to set the LED pin as output and set the initial value of the LED.
 *
 * @param led_port - Port of the LED.
 *       
 * @param led_pin - Pin of the LED.
 *
 * @return NO_ERROR   - no error in the input parameters.
 *
 * @return PORT_ERROR - port name wrong
 *
 * @return PIN_ERROR  - pin number wrong
 */
uint32_t led_init (portx led_port, uint8_t led_pin);


/**
 * @brief Toggle the value of the LED.
  *
  * @param led_port - Port of the LED.
  *       
  * @param led_pin - Pin of the LED.
  *
  * @return NO_ERROR   - no error in the input parameters.
  *
  * @return PORT_ERROR - port name wrong
  *
  * @return PIN_ERROR  - pin number wrong
  */
uint32_t led_toggle(portx led_port, uint8_t led_pin);

/**
 * @brief Switch off the LED.
 *
 * @param led_port - Port of the LED.
 *       
 * @param led_pin - Pin of the LED.
 *
 * @return NO_ERROR   - no error in the input parameters.
 *
 * @return PORT_ERROR - port name wrong
 *
 * @return PIN_ERROR  - pin number wrong
 */
uint32_t led_off(portx led_port, uint8_t led_pin);


/**
 * @brief Switch on the LED.
 *
 * @param led_port - Port of the LED.
 *       
 * @param led_pin - Pin of the LED.
 *
 * @return NO_ERROR   - no error in the input parameters.
 *
 * @return PORT_ERROR - port name wrong
 *
 * @return PIN_ERROR  - pin number wrong
 */
uint32_t led_on(portx led_port, uint8_t led_pin);

#endif