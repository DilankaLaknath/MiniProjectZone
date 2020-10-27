#ifndef __LEDdriver_H
#define __LEDdriver_H

#include <avr/io.h>
#include <util/delay.h>

/*Enumerators*/
enum portl {AL,BL,CL,DL,EL,FL,GL,HL,JL,KL,LL};


/*Functions*/

/**
 * @brief Function to initiate LED.
 *
 * Use this function to set the LED pin as output and set the initial value of the LED.
 *
 * @param led_port - Port of the LED.
 *        led_pin - Pin of the LED.
 *
 * @return Error code.
 */
uint32_t led_init (enum portl led_port, uint8_t led_pin);


/**
 * @brief Toggle the value of the LED.
  *
  * @param led_port - Port of the LED.
  *        led_pin - Pin of the LED.
  *
  * @return Error code.
  */
uint32_t led_toggle(enum portl led_port, uint8_t led_pin);

/**
 * @brief Switch off the LED.
 *
 * @param led_port - Port of the LED.
 *        led_pin - Pin of the LED.
 *
 * @return Error code.
 */
uint32_t led_off(enum portl led_port, uint8_t led_pin);


/**
 * @brief Switch on the LED.
 *
 * @param led_port - Port of the LED.
 *        led_pin - Pin of the LED.
 *
 * @return Error code.
 */
uint32_t led_on(enum portl led_port, uint8_t led_pin);

#endif