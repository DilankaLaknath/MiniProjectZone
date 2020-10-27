#include "GPIO_HAL.h"
#include "error.h"
	
interrupt_callback_t g_interrupt_callback = NULL;

/**
 * @brief Configure a pin to receive an input or an output.
 *
 * @param *port - Register relavant to the port of the pin to be activated (DDRx)
 *        pin - Pin number to be activated in the register
 *        iotype - Whether it is an input or an output pin. (input/output)
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_init(volatile uint8_t *port, uint8_t pin, enum io iotype )
{	
	uint32_t err=NO_ERROR;
	do 
	{
		if (port==NULL)
		{
			err=NULL_ERROR;
			break;
		}
		if (*port!=DDRA && *port!=DDRB && *port!=DDRC && *port!=DDRD && *port!=DDRE && *port!=DDRF && *port!=DDRG && *port!=DDRH && *port!=DDRJ && *port!=DDRK && *port!=DDRL)
				{
					err=PORT_ERROR;
					break;
				}
		if (pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (iotype)
		{
			*port |= (1 << pin);
		}
		else
		{
			*port &= ~(1 << pin);
		}
	} while (0);
	return err;
}

/**
 * @brief Set the value of the output pin.
 *
 * @param *port - Output pin port (PORTx)
 *        pin - Output pin number
 *        value - Output value (1 or 0)
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_set(volatile uint8_t *port, uint8_t pin, uint8_t value)
{
	uint32_t err=NO_ERROR;
	do 
	{	
		if (port==NULL)
		{
			err=NULL_ERROR;
			break;
		}
		if (*port!=PORTA && *port!=PORTB && *port!=PORTC && *port!=PORTD && *port!=PORTE && *port!=PORTF && *port!=PORTG && *port!=PORTH && *port!=PORTJ && *port!=PORTK && *port!=PORTL)
		{
			err=PORT_ERROR;
			break;
		}
		
		if (pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (value)
		{
			*port |= (1 << pin);
		}
		else
		{
			*port &= ~(1 << pin);
		}
	} while (0);
	return err;
}

/**
 * @brief Toggle the value of the output pin.
 *
 * @param *port - Output pin port (PORTx)
 *        pin - Output pin number
 *
 * @return Error code.
 */
uint32_t gpio_hal_pin_toggle(volatile uint8_t *port, uint8_t pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (port==NULL)
		{
			err=NULL_ERROR;
			break;
		}
		if (*port!=PORTA && *port!=PORTB && *port!=PORTC && *port!=PORTD && *port!=PORTE && *port!=PORTF && *port!=PORTG && *port!=PORTH && *port!=PORTJ && *port!=PORTK && *port!=PORTL)
		{
			err=PORT_ERROR;
			break;
		}
		
		if (pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		*port ^= (1 << pin);
	} while (0);
	return err;
}

/**
 * @brief Read the value of the input pin.
 *
 * @param *port - Input pin port (PINx)
 *        pin - Input pin number
 *
 * @return Value read from the pin.
 */
uint8_t gpio_hal_pin_read(volatile uint8_t *port, uint8_t pin)
{
	return *port & (1 << pin);
}

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
uint32_t gpio_hal_pin_config_interrupt(volatile uint8_t *port, uint8_t pin, enum sense sensetype, interrupt_callback_t callback)
{	
	uint32_t err=NO_ERROR;
	uint8_t bit;
	volatile uint8_t *reg;
	do
	{
		if (port==NULL)
		{
			err=NULL_ERROR;
			break;
		}
		if (*port!=PORTD && *port!=PORTE)
		{
			if (*port==PORTA || *port==PORTB || *port==PORTC || *port==PORTF || *port==PORTG || *port==PORTH || *port==PORTJ || *port==PORTK || *port==PORTL)
			{
				err=PIN_NOT_SUPPORT;
				break;
			}
			else
			{
				err=PORT_ERROR;
				break;
			}
		}
		
		if (pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (*port == PORTD && pin<4)
		{
			reg = &EICRA;
			bit = pin;
		}
		else if (*port == PORTE && pin>3)
		{
			reg = &EICRB;
			bit = pin-4;
		}
		else
		{
			err=PIN_NOT_SUPPORT;
			break;
		}
		switch (sensetype)
		{
			case low:
			{
				(*reg) &= ~(1 << (2 * bit + 1)) & ~(1 << (2 * bit));
				break;
			}
			case change:
			{
				(*reg) |= (1 << (2 * bit));
				(*reg) &= ~(1 << (2 * bit + 1));
				break;
			}
			case falling:
			{
				(*reg) |= (1 << (2 * bit + 1));
				(*reg) &= ~(1 << (2 * bit));
				break;
			}
			case rising:
			{
				(*reg) |= (1 << (2 * bit + 1)) | (1 << (2 * bit));
				break;
			}
		}
		EIMSK |= (1 << pin);
		sei();
		g_interrupt_callback=callback;
	} while (0);
	return err;
}

ISR(INT0_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(0);
	}
}

ISR(INT1_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(1);
	}
}

ISR(INT2_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(2);
	}
}

ISR(INT3_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(3);
	}
}

ISR(INT4_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(4);
	}
}

ISR(INT5_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(5);
	}
}

ISR(INT6_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(6);
	}
}

ISR(INT7_vect)
{
	if (g_interrupt_callback != NULL)
	{
		g_interrupt_callback(7);
	}
}