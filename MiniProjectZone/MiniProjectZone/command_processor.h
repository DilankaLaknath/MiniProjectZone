//////////////////// Header Files /////////////////////
#ifndef __CMD_PROCESSOR
#define __CMD_PROCESSOR

#include "LEDdriver.h"
#include "Buttondriver.h"
#include "packet_handler.h"

/*Enumerators*/
enum task
{
	TASK1,
	TASK2,
	TASK3
};

enum cmd_proc_errors
{
	NO_ERROR,
	NULL_ERROR,
	PACKET_ERROR,
	WRONG_INPUT
};

enum task1_state
{
	LED_ON,
	LED_OFF
};

enum task2_state
{
	INSIDE,
	OUTSIDE
};

uint32_t cmd_proc_process_request(packet_t * request, packet_t * response);

uint32_t find_task_to_execute(uint8_t packet_body);
uint8_t do_task1(uint8_t packet_body);
uint8_t do_task2(uint8_t packet_body);
uint8_t do_task3(uint8_t packet_body);

#endif