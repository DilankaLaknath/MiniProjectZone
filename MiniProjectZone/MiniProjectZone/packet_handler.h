/*
 * packet_handler.h
 *
 * Created: 10/27/2020 2:57:13 PM
 *  Author: Kavish Ranawella
 */ 


#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <avr/io.h>

/*Possible CRC errors*/
typedef enum{
	NO_CRC_ERROR,
	CRC_ERROR_DETECTED
}PACKET_CRC_ERR_STATE_t;

#define MAX_PACKET_SIZE	13

typedef struct
{
	uint8_t length;
	uint8_t data[MAX_PACKET_SIZE];
}packet_t;

/*!
 Transmit complete callback function pointer prototype
 for the [callback table](@ref PH_CALLBACKS_t)
 */
typedef void (*transmit_cmplt_cb_t) (uint8_t uart_number ,uint32_t status);

/*!
 Receive complete callback function pointer prototype
 for the [callback table](@ref PH_CALLBACKS_t)
 */
typedef void (*receive_cmplt_cb_t) (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error);


/*!
 Packet handler status which indicates busy or ready to send packets. 
 */
typedef enum{
    /** Packet handler is ready for the next transmission */
    PH_STATUS_READY_TO_SEND = 0,  
    /** Packet handler is already transmitting a packet */
    PH_STATUS_TRANSMITTING
}PH_STATUS_t;


typedef struct  {
    /** This is called when a transmission is complete */
	transmit_cmplt_cb_t tx_complete_cb;
    /** This is called when a packet has been received successfully */
	receive_cmplt_cb_t  rx_complete_cb;
}PH_CALLBACKS_t;


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
uint32_t ph_init(uint8_t uart_number, PH_CALLBACKS_t * cb);

/**
 * @brief
 * This function transmit a packet. It adds packet headers and CRC and then transmit. 
 *
 * @return
 * None
 */
uint32_t ph_transmit_packet(uint8_t uart_number, packet_t * packet);

/**
 * @brief
 * This function returns the current status of the packet handler. 
 *
 * @return
 * Status of the packet handler.
 * For the [status](@ref PH_STATUS_t)
 */
PH_STATUS_t ph_get_status(uint8_t uart_number);





#endif /* PACKET_HANDLER_H_ */