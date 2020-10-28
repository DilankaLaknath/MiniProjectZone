#include "GPIO_HAL.h"
#include "Buttondriver.h"
#include "error.h"

ButtonCallback_t g_button_callback;

uint32_t btn_init (portx btn_port, uint8_t btn_pin)
{	
	uint32_t err=NO_ERROR;
	g_button_callback = NULL;
	do 
	{
		if (btn_port!=PortD && btn_port!=PortE)
		{
			if (btn_port==PortA || btn_port==PortB || btn_port==PortC || btn_port==PortF || btn_port==PortG || btn_port==PortH || btn_port==PortJ || btn_port==PortK || btn_port==PortL )
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
		if (btn_port==PortD)
		{	
			if (btn_pin>3)
			{
				err=PIN_NOT_SUPPORT;
				break;
			}
			gpio_hal_pin_config_interrupt(&PORTD, btn_pin, change, btn_interrupt);
			gpio_hal_pin_init(&DDRD, btn_pin, input);
  		}
		else if (btn_port==PortE)
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

uint32_t btn_set_event_callback(ButtonCallback_t callback)
{
	uint32_t err=NO_ERROR;
	do 
	{
		if (callback == NULL)
		{
			err = CALLBACK_MISSING;
			break;
		}
		g_button_callback = callback;
	}while(0);
	return err;
}

void btn_interrupt(uint8_t int_num)
{	
	bool state;
	if (int_num<4)
	{
		state=(gpio_hal_pin_read(&PIND,int_num));
		if (g_button_callback != NULL)
		{	
			if (state)
			{
				g_button_callback(PortD,int_num, released);
			}
			else
			{
				g_button_callback(PortD,int_num, pressed);
			}
		}	
	}
	else
	{
		state=(gpio_hal_pin_read(&PINE,int_num));
		if (g_button_callback != NULL)
		{
			if (state)
			{
				g_button_callback(PortE,int_num, released);
			}
			else
			{
				g_button_callback(PortE,int_num, pressed);
			}
		}
	}
}


