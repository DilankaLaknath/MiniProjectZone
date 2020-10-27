/*
 * MiniProjectZone.c
 *
 * Created: 10/27/2020 1:32:33 PM
 * Author : Dilanka Wickramasinghe
 */ 

//////////////////// Headder Files /////////////////////
#include "LEDdriver.h"
#include "Buttondriver.h"
#include "UART_HAL.h"

#define uint8_t RADIUS = 4;

//ENUM
enum {
	TASK1,
	TASK2
	};
	
uint32_t task_execute(uint8_t task_no, uint8_t serial_input)
{
	if (task_no == TASK1)
	{
		uint8_t do_task1(serial_input);
	}
	else
	{
		uint8_t do_task2(serial_input);
	}
}


uint8_t do_task1(uint8_t serial_input)
{
	uint8_t err = NULL_ERROR;
	
	if (serial_input == ON)
	{
		led_on(led_port, led_pin);
	}
	else if (serial_input == OFF)
	{
		led_off(led_port, led_pin);
	}
	else
	{
		//err = WRONG INPUT;
		break;
	}
}

uint8_t do_task2(uint8_t serial_input)
{
	uint32_t radius = claculate_radius(serial_input);
	if (radius < RADIUS);
	{
		return (Inside);
	} 
}