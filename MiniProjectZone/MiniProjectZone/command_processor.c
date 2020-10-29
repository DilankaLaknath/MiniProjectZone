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
#define R_SQUARE 16
#define g_led_port_T1 PortB		//LED port number for Task1
#define g_led_pin_T1 7			//LED pin number for Task1
#define g_led_port_T2 PortB		//LED port number for Task2
#define g_led_pin_T2 7			//LED pin number for Task2


////////////////////// Private Functions Initialization///////////////////
uint16_t claculate_radius(uint8_t x , uint8_t y);
uint32_t contor_led(packet_t * request, packet_t * response);
uint32_t blink_led(packet_t * request, packet_t * response);
uint32_t is_point_inside(packet_t * request, packet_t * response);


////////////////////// Public Functions//////////////////////////////////
uint32_t cmd_proc_process_request(packet_t * request, packet_t * response)
{
	uint32_t err = NO_ERROR;
	
	response->length = 0;
	response->data[response->length++] = request->data[0];
	
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
			response->data[response->length++] = ERROR_UNSUPPORTED_CMD;
		break;		
	}
	return err;
}

////////////////////// Private Functions/////////////////////////////////
uint32_t contor_led(packet_t * request, packet_t * response)
{
	switch(request->data[1])
	{
		case LED_ON:
		response->data[response->length++] = LED_ON;
		led_on (g_led_port_T1, g_led_pin_T1);
		break;
		
		case LED_OFF:
		response->data[response->length++] = LED_OFF;
		led_off (g_led_port_T1, g_led_pin_T1);
		break;
		
		default:
		response->data[response->length++] = ERROR_UNSUPPORTED_LED_CMD;
		break;
	}
}

uint32_t blink_led(packet_t * request, packet_t * response)
{
	response->data[response->length++] = LED_BLINK;
	led_blink(g_led_port_T2, g_led_pin_T2, 3);
}


uint32_t is_point_inside(packet_t * request, packet_t * response)
{	if (request->data[1]==NULL && request->data[2]==NULL)
	{
		response->data[response->length++] = OUTSIDE_NULL_INPUT;
	}
	else
	{
		uint16_t r_square = claculate_radius(request->data[1], request->data[2]);
		if (r_square < R_SQUARE)
		{
			response->data[response->length++] = INSIDE;
		}
		else if (r_square > R_SQUARE)
		{
			response->data[response->length++] = OUTSIDE;
		}
		else
		{
			response->data[response->length++] = ON_THE_CIRCLE;
		}
	}
			
}


uint16_t claculate_radius(uint8_t x , uint8_t y)
{

	uint16_t x_square = (x-5)*(x-5);
	uint16_t y_square = (y-1)*(y-1);
	uint16_t r_square = x_square + y_square;
	return r_square;

}