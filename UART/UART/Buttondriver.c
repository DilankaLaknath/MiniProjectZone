#include "GPIO_HAL.h"
#include "Buttondriver.h"
#include "error.h"

ButtonCallback_t g_button_callback;

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
uint32_t btn_init (enum portb btn_port, uint8_t btn_pin)
{	
	uint32_t err=NO_ERROR;
	g_button_callback = NULL;
	do 
	{
		if (btn_port!=DB && btn_port!=EB)
		{
			if (btn_port==AB || btn_port==BB || btn_port==CB || btn_port==FB || btn_port==GB || btn_port==HB || btn_port==JB || btn_port==KB || btn_port==LB )
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
		if (btn_pin>7)
		{
			err=PIN_ERROR;
			break;
		}
		if (btn_port==DB)
		{	
			if (btn_pin>3)
			{
				err=PIN_NOT_SUPPORT;
				break;
			}
			gpio_hal_pin_config_interrupt(&PORTD, btn_pin, change, btn_interrupt);
			gpio_hal_pin_init(&DDRD, btn_pin, input);
  		}
		else if (btn_port==EB)
		{	
			if (btn_pin<4)
			{
				err=PIN_NOT_SUPPORT;
				break;
			}
			gpio_hal_pin_config_interrupt(&PORTE, btn_pin, change, btn_interrupt);
			gpio_hal_pin_init(&DDRE, btn_pin, input);
		}
	} while (0);
	return err;
}

/**
 * @brief Function to make the connection to the callback function of the button.
 *
 * @param callback - the function to be called back
 *
 * @return Error code.
 */
uint32_t btn_set_event_callback(ButtonCallback_t callback)
{
	g_button_callback = callback;
}


/**
 * @brief Function to execute the callback function of the button.
 *
 * @param int_num - The interrupt pin number that was triggered.
 *
 * @return Error code.
 */
void btn_interrupt(uint8_t int_num)
{	
	bool state;
	if (int_num<4)
	{
		state=(gpio_hal_pin_read(&PIND,int_num));
		if (g_button_callback != NULL)
		{
			g_button_callback(DB,int_num, state);
		}	
	}
	else
	{
		state=(gpio_hal_pin_read(&PINE,int_num));
		if (g_button_callback != NULL)
		{
			g_button_callback(EB,int_num, state);
		}
	}
		
}


