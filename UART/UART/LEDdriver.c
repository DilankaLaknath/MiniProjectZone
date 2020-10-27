#include "GPIO_HAL.h"
#include "LEDdriver.h"
#include "error.h"

uint8_t g_led_port_pin;

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
uint32_t led_init (enum portl led_port, uint8_t led_pin)
{	
	uint32_t err=NO_ERROR;
	do
	{
		if (led_port!=AL && led_port!=BL && led_port!=CL && led_port!=DL && led_port!=EL && led_port!=FL && led_port!=GL && led_port!=HL && led_port!=JL && led_port!=KL && led_port!=LL )
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		switch (led_port)
		{
			case AL:
			{
				gpio_hal_pin_init(&DDRA,led_pin,output);
				gpio_hal_pin_set(&PORTA, led_pin, 0);
				break;
			}
			case BL:
			{
				gpio_hal_pin_init(&DDRB,led_pin,output);
				gpio_hal_pin_set(&PORTB, led_pin, 0);
				break;
			}
			case CL:
			{
				gpio_hal_pin_init(&DDRC,led_pin,output);
				gpio_hal_pin_set(&PORTC, led_pin, 0);
				break;
			}
			case DL:
			{
				gpio_hal_pin_init(&DDRD,led_pin,output);
				gpio_hal_pin_set(&PORTD, led_pin, 0);
				break;
			}
			case EL:
			{
				gpio_hal_pin_init(&DDRE,led_pin,output);
				gpio_hal_pin_set(&PORTE, led_pin, 0);
				break;
			}
			case FL:
			{
				gpio_hal_pin_init(&DDRF,led_pin,output);
				gpio_hal_pin_set(&PORTF, led_pin, 0);
				break;
			}
			case GL:
			{	
				if (led_pin>5)
				{
					err=PIN_ERROR;
					break;
				}
				gpio_hal_pin_init(&DDRG,led_pin,output);
				gpio_hal_pin_set(&PORTG, led_pin, 0);
				break;
			}
			case HL:
			{
				gpio_hal_pin_init(&DDRH,led_pin,output);
				gpio_hal_pin_set(&PORTH, led_pin, 0);
				break;
			}
			case JL:
			{
				gpio_hal_pin_init(&DDRJ,led_pin,output);
				gpio_hal_pin_set(&PORTJ, led_pin, 0);
				break;
			}
			case KL:
			{
				gpio_hal_pin_init(&DDRK,led_pin,output);
				gpio_hal_pin_set(&PORTK, led_pin, 0);
				break;
			}
			case LL:
			{
				gpio_hal_pin_init(&DDRL,led_pin,output);
				gpio_hal_pin_set(&PORTL, led_pin, 0);
				break;
			}
		}
	}while (0);
	return err;		
}

/**
 * @brief Toggle the value of the LED.
  *
  * @param led_port - Port of the LED.
  *        led_pin - Pin of the LED.
  *
  * @return Error code.
  */
uint32_t led_toggle(enum portl led_port, uint8_t led_pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (led_port!=AL && led_port!=BL && led_port!=CL && led_port!=DL && led_port!=EL && led_port!=FL && led_port!=GL && led_port!=HL && led_port!=JL && led_port!=KL && led_port!=LL )
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		switch (led_port)
		{
			case AL:
			{
				gpio_hal_pin_toggle(&PORTA, led_pin);
				break;
			}
			case BL:
			{
				gpio_hal_pin_toggle(&PORTB, led_pin);
				break;
			}
			case CL:
			{
				gpio_hal_pin_toggle(&PORTC, led_pin);
				break;
			}
			case DL:
			{
				gpio_hal_pin_toggle(&PORTD, led_pin);
				break;
			}
			case EL:
			{
				gpio_hal_pin_toggle(&PORTE, led_pin);
				break;
			}
			case FL:
			{
				gpio_hal_pin_toggle(&PORTF, led_pin);
				break;
			}
			case GL:
			{
				if (led_pin>5)
				{
					err=PIN_ERROR;
					break;
				}
				gpio_hal_pin_toggle(&PORTG, led_pin);
				break;
			}
			case HL:
			{
				gpio_hal_pin_toggle(&PORTH, led_pin);
				break;
			}
			case JL:
			{
				gpio_hal_pin_toggle(&PORTJ, led_pin);
				break;
			}
			case KL:
			{
				gpio_hal_pin_toggle(&PORTK, led_pin);
				break;
			}
			case LL:
			{
				gpio_hal_pin_toggle(&PORTL, led_pin);
				break;
			}
		}
	}while (0);
	return err;
}

/**
 * @brief Switch off the LED.
 *
 * @param led_port - Port of the LED.
 *        led_pin - Pin of the LED.
 *
 * @return Error code.
 */
uint32_t led_off(enum portl led_port, uint8_t led_pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (led_port!=AL && led_port!=BL && led_port!=CL && led_port!=DL && led_port!=EL && led_port!=FL && led_port!=GL && led_port!=HL && led_port!=JL && led_port!=KL && led_port!=LL )
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		switch (led_port)
		{
			case AL:
			{
				gpio_hal_pin_set(&PORTA, led_pin, 0);
				break;
			}
			case BL:
			{
				gpio_hal_pin_set(&PORTB, led_pin, 0);
				break;
			}
			case CL:
			{
				gpio_hal_pin_set(&PORTC, led_pin, 0);
				break;
			}
			case DL:
			{
				gpio_hal_pin_set(&PORTD, led_pin, 0);
				break;
			}
			case EL:
			{
				gpio_hal_pin_set(&PORTE, led_pin, 0);
				break;
			}
			case FL:
			{
				gpio_hal_pin_set(&PORTF, led_pin, 0);
				break;
			}
			case GL:
			{
				if (led_pin>5)
				{
					err=PIN_ERROR;
					break;
				}
				gpio_hal_pin_set(&PORTG, led_pin, 0);
				break;
			}
			case HL:
			{
				gpio_hal_pin_set(&PORTH, led_pin, 0);
				break;
			}
			case JL:
			{
				gpio_hal_pin_set(&PORTJ, led_pin, 0);
				break;
			}
			case KL:
			{
				gpio_hal_pin_set(&PORTK, led_pin, 0);
				break;
			}
			case LL:
			{
				gpio_hal_pin_set(&PORTL, led_pin, 0);
				break;
			}
		}
	}while (0);
	return err;
}

/**
 * @brief Switch on the LED.
 *
 * @param led_port - Port of the LED.
 *        led_pin - Pin of the LED.
 *
 * @return Error code.
 */
uint32_t led_on(enum portl led_port, uint8_t led_pin)
{
	uint32_t err=NO_ERROR;
	do
	{
		if (led_port!=AL && led_port!=BL && led_port!=CL && led_port!=DL && led_port!=EL && led_port!=FL && led_port!=GL && led_port!=HL && led_port!=JL && led_port!=KL && led_port!=LL )
		{
			err=PORT_ERROR;
			break;
		}
		if (led_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		switch (led_port)
		{
			case AL:
			{
				gpio_hal_pin_set(&PORTA, led_pin, 1);
				break;
			}
			case BL:
			{
				gpio_hal_pin_set(&PORTB, led_pin, 1);
				break;
			}
			case CL:
			{
				gpio_hal_pin_set(&PORTC, led_pin, 1);
				break;
			}
			case DL:
			{
				gpio_hal_pin_set(&PORTD, led_pin, 1);
				break;
			}
			case EL:
			{
				gpio_hal_pin_set(&PORTE, led_pin, 1);
				break;
			}
			case FL:
			{
				gpio_hal_pin_set(&PORTF, led_pin, 1);
				break;
			}
			case GL:
			{
				if (led_pin>5)
				{
					err=PIN_ERROR;
					break;
				}
				gpio_hal_pin_set(&PORTG, led_pin, 1);
				break;
			}
			case HL:
			{
				gpio_hal_pin_set(&PORTH, led_pin, 1);
				break;
			}
			case JL:
			{
				gpio_hal_pin_set(&PORTJ, led_pin, 1);
				break;
			}
			case KL:
			{
				gpio_hal_pin_set(&PORTK, led_pin, 1);
				break;
			}
			case LL:
			{
				gpio_hal_pin_set(&PORTL, led_pin, 1);
				break;
			}
		}
	}while (0);
	return err;
}