/*
 * packet_handler.h
 *
 * Created: 10/27/2020 2:57:13 PM
 *  Author: Kavish Ranawella
 */ 


#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

/*!
 Transmit complete callback function pointer prototype
 for the [callback table](@ref PH_CALLBACKS_t)
 */
typedef void (*transmit_cmplt_cb_t) (uint32_t status);

/*!
 Receive complete callback function pointer prototype
 for the [callback table](@ref PH_CALLBACKS_t)
 */
typedef void (*receive_cmplt_cb_t) (uint8_t * tl_packet, uint32_t error);


/*!
 Packet handler status which indicates busy or ready to send packets. 
 */
typedef enum{
    /** Packet handler is ready for the next transmission */
    PH_STATUS_READY_TO_SEND = 0,  
    /** Packet handler is already transmitting a packet */
    PH_STATUS_TRANSMITTING
}PH_STATUS_t;


typedef struct {
    /** This is called when a transmission is complete */
	transmit_cmplt_cb_t tx_complete_cb;
    /** This is called when a packet has been received successfully */
	receive_cmplt_cb_t  rx_complete_cb;
}PH_CALLBACKS_t;


/** Polynomial used to calculate the CRC 
 * (@ref http://www.lammertbies.nl/comm/info/crc-calculation.html)
 */
#define POLY        0x1021
#define CRC_INIT    0xFFFF


/** Starting sequence of a packet */
#define STX1    0x02
#define STX2    0x02
#define STX3    0x02
#define STX4    0x02

/** States of packet collector state machine */
typedef enum{
    /** Waiting for start byte 1 */
    PACKET_COLLECTOR_STATE_STATE_STX_1 = 0,   
    /** Waiting for start byte 2 */
    PACKET_COLLECTOR_STATE_STATE_STX_2,   
    /** Waiting for start byte 3 */
    PACKET_COLLECTOR_STATE_STATE_STX_3,
    /** Waiting for start byte 4 */    
    PACKET_COLLECTOR_STATE_STATE_STX_4, 
    /** Waiting for byte of packet length */    
    PACKET_COLLECTOR_STATE_STATE_LENGTH, 
    /** Collecting data bytes */  
    PACKET_COLLECTOR_STATE_STATE_DATA, 
    /** Waiting for low byte of 16bit CRC */      
    PACKET_COLLECTOR_STATE_STATE_CRC_L, 
    /** Waiting for high byte of 16bit CRC */     
    PACKET_COLLECTOR_STATE_STATE_CRC_H,       
}PACKET_COLLECTOR_STATE_t;

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
void ph_init(uint8_t uart_number, PH_CALLBACKS_t * cb);

/**
 * @brief
 * This function transmit a packet. It adds packet headers and CRC and then transmit. 
 *
 * @return
 * None
 */
void ph_transmit_packet(void);

/**
 * @brief
 * This function returns the current status of the packet handler. 
 *
 * @return
 * Status of the packet handler.
 * For the [status](@ref PH_STATUS_t)
 */
PH_STATUS_t ph_get_status(void);





#endif /* PACKET_HANDLER_H_ */