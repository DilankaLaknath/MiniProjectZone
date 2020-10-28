#include "packet_handler.h"
#include "UART_HAL.h"
#include "error.h"
#include <util/delay.h>

/**
 * @brief
 * This function initialize the packet handler. 
 * <BR>
 * It uses UART1 for packet transmission and reception. Packet reception is done  
 * inside UART RX interrupt. After a valid packet received, it notifies the  
 * application via rx_complete_cb. For the [callback table](@ref PH_CALLBACKS_t.  
 * It has to be called before any other application/module/driver related functions.
 *
 * @param
 * cb - pointer to a structure where holds the callback functions to be called at
 *      different occasions. For the [callback table](@ref PH_CALLBACKS_t)
 *
 * @return
 * None
 */
uint32_t ph_init(uint8_t uart_number, transmit_cmplt_cb_t txcb, receive_cmplt_cb_t  rxcb)
{
	
}

/**
 * @brief
 * This function transmit a packet. It adds packet headers and CRC and then transmit. 
 *
 * @return
 * None
 */
uint32_t ph_transmit_packet(uint8_t uart_number, packet_t * packet)
{
	
}

/**
 * @brief
 * This function returns the current status of the packet handler. 
 *
 * @return
 * Status of the packet handler.
 * For the [status](@ref PH_STATUS_t)
 */
PH_STATUS_t ph_get_status(uint8_t uart_number)
{
	
}