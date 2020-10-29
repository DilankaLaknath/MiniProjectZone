//////////////////// Header Files /////////////////////
#ifndef __CMD_PROCESSOR
#define __CMD_PROCESSOR

#include "LEDdriver.h"
#include "Buttondriver.h"
#include "packet_handler.h"

/*Public Enumerators*/

enum firstBit
{
	cmd_control_led = 0,
	cmd_blink_led,
	cmd_find_points_inside_the_circle,
	ERROR_UNSUPPORTED_CMD
};

enum secondBit
{
	LED_ON = 0,
	LED_OFF,
	ERROR_UNSUPPORTED_LED_CMD,
	LED_BLINK,
	INSIDE,
	OUTSIDE,
	ON_THE_CIRCLE,
	OUTSIDE_NULL_INPUT
};


/**
 * @brief
 * This function perform the task according to the packet received 
 * 
 * @note
 * It checks the first byte of the packet and decide the task to perform  
 * if the byte = 0 perform task1 - turn on or off led according to the next byte  
 * if the byte = 1 perform task2 - blink the led 3 times when button pressed   
 * if the byte = 2 perform task3 - decide the given coordinate is inside the circle or not
 *
 * @param
 * *request - pointer of the packet received
 * *response - pointer of the response packet to send
 *
 * @return
 * response - response packet to send which contain task status or error status
 */
uint32_t cmd_proc_process_request(packet_t * request, packet_t * response);

#endif