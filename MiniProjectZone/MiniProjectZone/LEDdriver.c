#include "GPIO_HAL.h"
#include "LEDdriver.h"
#include "error.h"

uint8_t g_led_port_pin;
volatile uint8_t *DDRx = NULL;
volatile uint8_t *PORTx = NULL;

/**
 * @brief Function to map the IO registers.
 *
 * Use this function to map the registers relavant to the port used.
 *
 * @param led_port - Port of the LED.
 *
 * @return NO_ERROR   - no error in the input parameters.  
 *
 * @return PORT_ERROR - port name wrong
 */
uint32_t map_registers(portx led_port);

uint32_t led_init (portx led_port, uint8_t led_pin)
{	
	uint32_t err=NO_ERROR;
	do
	{
		if (map_registers(led_port))
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (led_pin>5 && led_port == PortG)
		{
			err=PIN_ERROR;
			break;
		}
		gpio_hal_pin_init(DDRx,led_pin,output);
		gpio_hal_pin_set(PORTx, led_pin, 0);
	}while (0);
	return err;		
}

uint32_t led_toggle(portx led_port, uint8_t led_pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (map_registers(led_port))
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (led_pin>5 && led_port == PortG)
		{
			err=PIN_ERROR;
			break;
		}
		gpio_hal_pin_toggle(PORTx, led_pin);
	}while (0);
	return err;
}

uint32_t led_off(portx led_port, uint8_t led_pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (map_registers(led_port))
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (led_pin>5 && led_port == PortG)
		{
			err=PIN_ERROR;
			break;
		}
		gpio_hal_pin_set(PORTx, led_pin, 0);
	}while (0);
	return err;
}

uint32_t led_on(portx led_port, uint8_t led_pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (map_registers(led_port))
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (led_pin>5 && led_port == PortG)
		{
			err=PIN_ERROR;
			break;
		}
		gpio_hal_pin_set(PORTx, led_pin, 1);
	}while (0);
	return err;
}

uint32_t map_registers(portx led_port)
{
	uint32_t err = NO_ERROR;
	do
	{
		if (led_port!=PortA && led_port!=PortB && led_port!=PortC && led_port!=PortD && led_port!=PortE && led_port!=PortF && led_port!=PortG && led_port!=PortH && led_port!=PortJ && led_port!=PortK && led_port!=PortL )
		{
			err=PORT_ERROR;
			break;
		}
		switch (led_port)
		{
			case PortA:
			{
				DDRx = &DDRA;
				PORTx = &PORTA;
				break;
			}
			case PortB:
			{
				DDRx = &DDRB;
				PORTx = &PORTB;
				break;
			}
			case PortC:
			{
				DDRx = &DDRC;
				PORTx = &PORTC;
				break;
			}
			case PortD:
			{
				DDRx = &DDRD;
				PORTx = &PORTD;
				break;
			}
			case PortE:
			{
				DDRx = &DDRE;
				PORTx = &PORTE;
				break;
			}
			case PortF:
			{
				DDRx = &DDRF;
				PORTx = &PORTF;
				break;
			}
			case PortG:
			{
				DDRx = &DDRG;
				PORTx = &PORTG;
				break;
			}
			case PortH:
			{
				DDRx = &DDRH;
				PORTx = &PORTH;
				break;
			}
			case PortJ:
			{
				DDRx = &DDRJ;
				PORTx = &PORTJ;
				break;
			}
			case PortK:
			{
				DDRx = &DDRK;
				PORTx = &PORTK;
				break;
			}
			case PortL:
			{
				DDRx = &DDRL;
				PORTx = &PORTL;
				break;
			}
		}
	}while(0);
	return err;
}