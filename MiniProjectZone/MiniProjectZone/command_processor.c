/*
 * MiniProjectZone
 *
 * Created: 10/27/2020 1:32:33 PM
 * Author : Dilanka Wickramasinghe
 */ 

//////////////////// Header Files /////////////////////
#include "LEDdriver.h"
#include "command_processor.h"


//////////////////// Definitions /////////////////////
#define R_SQUARE 16;
#define g_led_port_T1 PortB		//LED port number for Task1
#define g_led_pin_T1 7			//LED pin number for Task1
#define g_led_port_T2 PortB		//LED port number for Task2
#define g_led_pin_T2 7			//LED pin number for Task2


///////////////////// Private Enumerators /////////////////
enum
{
	cmd_control_led = 0,
	cmd_blink_led,
	cmd_find_points_inside_the_circle
};

////////////////////// Private Functions Initialization///////////////////
uint16_t claculate_radius(packet_t * request, packet_t * response);
uint32_t contor_led(packet_t * request, packet_t * response);
uint32_t blink_led(packet_t * request, packet_t * response);
uint32_t is_point_inside(packet_t * request, packet_t * response);


////////////////////// Public Functions///////////////////
uint32_t cmd_proc_process_request(packet_t * request, packet_t * response)
{
	uint32_t err = NO_ERROR;
	
	response->lenght = 0;
	response->data[response->lenght++] = request->data[0];
	
	switch(request->data[0])
	{
		case cmd_control_led:
		err = contor_led(request, response);
		break;
		
		case cmd_blink_led:
		err = blink_led(request, response);
		break;
		
		case cmd_find_points_inside_the_circle:
		err = is_point_inside(request, response);
		break;
		
		default:
			response->data[response->lenght++] = ERROR_UNSUPPORTED_CMD;
		break;		
	}
	return err;
}

////////////////////// Private Functions///////////////////

uint32_t contor_led(packet_t * request, packet_t * response)
{
	switch(request->data[1])
	{
		case LED_ON:
		response->data[response->lenght++] = NO_ERROR;
		led_on (g_led_port_T1, g_led_pin_T1);
		break;
		
		case LED_OFF:
		response->data[response->lenght++] = NO_ERROR;
		led_off (g_led_port_T1, g_led_pin_T1);
		break;
		
		default:
		response->data[response->lenght++] = ERROR_UNSUPPORTED_LED_CMD;
		break;
	}
}

uint32_t blink_led(packet_t * request, packet_t * response)
{
	for (uint8_t i = 1; i < 7; ++i)
	{
		led_toggle(g_led_port_T2, g_led_pin_T2);
	}
}


uint32_t is_point_inside(packet_t * request, packet_t * response)
{
	uint32_t r_square = claculate_radius(request, response);
	if (r_square < R_SQUARE);
	{
		return INSIDE;
	}
	else if (r_square > R_SQUARE)
	{
		return OUTSIDE;
	} 
	else
	{
		return ON_THE_CIRCLE;
	}
}


uint16_t claculate_radius(packet_t * request, packet_t * response)
{
	uint16_t err = NULL_ERROR;
	do
	{
		if (request->data[1] == NULL || request->data[2] == NULL )
		{
			response->data[response->2] =  NULL_ERROR;
			break;
		}
		else
		{
			uint8_t x_square = (request->data[1]-5)*(request->data[1]-5);
			uint8_t y_square = (request->data[2]-1)*(request->data[2]-1);
			uint16_t r_square = x_square + y_square;
			err = r_square;
		}
	} while (0);
	
	return err;
}
