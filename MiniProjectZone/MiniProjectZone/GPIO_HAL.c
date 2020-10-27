#include "GPIO_HAL.h"
#include "error.h"
	
interrupt_callback_t g_interrupt_callback = NULL;


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

uint8_t gpio_hal_pin_read(volatile uint8_t *port, uint8_t pin)
{
	return *port & (1 << pin);
}

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