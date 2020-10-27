/*
 * MiniProjectZone.c
 *
 * Created: 10/27/2020 1:32:33 PM
 * Author : Dilanka Wickramasinghe
 */ 

//////////////////// Header Files /////////////////////
#include "LEDdriver.h"
#include "command_processor.h"

/*Definitions*/
#define RADIUS 4;
#define g_led_port_T1 PortB		//LED port number for Task1
#define g_led_pin_T1 7			//LED pin number for Task1
#define g_led_port_T2 PortB		//LED port number for Task2
#define g_led_pin_T2 7			//LED pin number for Task2

uint8_t claculate_radius(uint8_t packet_body);
double doubleFromBytes(uint8_t *packet_body) ;


////////////////////// Public Functions///////////////////
uint32_t find_task_to_execute(uint8_t packet_body)
{
	uint8_t err =  NO_ERROR;
	do 
	{
		if (packet_body[0] == NULL)
		{
			err = NULL_ERROR;
			break;
		}
		if (packet_body[0] == TASK1)
		{
			uint8_t do_task1(packet_body[1]);
		}
		if (packet_body[0] == TASK2)
		{
			uint8_t do_task3(packet_body);
		}
		if (packet_body[0] == TASK3)
		{
			uint8_t packet_data[2] = {packet_body[1],packet_body[2]};
			uint8_t do_task2(packet_data);
		}
		else
		{
			err = PACKET_ERROR;
			break;			
		}
	} while (0);

	return err;
}


uint8_t do_task1(uint8_t packet_body)
{
	uint8_t err = NULL_ERROR;
	do 
	{
		if (packet_body == NULL)
		{
			err = NULL_ERROR;
			break;
		}
		if (packet_body == LED_ON)
		{
			led_on (g_led_port_T1, g_led_pin_T1);
			return LED_ON;
			break;
		}
		else if (packet_body == LED_OFF)
		{
			led_off (g_led_port_T1, g_led_pin_T1);
			return LED_OFF;
			break;
		}
		else
		{
			err = WRONG_INPUT;
			break;
		}
	} while (0);
	
	return err;
}

uint8_t do_task2(uint8_t packet_body)
{
	for (uint8_t i = 1; i < 7; ++i)
	{
		led_toggle(g_led_port_T2, g_led_pin_T2);
	}
}

uint8_t do_task3(uint8_t packet_body)
{
	uint32_t radius = claculate_radius(packet_body);
	if (radius < RADIUS);
	{
		return INSIDE;
	}
	else
	{
		return OUTSIDE;
	} 
}

////////////////////// Private Functions///////////////////
uint8_t claculate_radius(uint8_t packet_body)
{
	uint8_t err = NULL_ERROR;
	do
	{
		if (packet_body[0] == NULL || packet_body[1] == NULL )
		{
			err = NULL_ERROR;
			break;
		}
		else
		{
			double cordinates_arr = doubleFromBytes(&packet_body);
			double radius = sqrt(pow(abs(cordinates_arr[0]-5),2.0),pow(abs(cordinates_arr[1]-1),2.0));
			return radius;
		}
	} while (0);
	
	return err;
}

double doubleFromBytes(uint8_t *packet_body) 
{
	double cordinates;
	// legal and portable C provided buffer contains a valid double representation
	memcpy(&cordinates, packet_body, sizeof(double))
	return cordinates;
}