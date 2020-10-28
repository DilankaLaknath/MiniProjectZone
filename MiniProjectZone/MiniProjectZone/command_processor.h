//////////////////// Header Files /////////////////////
#ifndef __CMD_PROCESSOR
#define __CMD_PROCESSOR

#include "LEDdriver.h"
#include "Buttondriver.h"
#include "packet_handler.h"

/*Public Enumerators*/

enum cmd_proc_errors
{
	NO_ERROR,
	NULL_ERROR,
	ERROR_UNSUPPORTED_CMD,
	ERROR_UNSUPPORTED_LED_CMD
};

enum task1_state
{
	LED_ON,
	LED_OFF
};

enum task2_state
{
	INSIDE,
	OUTSIDE,
	ON_THE_CIRCLE
};

uint32_t cmd_proc_process_request(packet_t * request, packet_t * response);

#endif