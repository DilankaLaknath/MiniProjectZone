#include "GPIO_HAL.h"
#include "LEDdriver.h"
#include "error.h"
#include "timer_interrupts_hal.h"

uint8_t g_led_port_pin;
volatile uint8_t *DDRx = NULL;
volatile uint8_t *PORTx = NULL;

struct led_states {
	volatile uint8_t *led_port;
	uint8_t led_pin;
	bool blink;
	uint8_t toggle_count;
	};

#define g_max_number_of_leds  8  
struct led_states g_led_array[g_max_number_of_leds]; // used to keep current state of each LED (blinking or not)
uint8_t g_current_index = 0;


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
void led_blink_state_change(timer_number timer);

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
		if (g_current_index >= g_max_number_of_leds)
		{
			err = MAX_NUMBER_OF_LED_EXCEEDED;
			break;
		}
		//to check whether given LED is already initiated
		for (int i = 0; i< g_current_index; i++)
		{
			if ((g_led_array[i].led_port == PORTx) && (g_led_array[i].led_pin == led_pin))
			{
				err = DUPLICATE_LED_INITIATION;
			}
		}
		
		gpio_hal_pin_init(DDRx,led_pin,output);
		gpio_hal_pin_set(PORTx, led_pin, 0);
		
		init_timer_interrupts(TIMER_1, led_blink_state_change );
		
		g_led_array[g_current_index].led_port = PORTx;
		g_led_array[g_current_index].led_pin = led_pin;
		g_led_array[g_current_index].blink = false;
		g_led_array[g_current_index].toggle_count = 0;
		g_current_index ++;
				
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
		
		for (int i = 0; i< g_max_number_of_leds; i++)   // to stop blinking if already blinking happens
		{
			if ((g_led_array[i].led_port == PORTx) && (g_led_array[i].led_pin == led_pin))
			{
				g_led_array[i].blink = false;
				break;
			}
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
		
		for (int i = 0; i< g_max_number_of_leds; i++)   // to stop blinking if already blinking happens
		{
			if ((g_led_array[i].led_port == PORTx) && (g_led_array[i].led_pin == led_pin))
			{
				g_led_array[i].blink = false;
				break;
			}
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
		
		for (int i = 0; i< g_max_number_of_leds; i++)   // to stop blinking if already blinking happens
		{
			if ((g_led_array[i].led_port == PORTx) && (g_led_array[i].led_pin == led_pin))
			{
				g_led_array[i].blink = false;
				break;
			}
		}
				
		gpio_hal_pin_set(PORTx, led_pin, 1);
	}while (0);
	return err;
}

uint32_t led_blink(portx led_port, uint8_t led_pin, uint8_t count)
{
	map_registers(led_port);
	for (int i = 0; i< g_current_index; i++)
	{
		if ((g_led_array[i].led_port == PORTx) && (g_led_array[i].led_pin == led_pin))
		{
			g_led_array[i].blink = true;
			g_led_array[i].toggle_count = count*2;
			break;
		}
	}
	
	return 0;
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

void led_blink_state_change(timer_number timer)
{
	
	//gpio_hal_pin_toggle(&PORTB, 7);
	for (int i = 0; i< g_max_number_of_leds; i++)
	{
		if (g_led_array[i].blink == true)
		{
			g_led_array[i].toggle_count--;
			gpio_hal_pin_toggle(g_led_array[i].led_port, g_led_array[i].led_pin);
		}
		if (g_led_array[i].toggle_count == 0)
		{
			g_led_array[i].blink = false;
			//gpio_hal_pin_set(g_led_array[i].led_port, g_led_array[i].led_pin, 0); //uncomment if needed to off the led allways after blink
		}
	}
}

