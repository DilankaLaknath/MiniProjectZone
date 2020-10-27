#ifndef __GPIO_HAL_H
#define __GPIO_HAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*Enumerators*/
enum io {input=0,output=1};
enum sense {low, change, falling, rising};
enum portx {A,B,C,D,E,F,G,H,J,K,L};

/*Function pointers*/
typedef void (*interrupt_callback_t)(uint8_t int_num);


/*Functions*/

/**
 * @brief Configure a pin to receive an input or an output.
 *
 * @param *port - Register relavant to the port of the pin to be activated (DDRx)
 *        pin - Pin number to be activated in the register
 *        iotype - Whether it is an input or an output pin. (input/output)
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_init(volatile uint8_t *port, uint8_t pin, enum io iotype );


/**
 * @brief Set the value of the output pin.
 *
 * @param *port - Output pin port (PORTx)
 *        pin - Output pin number
 *        value - Output value (1 or 0)
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_set(volatile uint8_t *port, uint8_t pin, uint8_t value);


/**
 * @brief Toggle the value of the output pin.
 *
 * @param *port - Output pin port (PORTx)
 *        pin - Output pin number
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_toggle(volatile uint8_t *port, uint8_t pin);


/**
 * @brief Read the value of the input pin.
 *
 * @param *port - Input pin port (PINx)
 *        pin - Input pin number
 *
 * @return Value read from the pin.
 */
uint8_t gpio_hal_pin_read(volatile uint8_t *port, uint8_t pin);


/**
 * @brief Configure a pin as an interrupt pin.
 * @param *port -Port of the pin to configurred. (Portx)
 *        pin - Pin number to be configured.
 *        sensetype - Type of sensing to be used
 *               (low - The low level of INTn generates an interrupt request,
 *                change - Any logical change on INTn generates an interrupt request,
 *                rising - The rising edge between two samples of INTn generates an interrupt request,
 *                falling - The falling edge between two samples of INTn generates an interrupt request)
 *        callback - the function to be called back when an interrupt occurs.
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_config_interrupt(volatile uint8_t *port, uint8_t pin, enum sense sensetype, interrupt_callback_t callback);

#endif